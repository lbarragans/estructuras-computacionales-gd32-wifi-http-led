# 03 — Instalación manual en Windows

## 1. Crear carpetas

Desde el Explorador de archivos cree `C:\GD32`. Extraiga allí el SDK. No lo
extraiga dentro del repositorio.

Confirme visualmente que la carpeta contiene directamente `MBL`, `MSDK` y
`scripts`. Si existe otra carpeta del mismo nombre en medio, corrija la ruta.

## 2. Instalar la cadena de Embedded Builder

1. Ejecute el instalador oficial.
2. Conserve la opción de herramientas RISC‑V/Nuclei.
3. Finalice la instalación.
4. Localice la carpeta `NucleiRISCVGCC/bin`.
5. Copie su ruta: VS Code la solicitará al configurar.

No agregue rutas al azar. Si la interfaz permite explorar, seleccione la carpeta
`bin` real de la cadena Nuclei.

## 3. Extraer GD32 ISP CLI

1. Extraiga el paquete en `C:\GD32\GD32_ISP_CLI`.
2. Confirme que están el EXE, `OptionBytes` y la documentación.
3. Localice la definición XML del GD32VW553.
4. Cierre cualquier monitor serial que esté usando el mismo COM.

## 4. Verificar CH340

1. Conecte solo el USB‑C de la placa.
2. Abra Administrador de dispositivos.
3. Expanda “Puertos (COM y LPT)”.
4. Debe aparecer `USB-SERIAL CH340 (COMx)`.
5. Escriba el número en su bitácora.

Si únicamente ve puertos Bluetooth, Windows no detectó la placa. Cambie cable,
puerto USB o instale el driver.

## 5. Preparar VS Code

1. Abra VS Code normal, no Insiders.
2. Elija **File > Open Folder**.
3. Seleccione la raíz de este repositorio.
4. Confíe en la carpeta si reconoce su origen.
5. Instale las extensiones recomendadas.

Los subrayados rojos iniciales pueden deberse a que IntelliSense aún no conoce
los encabezados del SDK. No equivalen necesariamente a un error real del
compilador.

## 6. Evitar conflictos

- Cierre sesiones OpenOCD antes de UART.
- No conecte simultáneamente dos programas al mismo COM.
- Desactive temporalmente sincronización del SDK si está en OneDrive.
- No use una `image-all.bin` de otra carpeta por conveniencia.
- Ordene el Explorador por fecha para comprobar qué archivo es reciente.

## 7. Registro de instalación

Documente:

| Campo | Valor |
|---|---|
| versión SDK | |
| ruta SDK | |
| versión Embedded Builder | |
| versión GD32 ISP CLI | |
| versión CH340/driver | |
| COM de la placa | |
| referencia MCU | GD32VW553HMQ6 |

### Ejemplo del entorno en que se validó

Las rutas no deben copiarse ciegamente, pero ayudan a reconocer el nivel correcto:

```text
SDK:
C:/GD32/GD32VW55x_RELEASE_V1.0.3g

Toolchain:
.../GD32EmbeddedBuilder/plugins/com.gd.tools_.../Tools/NucleiRISCVGCC/bin

ISP CLI:
C:/GD32/GD32_ISP_CLI/GD32_ISP_CLI.exe

COM:
n (el número asignado a USB-SERIAL CH340)
```

Para una instalación nueva se recomienda `C:/GD32`, porque evita el problema de
espacios/OneDrive detectado durante el post-build.

## 8. Criterio de salida

La instalación está lista cuando:

- el SDK tiene estructura completa;
- existe la carpeta `NucleiRISCVGCC/bin`;
- el ISP CLI conserva la definición del VW553;
- el CH340 aparece como COM;
- VS Code abre el código.

Todavía no es necesario que la red aparezca: eso pertenece a build, programación
y ejecución.
