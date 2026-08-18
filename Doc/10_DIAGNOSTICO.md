# 10 — Diagnóstico por capas

## Árbol de decisión

### A. VS Code no configura

- confirme que la tarea se ejecuta desde la raíz del repositorio;
- confirme raíz SDK y `scripts/cmake/toolchain.cmake`;
- confirme carpeta exacta `NucleiRISCVGCC/bin`;
- use ruta corta sin espacios;
- confirme que `MSDK/app_http_led` contiene tres archivos.

### B. Configura pero no compila

- abra el primer error, no el último;
- distinga warning del SDK de error de aplicación;
- confirme nombres de APIs contra V1.0.3g;
- si no encuentra `app_cfg.h`, revise copia y includes;
- si no encuentra toolchain, la ruta apunta al nivel equivocado.

### C. La imagen es demasiado pequeña

- compare tamaño MSDK con imagen;
- ejecute tarea 3;
- no use un `image-all.bin` de `scripts/images` solo por su nombre;
- revise fecha y SHA‑256.

### D. ISP abre COM pero no responde

- seleccione CH340, no WCH‑Link;
- mantenga BOOT0 antes del RESET;
- siga manteniendo durante conexión;
- cierre monitores seriales;
- pruebe cable de datos;
- actualice COM si Windows lo cambió.

### E. “Init OptionBytes failed”

El EXE se ejecutó fuera de su estructura. La carpeta de trabajo debe contener los
recursos del paquete, especialmente `OptionBytes/GD32VW553.xml`. La tarea
solicita por separado EXE y directorio por este motivo.

### F. Programa y verifica, pero no aparece SSID

- BOOT0 debe quedar suelto;
- pulse RESET;
- espere 30 s;
- busque en 2,4 GHz;
- observe patrón LED;
- confirme imagen completa y offset;
- vuelva a copiar `VendorApp/main.c` al SDK antes del build.

### G. SSID aparece, no obtiene IP

- olvide y reconecte la red;
- desactive IP estática;
- espere DHCP;
- pruebe otro cliente;
- revise que no haya otra red con igual SSID.

### H. IP existe, panel no actualiza

- confirme gateway `192.168.237.1`;
- desactive VPN/proxy;
- abra el HTML local, no una copia de 11 bytes/JSON;
- confirme la dirección API del panel;
- pulse Actualizar;
- cierre pestañas antiguas con caché.

### I. ON/OFF invertidos

Revise polaridad en `led_set`. Para la placa validada, alto enciende PC13. No
invierta etiquetas HTML para esconder un error de hardware: corríjalo en la
abstracción GPIO.

### J. `mode=OFF` pero `on=true`

La tarea LED aplica la orden en hasta 100 ms. Espere y consulte otra vez. Si
persiste, revise polaridad, versión v5 programada y que no ejecuta una imagen
anterior.

## Errores observados y aprendizaje

| Síntoma | Causa/decisión |
|---|---|
| OpenOCD auth/DM version | usar UART ISP para imagen Wi‑Fi |
| CLI sin respuesta en COM9 | COM9 era WCH‑Link; usar CH340 |
| OptionBytes failed | ejecutar CLI con su carpeta de recursos |
| imagen 170392 antigua | post-build falló por rutas; reconstruir |
| red no aparecía | imagen incompleta/incorrecta |
| página directa vacía | usar panel local con API absoluta |
| botones local file no funcionaban | fetch relativo apuntaba a `file://`; usar URL absoluta |
| ON/OFF invertidos | polaridad PC13 corregida a activo alto |

## Evidencia mínima de recuperación

No declare resuelto por un solo indicador. Exija: imagen validada, páginas
verificadas, SSID, DHCP, `/status` y cambio físico.

