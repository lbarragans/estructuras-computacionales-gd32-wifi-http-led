# 05 — Integración manual con el SDK usando solo VS Code

## Objetivo

El SDK espera aplicaciones bajo `MSDK/app_<nombre>`. La aplicación de este
repositorio debe quedar en `MSDK/app_http_led`.

## 1. Abrir un espacio de trabajo con dos carpetas

1. Abra este repositorio en VS Code.
2. Use **File > Add Folder to Workspace**.
3. Agregue la raíz del SDK.
4. Use **File > Save Workspace As** si desea conservar la vista. No publique el
   archivo si contiene una ruta absoluta personal.

Ahora el Explorador muestra el repositorio y el SDK.

## 2. Copiar la aplicación

1. Expanda `VendorApp`.
2. En la carpeta SDK expanda `MSDK`.
3. Cree una carpeta llamada exactamente `app_http_led`.
4. Copie, con el Explorador de VS Code:
   - `CMakeLists.txt`;
   - `app_cfg.h`;
   - `main.c`.
5. Abra los archivos de destino y confirme que no quedaron anidados dentro de
   otra carpeta `VendorApp`.

Resultado:

```text
SDK/MSDK/app_http_led/CMakeLists.txt
SDK/MSDK/app_http_led/app_cfg.h
SDK/MSDK/app_http_led/main.c
```

## 3. Configurar desde Run Task

1. Regrese a la ventana cuya carpeta raíz es este repositorio.
2. Abra la paleta con **Ctrl+Shift+P** y elija **Tasks: Run Task**.
3. Seleccione **1. Configurar SDK Wi‑Fi (CMake)**.
4. VS Code preguntará:
   - raíz del SDK;
   - carpeta `bin` del toolchain.
5. Use barras normales `/` o rutas de Windows completas.

La tarea configura:

| Parámetro | Razón |
|---|---|
| `APP=app_http_led` | selecciona nuestra aplicación |
| `CONFIG_OS=FREERTOS` | requiere tareas y servicios del SDK |
| `CONFIG_BLE_FEATURE=MIN` | reduce BLE no usado |
| `CONFIG_MBEDTLS_VERSION=3.6.2` | versión esperada por este SDK |
| toolchain oficial | compila para Nuclei RISC‑V |

## 4. Qué hace CMake

CMake comienza en la raíz del SDK, no en `VendorApp`. El SDK configura MBL,
MSDK, librerías, FreeRTOS, lwIP y enlaces. Al alcanzar `app_http_led`, su
`CMakeLists.txt` agrega `main.c`.

## 5. IntelliSense

Después de configurar:

1. abra la paleta **C/C++: Select IntelliSense Configuration**;
2. elija la configuración derivada de `compile_commands.json`;
3. use **Developer: Reload Window** si los encabezados siguen subrayados.

El compilador real es la autoridad. Un subrayado de IntelliSense no invalida un
build exitoso.

## 6. Repetir después de editar

Edite el archivo del repositorio y vuelva a copiarlo al SDK antes de compilar.
Para evitar modificar el archivo equivocado, use **Compare Active File With...**
entre ambos `main.c`.

## Errores frecuentes

- `APP` con nombre distinto a la carpeta;
- copiar solo `main.c` y olvidar `app_cfg.h`;
- abrir la raíz equivocada al ejecutar tareas;
- usar otro SDK con APIs diferentes;
- ruta del toolchain apuntando un nivel por encima o debajo de `bin`.
