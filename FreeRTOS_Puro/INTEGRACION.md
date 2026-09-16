# Integracion FreeRTOS y SDK WiFi

1. Instale `GD32VW55x_RELEASE_V1.0.3g` en una ruta corta.
2. Cree `MSDK/app_http_led` dentro del SDK.
3. Copie allí `main.c`, `app_cfg.h` y `CMakeLists.txt` de esta carpeta.
4. Ejecute las tareas 1 a 3 del repositorio para compilar MBL/MSDK y construir
   `image-all.bin`.
5. Programe la imagen completa en `0x08000000` mediante UART/CH340.
6. Libere BOOT0, reinicie y espere hasta 30 segundos.
7. Conecte el cliente a `GD32_LED_LAB` y abra el panel local.

La ruta oficial detallada permanece en `Doc/03` a `Doc/08`.
