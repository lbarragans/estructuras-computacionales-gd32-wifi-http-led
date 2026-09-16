# Integracion de la capa Assembly

## Banco independiente

Integre `main.S` con startup y linker GD32VW553. El flujo simulado permite
validar parser, estados y GPIO sin inicializar la radio.

## Aplicacion WiFi

Para usar la logica detrás del servidor real, exponga `apply_http_request` como
simbolo global o separela en un modulo `.S`, y llámela desde la recepcion HTTP
del MSDK. La radio, FreeRTOS, lwIP y sockets siguen siendo los oficiales.

No describa esta ruta como WiFi validado hasta completar la lista de aceptacion
con `VendorApp`/`FreeRTOS_Puro` y el SDK V1.0.3g.
