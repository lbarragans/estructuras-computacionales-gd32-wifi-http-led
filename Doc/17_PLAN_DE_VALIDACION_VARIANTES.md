# 17 - Plan de validacion de variantes

## Referencia WiFi

Aplicar completamente `Doc/13_LISTA_DE_ACEPTACION.md`:

- construir MBL y MSDK con el SDK V1.0.3g;
- formar y verificar `image-all.bin` con MSDK en `0xA000`;
- programar por WCH-Link/OpenOCD desde `0x08000000`;
- comprobar SoftAP `GD32_LED_LAB` e IP `192.168.237.1`;
- probar `/status`, `/on`, `/off`, `/toggle`, `/blink/slow` y `/blink/fast`.

Resultado comprobado el 17-09-2026: compilacion completa, imagen verificada,
programacion terminada con `Verified OK`, red `GD32_LED_LAB`, host
`192.168.237.1`, puerto 80 accesible, respuestas HTTP 200 y control desde
`Web/GD32_LED_CONTROL_LOCAL.html`.

## FreeRTOS

La compilacion llego a `313/313` y produjo una imagen completa validada byte a
byte. La prueba de programacion quedo interrumpida antes de escribir porque el
WCH-Link no estaba disponible para OpenOCD. Estado correcto: **compila; prueba
fisica pendiente**.

Para cerrarla faltan, sin excepciones:

- WCH CMSIS-DAP visible en Windows;
- `Programming Finished`, `Verified OK` y reset;
- SoftAP visible;
- HTTP 200 en todas las rutas;
- respuesta fisica del LED.

## Assembly puro

Integrar `main.S` con startup y linker compatibles con GD32VW553. Observar:

- seis solicitudes simuladas en orden: estado y cinco comandos;
- `g_http_requests` aumenta seis veces y `g_led_commands` cinco por ciclo;
- modos 1, 0, alternado, lento y rapido;
- PC13 activo en alto;
- intervalos de 1000 ms y 250 ms.

La ruta Assembly solo se marca validada como capa de aplicacion. No debe
presentarse como validacion WiFi, HTTP/TCP real o SoftAP.

Resultado comprobado: compilacion, programacion y patron LED correctos.

## Criterio de cierre

No basta una descarga exitosa. La ruta completa requiere red visible, IP,
respuestas HTTP 200, contadores crecientes y control fisico del LED.

Por ello el repositorio distingue expresamente entre **compila**, **programa**
y **funciona físicamente**.
