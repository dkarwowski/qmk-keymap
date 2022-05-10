# Enable common features.
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# Size optimization options
BOOTMAGIC_ENABLE = no
GRAVE_ESC_ENABLE = no 
LTO_ENABLE = yes
MAGIC_ENABLE = no
MUSIC_MODE = no
QMK_SETTINGS = no
RGBLIGHT_ENABLE = no
RGB_ENABLE = no
RGB_MATRIX_ENABLE = no
SPACE_CADET_ENABLE = no
VIAL_ENABLE = no 
VIA_ENABLE = no

# Source files to populate
SRC += $(USER_PATH)/flexible_delim.c
SRC += $(USER_PATH)/key_repeat.c
SRC += $(USER_PATH)/layer_lock.c
SRC += $(USER_PATH)/sticky_shift.c
VPATH += keyboards/gboards
