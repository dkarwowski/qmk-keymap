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
const key_override_t ques_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUES, KC_GRV);

const key_override_t **key_overrides = (const key_override_t *[]){
    &scln_override,
    &coln_override,
    &ques_override,
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

// Toggle into pointer
#define KC_trk_T LT(TRACK, KC_T)

// clang-format off

#define _BASE                                                                                      \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
        Q   ,    W   ,    F   ,    P   ,    B    ,     J   ,    L   ,    U   ,    Y   ,  QUOTE ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
        A   ,    R   ,    S   ,  trk_T ,    G    ,     M   ,    N   ,    E   ,    I   ,    O   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
        Z   ,    X   ,    C   ,    D   ,    V    ,     K   ,    H   ,  COMMA ,   DOT  ,  SLASH ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                       os_NAV ,  SPACE , os_NUM  ,  os_SYM , SKYSFT
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define _TRACK                                                                                     \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
     cy_DPI , cy_SNP ,   XX   ,   XX   ,   XX    ,    XX   ,   XX   , tg_SNP , tg_SCR ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL ,   __   ,   XX    ,    XX   , os_SFT , os_CTL , os_ALT , os_GUI ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     ho_SNP , ho_SCR , os_SFT ,   XX   ,   XX    ,    XX   ,  BTN1  ,  BTN3  ,  BTN2  ,   __   ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                        BTN3  ,  BTN1  ,  BTN2   ,    XX   ,  LLCK
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define _SYM                                                                                       \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
      GRAVE ,  TILDE ,  AMPR  ,  ASTR  ,  PLUS   ,    AT   ,  HASH  ,  PERC  ,  PIPE  ,  BSLS  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
      LPRN  ,  RPRN  ,  LCBR  ,  RCBR  ,  EQUAL  ,  DOLLAR , os_SFT , os_CTL , os_ALT , os_GUI ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       LT   ,   GT   ,  LBRC  ,  RBRC  ,  EXLM   ,   CIRC  ,   UNDS ,  SCLN  ,  COLON ,  SLASH ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                       DELIMS ,  SPACE ,  MINS   ,    __   ,  LLCK
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define _NAV                                                                                       \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,    XX   ,   XX   ,   XX   ,   XX   ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,   XX    ,  REPEAT ,  LEFT  ,  DOWN  ,   UP   ,  RIGHT ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
      UNDO  ,   CUT  ,  COPY  ,  PASTE ,  REDO   ,    INS  ,  HOME  ,  PGDN  ,  PGUP  ,   END  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                         __   ,  LLCK  , os_SET  ,    XX   ,   XX
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define _NUM                                                                                       \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,  UNDS   ,   SLASH ,    7   ,    8   ,    9   ,  BSPC  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,  EQUAL  ,   ASTR  ,    4   ,    5   ,    6   ,  COMMA ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       LT   ,   GT   ,  LBRC  ,  RBRC  ,  PLUS   ,   MINUS ,    1   ,    2   ,    3   ,   DOT  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                       os_FUN ,  LLCK  ,   __    ,    ENT  ,    0
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define _FUN                                                                                       \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,   PSCR  ,   FN7  ,   FN8  ,   FN9  ,  FN12  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
     os_GUI , os_ALT , os_CTL , os_SFT ,   XX    ,   SLCK  ,   FN4  ,   FN5  ,   FN6  ,  FN11  ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,   PAUS  ,   FN1  ,   FN2  ,   FN3  ,  FN10  ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                         __   ,  LLCK  , os_SET  ,    XX   ,   XX
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define _SET                                                                                       \
  /*--------+--------+--------+--------+--------+ +--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,    XX   ,   XX   ,   XX   ,   XX   ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   ,   XX   ,   XX    ,    XX   ,   XX   ,   XX   ,   XX   ,   XX   ,   \
  /*--------|--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
       XX   ,   XX   ,   XX   , EEPROM ,  FLASH  ,   FLASH , EEPROM ,   XX   ,   XX   ,   XX   ,   \
  /*--------+--------|--------|--------|--------| |--------|--------|--------|--------|--------|*/ \
                         XX   ,   XX   ,   __    ,    XX   ,   XX
  /*                 +--------|--------|--------+ +--------|--------+                           */

#define LAYOUT_wrap(...) LAYOUT_split_3x5_3(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE]  = LAYOUT_wrap(_KC(_BASE,  XX)),
    [TRACK] = LAYOUT_wrap(_KC(_TRACK, XX)),
    [SYM]   = LAYOUT_wrap(_KC(_SYM,   XX)),
    [NUM]   = LAYOUT_wrap(_KC(_NUM,   XX)),
    [FUN]   = LAYOUT_wrap(_KC(_FUN,   XX)),
    [NAV]   = LAYOUT_wrap(_KC(_NAV,   XX)),
    [SET]   = LAYOUT_wrap(_KC(_SET,   XX)),
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
