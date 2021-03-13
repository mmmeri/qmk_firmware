#include "kb.h"
#include "outputselect.h"
#include "nrf24.h"
#include "logos.h"

enum stagsplit_layers {
    _QWERTY,
    _RAISE,
    _ALT_RAISE,
    _GAME,
    _CHILL,

};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    NRF_PAIR,
};

#define KC_LNGL KC_LEFT_ANGLE_BRACKET
#define KC_RNGL KC_RIGHT_ANGLE_BRACKET

static bool numlock = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = KEYMAP(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                     KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_TRNS,  KC_TRNS,          KC_TRNS,   KC_DEL,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,                  KC_T,  KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,  KC_TRNS, KC_TRNS,      KC_BSLS,  KC_PAUS,
        MO(_RAISE), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,          KC_MUTE, KC_G,  KC_H,  KC_J,  KC_K,  KC_L,  KC_SCLN, KC_QUOT,             KC_ENT,  KC_HOME,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_7,                 KC_V,  KC_B,  KC_N,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,    KC_UP,   KC_END,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_LBRC, KC_RBRC,    KC_MINS, KC_EQL, KC_BSPC, KC_ENTER, MO(_ALT_RAISE), MO(_ALT_RAISE), MO(_ALT_RAISE), KC_LEFT,    KC_DOWN, KC_RIGHT,
                                                                     TO(_GAME)
    ),

    [_RAISE] = KEYMAP(
        KC_GRV,     KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                    KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,          KC_BSPC,   KC_DEL,
        KC_TAB,     KC_Q,    KC_UP,    KC_LPRN, KC_RPRN,    KC_T,    KC_Y,            KC_T,  KC_Y,  KC_U,  KC_UP,  KC_O,  KC_P,  KC_LBRC, KC_RBRC,      KC_BSLS,  KC_PAUS,
        KC_TRNS,    KC_LEFT, KC_DOWN,  KC_LBRC, KC_RBRC,    KC_G,    KC_H,         NRF_PAIR, KC_G,  KC_HOME,KC_LEFT,KC_DOWN,KC_RGHT,  KC_SCLN, KC_QUOT, KC_ENT,  KC_HOME,
        KC_LSFT,MEH_T(KC_Z),MEH_T(KC_X),KC_LCBR,KC_RCBR,  KC_B,KC_7,                 KC_V,  KC_B,  KC_END,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,    KC_UP,   KC_END,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_BSPC,  KC_ENT,  KC_BSLS, KC_PIPE,    KC_LPRN, KC_RPRN, KC_BSPC, KC_ENTER, KC_APP, KC_MENU, KC_TRNS, KC_LEFT,    KC_DOWN, KC_RIGHT,
                                                                     KC_TRNS
    ),

    [_ALT_RAISE] = KEYMAP(
        KC_GRV,     KC_KP_1, KC_KP_2,     KC_KP_3,      KC_KP_4,        KC_KP_5, KC_KP_6,                KC_KP_6, KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_0, KC_TRNS, KC_TRNS,  KC_BSPC,   KC_DEL,
        KC_TAB,     KC_KP_7, KC_KP_8,     KC_KP_9,      KC_KP_ASTERISK, KC_T,    KC_Y,            KC_T,  KC_Y,  KC_U,  KC_UP,  KC_O,  KC_P,  KC_LBRC, KC_RBRC,      KC_BSLS,  KC_PAUS,
        KC_TRNS,    KC_KP_4, KC_KP_5,     KC_KP_6,      KC_KP_MINUS,    KC_G,    KC_H,         NRF_PAIR, KC_G,  KC_HOME,KC_LEFT,KC_DOWN,KC_RGHT,  KC_SCLN, KC_QUOT, KC_ENT,  KC_HOME,
        KC_LSFT,    KC_KP_1, KC_KP_2,     KC_KP_3,      KC_KP_PLUS,     KC_B,KC_7,                 KC_V,  KC_B,  KC_END,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,    KC_UP,   KC_END,
        KC_LCTL,    KC_KP_0, KC_KP_SLASH, KC_KP_COMMA,  KC_KP_ENTER,    KC_BSLS, KC_PIPE,    KC_LPRN, KC_RPRN, KC_BSPC, KC_ENTER, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT,    KC_DOWN, KC_RIGHT,
                                                                     KC_TRNS
    ),

    [_GAME] = KEYMAP(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                     KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_MINS,  KC_EQL,          KC_BSPC,   KC_DEL,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,                  KC_T,  KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,  KC_LBRC, KC_RBRC,      KC_BSLS,  KC_PAUS,
        KC_KP_2,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,          KC_MUTE, KC_G,  KC_H,  KC_J,  KC_K,  KC_L,  KC_SCLN, KC_QUOT,             KC_ENT,  KC_HOME,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_7,                 KC_V,  KC_B,  KC_N,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,    KC_UP,   KC_END,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_KP_0, KC_KP_1,    KC_MINS, KC_EQL, KC_BSPC, KC_ENTER, KC_APP, KC_MENU, KC_RCTRL, KC_LEFT,    KC_DOWN, KC_RIGHT,
                                                                     TO(_CHILL)
    ),

    [_CHILL] = KEYMAP(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                     KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_TRNS,  KC_TRNS,          KC_TRNS,   KC_DEL,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,                  KC_T,  KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,  KC_TRNS, KC_TRNS,      KC_BSLS,  KC_PAUS,
        MO(_RAISE), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,          KC_MUTE, KC_G,  KC_H,  KC_J,  KC_K,  KC_L,  KC_SCLN, KC_QUOT,             KC_ENT,  KC_HOME,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_7,                 KC_V,  KC_B,  KC_N,  KC_M,  KC_COMM, KC_UP,  KC_SLSH,  KC_RSFT,    KC_UP,   KC_END,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_DEL,  KC_SPC,  KC_LBRC, KC_RBRC,    KC_MINS, KC_EQL, KC_BSPC, KC_ENTER, KC_LEFT, KC_DOWN, KC_RIGHT, KC_LEFT,    KC_DOWN, KC_RIGHT,
                                                                     TO(_QWERTY)
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

void led_set_user(uint8_t usb_led) {
    if (usb_led & (1 << USB_LED_NUM_LOCK)) {
        numlock = true;
    } else {
        register_code(KC_NUMLOCK);
        unregister_code(KC_NUMLOCK);
        numlock = false;
    }

    if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_SCROLL_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_COMPOSE)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_KANA)) {
    } else {
    }
}

static void render_status(void) {
// QMK Logo and version information
// valorant_logo();

// oled_write_P(PSTR("\n\n"), false);
// oled_write_P(PSTR("                                                            STGSPLT\n"), false);
#ifdef NRF24_ENABLE
    if (nrf24_is_paired()) {
        oled_write_P(PSTR("Paired"), false);
    } else {
        oled_write_P(PSTR("Not paired..."), false);
    }
#else

    // // Host Keyboard Layer Status
    // // oled_write_P(PSTR("Layer: "), false);
    // oled_write_P(PSTR("                                                            "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            if (0) coding_logo();
            oled_write_P(PSTR("CDNG\n"), false);
            break;
        case _ALT_RAISE:
            oled_write_P(PSTR("ALT RAISE\n"), false);
            break;
        case _RAISE:
            if (0) raise_logo();
            oled_write_P(PSTR("RAISE\n"), false);
            break;
        case _GAME:
            if (0) valorant_logo();
            oled_write_P(PSTR("GAMING\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }
#endif

    // if (numlock) {
        // oled_write_P(PSTR("numlock"), false);
    // }
    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_180;
    }
    return rotation;
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();
    }
}

#ifdef ENCODER_ENABLE

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 1) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 0) {
        if (clockwise) {
            tap_code16(LCTL(KC_Y));
        } else {
            tap_code16(LCTL(KC_Z));
        }
    }
}

#endif
