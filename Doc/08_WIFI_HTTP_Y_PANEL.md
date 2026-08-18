# 08 — SoftAP, HTTP y panel local

## 1. Conexión

Después del reset busque `GD32_LED_LAB`. Es una red abierta para diagnóstico.
Conéctese aunque Windows o el teléfono diga “sin Internet”: la placa es una red
local y no ofrece salida a Internet.

El cliente debe recibir una IP `192.168.237.x`, máscara `255.255.255.0` y
gateway `192.168.237.1`. El gateway es la propia GD32.

## 2. Abrir el panel desde VS Code

1. **Ctrl+Shift+P > Tasks: Run Task**.
2. **5. Abrir panel HTML local**.
3. Confirme que la dirección de API sea `http://192.168.237.1`.
4. Pulse **Actualizar**.

El archivo se carga desde disco; las órdenes viajan a la placa. El navegador
puede marcar el archivo como local, lo cual es esperado.

## 3. Flujo de una orden

```text
clic Encender
  -> JavaScript fetch("http://192.168.237.1/on")
  -> Wi-Fi
  -> TCP puerto 80
  -> recv() en la GD32
  -> apply_led_command()
  -> g_led_mode = LED_MODE_ON
  -> tarea LED
  -> gpio_bit_set(PC13)
```

## 4. Estado asíncrono

La tarea HTTP cambia el modo y responde. La tarea LED aplica el nivel en su
siguiente periodo de 100 ms. Espere brevemente antes de interpretar `on`.

## 5. API y semántica

| Ruta | Cambia `g_led_mode` a |
|---|---|
| `/on` | `LED_MODE_ON` |
| `/off` | `LED_MODE_OFF` |
| `/toggle` | ON u OFF según nivel actual |
| `/blink/slow` | `LED_MODE_BLINK_SLOW` |
| `/blink/fast` | `LED_MODE_BLINK_FAST` |

`/status` devuelve versión, modo, nivel físico, peticiones y estado de arranque.

## 6. Por qué la raíz puede verse vacía

La placa sí respondió con HTML mediante clientes HTTP, pero algunos navegadores
Chromium mostraron una página vacía o realizaron conexiones adicionales. El
servidor educativo atiende solicitudes pequeñas y cierra la conexión; no
implementa HTTP/2, TLS, keep-alive complejo ni todas las conductas de un servidor
de escritorio. El panel local evita que esa diferencia bloquee el objetivo.

## 7. Pruebas sin terminal

- observe el texto de estado del panel;
- pulse ON/OFF y confirme LED;
- pulse lento/rápido y compare periodos;
- desconecte Wi‑Fi: el panel debe mostrar error;
- reconecte y pulse Actualizar;
- observe que el contador de peticiones aumenta.

## 8. DHCP y aislamiento

DHCP asigna automáticamente IP, máscara y gateway. No configure IP fija salvo
para diagnóstico avanzado. Desactive temporalmente VPN/proxy si intercepta redes
locales. Mantenga una sola interfaz Wi‑Fi conectada al SoftAP.

## 9. Seguridad

La cabecera CORS `Access-Control-Allow-Origin: *` permite que el HTML local
consuma la API. Es apropiada para esta práctica abierta, no para producción.
