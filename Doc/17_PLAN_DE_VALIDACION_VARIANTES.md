# 17 - Plan de validacion de variantes

## Referencia y FreeRTOS

Aplicar completamente `Doc/13_LISTA_DE_ACEPTACION.md`:

- construir MBL y MSDK con el SDK V1.0.3g;
- formar y verificar `image-all.bin` con MSDK en `0xA000`;
- programar por UART/CH340 desde `0x08000000`;
- comprobar SoftAP `GD32_LED_LAB` e IP `192.168.237.1`;
- probar `/status`, `/on`, `/off`, `/toggle`, `/blink/slow` y `/blink/fast`.

## Assembly puro

Integrar `main.S` con startup y linker compatibles con GD32VW553. Observar:

- seis solicitudes simuladas en orden: estado y cinco comandos;
- `g_http_requests` aumenta seis veces y `g_led_commands` cinco por ciclo;
- modos 1, 0, alternado, lento y rapido;
- PC13 activo en alto;
- intervalos de 1000 ms y 250 ms.

La ruta Assembly solo se marca validada como capa de aplicacion. No debe
presentarse como validacion WiFi, HTTP/TCP real o SoftAP.

## Criterio de cierre

No basta una descarga exitosa. La ruta completa requiere red visible, IP,
respuestas HTTP 200, contadores crecientes y control fisico del LED.
