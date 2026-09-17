# Ejercicio 12 — Control de LED por Wi‑Fi y HTTP con GD32VW553

> Guía extensa, manual y reproducible para la placa **LCKFB GD32VW553HMQ6**.  
> No usa PowerShell ni exige escribir comandos: todo se ejecuta con botones y
> tareas de VS Code.

## Qué se construye

La placa crea el punto de acceso Wi‑Fi `GD32_LED_LAB` y un servidor HTTP en
`192.168.237.1:80`. Un teléfono o computador se conecta directamente a esa red
y controla LED1, conectado a PC13, mediante una API y el panel
[Web/GD32_LED_CONTROL_LOCAL.html](Web/GD32_LED_CONTROL_LOCAL.html).

El laboratorio integra RISC-V, GPIO, el SDK Wi‑Fi/BLE oficial, MBL, MSDK,
FreeRTOS, lwIP, sockets TCP, HTTP, DHCP, SoftAP y una interfaz HTML.

## Implementaciones definitivas

| Camino | Archivos | Alcance |
|---|---|---|
| Referencia validada | `VendorApp/` | compilada, programada y validada físicamente: SoftAP, HTTP y LED |
| FreeRTOS puro | `FreeRTOS_Puro/` | compilación completa validada; prueba física pendiente |
| RISC-V puro | `Ensamblador_RISCV_Puro/main.S` | compilada, programada y validada por patrón LED, sin radio |

La radio 802.11, calibracion RF, SoftAP, DHCP, TCP/IP y sockets permanecen en
el SDK oficial. La variante Assembly no pretende reemplazar esas bibliotecas
propietarias: aisla la capa de aplicacion para estudiar instrucciones, parser y
maquina de estados. Consulte
[Doc/16_VARIANTES_DEL_EJERCICIO.md](Doc/16_VARIANTES_DEL_EJERCICIO.md) y
[Doc/17_PLAN_DE_VALIDACION_VARIANTES.md](Doc/17_PLAN_DE_VALIDACION_VARIANTES.md).

## Configuración validada

| Elemento | Valor comprobado |
|---|---|
| Placa | LCKFB GD32VW553 |
| MCU | GD32VW553HMQ6, QFN40, 4 MB Flash, 320 KB SRAM |
| LED | LED1 en PC13; nivel alto observado = encendido |
| SDK | `GD32VW55x_RELEASE_V1.0.3g` |
| Red | `GD32_LED_LAB`, abierta, canal 1, 2,4 GHz |
| IP del servidor | `192.168.237.1` |
| Programación | WCH-Link CMSIS-DAP v2, JTAG a 50 kHz |
| Imagen completa | MBL en offset 0; MSDK en offset `0xA000` |
| Dirección de descarga | `0x08000000` |

## Por qué esta publicación es manual

Durante el diagnóstico se usaron comandos para aislar errores. Aquí el flujo se
presenta gráficamente para que el estudiante vea cada etapa:

1. descargar herramientas desde GigaDevice;
2. extraer el SDK en una ruta corta y sin espacios;
3. revisar cómo la tarea copia `VendorApp` a `MSDK/app_http_led`;
4. configurar y compilar desde las tareas de VS Code;
5. inspeccionar los binarios producidos;
6. programar desde una tarea de VS Code mediante WCH-Link/OpenOCD;
7. conectar el equipo al SoftAP;
8. abrir el panel HTML y probar la API.

Los archivos `.ps1` son la implementación interna de tareas visibles de VS
Code. El procedimiento docente continúa siendo manual: el estudiante elige
cada tarea, lee su salida y comprueba el resultado físico.

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
17. [Variantes definitivas](Doc/16_VARIANTES_DEL_EJERCICIO.md)
18. [Validación de variantes](Doc/17_PLAN_DE_VALIDACION_VARIANTES.md)

## Por qué deben descargarse componentes de GigaDevice

`main.c` no puede implementar por sí solo una radio Wi‑Fi. El SDK oficial
aporta controladores, calibración RF, firmware, pila 802.11, FreeRTOS, lwIP,
código de arranque, archivos de enlace, bibliotecas y ejemplos. GD32 Embedded
Builder aporta la cadena Nuclei RISC‑V y OpenOCD. Las tareas de VS Code usan
OpenOCD con WCH-Link para programar la imagen completa. Estos componentes no se
redistribuyen porque conservan sus licencias, son grandes y reciben versiones
independientes.

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

1. Instale la cadena Nuclei incluida con GD32 Embedded Builder.
2. Conecte WCH-Link y confirme **WCH CMSIS-DAP** en Windows.
3. Extraiga el SDK en `C:\GD32\GD32VW55x_RELEASE_V1.0.3g`.
4. Mantenga disponible el SDK bare-metal usado por los ejercicios anteriores.
5. Abra exclusivamente la raíz de este ejercicio en VS Code.
6. Copie `tools/local_config.example.ps1` como `tools/local_config.ps1` y use
   las mismas rutas ya validadas en los ejercicios anteriores.
7. En VS Code ejecute **Terminal > Run Task > Verify GD32 Environment**.
8. Ejecute **Build + Flash Assembly** para validar localmente parser, modos y
   GPIO, sin afirmar que esta ruta contiene una pila WiFi.
9. Ejecute **Build + Flash Original WiFi** para repetir el camino WiFi validado.
   **Build + Flash FreeRTOS WiFi** queda disponible como variante cuya
   compilación se comprobó, pero cuya validación física todavía está pendiente.
   La tarea copia la aplicación a `MSDK/app_http_led`, limpia el build,
   construye `image-all.bin` y la programa mediante WCH-Link.
10. Espere `Programming Finished`, `Verified OK` y `Resetting Target`; el
    arranque de la radio puede tardar hasta 30 segundos.
11. Conéctese a `GD32_LED_LAB`, aunque Windows indique “sin Internet”.
12. Ejecute **Open local HTTP panel** y pruebe todos sus botones.

La ruta WiFi sigue deliberadamente la secuencia del ejemplo oficial
`softap_tcp_server`: `platform_init()`, `wifi_init()`, creación de tareas y
`sys_os_start()`. No añada `sys_os_init()` antes de `platform_init()`, porque
esta plataforma del MSDK ya prepara el wrapper del sistema operativo.

## Estado de validación al 17 de septiembre de 2026

| Variante | Compila | Programa | Evidencia física |
|---|---:|---:|---|
| Original `VendorApp` | Sí | Sí | `GD32_LED_LAB`, HTTP 200, panel local y LED |
| Assembly | Sí | Sí | patrón diagnóstico del LED |
| FreeRTOS | Sí | pendiente de repetir | no se declara funcional en placa todavía |

El último intento FreeRTOS terminó de construir los 313 objetivos y generó
`image-all.bin`. La programación no comenzó porque Windows/OpenOCD no encontró
el WCH-Link (`unable to find a matching CMSIS-DAP device`). Ese mensaje no es
un error de compilación, pero tampoco constituye una validación física.

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
