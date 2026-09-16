# 16 - Implementaciones definitivas

## Referencia validada

`VendorApp/` es la aplicacion completa comprobada sobre
`GD32VW55x_RELEASE_V1.0.3g`. Usa FreeRTOS, lwIP, SoftAP y las bibliotecas WiFi
oficiales. Esta ruta conserva la trazabilidad descrita en los documentos 00 a
15 y sigue siendo la referencia de hardware.

## FreeRTOS puro

`FreeRTOS_Puro/` contiene un espejo explícito de la aplicacion validada y los
archivos necesarios para copiarla como `MSDK/app_http_led`. Se mantiene esta
ruta separada para que la estructura sea coherente con los ejercicios 00 a 11.

## Assembly RISC-V puro

`Ensamblador_RISCV_Puro/main.S` implementa GPIO PC13 activo en alto, parser de
las seis rutas HTTP (estado y cinco comandos), modos OFF/ON/BLINK_SLOW/BLINK_FAST, contadores y un banco
de peticiones simulado. No contiene C de aplicacion.

Esta variante no crea un SoftAP ni sockets. Una pila WiFi completamente en
Assembly exigiría reimplementar controladores RF, firmware, FreeRTOS, lwIP,
DHCP y TCP, lo cual está fuera del objetivo y rompería la dependencia oficial
validada. Puede usarse como modulo de aplicacion detrás de la recepcion del SDK.
