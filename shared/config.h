// Copyright 2022 Google LLC
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Matches the default, but useful in macros.
#define TAPPING_TERM 200

// Most common for me to plug in the right side. Maybe worth moving to using
// EEPROM instead and being able to swap sides?
#if defined(SPLIT_KEYBOARD)
#    define MASTER_RIGHT
#endif

// Oneshots are heavily used for my mods and layers.
#if defined(NO_ACTION_ONESHOT)
#    error "Oneshot keys are required, integral to layout."
#endif
// Hypothetically toggle oneshots with a double-tap.
#define ONESHOT_TAP_TOGGLE 2
// Timeout oneshots in case I activate and walk away.
#define ONESHOT_TIMEOUT 5000

// Enable longer combos / chains; not used at the moment.
#define QMK_KEYS_PER_SCAN 4

// https://docs.qmk.fm/#/tap_hold?id=permissive-hold
#define PERMISSIVE_HOLD

// Auto-shifting on letters is unwanted.
#define NO_AUTO_SHIFT_ALPHA
// Auto-shift timeout matches the tapping term.
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
// Use defaults for how auto-shift functions.
#define AUTO_SHIFT_NO_SETUP
