# Preguntas de comprensión y respuestas razonadas

## ¿Por qué no basta la biblioteca periférica de ejercicios anteriores?

Wi-Fi requiere firmware de radio, FreeRTOS, lwIP, administración del dispositivo,
MBL y un formato de imagen concreto. La biblioteca periférica controla GPIO o
temporizadores, pero no reproduce esa pila completa.

## ¿Por qué el SDK no se publica en este repositorio?

Es una dependencia del fabricante con licencia, tamaño y versiones propios. El
repo publica el código original del ejercicio y documenta cómo integrarlo con
una copia obtenida de la fuente oficial.

## ¿Qué diferencia hay entre MBL, MSDK e `image-all.bin`?

- **MBL** arranca primero y prepara la transferencia al firmware principal.
- **MSDK** contiene aplicación, RTOS, red, Wi-Fi y servidor HTTP.
- **image-all.bin** coloca ambos en los desplazamientos esperados.

## ¿Por qué MSDK empieza en `0x0800A000`?

Los primeros `0xA000` bytes corresponden a la región de arranque. El constructor
rellena con `0xFF` el espacio no ocupado y coloca la aplicación en ese contrato
de memoria. Cambiarlo impide el arranque correcto.

## ¿Por qué no se programa con OpenOCD en esta guía?

La validación real de la imagen Wi-Fi completa produjo estados de módulo de
depuración no autenticado o no examinado. El cargador UART de ROM con GD32 ISP
CLI fue el camino reproducible comprobado. VS Code sigue siendo la única
interfaz porque ejecuta el programador oficial mediante una tarea.

## ¿Por qué se usa CH340 y no `WCH-Link SERIAL`?

CH340 está cableado al UART del cargador de la placa. El puerto serial compuesto
del WCH-Link pertenece al depurador y abrirlo no implica que los bytes lleguen
al bootloader seleccionado por BOOT0.

## ¿Por qué aparece una red pero no carga la interfaz?

El SoftAP activo solo demuestra la capa inalámbrica y parte de IP. Se deben
comprobar por separado DHCP, puerta de enlace, TCP/80, `/status` y el navegador.
Además, caché, actualización a HTTPS y redes sin Internet alteran navegadores.

## ¿Por qué se conserva un panel HTML local?

Separa renderizado y API embebida. El archivo se abre desde VS Code, pero sus
botones consultan directamente la placa. Se conserva el objetivo de controlar
hardware por Wi-Fi sin depender de peculiaridades de la página raíz.

## ¿Por qué la API habilita CORS?

El HTML local tiene origen `file://` y la API origen
`http://192.168.237.1`. Son distintos. Las cabeceras CORS autorizan al panel a
consumir la API.

## ¿Por qué el LED puede parecer invertido?

La relación entre nivel lógico y luz depende del circuito. Aquí PC13 fue
validado como activo en alto. `led_set()` encapsula esa polaridad para que el
servidor trabaje con estados lógicos claros.

## ¿Por qué el servidor usa `select()`?

Permite atender varios sockets sin una tarea bloqueante por cliente. Una tarea
vigila el socket de escucha y los clientes listos mientras mantiene el indicador.

## ¿Por qué hay un límite de clientes?

Cada socket consume memoria de lwIP. Un arreglo fijo hace explícito el
presupuesto y evita crecimiento dinámico no controlado.

## ¿Por qué no confiar en una imagen solo por su nombre?

`image-all.bin` no certifica contenido ni fecha. Una imagen vieja puede arrancar
otro SoftAP. El constructor usa binarios recién generados, verifica segmentos y
muestra su SHA-256.

## ¿Qué demuestra realmente una compilación correcta?

Demuestra compatibilidad de fuentes, bibliotecas y enlace. No demuestra que se
programó esa versión, que arrancó, que inició la radio ni que HTTP responde. La
lista de aceptación exige evidencias separadas.

## ¿Qué puede personalizar un estudiante?

SSID, canal y seguridad; después interfaz y endpoints; finalmente actuadores.
Cada cambio debe validarse por capas y nunca debe publicar credenciales o rutas
personales.

