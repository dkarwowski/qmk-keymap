// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "flexible_delim.h"

enum delim_mode {
    INACTIVE = 0,
    WAITING,
    REPLACING,
};

struct delim_state {
    enum delim_mode mode;
    uint16_t        keycode;
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

    return true;
}
