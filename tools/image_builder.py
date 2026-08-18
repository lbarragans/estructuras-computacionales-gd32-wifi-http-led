#!/usr/bin/env python3
"""Construye la imagen completa del GD32VW553 de forma determinista.

Este archivo se invoca desde la tarea 3 de VS Code. No compila firmware y no
modifica el SDK. Únicamente:

1. lee el MBL recién compilado;
2. lee la aplicación MSDK recién compilada;
3. coloca MBL en el offset 0;
4. rellena con 0xFF hasta 0xA000;
5. coloca MSDK en 0xA000;
6. vuelve a leer el resultado y lo compara byte a byte;
7. muestra tamaño y SHA-256 para la bitácora.

Se incluye porque el post-build V1.0.3g produjo imágenes antiguas/truncadas
cuando el SDK estaba en rutas de Windows con espacios. El algoritmo es pequeño,
auditable y no depende de PowerShell.
"""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path

MSDK_OFFSET = 0xA000


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Ensambla MBL + relleno + MSDK para GD32VW553"
    )
    parser.add_argument("--sdk", type=Path, required=True, help="Raíz del SDK")
    parser.add_argument("--output", type=Path, required=True, help="image-all.bin")
    return parser.parse_args()


def require_file(path: Path, label: str) -> bytes:
    if not path.is_file():
        raise SystemExit(f"[FALTA] {label}: {path}")
    data = path.read_bytes()
    if not data:
        raise SystemExit(f"[VACÍO] {label}: {path}")
    return data


def main() -> None:
    args = parse_arguments()
    sdk = args.sdk.resolve()
    output = args.output.resolve()

    mbl_path = sdk / "MBL" / "project" / "cmake" / "bin" / "mbl.bin"
    msdk_path = (
        sdk / "MSDK" / "projects" / "cmake" / "output" / "bin" / "msdk.bin"
    )

    mbl = require_file(mbl_path, "MBL")
    msdk = require_file(msdk_path, "MSDK")

    if len(mbl) > MSDK_OFFSET:
        raise SystemExit(
            f"[ERROR] MBL ocupa {len(mbl)} bytes y supera 0x{MSDK_OFFSET:X}."
        )

    # La Flash borrada vale 0xFF. El hueco no se rellena con ceros.
    image = bytearray(b"\xFF" * (MSDK_OFFSET + len(msdk)))
    image[0 : len(mbl)] = mbl
    image[MSDK_OFFSET : MSDK_OFFSET + len(msdk)] = msdk

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(image)

    # Verificación independiente de lo escrito.
    stored = output.read_bytes()
    if stored[: len(mbl)] != mbl:
        raise SystemExit("[ERROR] La región MBL no coincide.")
    if any(byte != 0xFF for byte in stored[len(mbl) : MSDK_OFFSET]):
        raise SystemExit("[ERROR] El hueco MBL–MSDK no contiene solo 0xFF.")
    if stored[MSDK_OFFSET:] != msdk:
        raise SystemExit("[ERROR] La región MSDK no coincide.")

    digest = hashlib.sha256(stored).hexdigest().upper()
    print("[OK] Imagen completa reconstruida y validada byte a byte.")
    print(f"MBL:       {mbl_path} ({len(mbl)} bytes)")
    print(f"MSDK:      {msdk_path} ({len(msdk)} bytes)")
    print(f"Salida:    {output} ({len(stored)} bytes)")
    print("Flash:     0x08000000")
    print("MSDK:      0x0800A000")
    print(f"SHA-256:   {digest}")


if __name__ == "__main__":
    main()
