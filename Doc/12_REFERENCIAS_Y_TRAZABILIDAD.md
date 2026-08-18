# 12 — Referencias oficiales y trazabilidad

## Fuentes primarias de GigaDevice

1. [Serie GD32VW553](https://www.gigadevice.com/product/mcu/wireless-mcus/gd32vw553-series)
   — características, SDK y descargas.
2. [GD32VW553HMQ6](https://www.gigadevice.com/product/mcu/mcus-product-selector/gd32vw553hmq6)
   — encapsulado, memoria, interfaces y documentación asociada.
3. `GD32VW553xx_Datasheet` — características eléctricas, pines y memoria.
4. `GD32VW55x_User_Manual` — periféricos y registros.
5. **AN154 GD32VW553 Quick Development Guide** — build e imágenes.
6. **AN158 GD32VW553 Wi‑Fi Development Guide** — inicialización y SoftAP.
7. **AN185 GD32VW553 Network Application Development Guide** — lwIP/sockets.
8. SDK `GD32VW55x_RELEASE_V1.0.3g`.
9. Ejemplo `MSDK/examples/wifi/softap_tcp_server`.
10. Manual incluido con GD32 ISP CLI.

La ficha oficial lista GD32 Embedded Builder, GD32 ISP CLI Windows y el SDK entre
las herramientas de la serie.

## Fuente de la placa

- [Documentación LCKFB: LED con biblioteca](https://wiki.lckfb.com/zh-hans/gd32vw553/beginner/lib-led.html)
  — identifica LED1 en PC13 y nivel alto.
- [Entorno LCKFB](https://wiki.lckfb.com/zh-hans/gd32vw553/beginner/environment-setup.html).
- [Aplicaciones Wi‑Fi/BLE LCKFB](https://wiki.lckfb.com/zh-hans/gd32vw553/beginner/wifi-bluetooth-applications.html)
  — remite a la documentación oficial del fabricante para Wi‑Fi.

## Matriz de trazabilidad

| Decisión | Fuente/evidencia |
|---|---|
| MCU HMQ6, 4 MB/320 KB | ficha oficial HMQ6 |
| Wi‑Fi 2,4 GHz b/g/n/ax | ficha oficial |
| secuencia plataforma/Wi‑Fi/tareas | ejemplo SoftAP oficial |
| contraseña `NULL`, red abierta | AN158/API del SDK |
| canal 1 y SSID visible | configuración del laboratorio |
| MSDK en `0xA000` | AN154 y scripts del SDK |
| MBL + MSDK en imagen completa | AN154/build SDK |
| PC13 y nivel alto | LCKFB + prueba física |
| COM CH340 | enumeración Windows y activación ISP |
| UART frente a OpenOCD | prueba reproducida y verificada |
| reconstrucción de imagen | inconsistencia observada del post-build |
| panel HTML local | API validada y comportamiento del navegador |

## Hecho documental frente a hecho experimental

La guía distingue:

- **documentado:** aparece en manual, ficha, nota o ejemplo oficial;
- **observado:** se midió o reprodujo en la placa y PC usados;
- **decisión pedagógica:** se eligió para reducir variables, como red abierta.

No se presenta una observación local como regla universal. Por ejemplo, la ruta
UART es la ruta validada, no una afirmación de que JTAG nunca pueda funcionar.

## Cómo actualizar la guía

Si cambia el SDK:

1. registre versión y fecha;
2. compare APIs y ejemplo SoftAP;
3. verifique layout MBL/MSDK;
4. recompile;
5. repita la lista de aceptación;
6. actualice CHANGELOG y matriz.

