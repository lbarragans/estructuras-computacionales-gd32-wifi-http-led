# Procedimiento completo usando únicamente Visual Studio Code

Este capítulo describe la experiencia exacta del estudiante. No requiere abrir
otro IDE, escribir instrucciones en una consola ni editar scripts de
automatización. Las acciones se lanzan desde la paleta de tareas de VS Code.

## 1. Preparación única del equipo

1. Instale **Visual Studio Code estable**.
2. Abra VS Code y seleccione el icono **Extensiones** de la barra izquierda.
3. Instale las extensiones recomendadas cuando VS Code muestre la notificación:
   - C/C++ de Microsoft;
   - CMake Tools de Microsoft;
   - Python de Microsoft.
4. Instale fuera del proyecto los paquetes oficiales indicados en
   [02_DESCARGAS_OFICIALES.md](02_DESCARGAS_OFICIALES.md). No copie el SDK
   completo al repositorio.
5. Extraiga el SDK `GD32VW55x_RELEASE_V1.0.3g` en una ruta corta, permanente y
   sin cambiar su estructura interna.
6. Extraiga GD32 ISP CLI y conserve juntos el ejecutable, `OptionBytes`, `Doc`
   y las demás carpetas del paquete. El ejecutable necesita esos recursos para
   reconocer `GD32VW553HMQ6`.

## 2. Abrir correctamente el ejercicio

1. En VS Code seleccione **Archivo > Abrir carpeta...**.
2. Elija la carpeta que contiene `README.md`, `VendorApp`, `Web` y `.vscode`.
3. Confirme la confianza en el contenido si VS Code pregunta por ella.
4. Compruebe en el Explorador que el nombre de la carpeta raíz sea el del
   ejercicio y no una carpeta superior con muchos ejercicios.

Abrir únicamente `main.c` no es suficiente: VS Code necesita leer
`.vscode/tasks.json` desde la raíz para presentar las tareas.

## 3. Integrar la aplicación en el SDK

La compilación oficial ocurre dentro del árbol MSDK. Por eso se realiza una
copia visible y deliberada:

1. En el Explorador de Windows abra `VendorApp` dentro de este proyecto.
2. Abra en otra ventana `<SDK>\MSDK`.
3. Cree allí una carpeta llamada `app_http_led`.
4. Copie dentro de ella `main.c`, `app_cfg.h` y `CMakeLists.txt`.
5. Si recompila después de modificar el código, vuelva a copiar los archivos y
   acepte reemplazarlos.

Esta acción manual evita esconder la integración tras automatización y ayuda a
entender qué parte pertenece al curso y qué parte aporta GigaDevice.

## 4. Configurar con la tarea de VS Code

1. Abra **Terminal > Ejecutar tarea...**. Este menú no obliga a escribir en la
   terminal: solo abre una lista gráfica.
2. Seleccione **1. Configurar SDK Wi-Fi (CMake)**.
3. VS Code solicitará la raíz del SDK. Pegue la carpeta que contiene `MSDK`,
   `MBL` y `scripts`.
4. Cuando solicite el directorio del toolchain, pegue la carpeta `bin` que
   contiene `riscv-nuclei-elf-gcc.exe`.
5. Espere el mensaje de CMake que confirma que se generaron los archivos de
   construcción.

La salida inferior es un registro, no una segunda interfaz de trabajo. Si hay
un error, copie su texto para diagnosticarlo; no introduzca comandos allí.

## 5. Compilar el MBL y el MSDK

1. Vuelva a **Terminal > Ejecutar tarea...**.
2. Seleccione **2. Compilar MBL + MSDK**.
3. Espere a que termine el enlace de `MBL.elf` y `MSDK.elf`.
4. Advertencias de terceros sobre variables no usadas en lwIP no equivalen a
   un fallo. Un error se identifica con `FAILED` o salida distinta de cero.
5. Verifique con el Explorador de Windows que existan:
   - `<SDK>\MBL\project\cmake\bin\mbl.bin`;
   - `<SDK>\MSDK\projects\cmake\output\bin\msdk.bin`.

## 6. Construir la imagen completa

1. Ejecute **3. Construir y validar image-all.bin**.
2. Seleccione nuevamente la raíz del SDK si se solicita.
3. La tarea llama al programa Python auditable incluido en el repo.
4. Revise en la salida la dirección del MBL (`0x08000000`), la del MSDK
   (`0x0800A000`), tamaños, SHA-256 y validación byte a byte.
5. El resultado queda en `build/debug/artifacts/image-all.bin`.

No use una `image-all.bin` antigua del SDK. El archivo correcto debe contener
el `msdk.bin` recién enlazado; de lo contrario puede aparecer `test_ap` pero no
el servidor desarrollado en este ejercicio.

## 7. Colocar la placa en el cargador UART

Use el cable USB-C que enumera el puente CH340. Para esta fase no confunda el
puerto del WCH-Link con el puerto del bootloader.

1. Mantenga presionado **BOOT0**.
2. Pulse y suelte **RESET** sin soltar BOOT0.
3. Mantenga BOOT0 hasta que la tarea indique que activó el dispositivo.
4. Suelte BOOT0.

En el Administrador de dispositivos, el puerto útil aparece como
`USB-SERIAL CH340 (COMn)`. Anote únicamente el número `n`.

## 8. Programar desde una tarea de VS Code

1. Ejecute **4. Programar por UART con GD32 ISP CLI**.
2. Indique la carpeta que contiene `GD32_ISP_CLI.exe` y sus recursos.
3. Indique el número del COM del CH340, sin la palabra `COM`.
4. Complete la secuencia BOOT0/RESET.
5. Espere **Downloading** y luego **Verifying**.
6. La versión conocida del ISP CLI puede devolver código `1` aun después de
   verificar. La evidencia válida es que todas las páginas terminen en
   `[Successful]`; el código aislado no debe interpretarse sin esa salida.
7. Al terminar, suelte BOOT0 y pulse RESET una vez para arrancar desde flash.

## 9. Conectarse y abrir el panel

1. Busque `GD32_LED_LAB`. Con un perfil oficial sin personalizar, compruebe
   también `test_ap`.
2. Conéctese aunque Windows indique que la red no tiene Internet.
3. Compruebe que la puerta de enlace sea `192.168.237.1`.
4. Ejecute **5. Abrir panel HTML local** desde VS Code.
5. El panel envía solicitudes a `http://192.168.237.1`; no depende de que el
   navegador renderice correctamente la página empotrada de `/`.
6. Pruebe Encender, Apagar, Alternar y los dos ritmos de parpadeo.

## 10. Ciclo después de modificar código

1. Guarde `VendorApp/main.c`.
2. Cópielo manualmente a `<SDK>\MSDK\app_http_led`.
3. Ejecute las tareas 2 y 3.
4. Entre al bootloader UART y ejecute la tarea 4.
5. Reinicie la placa, reconecte el PC al SoftAP y ejecute la tarea 5.

## 11. Evidencias recomendadas

- lista de tareas de VS Code;
- enlace exitoso de MBL y MSDK;
- SHA-256 y validación de la imagen;
- páginas verificadas por ISP CLI;
- conexión al SoftAP y puerta de enlace;
- panel local mostrando `/status`;
- respuesta física de PC13 a cada botón.

