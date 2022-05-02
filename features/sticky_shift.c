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
#include "sticky_shift.h"

#if !defined(STICKY_SHIFT_TAPPING_TERM)
#    define STICKY_SHIFT_TAPPING_TERM TAPPING_TERM
#endif

#if !defined(STICKY_SHIFT_TIMEOUT)
#    define STICKY_SHIFT_TIMEOUT 5000
#endif

enum shift_mode {
    MOD_SHIFT = 0,
    ONE_SHOT,
    CAPS_WORD,
    CAPS_LOCK,
};

struct shift_state {
    enum shift_mode mode;
    uint16_t        idle_timer;

    bool     key_down;
    uint16_t tap_timer;
};

static struct shift_state current_state;

void press_sticky_shift(keyrecord_t* record) {
    current_state.key_down = record->event.pressed;

    if (timer_elapsed(current_state.tap_timer) > STICKY_SHIFT_TAPPING_TERM) {
        // If the key is pressed / release beyond our last tapping term, we
        // kill the state immediately.
        current_state.mode = MOD_SHIFT;
    }

    if (current_state.key_down) {
        current_state.tap_timer = record->event.time;
    } else if (timer_elapsed(current_state.tap_timer) < STICKY_SHIFT_TAPPING_TERM) {
        current_state.idle_timer = record->event.time;
        ++current_state.mode;
    }
}

bool process_sticky_shift(uint16_t keycode, keyrecord_t* record, uint16_t caps_word_keycode) {
    if (keycode == caps_word_keycode) {
        press_sticky_shift(record);
        return false;
    }

    switch (current_state.mode) {
        case MOD_SHIFT:
            if (!current_state.key_down) {
                return true;
            }
            // Fallthrough
        case CAPS_WORD:
        case CAPS_LOCK:
            break;
        case ONE_SHOT:
            current_state.mode = MOD_SHIFT;
    }

    if (timer_elapsed(current_state.idle_timer) > STICKY_SHIFT_TIMEOUT) {
        current_state.mode = MOD_SHIFT;
        return true;
    }

    // Key releases don't impact us much.
    if (!record->event.pressed) {
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

    // Remove the shift mod if it's populated.
    del_weak_mods(MOD_BIT(KC_LSFT));

    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));

        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case KC_MINS:
            send_keyboard_report();
    }

    return true;
}
