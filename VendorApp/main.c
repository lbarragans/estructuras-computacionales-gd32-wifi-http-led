#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app_cfg.h"
#include "gd32vw55x_platform.h"
#include "lwip/sockets.h"
#include "lwip/priv/sockets_priv.h"
#include "wifi_init.h"
#include "wifi_management.h"

/*
 * EJERCICIO 12: SOFTAP + SERVIDOR HTTP + CONTROL DE LED
 *
 * Este archivo se copia manualmente a:
 *   <SDK>/MSDK/app_http_led/main.c
 *
 * Separación de responsabilidades:
 *   - main(): inicializa plataforma, Wi-Fi, tareas y scheduler.
 *   - wifi_http_task(): crea el SoftAP y ejecuta el servidor.
 *   - http_server(): acepta clientes TCP e interpreta HTTP mínimo.
 *   - led_task(): aplica el modo solicitado sin bloquear la red.
 *   - led_set(): único lugar que conoce la polaridad física de PC13.
 *
 * IMPORTANTE: es un servidor educativo en una red aislada. No implementa
 * autenticación, TLS ni un parser HTTP completo.
 */

/* Configuración físicamente validada: SoftAP abierto, visible y canal 1. */
static char wifi_ssid[] = "GD32_LED_LAB";

/* Parámetros de red, RTOS y hardware centralizados. */
#define WIFI_CHANNEL              1U
#define HTTP_PORT                 80U
#define HTTP_MAX_CLIENTS          8U
#define HTTP_TASK_STACK           4096U
#define LED_TASK_STACK            768U
#define LED_GPIO_PORT             GPIOC
#define LED_GPIO_PIN              GPIO_PIN_13
#define LED_GPIO_CLOCK            RCU_GPIOC
#define BLINK_SLOW_MS             1000U
#define BLINK_FAST_MS             250U
#define LED_TASK_PERIOD_MS        100U

typedef enum {
    APP_STATE_BOOTING = 0,
    APP_STATE_AP_STARTING,
    APP_STATE_AP_READY,
    APP_STATE_WIFI_INIT_FAILED,
    APP_STATE_TASK_CREATE_FAILED,
    APP_STATE_AP_START_FAILED,
    APP_STATE_HTTP_SERVER_FAILED
} app_state_t;

typedef enum {
    LED_MODE_OFF = 0,
    LED_MODE_ON,
    LED_MODE_BLINK_SLOW,
    LED_MODE_BLINK_FAST
} led_mode_t;

/*
 * Variables globales observables.
 *
 * Se declaran volatile porque distintas tareas las leen o escriben y también son
 * útiles para inspección. Volatile no sustituye mutexes ni hace atómicas las
 * expresiones compuestas; esa distinción se explica en Doc/09.
 */
volatile app_state_t g_app_state = APP_STATE_BOOTING;
volatile led_mode_t g_led_mode = LED_MODE_BLINK_SLOW;
volatile uint8_t g_led_is_on = 0U;
volatile int32_t g_wifi_init_result = -999;
volatile int32_t g_wifi_start_result = -999;
volatile int32_t g_http_server_result = -999;
volatile uint32_t g_http_requests = 0U;
volatile uint32_t g_led_commands = 0U;
volatile uint32_t g_clients_served = 0U;
volatile uint32_t g_led_task_alive = 0U;
volatile uint32_t g_wifi_task_alive = 0U;
void *g_led_task_handle = NULL;
void *g_wifi_task_handle = NULL;

/*
 * Página embebida de respaldo. El panel recomendado está en Web/ y usa la misma
 * API mediante URLs absolutas. Mantener una página mínima aquí permite comprobar
 * que el MCU puede entregar contenido, incluso sin copiar archivos al cliente.
 */
static const char http_page[] =
    "<!doctype html><html lang='es'><head>"
    "<meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>GD32 LED</title><style>"
    "body{font-family:system-ui;background:#101827;color:#f8fafc;margin:0;padding:24px}"
    ".card{max-width:520px;margin:auto;background:#1e293b;padding:24px;border-radius:18px}"
    "h1{font-size:1.6rem}.state{padding:12px;background:#0f172a;border-radius:10px;margin:16px 0}"
    ".grid{display:grid;grid-template-columns:1fr 1fr;gap:12px}"
    "button{border:0;border-radius:10px;padding:14px;font-size:1rem;font-weight:700;color:white;background:#2563eb}"
    ".off{background:#475569}.slow{background:#0f766e}.fast{background:#b45309}"
    "small{display:block;color:#94a3b8;margin-top:18px}</style></head><body>"
    "<div class='card'><h1>GD32VW553 - Control Wi-Fi</h1>"
    "<div class='state'>PC13: <strong id='state'>consultando...</strong></div>"
    "<div class='grid'><button onclick=\"cmd('/on')\">Encender</button>"
    "<button class='off' onclick=\"cmd('/off')\">Apagar</button>"
    "<button onclick=\"cmd('/toggle')\">Alternar</button>"
    "<button class='slow' onclick=\"cmd('/blink/slow')\">Parpadeo lento</button>"
    "<button class='fast' onclick=\"cmd('/blink/fast')\">Parpadeo rapido</button>"
    "<button class='off' onclick=\"update()\">Actualizar</button></div>"
    "<small>SoftAP abierto GD32_LED_LAB - http://192.168.237.1</small></div>"
    "<script>const api='http://192.168.237.1';"
    "async function cmd(p){await fetch(api+p,{cache:'no-store'});await update()}"
    "async function update(){let r=await fetch(api+'/status',{cache:'no-store'});let j=await r.json();"
    "document.getElementById('state').textContent=j.mode+' / '+(j.on?'encendido':'apagado')}"
    "update();setInterval(update,1500)</script></body></html>";

static void led_set(uint8_t turn_on)
{
    /* En la LCKFB GD32VW553HMQ6 probada, PC13 es activo en nivel alto. */
    if (turn_on != 0U) {
        gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
        g_led_is_on = 1U;
    } else {
        gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
        g_led_is_on = 0U;
    }
}

static void led_init(void)
{
    /* Un periférico GD32 debe recibir reloj antes de configurar sus registros. */
    rcu_periph_clock_enable(LED_GPIO_CLOCK);
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
    gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,
                            LED_GPIO_PIN);
    led_set(1U);
}

static const char *mode_name(led_mode_t mode)
{
    switch (mode) {
    case LED_MODE_ON: return "ON";
    case LED_MODE_BLINK_SLOW: return "BLINK_SLOW";
    case LED_MODE_BLINK_FAST: return "BLINK_FAST";
    default: return "OFF";
    }
}

static void apply_led_command(const char *request)
{
    /*
     * Solo se inspecciona la línea inicial de una petición GET. El espacio final
     * diferencia, por ejemplo, "/on" de "/one". Esto no es un parser RFC completo.
     */
    if (strncmp(request, "GET /on ", 8U) == 0) {
        g_led_mode = LED_MODE_ON;
    } else if (strncmp(request, "GET /off ", 9U) == 0) {
        g_led_mode = LED_MODE_OFF;
    } else if (strncmp(request, "GET /toggle ", 12U) == 0) {
        g_led_mode = (g_led_is_on != 0U) ? LED_MODE_OFF : LED_MODE_ON;
    } else if (strncmp(request, "GET /blink/slow ", 16U) == 0) {
        g_led_mode = LED_MODE_BLINK_SLOW;
    } else if (strncmp(request, "GET /blink/fast ", 16U) == 0) {
        g_led_mode = LED_MODE_BLINK_FAST;
    } else {
        return;
    }
    g_led_commands++;
}

static void send_all(int socket_fd, const char *data, uint32_t length)
{
    /* send() puede aceptar menos bytes que length; repetir evita truncamientos. */
    uint32_t sent = 0U;
    while (sent < length) {
        int result = send(socket_fd, data + sent, length - sent, 0);
        if (result <= 0) {
            break;
        }
        sent += (uint32_t)result;
    }
}

static void send_response(int client_fd, const char *request)
{
    char header[192];
    char json[128];
    const char *body;
    const char *content_type;
    uint32_t body_length;

    /* Las rutas de control cambian el modo antes de construir la respuesta. */
    apply_led_command(request);

    if (strncmp(request, "GET /status ", 12U) == 0) {
        (void)snprintf(json, sizeof(json),
            "{\"version\":\"v5\",\"mode\":\"%s\",\"on\":%s,\"requests\":%lu,\"app_state\":%lu}",
            mode_name(g_led_mode), (g_led_is_on != 0U) ? "true" : "false",
            (unsigned long)g_http_requests, (unsigned long)g_app_state);
        body = json;
        content_type = "application/json";
    } else if ((strncmp(request, "GET / ", 6U) == 0) ||
               (strncmp(request, "GET /?", 6U) == 0) ||
               (strncmp(request, "GET /index.html ", 16U) == 0)) {
        body = http_page;
        content_type = "text/html; charset=utf-8";
    } else if (strncmp(request, "GET /favicon.ico ", 17U) == 0) {
        body = "";
        content_type = "image/x-icon";
    } else {
        body = "{\"ok\":true}";
        content_type = "application/json";
    }

    /*
     * Content-Length permite al cliente saber dónde termina el cuerpo.
     * Connection: close simplifica el ciclo de vida en este servidor pequeño.
     * CORS abierto permite al HTML local consultar la API de la placa.
     */
    body_length = (uint32_t)strlen(body);
    (void)snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %lu\r\n"
        "Connection: close\r\nCache-Control: no-store\r\n"
        "Access-Control-Allow-Origin: *\r\n\r\n",
        content_type, (unsigned long)body_length);
    send_all(client_fd, header, (uint32_t)strlen(header));
    send_all(client_fd, body, body_length);
}

static uint8_t diagnostic_led_value(app_state_t state, uint32_t tick)
{
    /* Convierte estados de arranque/error en patrones contables de LED. */
    uint32_t pulse_count;
    uint32_t cycle_tick;

    if (state == APP_STATE_BOOTING) {
        return 1U;
    }
    if (state == APP_STATE_AP_STARTING) {
        return ((tick % 10U) == 0U) ? 1U : 0U;
    }

    switch (state) {
    case APP_STATE_WIFI_INIT_FAILED: pulse_count = 2U; break;
    case APP_STATE_TASK_CREATE_FAILED: pulse_count = 3U; break;
    case APP_STATE_AP_START_FAILED: pulse_count = 4U; break;
    case APP_STATE_HTTP_SERVER_FAILED: pulse_count = 5U; break;
    default: pulse_count = 0U; break;
    }

    cycle_tick = tick % 20U;
    if (cycle_tick < (pulse_count * 2U)) {
        return ((cycle_tick & 1U) == 0U) ? 1U : 0U;
    }
    return 0U;
}

static void led_task(void *parameter)
{
    uint32_t elapsed_ms = 0U;
    uint32_t diagnostic_tick = 0U;
    (void)parameter;

    /* Esta tarea nunca termina: cede CPU cada LED_TASK_PERIOD_MS. */
    for (;;) {
        led_mode_t mode;
        uint32_t interval;

        g_led_task_alive++;
        /* Antes de que la red esté lista, el LED pertenece al diagnóstico. */
        if (g_app_state != APP_STATE_AP_READY) {
            led_set(diagnostic_led_value(g_app_state, diagnostic_tick));
            diagnostic_tick++;
            sys_ms_sleep(LED_TASK_PERIOD_MS);
            continue;
        }

        /* Después, el LED refleja el modo pedido por HTTP. */
        mode = g_led_mode;
        interval = (mode == LED_MODE_BLINK_FAST) ? BLINK_FAST_MS : BLINK_SLOW_MS;
        if (mode == LED_MODE_ON) {
            elapsed_ms = 0U;
            led_set(1U);
        } else if (mode == LED_MODE_OFF) {
            elapsed_ms = 0U;
            led_set(0U);
        } else {
            elapsed_ms += LED_TASK_PERIOD_MS;
            if (elapsed_ms >= interval) {
                elapsed_ms = 0U;
                led_set((uint8_t)(g_led_is_on == 0U));
            }
        }
        sys_ms_sleep(LED_TASK_PERIOD_MS);
    }
}

static int http_server(void)
{
    int listen_fd;
    int client_fds[HTTP_MAX_CLIENTS];
    struct sockaddr_in address;
    int reuse = 1;
    uint32_t index;

    /* AF_INET + SOCK_STREAM representa un socket TCP/IPv4. */
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        return -1;
    }
    (void)setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
                     (const char *)&reuse, sizeof(reuse));
    sys_memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_len = sizeof(address);
    address.sin_port = htons(HTTP_PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind asocia el socket a todas las interfaces locales y al puerto 80. */
    if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) != 0) {
        close(listen_fd);
        return -2;
    }
    if (listen(listen_fd, 4) != 0) {
        close(listen_fd);
        return -3;
    }

    for (index = 0U; index < HTTP_MAX_CLIENTS; index++) {
        client_fds[index] = -1;
    }

    /*
     * select() permite observar el socket de escucha y varios clientes sin
     * quedar bloqueados indefinidamente en uno de ellos.
     */
    for (;;) {
        fd_set read_set;
        struct timeval timeout;
        int max_fd = listen_fd;
        int ready;

        FD_ZERO(&read_set);
        FD_SET(listen_fd, &read_set);
        for (index = 0U; index < HTTP_MAX_CLIENTS; index++) {
            if (client_fds[index] >= 0) {
                FD_SET(client_fds[index], &read_set);
                if (client_fds[index] > max_fd) {
                    max_fd = client_fds[index];
                }
            }
        }

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        ready = select(max_fd + 1, &read_set, NULL, NULL, &timeout);
        if (ready <= 0) {
            continue;
        }

        /* Una marca en listen_fd indica una conexión TCP pendiente. */
        if (FD_ISSET(listen_fd, &read_set)) {
            int new_fd;
            struct sockaddr_in client_address;
            socklen_t client_length = sizeof(client_address);

            new_fd = accept(listen_fd, (struct sockaddr *)&client_address,
                            &client_length);
            if (new_fd >= 0) {
                for (index = 0U; index < HTTP_MAX_CLIENTS; index++) {
                    if (client_fds[index] < 0) {
                        client_fds[index] = new_fd;
                        new_fd = -1;
                        break;
                    }
                }
                if (new_fd >= 0) {
                    close(new_fd);
                }
            }
        }

        /* Cada cliente se atiende una vez y luego se cierra deliberadamente. */
        for (index = 0U; index < HTTP_MAX_CLIENTS; index++) {
            int client_fd = client_fds[index];
            if ((client_fd >= 0) && FD_ISSET(client_fd, &read_set)) {
                char request[384];
                int received;

                sys_memset(request, 0, sizeof(request));
                received = recv(client_fd, request, sizeof(request) - 1U, 0);
                if (received > 0) {
                    g_http_requests++;
                    send_response(client_fd, request);
                    g_clients_served++;
                }
                shutdown(client_fd, SHUT_RDWR);
                close(client_fd);
                client_fds[index] = -1;
            }
        }
    }
}

static void wifi_http_task(void *parameter)
{
    (void)parameter;
    g_wifi_task_alive++;
    g_app_state = APP_STATE_AP_STARTING;
    printf("[WIFI] Iniciando SoftAP abierto '%s' en canal %lu\r\n",
           wifi_ssid, (unsigned long)WIFI_CHANNEL);

    /*
     * Esta es exactamente la configuración del repositorio validado:
     * password=NULL, autenticación abierta y SSID visible.
     */
    g_wifi_start_result = wifi_management_ap_start(
        wifi_ssid, NULL, WIFI_CHANNEL, AUTH_MODE_OPEN, 0U);
    if (g_wifi_start_result != 0) {
        g_app_state = APP_STATE_AP_START_FAILED;
        printf("[ERROR] wifi_management_ap_start=%ld\r\n",
               (long)g_wifi_start_result);
        sys_task_delete(NULL);
        return;
    }

    g_app_state = APP_STATE_AP_READY;
    g_led_mode = LED_MODE_BLINK_SLOW;
    printf("[OK] Red visible: %s (sin clave)\r\n", wifi_ssid);
    printf("[OK] Abra http://192.168.237.1\r\n");

    g_http_server_result = http_server();
    g_app_state = APP_STATE_HTTP_SERVER_FAILED;
    printf("[ERROR] HTTP=%ld\r\n", (long)g_http_server_result);
    (void)wifi_management_ap_stop();
    sys_task_delete(NULL);
}

int main(void)
{
    /*
     * Secuencia basada en el patrón del ejemplo SoftAP oficial:
     * plataforma -> GPIO -> Wi-Fi -> tareas -> scheduler.
     */
    platform_init();
    led_init();
    printf("\r\n[BOOT] GD32VW553 Wi-Fi HTTP LED v5\r\n");

    g_wifi_init_result = wifi_init();
    printf("[BOOT] wifi_init=%ld\r\n", (long)g_wifi_init_result);

    /* La tarea LED se crea incluso si Wi-Fi falla para poder señalizar el error. */
    g_led_task_handle = sys_task_create_dynamic(
        (const uint8_t *)"led status", LED_TASK_STACK,
        OS_TASK_PRIORITY(1), led_task, NULL);
    if (g_led_task_handle == NULL) {
        g_app_state = APP_STATE_TASK_CREATE_FAILED;
        printf("[ERROR] No se pudo crear led_task\r\n");
    } else if (g_wifi_init_result != 0) {
        g_app_state = APP_STATE_WIFI_INIT_FAILED;
    } else {
        /* La tarea de red solo se crea si wifi_init terminó correctamente. */
        g_wifi_task_handle = sys_task_create_dynamic(
            (const uint8_t *)"wifi http", HTTP_TASK_STACK,
            OS_TASK_PRIORITY(0), wifi_http_task, NULL);
        if (g_wifi_task_handle == NULL) {
            g_app_state = APP_STATE_TASK_CREATE_FAILED;
            printf("[ERROR] No se pudo crear wifi_http_task\r\n");
        }
    }

    /* A partir de aquí FreeRTOS toma el control y ejecuta las tareas creadas. */
    sys_os_start();
    for (;;) { }
}
