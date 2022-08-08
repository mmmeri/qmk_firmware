#include "kb.h"
#include "split_util.h"

enum stagsplit_layers {
    _QWERTY,
    _GAME,
    _GP,
    _UE4,
    _CHILL,

    _MAX_PERSISTENT_LAYERS,

    _RAISE,
    _LWR,

    _MAX_LAYERS
};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    NRF_PAIR,
};

#define KC_LNGL KC_LEFT_ANGLE_BRACKET
#define KC_RNGL KC_RIGHT_ANGLE_BRACKET
#define KC_MPP KC_MEDIA_PLAY_PAUSE
#define ____ KC_TRNS
#define _____ KC_TRNS
#define ______ KC_TRNS
#define _______ KC_TRNS


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                                         KC_7,  KC_8,  KC_9,     KC_0,    _______, KC_DEL,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,                                 KC_Y,   KC_U,  KC_I,  KC_O,     KC_P,    _______, _______,
        MO(_RAISE), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                                  KC_H,  KC_J,  KC_K,     KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(_GAME),                  KC_MUTE,  KC_B,   KC_N,  KC_M,  KC_COMM,  KC_DOT,  KC_SLSH, KC_UP,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_LBRC, KC_RBRC,           KC_MINS, KC_EQL,   KC_BSPC,  KC_ENTER,   MO(_LWR), KC_LEFT, KC_DOWN, KC_RIGHT
    ),

    [_GAME] = LAYOUT(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                               ____,  ____,  ____, ____, ____, ____,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,                       ____,   ____,  ____,  ____, ____, ____, ____,
        KC_KP_2,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                        ____,  ____,  ____, ____, ____, ____,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(_GP),           ____,  ____,  ____,  ____,  ____, ____, ____, ____,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_KP_0, KC_KP_1,       ____,  ____,   ____,    ____,   ____,    ____, ____, ____
    ),

    // [] = LAYOUT(
    //     ______,     ____,    ____,    ____,    ____,    ____,    ____,                               ____,  ____,  ____, ____, ____, ____,
    //     ______,     ____,    ____,    ____,    ____,    ____,    ____,                       ____,   ____,  ____,  ____, ____, ____, ____,
    //     ______,    ____,    ____,    ____,    ____,    ____,                                        ____,  ____,  ____, ____, ____, ____,
    //     ______,    ____,    ____,    ____,    ____,    ____,    TO(_CHILL),           ____,  ____,  ____,  ____,  ____, ____, ____, ____,
    //     ______,    ______, ______, ______,  ______,    ______, ______,       ____,  ____,   ____,    ____,   ____,    ____, ____, ____
    // ),

    [_GP] = LAYOUT(
        ______,     ____,    ____,    ____,    ____,    ____,    ____,                               ____,  ____,  ____, ____, ____, ____,
        ______,     ____,    ____,    ____,    ____,    ____,    ____,                       ____,   ____,  ____,  ____, ____, ____, ____,
        ______,     ____,    ____,    ____,    ____,    ____,                                        ____,  ____,  ____, ____, ____, ____,
        ______,    ____,    ____,    ____,    ____,    ____,    TO(_UE4),           ____,  ____,  ____,  ____,  ____, ____, ____, ____,
        ______,    ______, ______, ______,  ______,    ______, KC_INS,       ____,  ____,   ____,    ____,   ____,    ____, ____, ____
    ),


    [_UE4] = LAYOUT(
        ______,     ____,    ____,    ____,    ____,    ____,    ____,                               ____,  ____,  ____, ____, ____, ____,
        ______,     ____,    ____,    ____,    ____,    ____,    ____,                       ____,   ____,  ____,  ____, ____, ____, ____,
        ______,    ____,    ____,    ____,    ____,    ____,                                        ____,  ____,  ____, ____, ____, ____,
        ______,    ____,    ____,    ____,    ____,    ____,    TO(_CHILL),           ____,  ____,  ____,  ____,  ____, ____, ____, ____,
        ______,    ______, ______, ______,  ______,    ______, ______,       ____,  ____,   ____,    ____,   ____,    ____, ____, ____
    ),

    [_CHILL] = LAYOUT(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                                         ____,  ____,  ____,     ____,    _______, KC_DEL,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,                                 ____,   ____,  ____,  ____,     ____,    _______, _______,
        MO(_RAISE), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                                  ____,  ____,  ____,     ____,    KC_SCLN, KC_QUOT,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(_QWERTY),                  KC_MUTE,  ____,   ____,  ____,  _____,  _____,  KC_UP, _______,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_LBRC, KC_RBRC,           KC_MINS, KC_EQL,   KC_BSPC,  KC_ENTER,   _____, KC_LEFT, KC_DOWN, KC_RIGHT
    ),

    [_RAISE] = LAYOUT(
        KC_GRV,     KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                                KC_F7, KC_F8, KC_F9,   KC_F10,    KC_F11,  KC_F12,
        KC_TAB,     ____,    KC_UP,   ____,    ____,    ____,    ____,                        ____, KC_PGUP,  KC_UP, KC_PGDN, KC_PAUS,   KC_TRNS, KC_TRNS,
        KC_TRNS,    KC_LEFT, KC_DOWN, KC_RIGHT,____,    ____,                                       KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT, KC_SCLN, KC_QUOT,
        _______,    ____,    ____,   MEH(KC_C),____,    ____,    ____,                     KC_MPP,  ____,   KC_END,  ____,  ____,  ____,  ____,   KC_PSCR,
        _______,    _______, _______, KC_BSPC, KC_ENT,  KC_PIPE, KC_BSLS,       KC_LPRN, KC_RPRN,  KC_BSPC, KC_ENTER, KC_APP, KC_MENU, KC_DOWN, KC_RIGHT
    ),
	
    [_LWR] = LAYOUT(
        KC_GRV,     KC_KP_1,   KC_KP_2,     KC_KP_3,     KC_KP_4,        KC_KP_5, KC_KP_6,                   KC_KP_7, KC_KP_8, KC_KP_9,KC_KP_0, ____, ____,
        KC_TAB,     KC_KP_7,   KC_KP_8,     KC_KP_9,     KC_KP_ASTERISK, ____,    ____,                       ____,   ____,  ____,  ____, ____, ____, ____,
        KC_TRNS,    KC_KP_4,   KC_KP_5,     KC_KP_6,     KC_KP_MINUS,    ____,                                        ____,  ____,  ____, ____, ____, ____,
        KC_LSFT,    KC_KP_1,   KC_KP_2,     KC_KP_3,     KC_KP_PLUS,     ____,    ____,                 ____,  ____,  ____,  ____,  ____, ____, ____, ____,
        KC_LCTL,    KC_KP_0,   KC_KP_SLASH, KC_KP_COMMA, KC_KP_ENTER,    KC_PIPE, KC_BSLS,       ____,  ____,   ____,    ____,   ____,    ____, ____, ____
    ),

};
// clang-format on

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    keyevent_t event = record->event;
    (void)event;

    switch (id) {}
    return MACRO_NONE;
}

void matrix_init_user(void) {
#ifdef NRF24_ENABLE
    wait_ms(500);  // give time for usb to initialize
    set_output(OUTPUT_NRF24);
#endif
}

void matrix_scan_user(void) {}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_left()) {
        return OLED_ROTATION_0;
    }
    return rotation;
}

const char *get_layer_text(uint8_t layer)
{
    switch (layer) {
        case _QWERTY:
            return (PSTR("CODING\n"));
            break;
        case _LWR:
            return (PSTR("LOWER\n"));
            break;
        case _RAISE:
            return (PSTR("RAISE\n"));
            break;
        case _GAME:
            return (PSTR("GAMING\n"));
            break;
        case _UE4:
            return (PSTR("UE4\n"));
            break;
        case _GP:
            return (PSTR("GUITAR PRO\n"));
            break;
        default:
            return (PSTR("UNDEFINED\n"));
    }

    return "";
}

static void render_status(void) {
    int prev_layer = get_highest_layer(layer_state) - 1;
    if (prev_layer < 0) {
        prev_layer = _MAX_PERSISTENT_LAYERS - 1;
    }
    int next_layer = (get_highest_layer(layer_state) + 1) % _MAX_PERSISTENT_LAYERS;

    oled_write_P("\n", false);
    oled_write_P(get_layer_text(prev_layer), false);
    oled_write_P("^\n", false);
    oled_write_P(">>> ", false);
    oled_write_P(get_layer_text(get_highest_layer(layer_state)), false);
    oled_write_P("V\n", false);
    oled_write_P(get_layer_text(next_layer), false);
}

void oled_task_user(void) {
    // if (is_keyboard_left()) {
        render_status();
    // }
}


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
#ifdef NRF24_ENABLE
        case NRF_PAIR:
            if (record->event.pressed && !nrf24_is_paired()) {
                nrf24_pair();
            }
            break;
#endif
    }
    return true;
}


#ifdef ENCODER_ENABLE

#define TUPLE_CPY_2(A, B) memcpy(keys, (const uint16_t[]){(A), (B)}, 2 * sizeof(uint16_t));

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 1) {
        uint16_t keys[2];
        int clockwise_index = clockwise ? 1 : 0;

        switch (get_highest_layer(layer_state))
        {
            case _GP:
                if (keyboard_report->mods & MOD_BIT(KC_LSFT) || keyboard_report->mods & MOD_BIT(KC_RSFT)) {
                    TUPLE_CPY_2(
                        LALT(LSFT(KC_DOWN)),
                        LALT(LSFT(KC_UP))
                    );
                }
                else {
                    TUPLE_CPY_2(
                        KC_MINUS,
                        KC_PLUS
                    );
                }
                break;
            case _RAISE:
                TUPLE_CPY_2(KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK);
                break;
            default:
                TUPLE_CPY_2(KC_VOLD, KC_VOLU);
                break;
        }

        tap_code16(keys[clockwise_index]);
    } else if (index == 0) {
        if (clockwise) {
            tap_code16(LCTL(KC_Y));
        } else {
            tap_code16(LCTL(KC_Z));
        }
    }

    return true;
}

#endif
