// Copyright 2022 Google LLC
// SPDX-License-Identifier: Apache-2.0
#include "dkarwowski.h"

#if defined(COMBO_ENABLE)
#    include "g/keymap_combo.h"
#endif

enum custom_keycodes {
    LAYER_LOCK = CHARYBDIS_SAFE_RANGE,
    STICKY_SHIFT,
    KEY_REPEAT,
    ID_DELIMS,
};

const key_override_t scln_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SCLN);
const key_override_t coln_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);

const key_override_t **key_overrides = (const key_override_t *[]){
    &scln_override,
    &coln_override,
    NULL,
};

// Custom keys for the KC_ wrapper.
#define KC_SKYSFT STICKY_SHIFT
#define KC_REPEAT KEY_REPEAT
#define KC_LLCK LAYER_LOCK
#define KC_DELIMS ID_DELIMS

// Charybdis pointer-device keys.
#define KC_cy_DPI DPI_MOD
#define KC_cy_SNP S_D_MOD
#define KC_ho_SCR DRGSCRL
#define KC_ho_SNP SNIPING
#define KC_tg_SCR DRG_TOG
#define KC_tg_SNP SNP_TOG

#define LAYOUT_BASE                                                                                \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
        Q   ,    W   ,    F   ,    P   ,    B    ,     J   ,    L   ,    U   ,    Y   ,  QUOT  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
        A   ,    R   ,    S   ,    T   ,    G    ,     M   ,    N   ,    E   ,    I   ,    O   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
        Z   ,    X   ,    C   ,    D   ,    V    ,     K   ,    H   ,  COMM  ,   DOT  ,  SLSH  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                       os_NAV ,  SPACE , os_NUM  ,  os_SYM , SKYSFT ,   XX
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

layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, TRACK));
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_flexible_delim(keycode, record, ID_DELIMS)) {
        return false;
    }
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
