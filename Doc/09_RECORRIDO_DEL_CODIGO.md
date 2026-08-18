# 09 — Recorrido riguroso por el código C

## Includes

- `stdint.h`: tipos de tamaño fijo.
- `stdio.h`: `printf` y `snprintf`.
- `string.h`: comparación y longitud.
- `app_cfg.h`: configuración de aplicación.
- `gd32vw55x_platform.h`: plataforma, GPIO, RTOS.
- `lwip/sockets.h`: API BSD sockets.
- `wifi_init.h` y `wifi_management.h`: radio/SoftAP.

## Constantes

Canal, puerto, pilas, GPIO e intervalos se nombran para evitar números mágicos.
El puerto 80 es HTTP convencional. Ocho clientes es un límite deliberado para no
consumir descontroladamente descriptores.

## Estados

`app_state_t` describe el arranque. `led_mode_t` describe una orden estable.
Separarlos evita usar el LED de usuario como único estado de todo el sistema.

## Variables `volatile`

Las tareas acceden a variables compartidas. `volatile` obliga a realizar
lecturas/escrituras observables, pero no convierte operaciones compuestas en
atómicas ni reemplaza mutexes. Aquí son valores simples y la práctica prioriza
observabilidad; un producto debe revisar sincronización formal.

## `led_set`

Es la única función que conoce la polaridad física. Centralizarla evita invertir
la lógica en cada comando. También actualiza `g_led_is_on`, que representa el
nivel aplicado.

## `apply_led_command`

Compara la primera línea HTTP con rutas conocidas. Solo incrementa
`g_led_commands` si reconoce una orden. Este parser no valida todo RFC HTTP; es
intencionalmente limitado y no debe exponerse a Internet.

## `send_all`

`send` puede transmitir menos bytes de los solicitados. El bucle continúa hasta
completar o detectar error. Asumir una única llamada es un bug frecuente.

## `send_response`

1. aplica comandos;
2. selecciona cuerpo y Content-Type;
3. calcula Content-Length;
4. agrega `Connection: close`, no-cache y CORS;
5. envía cabecera y cuerpo.

`snprintf` limita escritura al tamaño del buffer. Revise siempre que los
buffers sigan siendo suficientes al ampliar JSON.

## Diagnóstico

`diagnostic_led_value` convierte un estado de error en pulsos. La tarea LED lo
usa hasta que el AP queda listo.

## Tarea LED

Corre cada 100 ms. ON/OFF aplican nivel; BLINK acumula tiempo y alterna al alcanzar
250 o 1000 ms. No bloquea el servidor durante todo el periodo.

## Servidor

1. crea socket;
2. permite reutilizar dirección;
3. enlaza `INADDR_ANY:80`;
4. escucha;
5. usa `select` sobre escucha y clientes;
6. acepta hasta ocho;
7. recibe una solicitud, responde y cierra.

`select` evita que un cliente conectado pero silencioso congele por completo la
aceptación de otros clientes.

## Tarea Wi‑Fi

Inicia SoftAP con SSID, contraseña `NULL`, canal 1, autenticación abierta y
`hidden=0`. Si falla, fija un estado visible. Si funciona, inicia servidor.

## `main`

La secuencia esencial, tomada del patrón del ejemplo oficial, es:

1. `platform_init`;
2. GPIO;
3. `wifi_init`;
4. creación de tareas;
5. `sys_os_start`.

Después de iniciar el scheduler, las tareas contienen los bucles permanentes.

## Limitaciones intencionales

- no TLS;
- no autenticación;
- parser parcial;
- una solicitud por conexión;
- buffers estáticos;
- red abierta;
- sin persistencia de modo;
- sin watchdog de aplicación.

Estas limitaciones deben explicarse, no ocultarse.

