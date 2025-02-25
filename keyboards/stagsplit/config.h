#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    MerikanKoyun
#define PRODUCT         stagsplit
#define DESCRIPTION     stagsplit is a staggered split keyboard


// Splitiness
#define MASTER_LEFT
#define SOFT_SERIAL_PIN D2
#define SPLIT_USB_DETECT

/* key matrix size */
#define MATRIX_ROWS 12 // 5+6
#define MATRIX_COLS 10

/* key matrix pins */
#define MATRIX_ROW_PINS { C6, D7, E6, B4, B5, B7 }
#define MATRIX_COL_PINS { F6, F7, B1, B3, B2, B6, D5, NO_PIN, NO_PIN, NO_PIN }
#define MATRIX_ROW_PINS_RIGHT { C6, B5, B4, E6, D7, NO_PIN }
#define MATRIX_COL_PINS_RIGHT { F6, F7, B1, B3, B2, B6, F0, F1, C7, D5 }
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* number of backlight levels */

#ifdef BACKLIGHT_PIN
#define BACKLIGHT_LEVELS 3
#endif

/* Set 0 if debouncing isn't needed */
#define DEBOUNCING_DELAY 5


// Encoder
#define ENCODERS_PAD_A { F4 }
#define ENCODERS_PAD_B { F5 }
#define ENCODER_RESOLUTION 2

#define OLED_DISPLAY_128X64

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* prevent stuck modifiers */
#define PREVENT_STUCK_MODIFIERS


#ifdef RGB_DI_PIN
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 0
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif

#endif