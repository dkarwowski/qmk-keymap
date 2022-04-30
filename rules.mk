# Enable behaviours we need
MOUSEKEY_ENABLE = yes
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Size optimization options
LTO_ENABLE = yes
VIA_ENABLE = no
VIAL_ENABLE = no 
BOOTMAGIC_ENABLE = no
RGBLIGHT_ENABLE = no
RGB_ENABLE = no
RGB_MATRIX_ENABLE = no
QMK_SETTINGS = no
MUSIC_MODE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no 
MAGIC_ENABLE = no

# Source files to populate
SRC += features/caps_word.c
SRC += features/layer_lock.c
SRC += features/key_repeat.c
