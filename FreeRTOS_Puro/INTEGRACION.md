# Integracion FreeRTOS y SDK WiFi

1. Instale `GD32VW55x_RELEASE_V1.0.3g` en una ruta corta.
2. Configure `tools/local_config.ps1` con MSDK, toolchain Nuclei y OpenOCD.
3. En VS Code ejecute **Terminal > Run Task > Build + Flash FreeRTOS WiFi**.
4. La tarea crea `MSDK/app_http_led`, copia `main.c`, `app_cfg.h` y
   `CMakeLists.txt`, limpia el build y construye `image-all.bin`.
5. La imagen completa se programa en `0x08000000` mediante WCH-Link
   CMSIS-DAP v2, USB bulk y JTAG a 50 kHz.
6. Espere hasta 30 segundos, conecte el cliente a `GD32_LED_LAB` y abra el
   panel local desde la tarea **Open local HTTP panel**.

La inicializacion reproduce el ejemplo oficial `softap_tcp_server` del SDK:
`platform_init()`, `wifi_init()`, tareas y `sys_os_start()`. No llame
`sys_os_init()` adicionalmente en esta aplicacion WiFi.

La ruta oficial detallada permanece en `Doc/03` a `Doc/08`.
