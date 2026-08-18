# 06 — Compilación, enlace e imagen completa

## 1. Compilar desde VS Code

Después de la tarea 1:

1. abra la paleta y elija **Tasks: Run Build Task**;
2. seleccione **2. Compilar MBL y MSDK**;
3. espere el enlace de ambos proyectos;
4. trate los `warning` del SDK por separado; un `error` o enlace fallido sí
   detiene el procedimiento.

## 2. Productos esperados

```text
SDK/MBL/project/cmake/bin/mbl.bin
SDK/MSDK/projects/cmake/output/bin/msdk.bin
SDK/MSDK/projects/cmake/output/bin/MSDK.elf
```

Revise desde el Explorador que `mbl.bin` y `msdk.bin` tengan fecha posterior
al inicio del build y tamaño mayor que cero.

## 3. MBL y MSDK

MBL es el cargador que arranca primero. MSDK es la aplicación principal y sus
pilas inalámbricas. La partición reservada hace que MSDK empiece en offset
`0xA000`, equivalente a la dirección `0x0800A000`.

## 4. Por qué no confiar ciegamente en el post-build

Con el SDK V1.0.3g bajo OneDrive y rutas con espacios se observaron mensajes de
ruta inválida, `mbl_len = 0` y copia de cero archivos. Permaneció una
`image-all.bin` antigua de 170392 bytes, mientras el `msdk.bin` recién
compilado medía más de 580 KB.

La tarea **3. Construir y validar image-all.bin** elimina la ambigüedad.

## 5. Algoritmo de la tarea 3

```text
leer mbl.bin
leer msdk.bin
comprobar tamaño MBL <= 0xA000
crear buffer de 0xA000 + tamaño(MSDK), lleno de 0xFF
copiar MBL al offset 0
copiar MSDK al offset 0xA000
guardar build/debug/artifacts/image-all.bin
volver a leer
comparar MBL, relleno y MSDK
calcular SHA-256
```

## 6. Interpretar el tamaño

`tamaño(image-all) = 0xA000 + tamaño(msdk.bin)`.

Si MSDK mide 583792 bytes, la imagen debe medir 624752 bytes. Un tamaño cercano
al MBL o menor que MSDK es imposible para una imagen completa.

## 7. ELF, BIN y HEX

- ELF: secciones, símbolos y depuración.
- BIN: bytes contiguos sin dirección incluida; el programador necesita dirección.
- HEX: registros de texto con direcciones.

Aquí se programa BIN, por eso `0x08000000` es un dato crítico.

## 8. Informe de RAM al 100 %

El linker del SDK puede mostrar una región RAM completamente asignada por su
diseño. Esto no prueba por sí solo que el heap se agote en ejecución. Evalúe el
mapa y las reservas del SDK antes de concluir un desbordamiento.

## Criterio de aceptación

- build finalizó;
- ambos binarios son recientes;
- tarea 3 dice `[OK]`;
- dirección mostrada es `0x08000000`;
- MSDK mostrado es `0x0800A000`;
- SHA‑256 quedó registrado.
