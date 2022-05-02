# Enable behaviours we need
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Size optimization options
BOOTMAGIC_ENABLE = no
GRAVE_ESC_ENABLE = no 
LTO_ENABLE = yes
MAGIC_ENABLE = no
MOUSEKEY_ENABLE = no
MUSIC_MODE = no
QMK_SETTINGS = no
RGBLIGHT_ENABLE = no
RGB_ENABLE = no
RGB_MATRIX_ENABLE = no
SPACE_CADET_ENABLE = no
VIAL_ENABLE = no 
VIA_ENABLE = no

# Source files to populate
SRC += features/sticky_shift.c
SRC += features/layer_lock.c
SRC += features/key_repeat.c
VPATH += keyboards/gboards
