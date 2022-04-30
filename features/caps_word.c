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
#include "caps_word.h"

#ifndef CAPS_WORD_IDLE_TIMEOUT
#    define CAPS_WORD_IDLE_TIMEOUT 5000
#endif

static uint16_t caps_word_idle_timer = 0;
static bool     caps_word_active     = false;

void caps_word_set(bool active) {
    if (active != caps_word_active) {
        if (active) {
            clear_mods();
            clear_oneshot_mods();
            caps_word_idle_timer = timer_read() + CAPS_WORD_IDLE_TIMEOUT;
        } else {
            unregister_weak_mods(MOD_BIT(KC_LSFT));
        }

        caps_word_active = active;
    }
}

bool process_caps_word(uint16_t keycode, keyrecord_t* record, uint16_t caps_word_keycode) {
    if (!record->event.pressed) return true;

    if (!caps_word_active && keycode == caps_word_keycode) {
        caps_word_set(true);
        return false;
    } else if (!caps_word_active) {
        return true;
    } else {
        caps_word_idle_timer = record->event.time + CAPS_WORD_IDLE_TIMEOUT;
    }

    if (!((get_mods() | get_oneshot_mods()) & ~MOD_MASK_SHIFT)) {
        switch (keycode) {
            case QK_MOMENTARY ... QK_MOMENTARY_MAX:
            case QK_TO ... QK_TO_MAX:
            case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
            case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
            case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
                return true;

            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
                if (record->tap.count == 0) caps_word_set(false);
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
                if (record->tap.count == 0) return true;
                keycode &= 0xff;
                break;
        }
    }

    clear_weak_mods();

    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));

        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            send_keyboard_report();
            return true;
    }

    caps_word_set(false);
    return true;
}

void scan_caps_word() {
    if (caps_word_active && timer_expired(timer_read(), caps_word_idle_timer)) {
        caps_word_set(false);
    }
}
