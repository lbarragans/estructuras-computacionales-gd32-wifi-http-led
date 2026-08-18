cmake_minimum_required(VERSION 3.15)

# Este archivo NO configura el SDK completo. Se copia junto con main.c y
# app_cfg.h como MSDK/app_http_led dentro del SDK oficial. El SDK ya creó
# TARGET_EXE y definió toolchain, linker, FreeRTOS, lwIP y bibliotecas Wi-Fi.
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

# Registrar la unidad de compilación de la aplicación en el ejecutable MSDK.
target_sources(${TARGET_EXE}
    PRIVATE
        main.c
)
