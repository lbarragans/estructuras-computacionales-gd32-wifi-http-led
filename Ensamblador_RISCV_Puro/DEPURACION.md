# Depuracion de la capa Assembly

Detenga la ejecucion después de uno o más ciclos del banco local y observe:

- `g_http_requests`: seis incrementos por ciclo;
- `g_led_commands`: cinco incrementos por ciclo;
- `g_clients_served`: seis incrementos por ciclo;
- `g_led_mode`: 0=OFF, 1=ON, 2=lento, 3=rapido;
- `g_led_is_on`: estado logico aplicado a PC13.

`apply_http_request` es un simbolo global. Una integracion con el SDK puede
invocarlo con `a0` apuntando a la primera linea de una solicitud terminada.
Esta prueba verifica la capa de aplicacion; no acredita radio, SoftAP o TCP.
