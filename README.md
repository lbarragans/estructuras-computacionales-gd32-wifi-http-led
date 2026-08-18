# Ejercicio 12 — Control de LED por Wi‑Fi y HTTP con GD32VW553

> Guía extensa, manual y reproducible para la placa **LCKFB GD32VW553HMQ6**.  
> No usa PowerShell ni exige escribir comandos: todo se ejecuta con botones y
> tareas de VS Code.

## Qué se construye

La placa crea el punto de acceso Wi‑Fi `GD32_LED_LAB` y un servidor HTTP en
`192.168.237.1:80`. Un teléfono o computador se conecta directamente a esa red
y controla LED1, conectado a PC13, mediante una API y el panel
[Web/GD32_LED_CONTROL_LOCAL.html](Web/GD32_LED_CONTROL_LOCAL.html).

El laboratorio integra RISC‑V, GPIO, el SDK Wi‑Fi/BLE oficial, MBL, MSDK,
FreeRTOS, lwIP, sockets TCP, HTTP, DHCP, SoftAP y una interfaz HTML.

## Configuración validada

| Elemento | Valor comprobado |
|---|---|
| Placa | LCKFB GD32VW553 |
| MCU | GD32VW553HMQ6, QFN40, 4 MB Flash, 320 KB SRAM |
| LED | LED1 en PC13; nivel alto observado = encendido |
| SDK | `GD32VW55x_RELEASE_V1.0.3g` |
| Red | `GD32_LED_LAB`, abierta, canal 1, 2,4 GHz |
| IP del servidor | `192.168.237.1` |
| Programación | bootloader UART por USB‑C/CH340 |
| Imagen completa | MBL en offset 0; MSDK en offset `0xA000` |
| Dirección de descarga | `0x08000000` |

## Por qué esta publicación es manual

Durante el diagnóstico se usaron comandos para aislar errores. Aquí el flujo se
presenta gráficamente para que el estudiante vea cada etapa:

1. descargar herramientas desde GigaDevice;
2. extraer el SDK en una ruta corta y sin espacios;
3. copiar manualmente `VendorApp` a `MSDK/app_http_led`;
4. configurar y compilar desde las interfaces gráficas;
5. inspeccionar los binarios producidos;
6. programar desde una tarea de VS Code que llama al ISP oficial;
7. conectar el equipo al SoftAP;
8. abrir el panel HTML y probar la API.

El repositorio no contiene archivos `.ps1`. GitHub reflejará principalmente el
código C, HTML y CMake, y la automatización no ocultará el aprendizaje.

## Lectura recomendada

1. [Ruta de aprendizaje](Doc/00_RUTA_DE_APRENDIZAJE.md)
2. [Hardware y placa](Doc/01_HARDWARE_Y_PLACA.md)
3. [Descargas oficiales](Doc/02_DESCARGAS_OFICIALES.md)
4. [Instalación manual](Doc/03_INSTALACION_MANUAL.md)
5. [Cada archivo explicado](Doc/04_ESTRUCTURA_ARCHIVO_POR_ARCHIVO.md)
6. [Integración con el SDK](Doc/05_INTEGRACION_CON_EL_SDK.md)
7. [Compilación e imagen](Doc/06_COMPILACION_E_IMAGEN.md)
8. [Programación UART](Doc/07_PROGRAMACION_UART.md)
9. [Wi‑Fi, HTTP y panel](Doc/08_WIFI_HTTP_Y_PANEL.md)
10. [Código explicado](Doc/09_RECORRIDO_DEL_CODIGO.md)
11. [Diagnóstico](Doc/10_DIAGNOSTICO.md)
12. [Glosario](Doc/11_GLOSARIO.md)
13. [Referencias y trazabilidad](Doc/12_REFERENCIAS_Y_TRAZABILIDAD.md)
14. [Lista de aceptación](Doc/13_LISTA_DE_ACEPTACION.md)
15. [VS Code paso a paso](Doc/14_VSCODE_PASO_A_PASO.md)
16. [Preguntas y respuestas razonadas](Doc/15_PREGUNTAS_Y_RESPUESTAS.md)

## Por qué deben descargarse componentes de GigaDevice

`main.c` no puede implementar por sí solo una radio Wi‑Fi. El SDK oficial
aporta controladores, calibración RF, firmware, pila 802.11, FreeRTOS, lwIP,
código de arranque, archivos de enlace, bibliotecas y ejemplos. GD32 Embedded
Builder aporta la cadena Nuclei RISC‑V. GD32 ISP CLI implementa el protocolo del
bootloader y se ejecuta desde una tarea de VS Code. No se redistribuyen porque conservan sus
licencias, son grandes y reciben versiones independientes.

## Arquitectura

```text
Panel HTML local
  └─ Wi‑Fi 2,4 GHz ─> SoftAP GD32_LED_LAB
                         └─ 192.168.237.1:80
                              └─ servidor HTTP/lwIP
                                   ├─ /status
                                   ├─ /on y /off
                                   ├─ /toggle
                                   └─ /blink/slow y /blink/fast
FreeRTOS
  ├─ tarea Wi‑Fi/HTTP
  └─ tarea LED ─> GPIOC PC13 ─> LED1
```

## Mapa de la imagen

```text
0x08000000  offset 0x0000   MBL (cargador de arranque)
            espacio         bytes 0xFF
0x0800A000  offset 0xA000   MSDK (aplicación Wi‑Fi)
```

- `image-all.bin`: MBL + relleno + MSDK; se programa en `0x08000000`.
- `image-ota.bin`: aplicación para el flujo OTA; comienza en `0x0800A000`.
- `MSDK.elf`: archivo enlazado con símbolos; no sustituye la imagen completa.

Una descarga puede finalizar sin errores y aun así no producir Wi‑Fi si se
programó un binario incompleto o una dirección incorrecta.

## Procedimiento gráfico resumido

1. Instale la cadena de Embedded Builder y extraiga GD32 ISP CLI.
2. Instale el driver CH340 si el USB‑C no aparece como `USB-SERIAL CH340`.
3. Extraiga el SDK en `C:\GD32\GD32VW55x_RELEASE_V1.0.3g`.
4. Cree `MSDK\app_http_led` dentro del SDK.
5. Copie allí los tres archivos de `VendorApp`.
6. En la paleta de VS Code ejecute **Tasks: Run Task > 1. Configurar SDK Wi‑Fi**.
7. Ejecute **2. Compilar MBL y MSDK**.
8. Ejecute **3. Construir y validar image-all.bin**.
9. Mantenga BOOT0 y pulse y suelte RESET.
10. Ejecute **4. Programar por UART** y responda los cuadros de VS Code.
11. Espere descarga y verificación de todas las páginas.
12. Suelte BOOT0, pulse RESET y espere hasta 30 segundos.
13. Conéctese a `GD32_LED_LAB`, aunque el sistema avise “sin Internet”.
14. Abra el HTML local y use sus botones.

## API

| Ruta | Acción |
|---|---|
| `/status` | estado y contadores en JSON |
| `/on` | modo encendido |
| `/off` | modo apagado |
| `/toggle` | alterna ON/OFF |
| `/blink/slow` | parpadeo lento |
| `/blink/fast` | parpadeo rápido |

`on` en `/status` indica el nivel físico instantáneo; en modo de parpadeo
cambia aunque el campo `mode` permanezca igual.

## Por qué se recomienda el HTML local

La API fue validada y devuelve HTTP 200, pero algunos navegadores Chromium
mostraron una página vacía al abrir directamente la raíz servida por el servidor
embebido mínimo. El panel local usa URL absoluta, muestra errores y separa la
interfaz de la API. Los botones siguen controlando la placa por Wi‑Fi: “local”
solo describe dónde se carga el HTML.

## Diagnóstico por LED

| Patrón | Significado |
|---|---|
| encendido inicial | plataforma inicializando |
| pulso periódico | SoftAP iniciándose |
| parpadeo lento | aplicación lista |
| 2 pulsos y pausa | error de `wifi_init` |
| 3 pulsos y pausa | error al crear tarea |
| 4 pulsos y pausa | error al iniciar SoftAP |
| 5 pulsos y pausa | error del servidor HTTP |

## Dependencias que no se publican

No se incluyen SDK, compilador, programador, drivers, binarios, rutas privadas,
credenciales ni automatizaciones. Consulte [descargas](Doc/02_DESCARGAS_OFICIALES.md)
y [avisos de terceros](THIRD_PARTY_NOTICES.md).

## Seguridad

La red abierta y HTTP se usan solo para un laboratorio controlado. Un producto
real necesita WPA2/WPA3, autenticación, autorización, cifrado, límites de
peticiones, actualización segura, firma de firmware y protección de secretos.

## Fuentes

La guía contrasta la página y documentación oficiales del GD32VW553HMQ6, AN154,
AN158, AN185, el ejemplo `MSDK/examples/wifi/softap_tcp_server` y la
documentación de la placa LCKFB para LED1/PC13. La procedencia de cada decisión
está en [Doc/12](Doc/12_REFERENCIAS_Y_TRAZABILIDAD.md).
