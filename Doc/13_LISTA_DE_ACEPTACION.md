# 13 — Lista de aceptación final

## Herramientas

- [ ] SDK V1.0.3g en ruta corta.
- [ ] CMake y Ninja disponibles para las tareas.
- [ ] toolchain Nuclei localizado.
- [ ] ISP CLI completo con OptionBytes.
- [ ] CH340 aparece como COM.

## Integración

- [ ] espacio de trabajo VS Code contiene repositorio y SDK.
- [ ] `MSDK/app_http_led` tiene tres archivos.
- [ ] los archivos coinciden con `VendorApp`.
- [ ] tarea 1 termina sin error.

## Build

- [ ] tarea 2 termina.
- [ ] `mbl.bin` reciente y no vacío.
- [ ] `msdk.bin` reciente y no vacío.
- [ ] `MSDK.elf` reciente.
- [ ] tarea 3 valida byte a byte.
- [ ] imagen mide `0xA000 + tamaño(MSDK)`.
- [ ] SHA‑256 registrado.

## Programación

- [ ] solo USB‑C conectado.
- [ ] COM corresponde a CH340.
- [ ] BOOT0 mantenido durante RESET/conexión.
- [ ] dispositivo detectado como HMQ6.
- [ ] descarga desde `0x08000000`.
- [ ] todas las páginas verificadas.
- [ ] BOOT0 liberado y RESET normal.

## Red

- [ ] aparece `GD32_LED_LAB`.
- [ ] cliente acepta red sin Internet.
- [ ] IP cliente es `192.168.237.x`.
- [ ] gateway es `192.168.237.1`.
- [ ] panel local muestra estado.

## Funciones

- [ ] Encender enciende físicamente.
- [ ] Apagar apaga físicamente.
- [ ] Alternar cambia estado.
- [ ] lento y rápido son distinguibles.
- [ ] `requests` aumenta.
- [ ] `version` es `v5`.
- [ ] `app_state` es 2.

## Comprensión

- [ ] estudiante explica MBL/MSDK.
- [ ] justifica `0xA000`.
- [ ] diferencia CH340/WCH‑Link.
- [ ] explica “sin Internet”.
- [ ] explica panel local/API remota.
- [ ] enumera riesgos de seguridad.

## Cierre

El ejercicio se considera reproducido solo cuando todas las secciones aplicables
están completas. Una captura aislada de “Successful” no sustituye la validación.

