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
#include QMK_KEYBOARD_H

#include "features/sticky_shift.h"
#include "features/layer_lock.h"
#include "features/key_repeat.h"

#if defined(COMBO_ENABLE)
#   include "g/keymap_combo.h"
#endif

enum layers { BASE = 0, TRACK, SYM, NAV, NUM, FUN, SET };

// Charybdis already has some custom keycodes, adapt to that.
#ifdef KEYBOARD_charybdis
#    define MY_SAFE_RANGE CHARYBDIS_SAFE_RANGE
#else
#    define MY_SAFE_RANGE SAFE_RANGE
#endif // KEYBOARD_charybdis

enum custom_keycodes {
    LAYER_LOCK = MY_SAFE_RANGE,
    STICKY_SHIFT,
    KEY_REPEAT,
    ID_DELIMS,
};

const key_override_t scln_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SCLN);
const key_override_t coln_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);
const key_override_t del_override  = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const key_override_t **key_overrides = (const key_override_t *[]){
    &scln_override,
    &coln_override,
    &del_override,
    NULL,
};

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

// Custom keys for the KC_ wrapper.
#define KC_STKSFT STICKY_SHIFT
#define KC_REPEAT KEY_REPEAT
#define KC_LLCK LAYER_LOCK
#define KC_DELIMS ID_DELIMS

// Charybdis pointer-device keys.
#ifdef KEYBOARD_charybdis
#    define KC_cy_DPI DPI_MOD
#    define KC_cy_SNP S_D_MOD
#    define KC_ho_SCR DRGSCRL
#    define KC_ho_SNP SNIPING
#    define KC_tg_SCR DRG_TOG
#    define KC_tg_SNP SNP_TOG
#else
#    define KC_cy_DPI KC_NO
#    define KC_cy_SNP KC_NO
#    define KC_ho_SCR KC_NO
#    define KC_ho_SNP KC_NO
#    define KC_tg_SCR KC_NO
#    define KC_tg_SNP KC_NO
#endif // KEYBOARD_charybdis

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

#define LAYOUT_BASE                                                                                \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
        Q   ,    W   ,    F   ,    P   ,    B    ,     J   ,    L   ,    U   ,    Y   ,  QUOT  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
        A   ,    R   ,    S   ,    F   ,    G    ,     M   ,    N   ,    E   ,    I   ,    O   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
        Z   ,    X   ,    C   ,    D   ,    V    ,     K   ,    H   ,  COMM  ,   DOT  ,  SLSH  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                       os_NAV ,  SPACE , os_NUM  ,  os_SYM , STKSFT ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define LAYOUT_TRACK                                                                               \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   , cy_DPI , cy_SNP  ,    XX   ,   XX   , tg_SNP , tg_SCR ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,   XX    ,    XX   , os_SFT , os_CTL , os_ALT , os_GUI ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       __   ,   XX   , ho_SNP , ho_SCR ,   XX    ,    XX   ,  BTN1  ,  BTN3  ,  BTN2  ,   XX   ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                        BTN3  ,  BTN1  ,  BTN2   ,    XX   ,  LLCK  ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define LAYOUT_SYM                                                                                 \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       GRV  ,  HASH  ,  PERC  ,  EXLM  ,   EQL   ,    XX   ,   XX   ,   XX   ,   XX   ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
      LPRN  ,  RPRN  ,  LCBR  ,  RCBR  ,  UNDS   ,    XX   , os_SFT , os_CTL , os_ALT , os_GUI ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
      LBRC  ,  RBRC  ,   LT   ,   GT   ,  BSLS   ,    XX   , DELIMS ,   XX   ,   XX   ,   XX   ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                        AMPR  ,  MINS  ,  ASTR   ,    __   ,  LLCK  ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define LAYOUT_NAV                                                                                 \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,   REDO  ,  PASTE ,  COPY  ,   CUT  ,  UNDO  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,   XX    ,  REPEAT ,  LEFT  ,  DOWN  ,   UP   ,  RIGHT ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,    INS  ,  HOME  ,  PGDN  ,  PGUP  ,   END  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                         __   ,  LLCK  , os_SET  ,    XX   ,   XX   ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define LAYOUT_NUM                                                                                 \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,   SLSH  ,    7   ,    8   ,    9   ,  PLUS  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,   XX    ,   MINS  ,    4   ,    5   ,    6   ,   DOT  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,     0   ,    1   ,    2   ,    3   ,  COMMA ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                       os_FUN ,  LLCK  ,   __    ,    ENT  ,  BSPC  ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define LAYOUT_FUN                                                                                 \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,   PSCR  ,   FN7  ,   FN8  ,   FN9  ,  FN12  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,   XX    ,   SLCK  ,   FN4  ,   FN5  ,   FN6  ,  FN11  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,   PAUS  ,   FN1  ,   FN2  ,   FN3  ,  FN10  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                         __   ,  LLCK  , os_SET  ,    XX   ,   XX   ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define LAYOUT_SET                                                                                 \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,    XX   ,   XX   ,   XX   ,   XX   ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,    XX   ,   XX   ,   XX   ,   XX   ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   , EEPROM ,  FLASH  ,   FLASH , EEPROM ,   XX   ,   XX   ,   XX   ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                         XX   ,   XX   ,   __    ,    XX   ,   XX   ,   XX
  /*                 +--------|--------|--------+ +--------|--------|--------+                  */

#define _POINTER_MOD(\
    L00, L01, L02, L03, L04, R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14, R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24, R20, R21, R22, R23, R24, \
    ...)                                              \
                L00 , L01, L02, L03,           L04 ,  \
                R00 , R01, R02, R03,           R04 ,  \
                L10 , L11, L12, L13,           L14 ,  \
                R10 , R11, R12, R13,           R14 ,  \
      LT(TRACK, L20), L21, L22, L23,           L24 ,  \
                R20 , R21, R22, R23, LT(TRACK, R24),  \
      __VA_ARGS__
#define POINTER(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrap(...) LAYOUT_split_3x5_3(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_wrap(POINTER(LAYOUT_KC(LAYOUT_BASE))),
    [TRACK] = LAYOUT_wrap(LAYOUT_KC(LAYOUT_TRACK)),
    [SYM] = LAYOUT_wrap(LAYOUT_KC(LAYOUT_SYM)),
    [NUM] = LAYOUT_wrap(LAYOUT_KC(LAYOUT_NUM)),
    [FUN] = LAYOUT_wrap(LAYOUT_KC(LAYOUT_FUN)),
    [NAV] = LAYOUT_wrap(LAYOUT_KC(LAYOUT_NAV)),
    [SET] = LAYOUT_wrap(LAYOUT_KC(LAYOUT_SET)),
};

// clang-format on

#ifdef KEYBOARD_charybdis
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, LAYER_POINTER));
    return state;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_sticky_shift(keycode, record, STICKY_SHIFT)) {
        return false;
    }
    if (!process_layer_lock(keycode, record, LAYER_LOCK)) {
        return false;
    }
    if (!process_key_repeat(keycode, record, KEY_REPEAT)) {
        return false;
    }

    return true;
}

void matrix_scan_user(void) {}
