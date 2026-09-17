cmake_minimum_required(VERSION 3.20)
project(GD32VW553_WiFi_HTTP_LED_Assembly LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET_NAME GD32VW55x)
set(GD32_SDK_ROOT "" CACHE PATH "Raiz del SDK bare-metal GD32VW55x")
if(NOT GD32_SDK_ROOT AND DEFINED ENV{GD32_SDK_ROOT})
    file(TO_CMAKE_PATH "$ENV{GD32_SDK_ROOT}" GD32_SDK_ROOT)
endif()
if(NOT GD32_SDK_ROOT)
    message(FATAL_ERROR "Configure GD32_SDK_ROOT en tools/local_config.ps1")
endif()

get_filename_component(GD32_SDK_ROOT "${GD32_SDK_ROOT}" ABSOLUTE)
set(FIRMWARE_DIR "${GD32_SDK_ROOT}/Firmware")
set(UTILITIES_DIR "${GD32_SDK_ROOT}/Utilities")
set(PERIPHERAL_DIR "${FIRMWARE_DIR}/GD32VW55x_standard_peripheral")
set(RISCV_DIR "${FIRMWARE_DIR}/RISCV")
set(LINKER_SCRIPT "${RISCV_DIR}/env_Eclipse/GD32VW553xM.lds")
foreach(path "${PERIPHERAL_DIR}/system_gd32vw55x.c" "${UTILITIES_DIR}/gd32vw553h_eval.c" "${LINKER_SCRIPT}")
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "No se encontro: ${path}")
    endif()
endforeach()

set(APP_VARIANT "assembly" CACHE STRING "Esta ruta bare-metal solo admite assembly")
if(NOT APP_VARIANT STREQUAL "assembly")
    message(FATAL_ERROR "El ejercicio 12 usa APP_VARIANT=assembly; Original y FreeRTOS se construyen con el MSDK")
endif()

file(GLOB PERIPHERAL_SOURCES CONFIGURE_DEPENDS "${PERIPHERAL_DIR}/Source/*.c")
file(GLOB RISCV_ENV_SOURCES CONFIGURE_DEPENDS "${RISCV_DIR}/env_Eclipse/*.c" "${RISCV_DIR}/env_Eclipse/*.S" "${RISCV_DIR}/env_Eclipse/*.s")
file(GLOB RISCV_STUB_SOURCES CONFIGURE_DEPENDS "${RISCV_DIR}/stubs/*.c" "${RISCV_DIR}/stubs/*.S" "${RISCV_DIR}/stubs/*.s")

add_executable(${TARGET_NAME}
    Ensamblador_RISCV_Puro/main.S
    "${UTILITIES_DIR}/gd32vw553h_eval.c"
    "${PERIPHERAL_DIR}/system_gd32vw55x.c"
    ${PERIPHERAL_SOURCES} ${RISCV_ENV_SOURCES} ${RISCV_STUB_SOURCES})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME GD32VW55x SUFFIX .elf)
target_include_directories(${TARGET_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
    "${UTILITIES_DIR}"
    "${PERIPHERAL_DIR}"
    "${PERIPHERAL_DIR}/Include"
    "${RISCV_DIR}/drivers"
    "${RISCV_DIR}/env_Eclipse"
    "${RISCV_DIR}/stubs")
set(ARCH_FLAGS -march=rv32imafdc -mabi=ilp32d -mcmodel=medany -msmall-data-limit=8 -mdiv)
target_compile_options(${TARGET_NAME} PRIVATE ${ARCH_FLAGS} -ffunction-sections -fdata-sections -g3 $<$<CONFIG:Debug>:-Og> $<$<CONFIG:Release>:-Os>)
target_link_options(${TARGET_NAME} PRIVATE ${ARCH_FLAGS} -nostartfiles "-T${LINKER_SCRIPT}" -Wl,--gc-sections "-Wl,-Map,${CMAKE_CURRENT_BINARY_DIR}/GD32VW55x.map" --specs=nano.specs --specs=nosys.specs)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O ihex "$<TARGET_FILE:${TARGET_NAME}>" "${CMAKE_CURRENT_BINARY_DIR}/GD32VW55x.hex"
    COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${TARGET_NAME}>" "${CMAKE_CURRENT_BINARY_DIR}/GD32VW55x.bin"
    COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${TARGET_NAME}>"
    COMMAND "${CMAKE_COMMAND}" -DOBJDUMP=${CMAKE_OBJDUMP} -DELF=$<TARGET_FILE:${TARGET_NAME}> -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/GD32VW55x.lst -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_listing.cmake"
    VERBATIM)
