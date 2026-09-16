# Assembly RISC-V puro - capa HTTP/LED

`main.S` implementa sin C de aplicacion:

- parser de `/status`, `/on`, `/off`, `/toggle`, `/blink/slow` y
  `/blink/fast`;
- modos OFF, ON, lento y rapido;
- PC13 activo en alto;
- SysTimer y banco automatico de seis solicitudes;
- contadores compatibles con la aplicacion de referencia.

No implementa radio, SoftAP, DHCP, TCP ni sockets. Es una variante pura y
ejecutable de la capa de aplicacion, no un reemplazo del SDK WiFi.
