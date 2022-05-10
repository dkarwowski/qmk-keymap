// Copyright 2022 Google LLC.
// SPDX-License-Identifier: Apache-2.0
#include "key_repeat.h"

static bool     repeat_active = false;
static uint16_t previous_key  = KC_NO;

void handle_repeat(bool active) {
    if (repeat_active == active) {
        return;
    }

    repeat_active = active;
    if (repeat_active) {
        register_code16(previous_key);
    } else {
        unregister_code16(previous_key);
    }
}

bool process_key_repeat(uint16_t keycode, keyrecord_t* record, uint16_t repeat_key) {
    if (keycode == repeat_key) {
        handle_repeat(record->event.pressed);
        return false;
    }

    // Don't store anything about layer changes, those aren't "repeatable".
    switch (keycode) {
        case QK_DEF_LAYER ... QK_DEF_LAYER_MAX:
        case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
        case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
        case QK_MOMENTARY ... QK_MOMENTARY_MAX:
        case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
        case QK_TO ... QK_TO_MAX:
        case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
            return true;

        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            keycode &= 0xFF;
    }

    if (record->event.pressed) {
        previous_key = keycode;

        uint8_t mods = get_mods() | get_oneshot_mods();
        if (mods & MOD_MASK_CTRL) previous_key |= QK_LCTL;
        if (mods & MOD_MASK_SHIFT) previous_key |= QK_LSFT;
        if (mods & MOD_MASK_GUI) previous_key |= QK_LGUI;
        if (mods & MOD_BIT(KC_LALT)) previous_key |= QK_LALT;
        if (mods & MOD_BIT(KC_RALT)) previous_key |= QK_RALT;
    }

    return true;
}
