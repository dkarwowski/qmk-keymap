// Copyright 2022 Google LLC.
// SPDX-License-Identifier: Apache-2.0
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
    bool            dirty;
    bool            was_in_mode;
    bool            key_down;
    uint16_t        tap_timer;
};

static struct shift_state state;

void press_sticky_shift(keyrecord_t* record) {
    state.key_down = record->event.pressed;

    bool within_tap_window = timer_elapsed(state.tap_timer) < STICKY_SHIFT_TAPPING_TERM;

    if (state.key_down) {
        if (!within_tap_window || state.mode == CAPS_LOCK || state.dirty) {
            state.was_in_mode = state.mode != MOD_SHIFT;
            state.mode        = MOD_SHIFT;
        }

        if (within_tap_window && state.mode != MOD_SHIFT) {
            ++state.mode;
        } else {
            register_code(KC_LSFT);
        }

        state.idle_timer = record->event.time;
        state.tap_timer  = record->event.time;
        state.dirty      = false;
    } else {
        unregister_code(KC_LSFT);
        if (within_tap_window && state.mode == MOD_SHIFT && !state.was_in_mode && !state.dirty) {
            add_oneshot_mods(MOD_BIT(KC_LSFT));
            state.mode = ONE_SHOT;
        }
        state.was_in_mode = false;
    }
}

// Define shifted characters for 'locked' caps. Provide a true/false value for
// whether or not CAPS_WORD gets ended by the character.
bool handle_caps_locking(uint16_t keycode) {
    clear_weak_mods();

    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));

        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case KC_MINS:
            return true;

        default:
            return false;
    }
}

bool process_sticky_shift(uint16_t keycode, keyrecord_t* record, uint16_t caps_word_keycode) {
    if (keycode == caps_word_keycode) {
        press_sticky_shift(record);
        return false;
    }

    // Ignore key releases for stuff that isn't our keycode.
    if (!record->event.pressed) {
        return true;
    }

    // If we've escaped the idle timer, drop the shift info and bail.
    if (timer_elapsed(state.idle_timer) > STICKY_SHIFT_TIMEOUT) {
        state.mode = MOD_SHIFT;
        del_weak_mods(MOD_BIT(KC_LSFT));
        if (state.mode == ONE_SHOT) {
            del_oneshot_mods(MOD_BIT(KC_LSFT));
        }
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

    // Mark dirty whenever we get a keycode, so that we can skip oneshot mode
    // if it's going to otherwise get activated.
    state.dirty = true;

    // If we haven't escaped the idle timer, we leave the keycode for regular
    // processing since we should still have KC_LSFT registered if we're
    // holding down our sticky shift key.
    if (state.mode == MOD_SHIFT) {
        return true;
    }

    // Update the idle timer; when relevant, we'll use this to determine
    // whether or not the next call will succeed.
    state.idle_timer = record->event.time;

    switch (state.mode) {
        case ONE_SHOT:
            add_weak_mods(MOD_BIT(KC_LSFT));
            state.mode = MOD_SHIFT;
            break;
        case CAPS_WORD:
            if (!handle_caps_locking(keycode)) {
                state.mode = MOD_SHIFT;
            }
            break;
        case CAPS_LOCK:
            handle_caps_locking(keycode);
            break;
        default:
            // not possible.
    }

    // Ensure the keyboard registers the key as pressed.
    send_keyboard_report();

    return true;
}
