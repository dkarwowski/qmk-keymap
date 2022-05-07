// Copyright 2022 Google LLC
// SPDX-License-Identifier: Apache-2.0
#pragma once

// default but used in macros
#define TAPPING_TERM 200

#if defined(SPLIT_KEYBOARD)
#    define MASTER_RIGHT
#endif

#undef NO_ACTION_ONESHOT
#define ONESHOT_TAP_TOGGLE 2
#define ONESHOT_TIMEOUT 5000

#define QMK_KEYS_PER_SCAN 4
#define COMBO_COUNT 5

#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_FORCE_HOLD
#define PERMISSIVE_HOLD

#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#define AUTO_SHIFT_NO_SETUP
