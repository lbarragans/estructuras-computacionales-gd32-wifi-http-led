[CmdletBinding()]
param([ValidateSet("Debug", "Release")] [string]$BuildType = "Debug", [switch]$Clean, [switch]$Flash)
$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot

# Los ZIP creados en UTC pueden quedar varias horas por delante del reloj local
# de Windows. Ninja interpreta entonces que CMakeLists.txt cambia sin parar y
# entra en un ciclo de reconfiguracion. Normalizar solo los archivos de entrada
# de CMake antes de borrar/configurar evita ese problema sin tocar build/.
$TimestampInputs = @(
    (Join-Path $ProjectRoot "CMakeLists.txt"),
    (Join-Path $ProjectRoot "CMakePresets.json"),
    (Join-Path $ProjectRoot "Ensamblador_RISCV_Puro/main.S"),
    (Join-Path $ProjectRoot "Inc/gd32vw55x_libopt.h"),
    (Join-Path $ProjectRoot "cmake/toolchain-riscv.cmake"),
    (Join-Path $ProjectRoot "cmake/generate_listing.cmake")
)
foreach ($InputFile in $TimestampInputs) {
    if (Test-Path $InputFile) { (Get-Item $InputFile).LastWriteTime = Get-Date }
}

if ($Clean) {
    $BuildFolder = Join-Path $ProjectRoot ("build/" + $BuildType.ToLowerInvariant())
    if (Test-Path $BuildFolder) { Remove-Item $BuildFolder -Recurse -Force }
}
& (Join-Path $PSScriptRoot "configure.ps1") -BuildType $BuildType -Variant assembly
if ($LASTEXITCODE -ne 0) { throw "La configuracion Assembly fallo." }
$Preset = if ($BuildType -eq "Release") { "build-release" } else { "build-debug" }
Push-Location $ProjectRoot
try {
    & cmake --build --preset $Preset
    if ($LASTEXITCODE -ne 0) { throw "La compilacion Assembly fallo." }
} finally { Pop-Location }
if ($Flash) { & (Join-Path $PSScriptRoot "flash.ps1") -BuildType $BuildType }
