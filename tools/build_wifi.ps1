[CmdletBinding()]
param([ValidateSet("original", "freertos")] [string]$Variant = "freertos", [ValidateRange(1,64)] [int]$Jobs = 12, [switch]$Clean, [switch]$Flash)
$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$ConfigFile = Join-Path $PSScriptRoot "local_config.ps1"
if (-not (Test-Path $ConfigFile)) { throw "Copie tools/local_config.example.ps1 como tools/local_config.ps1 y edite las rutas." }
. $ConfigFile
foreach ($Name in @("GD32_MSDK_ROOT", "NUCLEI_TOOLCHAIN_DIR", "OPENOCD_ROOT")) {
    $Value = Get-Variable -Name $Name -ValueOnly -ErrorAction SilentlyContinue
    if ([string]::IsNullOrWhiteSpace($Value)) { throw "$Name no esta configurada." }
}
$SourceFolder = if ($Variant -eq "original") { Join-Path $ProjectRoot "VendorApp" } else { Join-Path $ProjectRoot "FreeRTOS_Puro" }
$AppName = "app_http_led"
$SdkAppFolder = Join-Path $GD32_MSDK_ROOT "MSDK/$AppName"
$WifiBuildFolder = Join-Path $ProjectRoot "build/wifi-$Variant"
$ToolchainFile = Join-Path $GD32_MSDK_ROOT "scripts/cmake/toolchain.cmake"
$ImageBuilder = Join-Path $ProjectRoot "tools/image_builder.py"
$OpenOcdBin = Join-Path $OPENOCD_ROOT "bin"
foreach ($Path in @($ToolchainFile, $ImageBuilder, (Join-Path $SourceFolder "main.c"), (Join-Path $SourceFolder "app_cfg.h"), (Join-Path $SourceFolder "CMakeLists.txt"), (Join-Path $NUCLEI_TOOLCHAIN_DIR "riscv-nuclei-elf-gcc.exe"), (Join-Path $OpenOcdBin "openocd.exe"))) {
    if (-not (Test-Path $Path)) { throw "No se encontro: $Path" }
}
New-Item -ItemType Directory -Path $SdkAppFolder -Force | Out-Null
foreach ($FileName in @("main.c", "app_cfg.h", "CMakeLists.txt")) {
    $Destination = Join-Path $SdkAppFolder $FileName
    Copy-Item (Join-Path $SourceFolder $FileName) $Destination -Force
    (Get-Item $Destination).LastWriteTime = Get-Date
}
$env:PATH = "$NUCLEI_TOOLCHAIN_DIR;$OpenOcdBin;$env:PATH"
if ($Clean -and (Test-Path $WifiBuildFolder)) {
    & cmake -E remove_directory $WifiBuildFolder
    if ($LASTEXITCODE -ne 0) { throw "No se pudo limpiar $WifiBuildFolder" }
}

# Estos dos valores reproducen la configuración del repositorio que fue
# validado físicamente: FreeRTOS explícito y perfil BLE mínimo. El script del
# SDK usa BLE MAX por defecto, llena el 100% de la RAM y no corresponde a la
# compilación comprobada del laboratorio.
& cmake -S $GD32_MSDK_ROOT -B $WifiBuildFolder -G Ninja `
    "-DAPP=$AppName" `
    "-DCMAKE_BUILD_TYPE=Debug" `
    "-DCONFIG_OS=FREERTOS" `
    "-DCONFIG_BLE_FEATURE=MIN" `
    "-DCONFIG_MBEDTLS_VERSION=3.6.2" `
    "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile"
if ($LASTEXITCODE -ne 0) { throw "La configuración MSDK fallo." }

& cmake --build $WifiBuildFolder --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw "La compilación MSDK fallo." }

$PythonLauncher = Get-Command py.exe -ErrorAction SilentlyContinue
if ($null -ne $PythonLauncher) {
    & $PythonLauncher.Source -3 $ImageBuilder --sdk $GD32_MSDK_ROOT --output (Join-Path $GD32_MSDK_ROOT "scripts/images/image-all.bin")
} else {
    $Python = Get-Command python.exe -ErrorAction Stop
    & $Python.Source $ImageBuilder --sdk $GD32_MSDK_ROOT --output (Join-Path $GD32_MSDK_ROOT "scripts/images/image-all.bin")
}
if ($LASTEXITCODE -ne 0) { throw "La reconstrucción de image-all.bin fallo." }
$ImagePath = Join-Path $GD32_MSDK_ROOT "scripts/images/image-all.bin"
if (-not (Test-Path $ImagePath)) { throw "No se genero image-all.bin: $ImagePath" }
Write-Host "Imagen WiFi creada: $ImagePath"
if ($Flash) { & (Join-Path $PSScriptRoot "flash_wifi.ps1") }
