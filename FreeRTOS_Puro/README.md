# FreeRTOS puro - aplicacion WiFi completa

Esta carpeta es el camino operativo completo del ejercicio. Contiene un espejo
de `VendorApp` para copiarlo como `MSDK/app_http_led` dentro de
`GD32VW55x_RELEASE_V1.0.3g`.

Incluye:

- inicializacion oficial de plataforma y WiFi;
- SoftAP abierto `GD32_LED_LAB`;
- FreeRTOS y tareas WiFi/HTTP y LED;
- servidor TCP/lwIP en puerto 80;
- API de estado y cinco comandos;
- diagnostico de errores mediante PC13.

No intente compilar esta carpeta con el CMake bare-metal. Desde VS Code use
**Build + Flash FreeRTOS WiFi**: la tarea la integra en el MSDK oficial,
construye la imagen completa y la programa mediante WCH-Link.
