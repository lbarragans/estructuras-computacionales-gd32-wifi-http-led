# 11 — Glosario

**API:** contrato de rutas y respuestas que permite controlar la placa.

**Bootloader:** programa mínimo que recibe y escribe firmware.

**BOOT0:** entrada muestreada durante reset para elegir modo de arranque.

**Build:** transformación de fuentes en objetos, enlace e imágenes.

**CH340:** puente USB–UART presente en la placa.

**CMake:** generador que configura el sistema de construcción.

**COM:** nombre que Windows asigna a un puerto serial.

**CORS:** política del navegador para solicitudes entre orígenes. La cabecera
`Access-Control-Allow-Origin: *` permite el panel local.

**DHCP:** protocolo que entrega IP, máscara y gateway al cliente.

**ELF:** archivo enlazado con secciones y símbolos.

**Flash:** memoria no volátil donde residen MBL y MSDK.

**FreeRTOS:** sistema operativo de tiempo real que planifica tareas.

**GPIO:** entrada/salida digital; PC13 controla LED1.

**HTTP:** protocolo de aplicación de petición/respuesta.

**IP:** dirección lógica. La placa usa `192.168.237.1`.

**ISP:** programación en sistema mediante bootloader.

**JTAG:** interfaz de depuración. No es la ruta de programación recomendada aquí.

**lwIP:** pila TCP/IP ligera usada por el SDK.

**MBL:** cargador inicial colocado en `0x08000000`.

**MSDK:** aplicación y entorno inalámbrico colocado en `0x0800A000`.

**Ninja:** ejecutor del build generado por CMake.

**Offset:** desplazamiento relativo; MSDK usa `0xA000`.

**PC13:** pin 13 del puerto GPIOC.

**Puerto 80:** número TCP convencional de HTTP.

**RISC‑V:** arquitectura de conjunto de instrucciones del núcleo.

**RTOS:** sistema que planifica tareas con prioridades y servicios.

**SHA‑256:** huella para identificar exactamente una imagen.

**Socket:** extremo de comunicación de red.

**SoftAP:** modo en que la placa funciona como punto de acceso.

**SSID:** nombre anunciado por la red Wi‑Fi.

**TCP:** transporte confiable y orientado a conexión usado por HTTP.

**Toolchain:** compilador, ensamblador, linker y utilidades para una arquitectura.

**UART:** comunicación serial usada por el bootloader.

**Verificación:** lectura/comparación posterior a programar Flash.

**WCH‑Link:** adaptador de depuración de la placa; distinto del CH340.

**Wi‑Fi 6 / 802.11ax:** estándar soportado por GD32VW553 en 2,4 GHz.

## Diferencias que suelen confundirse

- SSID no es dirección IP.
- COM no es puerto TCP.
- RESET no borra Flash.
- ELF no es automáticamente una imagen binaria completa.
- “sin Internet” no significa “sin red local”.
- `volatile` no es un mutex.
- compilar no significa programar.
- programar no significa que la aplicación arrancó.

