# 07 — Programación UART desde VS Code

## Por qué UART

Los ejercicios simples se cargaron con OpenOCD/JTAG. Al usar la imagen Wi‑Fi
completa aparecieron errores de autenticación y versión del Debug Module. UART
ISP usa el bootloader de fábrica y permitió descargar y verificar la imagen
completa. No se afirma que JTAG sea imposible en toda configuración; se documenta
la ruta que fue reproducible en esta placa.

## Preparación

1. Desconecte WCH‑Link.
2. Conecte el USB‑C.
3. Confirme el COM del CH340.
4. Cierre monitores seriales.
5. Confirme que la tarea 3 creó `build/debug/artifacts/image-all.bin`.

## Entrar al bootloader

Mantenga BOOT0, pulse y suelte RESET y siga sosteniendo BOOT0. No ejecute la tarea
antes de preparar este estado porque el CLI intentará conectar inmediatamente.

## Ejecutar

1. **Ctrl+Shift+P > Tasks: Run Task**.
2. **4. Programar por UART (preparar BOOT0 antes)**.
3. Complete los cuadros:
   - ruta completa del EXE;
   - carpeta que contiene EXE y `OptionBytes`;
   - número COM sin la palabra COM.
4. Cuando aparezca `Activating device [Successful]`, suelte BOOT0.
5. Espere descarga y verificación.

La tarea usa 57600 baudios, 8 bits, paridad EVEN, 1 stop bit, HMQ6, dirección
`0x08000000`, verificación y borrado necesario.

## Salida correcta

Se esperan:

- apertura de puerto;
- detección `Device GD32VW553HMQ6`;
- activación exitosa;
- páginas descargadas;
- las mismas páginas verificadas con `[Successful]`.

Algunas versiones del CLI pueden devolver código de proceso 1 aun después de
verificar todas las páginas. No ignore cualquier código 1: compruebe primero que
no exista `NO response`, `failed` o una página sin verificación. La evidencia
principal es la tabla completa de verificación y luego el arranque funcional.

## Arranque normal

1. BOOT0 suelto.
2. Pulse RESET una vez.
3. Espere 30 segundos.
4. Busque `GD32_LED_LAB`.

## Riesgos

- No programe `image-ota.bin` en `0x08000000`.
- No programe `image-all.bin` en `0x0800A000`.
- No desconecte alimentación durante erase/download.
- No elija el COM de WCH‑Link.
- No deje BOOT0 presionado al reiniciar la aplicación.
