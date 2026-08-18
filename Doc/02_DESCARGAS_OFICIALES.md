# 02 — Descargas oficiales, versiones y licencias

## Principio

Descargue herramientas y documentación desde la página oficial de GigaDevice.
No use binarios reenviados sin origen verificable. Este repositorio contiene la
aplicación educativa, pero no puede reemplazar el soporte del fabricante.

## 1. SDK `GD32VW55x_RELEASE_V1.0.3g`

### Por qué se necesita

Incluye componentes que no están en este repositorio:

- MBL y su proyecto;
- inicialización de plataforma;
- bibliotecas RF y Wi‑Fi;
- FreeRTOS;
- lwIP;
- firmware/calibración inalámbrica;
- archivos de enlace y CMake;
- APIs `wifi_init` y `wifi_management`;
- ejemplos oficiales;
- documentación y notas de aplicación.

Sin el SDK, los encabezados de `main.c` no existen y tampoco las bibliotecas que
implementan sus funciones.

### Qué verificar después de extraer

```text
GD32VW55x_RELEASE_V1.0.3g/
├── MBL/
├── MSDK/
│   ├── examples/wifi/softap_tcp_server/
│   ├── lwip/
│   ├── app/
│   └── projects/
└── scripts/cmake/toolchain.cmake
```

## 2. GD32 Embedded Builder

Se requiere por la cadena Nuclei RISC‑V GNU: compilador, ensamblador, enlazador,
`objcopy`, `objdump` y GDB. Aunque VS Code edite el código, sigue haciendo
falta un compilador que genere instrucciones para el GD32VW553.

## 3. GD32 ISP CLI para Windows

El ejecutable oficial implementa el protocolo UART del bootloader. VS Code lo
invoca con la tarea **4. Programar por UART**; el estudiante no escribe comandos.
Conserve también `OptionBytes`: mover únicamente el EXE puede abrir el puerto y
fallar al inicializar el dispositivo.

## 4. Driver CH340

Solo se instala si Windows no reconoce el USB‑C. Después de instalar:

1. desconecte y reconecte la placa;
2. abra Administrador de dispositivos;
3. expanda “Puertos (COM y LPT)”;
4. anote el COM de `USB-SERIAL CH340`.

## 5. Visual Studio Code

Se utiliza como editor y visor documental. Extensiones recomendadas:

- C/C++ de Microsoft;
- CMake Tools de Microsoft;
- Python de Microsoft.

No se necesita una extensión “GD32” para comprender o editar el repositorio.
La compilación depende del SDK y toolchain, no del coloreado del editor.

## 6. Python 3

La tarea que arma `image-all.bin` ejecuta `tools/image_builder.py`. Instale
Python 3 y, dentro de VS Code, seleccione el intérprete con **Python: Select
Interpreter**. El estudiante no escribe instrucciones: la tarea invoca el
intérprete disponible.

## 7. Documentos

Obtenga desde la ficha oficial:

- datasheet GD32VW553xx;
- user manual GD32VW55x;
- AN154 Quick Development Guide;
- AN158 Wi‑Fi Development Guide;
- AN185 Network Application Development Guide.

Conserve la revisión y fecha. Una API puede cambiar entre versiones del SDK.

## 8. Rutas recomendadas

Evite OneDrive, nombres extensos, espacios y caracteres especiales en el SDK:

```text
C:\GD32\GD32VW55x_RELEASE_V1.0.3g
C:\GD32\EmbeddedBuilder
C:\GD32\GD32_ISP_CLI
C:\Proyectos\12_WiFi_HTTP_LED_Control
```

La recomendación proviene de una falla real: los scripts post-build del SDK
interpretaron mal rutas con espacios y produjeron una `image-all.bin` antigua o
truncada. Una ruta corta reduce esa clase de error.

## 9. Licencias

- El SDK y herramientas conservan las licencias de GigaDevice.
- FreeRTOS, lwIP y demás terceros conservan sus propias licencias.
- No copie todo el SDK al repositorio.
- No publique binarios oficiales como si fueran código propio.
- Lea [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md).

## 10. Lista de descarga

- [ ] SDK exacto descargado y extraído.
- [ ] Embedded Builder instalado.
- [ ] GD32 ISP CLI extraído con su carpeta `OptionBytes`.
- [ ] HMQ6 aparece en el selector.
- [ ] CH340 reconocido.
- [ ] Python 3 seleccionado en VS Code.
- [ ] documentos guardados con revisión identificable.
