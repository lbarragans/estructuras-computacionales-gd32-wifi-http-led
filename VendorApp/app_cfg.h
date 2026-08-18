#ifndef APP_CFG_H
#define APP_CFG_H

#include "platform_def.h"

/*
 * Este laboratorio usa solo Wi-Fi. Deshabilitar BLE y coexistencia reduce las
 * funciones que el SDK debe incorporar y evita mezclar dos subsistemas RF
 * durante la primera validación.
 */
#undef CFG_BLE_SUPPORT
#undef CFG_COEX

/* Conserva mensajes de arranque y error en la salida de diagnóstico del SDK. */
#define CONFIG_DEBUG_PRINT_ENABLE

#endif
