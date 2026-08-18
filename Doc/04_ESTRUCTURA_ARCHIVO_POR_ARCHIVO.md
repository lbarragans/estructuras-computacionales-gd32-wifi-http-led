# 04 — Estructura y función de cada archivo

## Árbol

```text
.
├── .vscode/
│   ├── extensions.json
│   ├── settings.json
│   └── tasks.json
├── Doc/
├── VendorApp/
│   ├── CMakeLists.txt
│   ├── app_cfg.h
│   └── main.c
├── Web/
│   └── GD32_LED_CONTROL_LOCAL.html
├── tools/
│   └── image_builder.py
├── .gitattributes
├── .gitignore
├── CHANGELOG.md
├── README.md
└── THIRD_PARTY_NOTICES.md
```

## Archivos raíz

### `README.md`

Portada, resultado, arquitectura, dependencias y navegación. Debe permitir a un
lector decidir si tiene la placa y herramientas correctas.

### `.gitignore`

Evita publicar `build/`, binarios, mapas, objetos, configuraciones personales y
temporales. Una imagen compilada puede contener material del SDK o información
local; no debe subirse por accidente.

### `.gitattributes`

Normaliza finales de línea. Los avisos LF/CRLF de Git no son errores de C; indican
conversión entre convenciones Unix y Windows.

### `CHANGELOG.md`

Registra decisiones y correcciones: UART frente a OpenOCD, reconstrucción de
imagen, polaridad PC13, servidor v5 y panel local.

### `THIRD_PARTY_NOTICES.md`

Separa el código educativo de componentes del fabricante y terceros.

## `.vscode`

### `extensions.json`

Recomienda C/C++ y CMake Tools. No instala software sin consentimiento.

### `settings.json`

Evita configurar CMake al abrir y señala `compile_commands.json` para
IntelliSense después de la configuración.

### `tasks.json`

Es la interfaz del flujo VS Code. Cada tarea usa `type: process`: llama
directamente a CMake, Python o el ISP oficial, sin PowerShell.

- tarea 1: genera Ninja desde el SDK;
- tarea 2: compila MBL/MSDK;
- tarea 3: ensambla y valida la imagen;
- tarea 4: programa por UART;
- tarea 5: abre el panel.

Los `inputs` son cuadros de VS Code para SDK, toolchain, CLI y COM. No contienen
rutas personales publicadas.

## `VendorApp`

### `CMakeLists.txt`

Se copia a `MSDK/app_http_led`. Añade el directorio a includes y registra
`main.c` en el ejecutable que el SDK ya creó. No define por sí solo el
toolchain ni el linker: esos pertenecen al SDK.

### `app_cfg.h`

Selecciona opciones de aplicación. Desactiva BLE/coexistencia porque este
laboratorio usa Wi‑Fi; habilita impresión de diagnóstico.

### `main.c`

Contiene GPIO, estados, tareas, SoftAP, socket TCP, parser HTTP, página embebida,
API, contadores y secuencia de arranque. Se explica por bloques en Doc/09.

## `Web`

### `GD32_LED_CONTROL_LOCAL.html`

Panel independiente. Se carga desde el disco, pero envía `fetch` a
`192.168.237.1`. Incluye estilos, botones, sondeo de estado y mensajes de error.
Puede abrirse y estudiarse sin recompilar firmware.

## `tools/image_builder.py`

Única utilidad auxiliar. No usa PowerShell. Une los binarios recién compilados,
rellena el hueco con `0xFF`, compara byte a byte y calcula SHA‑256. Se mantiene
pequeña y comentada para que pueda auditarse.

## `Doc`

Cada documento responde una pregunta distinta. Separarlos evita un README
imposible de navegar y permite enlazar la solución exacta durante una clase.

## Archivos externos que aparecerán al trabajar

| Archivo | Origen | ¿Se publica? |
|---|---|---|
| `mbl.bin` | build MBL | no |
| `msdk.bin` | build MSDK | no |
| `MSDK.elf` | linker | no |
| `image-all.bin` | tarea 3 | no |
| `compile_commands.json` | CMake | no |

