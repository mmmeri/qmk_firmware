#ifndef KB_H
#define KB_H

#include "quantum.h"

#define L__ KC_NO
#define R__ KC_NO

#define KEYMAP( \
	L00, L01, L02, L03, L04, L05, L06,  \
	L10, L11, L12, L13, L14, L15, L16,  \
	L20, L21, L22, L23, L24, L25,       \
	L30, L31, L32, L33, L34, L35, L36,  \
	L40, L41, L42, L43, L44, L45, L46  \
) { \
	{ L00, L01, L02, L03, L04, L05, L06 }, \
	{ L10, L11, L12, L13, L14, L15, L16 }, \
	{ L20, L21, L22, L23, L24, L25, L__ }, \
	{ L30, L31, L32, L33, L34, L35, L36 }, \
	{ L40, L41, L42, L43, L44, L45, L46 }  \
}

#endif