// Copyright 2022 Google LLC.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include QMK_KEYBOARD_H

#include "sticky_shift.h"
#include "layer_lock.h"
#include "key_repeat.h"
#include "flexible_delim.h"

enum layers { BASE = 0, TRACK, SYM, NAV, NUM, FUN, SET };

// Shorteners for easier-to-read keymaps.
#define KC_XX KC_NO
#define KC___ KC_TRNS

// One-shot layer keys.
#define KC_os_FUN OSL(FUN)
#define KC_os_NAV OSL(NAV)
#define KC_os_NUM OSL(NUM)
#define KC_os_SET OSL(SET)
#define KC_os_SYM OSL(SYM)

// One-shot mods.
#define KC_os_ALT OSM(MOD_LALT)
#define KC_os_CTL OSM(MOD_LCTL)
#define KC_os_GUI OSM(MOD_LGUI)
#define KC_os_SFT OSM(MOD_LSFT)

// Nav keys for easier copy / paste / etc.
#define KC_REDO KC_AGIN
#define KC_PASTE S(KC_INS)
#define KC_COPY C(KC_INS)
#define KC_CUT S(KC_DEL)

// F* keys may / may not map to pinouts, so write a simple wrapper to avoid
// those overlaps. Dumb, but an easier fix than finding ways to output the
// literals instead of selectively evaluating.
#define KC_FN1 KC_F1
#define KC_FN2 KC_F2
#define KC_FN3 KC_F3
#define KC_FN4 KC_F4
#define KC_FN5 KC_F5
#define KC_FN6 KC_F6
#define KC_FN7 KC_F7
#define KC_FN8 KC_F8
#define KC_FN9 KC_F9
#define KC_FN10 KC_F10
#define KC_FN11 KC_F11
#define KC_FN12 KC_F12

#define KC_EEPROM EEP_RST
#define KC_FLASH QK_BOOTLOADER

// clang-format off

#define _LAYOUT_KC(                                      \
    l00, l01, l02, l03, l04,    r00, r01, r02, r03, r04, \
    l10, l11, l12, l13, l14,    r10, r11, r12, r13, r14, \
    l20, l21, l22, l23, l24,    r20, r21, r22, r23, r24, \
              l30, l31, l32,    r30, r31, r32            \
    )                                                    \
      KC_##l00, KC_##l01, KC_##l02, KC_##l03, KC_##l04,  \
      KC_##r00, KC_##r01, KC_##r02, KC_##r03, KC_##r04,  \
      KC_##l10, KC_##l11, KC_##l12, KC_##l13, KC_##l14,  \
      KC_##r10, KC_##r11, KC_##r12, KC_##r13, KC_##r14,  \
      KC_##l20, KC_##l21, KC_##l22, KC_##l23, KC_##l24,  \
      KC_##r20, KC_##r21, KC_##r22, KC_##r23, KC_##r24,  \
      KC_##l30, KC_##l31, KC_##l32,                      \
      KC_##r30, KC_##r31, KC_##r32
#define LAYOUT_KC(...) _LAYOUT_KC(__VA_ARGS__)

// clang-format on
