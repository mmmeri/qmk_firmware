#include "shard_left.h"

enum stagsplit_layers {
    _QWERTY,
    _RAISE,
    _LWR,
    _GAME,
    _CHILL,

};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    NRF_PAIR,
};

#define KC_LNGL KC_LEFT_ANGLE_BRACKET
#define KC_RNGL KC_RIGHT_ANGLE_BRACKET
#define ____ KC_TRNS
#define _____ KC_TRNS
#define ______ KC_TRNS
#define _______ KC_TRNS


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = KEYMAP(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,      
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,      
        MO(_RAISE), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,               
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(_GAME), 
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_LBRC, KC_RBRC
    )
};
// clang-format on


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_ESC:
            if (record->event.pressed) {
                if (keyboard_report->mods & MOD_BIT(KC_LSFT) || keyboard_report->mods & MOD_BIT(KC_RSFT)) {
                    tap_code16(KC_TILDE);
                } else {
                    tap_code16(KC_ESC);
                }
            }
            return false;
    }
    return true;
}

