# Historial de revisión

## 2026-09-17 — Validación física y documentación final

- Se registró la validación completa de Original WiFi: SoftAP, HTTP 200, panel
  local y control físico del LED.
- Se registró Assembly como compilado, programado y validado mediante patrón
  LED, aclarando que no implementa radio.
- Se corrigió el estado de FreeRTOS: compilación completa confirmada y prueba
  física pendiente.
- Se documentó WCH-Link CMSIS-DAP v2, JTAG a 50 kHz, recuperación BOOT/RESET y
  el uso recomendado del panel HTML local.

## v5 — Publicación didáctica solo con VS Code

- flujo completo mediante tareas y cuadros de VS Code;
- eliminación de todos los archivos PowerShell;
- programación UART invocada directamente desde VS Code;
- utilidad Python comentada para ensamblar MBL + MSDK y validar byte a byte;
- corrección física de PC13: nivel alto enciende LED1 en la placa comprobada;
- panel HTML local independiente con API absoluta y manejo de errores;
- documentación desde cero, estructura archivo por archivo, glosario, matriz de
  trazabilidad, árbol de diagnóstico y lista de aceptación;
- separación explícita entre hechos oficiales, observaciones y decisiones
  pedagógicas.

## v4 — Compatibilidad con navegadores modernos

- servidor HTTP basado en `select()` y hasta ocho conexiones simultáneas;
- evita que las conexiones anticipadas de Edge/Chrome bloqueen la petición real;
- atributos `onclick` corregidos y API HTTP explícita;
- respuestas con CORS para que la página descargada también pueda controlar la placa.

## v3 — HTTP integrado y programación UART verificada

- servidor HTTP real sobre el SoftAP validado con el ejemplo oficial;
- SSID abierto `GD32_LED_LAB` y página en `192.168.237.1`;
- control de PC13 y endpoint JSON `/status`;
- programación mediante GD32 ISP CLI y USB-SERIAL CH340;
- reconstrucción determinista de `image-all.bin` desde MBL + MSDK;
- comparación byte a byte para impedir el uso de artefactos antiguos;
- eliminación de F5/OpenOCD del flujo de aceptación Wi-Fi.

## v2 — Corrección de arranque SoftAP

- cambio de WPA2/WPA3 a SoftAP abierto para la primera validación;
- cambio de canal 11 a canal 1;
- SSID visible mediante `hidden=0`;
- programación de `image-all.bin` en `0x08000000`;
- OpenOCD oficial del SDK;
- copia y validación de imágenes MBL/MSDK;
- corrección de `sys_ms_sleep`;
- comprobación de errores y handles de tareas;
- diagnóstico por patrones del LED PC13;
- depuración sin comando `load`;
- ampliación de documentación y lista de aceptación.
