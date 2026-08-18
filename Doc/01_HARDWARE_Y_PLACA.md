# 01 — Hardware, conectores, botones y LED

## 1. Identificación

La guía fue validada en la placa LCKFB basada en **GD32VW553HMQ6**. Antes de
programar, lea el marcado del chip o la referencia de su placa. No seleccione
HMQ7, KMQ6 u otra variante por semejanza: capacidad, encapsulado y opciones
pueden variar.

El HMQ6 oficial ofrece núcleo RISC‑V de hasta 160 MHz, 4 MB de Flash, 320 KB de
SRAM, Wi‑Fi 6 en 2,4 GHz y BLE 5.2.

## 2. Conexiones usadas

| Elemento | Función en el laboratorio |
|---|---|
| USB‑C de la placa | alimentación y UART mediante CH340 |
| BOOT0 | selecciona el bootloader al reiniciar |
| RESET | reinicia el MCU y captura el estado de BOOT0 |
| LED1 / PC13 | salida visible y diagnóstico |
| antena | enlace Wi‑Fi; no la cubra con metal |
| WCH‑Link | depuración JTAG; no es el COM del bootloader UART |

## 3. CH340 frente a WCH‑Link

Con USB‑C, Windows debe mostrar algo como `USB-SERIAL CH340 (COM10)`. Ese COM
es el usado por la tarea de programación de VS Code.

Al conectar el depurador puede aparecer `WCH-Link SERIAL (COM9)`. Ese puerto
pertenece al adaptador de depuración. Durante las pruebas el bootloader no
respondió allí. Seleccionar el número incorrecto produce “Opening port
Successful” seguido de “NO response from the device”.

La apertura del puerto solo demuestra que Windows permitió abrir el COM; no que
el MCU esté en bootloader ni que el cableado lógico sea correcto.

## 4. Secuencia BOOT0 + RESET

1. Conecte solamente el USB‑C para la programación UART.
2. Mantenga presionado BOOT0.
3. Sin soltar BOOT0, pulse RESET.
4. Suelte RESET, continúe sosteniendo BOOT0.
5. Ejecute la tarea **4. Programar por UART** en VS Code.
6. Cuando el dispositivo sea activado, puede soltar BOOT0.
7. Después de programar y verificar, asegúrese de que BOOT0 quede suelto.
8. Pulse RESET una vez para arrancar la aplicación.

BOOT0 no “carga el programa”; solo hace que el reset elija el cargador ROM en vez
de la aplicación normal.

## 5. LED1 y PC13

La documentación LCKFB identifica LED1 en PC13. En la placa ensayada, escribir
nivel alto en PC13 enciende el LED. Por eso `led_set(1)` llama
`gpio_bit_set`, y `led_set(0)` llama `gpio_bit_reset`.

No asuma que todos los LEDs de desarrollo son activos en bajo. La polaridad se
obtiene del esquema o de una prueba controlada. Si la API dice ON pero el LED se
apaga, revise primero la polaridad, no la red.

## 6. Alimentación y RF

- Use un cable USB de datos, no uno exclusivamente de carga.
- Evite hubs inestables durante programación.
- Mantenga la antena libre.
- El SoftAP trabaja en 2,4 GHz; un cliente configurado solo para 5 GHz no lo verá.
- Espere hasta 30 segundos tras RESET: inicializar RF, RTOS y red no es inmediato.

## 7. Comprobaciones manuales

- En Administrador de dispositivos aparece CH340 sin símbolo de error.
- Al desconectar USB‑C desaparece ese COM.
- Al reconectar puede cambiar el COM; responda el nuevo número en VS Code.
- BOOT0 y RESET no están confundidos.
- LED1 responde a la aplicación, no a otro GPIO.

## 8. Preguntas

1. ¿Por qué “puerto abierto” no garantiza comunicación con el bootloader?
2. ¿Qué diferencia hay entre mantener BOOT0 y mantener RESET?
3. ¿Por qué se desconecta WCH‑Link en el flujo UART?
4. ¿Qué evidencia prueba la polaridad de PC13?
