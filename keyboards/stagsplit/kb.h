#ifndef KB_H
#define KB_H

#include "quantum.h"

#define L__ KC_NO
#define R__ KC_NO

#define KEYMAP( \
	L00, L01, L02, L03, L04, L05, L06,              R01, R02, R03, R04, R05, R06, R07, R08, R09,   \
	L10, L11, L12, L13, L14, L15, L16,         R10, R11, R12, R13, R14, R15, R16, R17, R18, R19,   \
	L20, L21, L22, L23, L24, L25, L26,         R20, R21, R22, R23, R24, R25, R26, R27, R28, R29,   \
	L30, L31, L32, L33, L34, L35, L36,         R30, R31, R32, R33, R34, R35, R36, R37, R38, R39,   \
	L40, L41, L42, L43, L44, L45, L46,         R40, R41, R42, R43, R44, R45, R46, R47, R48, R49,   \
	                                  L56 \
) { \
	{ L00, L01, L02, L03, L04, L05, L06, KC_NO, KC_NO, KC_NO, }, \
	{ L10, L11, L12, L13, L14, L15, L16, KC_NO, KC_NO, KC_NO, }, \
	{ L20, L21, L22, L23, L24, L25, L26, KC_NO, KC_NO, KC_NO, }, \
	{ L30, L31, L32, L33, L34, L35, L36, KC_NO, KC_NO, KC_NO, }, \
	{ L40, L41, L42, L43, L44, L45, L46, KC_NO, KC_NO, KC_NO, }, \
	{ L__, L__, L__, L__, L__, L__, L56, KC_NO, KC_NO, KC_NO, }, \
	{ R__, R01, R02, R03, R04, R05, R06, R07,   R08,   R09,   }, \
	{ R10, R11, R12, R13, R14, R15, R16, R17,   R18,   R19,   }, \
	{ R20, R21, R22, R23, R24, R25, R26, R27,   R28,   R29,   }, \
	{ R30, R31, R32, R33, R34, R35, R36, R37,   R38,   R39,   }, \
	{ R40, R41, R42, R43, R44, R45, R46, R47,   R48,   R49,   }  \
}

#endif