# Panel web local

`GD32_LED_CONTROL_LOCAL.html` se abre desde VS Code con la tarea 5.

Aunque el archivo se carga desde el disco, sus botones ejecutan peticiones HTTP
a `http://192.168.237.1`. Por tanto:

- el PC debe estar conectado a `GD32_LED_LAB`;
- la placa debe haber entregado una IP por DHCP;
- un proxy o VPN no debe bloquear la red local;
- el LED sigue siendo controlado remotamente por Wi‑Fi.

El panel usa una URL absoluta porque una URL relativa desde un archivo local
apuntaría a `file://` y no al microcontrolador. Incluye CORS, actualización
periódica, estado visible y manejo de errores.

Para estudiar el código:

1. abra el HTML en VS Code;
2. identifique la función `apiBase`;
3. siga `command`;
4. siga `updateStatus`;
5. compare las rutas con `apply_led_command` en `VendorApp/main.c`.
