// Copyright 2022 Google LLC.
// SPDX-License-Identifier: Apache-2.0
#include "flexible_delim.h"

enum delim_mode {
    INACTIVE = 0,
    WAITING,
    REPLACING,
};

struct delim_state {
    enum delim_mode mode;
    uint16_t        keycode;
    int16_t         last_delim;
};

#if !defined(FLEXIBLE_DELIM_DEFAULT)
#    define FLEXIBLE_DELIM_DEFAULT KC_UNDS
#endif

static struct delim_state current_state = {
    .mode    = INACTIVE,
    .keycode = KC_NO,
};

bool process_flexible_delim(uint16_t keycode, keyrecord_t* record, uint16_t set_delims_key) {
    if (!record->event.pressed) {
        return true;
    }

    if (keycode == set_delims_key) {
        switch (current_state.mode) {
            case INACTIVE:
                current_state.mode = WAITING;
                break;
            case WAITING:
            case REPLACING:
                current_state.mode = INACTIVE;
                break;
        }
        return false;
    }

    if (current_state.mode == INACTIVE) {
        return true;
    }

    switch (keycode) {
        case QK_MOMENTARY ... QK_MOMENTARY_MAX:
        case QK_TO ... QK_TO_MAX:
        case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
        case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
        case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
            // Ignore layer changes from being processed.
            return true;

        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            // If we're not tapping, exit out -- allow combo mods.
            if (record->tap.count == 0) {
                return true;
            }
            // Get the base keycode for a mod-tap if we're tapping.
            keycode &= 0xff;
            break;
    }

    if (current_state.mode == WAITING) {
        uint8_t mods          = get_mods() | get_oneshot_mods();
        current_state.keycode = keycode | ((mods & MOD_MASK_SHIFT) ? QK_LSFT : 0);
        current_state.mode    = REPLACING;
        return false;
    }

    if (current_state.mode != REPLACING) {
        return true;
    }

    switch (keycode) {
        case KC_SPACE:
            if (current_state.last_delim == 0) {
                tap_code16(KC_BSPC);
                tap_code16(KC_SPACE);
                current_state.mode    = INACTIVE;
                current_state.keycode = KC_NO;
            } else {
                current_state.last_delim = 0;
                tap_code16(current_state.keycode);
            }
            return false;
        case KC_BSPC:
            --current_state.last_delim;
            break;
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
            if (current_state.last_delim >= 0) {
                ++current_state.last_delim;
            }
            break;
        default:
            current_state.mode    = INACTIVE;
            current_state.keycode = KC_NO;
            break;
    }

    return true;
}
