# 00 — Ruta de aprendizaje y alcance

## Pregunta central

¿Cómo puede un microcontrolador RISC‑V crear su propia red Wi‑Fi, asignar
direcciones IP, aceptar conexiones TCP, interpretar HTTP y convertir una orden
de red en un cambio físico sobre un GPIO?

## Competencias

Al finalizar, el estudiante debe poder:

- distinguir MCU, placa, SDK, cadena de compilación y programador;
- explicar MBL y MSDK;
- justificar el offset `0xA000`;
- entrar al bootloader con BOOT0 y RESET;
- diferenciar CH340 de WCH‑Link;
- explicar SoftAP, DHCP, gateway, TCP, puerto y HTTP;
- seguir una petición desde el navegador hasta PC13;
- interpretar los contadores de `/status`;
- diagnosticar si el fallo está en build, flash, arranque, RF, IP, TCP, HTTP o UI;
- reconocer por qué el ejemplo no es seguro para producción.

## Prerrequisitos

- C básico: funciones, estructuras, punteros, `enum`, `volatile`;
- archivos y carpetas de Windows;
- nociones de hexadecimal;
- diferencia elemental entre hardware y software.

No es requisito dominar FreeRTOS, lwIP o redes. Se introducen aquí.

## Método

El laboratorio usa cuatro capas de evidencia:

1. **Estructural:** existen los archivos esperados.
2. **Compilación:** MBL y MSDK se generan sin errores.
3. **Programación:** la tarea de VS Code llama al ISP y verifica las páginas.
4. **Comportamiento:** aparece el SSID, existe DHCP, responde `/status` y cambia LED1.

No avance si una capa falla. Repetir todo sin saber cuál capa falló oculta el
problema.

## Duración sugerida

| Bloque | Tiempo |
|---|---:|
| lectura y descargas | 45–90 min |
| instalación | 30–60 min |
| integración del código | 30 min |
| compilación inicial | 15–30 min |
| programación UART | 10–20 min |
| pruebas de red | 20–40 min |
| análisis del código | 60–120 min |

## Evidencias a entregar

- captura del puerto `USB-SERIAL CH340`;
- captura de compilación sin error;
- tamaños y fechas de `mbl.bin`, `msdk.bin` e `image-all.bin`;
- captura de verificación del programador;
- captura del SSID;
- dirección IP recibida por el cliente;
- JSON de `/status`;
- demostración de ON, OFF y ambos parpadeos;
- respuesta argumentada a las preguntas de cada capítulo.

## Regla de oro

Una descarga “Successful” solo demuestra que ciertos bytes llegaron a Flash. No
demuestra que sean los bytes correctos, que estén en la dirección correcta ni
que la aplicación haya iniciado. Por eso se validan todas las capas.
