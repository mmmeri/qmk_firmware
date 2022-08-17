#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    MerikanKoyun
#define PRODUCT         SHARD


// Splitiness
#define MASTER_RIGHT
#define SOFT_SERIAL_PIN D2
#define SPLIT_USB_DETECT
#define SPLIT_LAYER_STATE_ENABLE

/* key matrix size */
#define MATRIX_ROWS 10
#define MATRIX_COLS 7

/* key matrix pins */
#define MATRIX_ROW_PINS { B5, B4, E6, D7, C6 }
#define MATRIX_COL_PINS { B6, B2, B3, B1, F7, F6, F5 }
#define MATRIX_ROW_PINS_RIGHT { B5, B4, E6, D7, C6 }
#define MATRIX_COL_PINS_RIGHT { F5, F6, F7, B1, B3, B2, B6 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* number of backlight levels */

#ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 3
#endif

// Encoder
#define ENCODERS_PAD_A { D4 }
#define ENCODERS_PAD_B { F4 }
#define ENCODER_RESOLUTION 2

//#define OLED_DISPLAY_128X64
#define OLED_DISPLAY_128X32

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

#endif