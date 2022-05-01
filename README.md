## QMK Keymap

Directory contains my current [QMK] keymap, targeted for a [Charybdis Nano].
The base layer uses a Colemak-DH layout rather than QWERTY and expects the
trackball to exist, thus dropping the 6th thumb cluster key.

## Features

### Caps Word

* [`caps_word.h`](features/caps_word.h)
* [`caps_word.c`](features/caps_word.c)

Based off of [`getreuer`]'s implementation.

Supports shifting characters until a word-breaking character is entered.

### Layer Lock

* [`layer_lock.h`](features/layer_lock.h)
* [`layer_lock.c`](features/layer_lock.c)

Based off of [`getreuer`]'s implementation.

Supports locking the currect layer temporarily in place, overriding one-shot
state that I prefer to use.

### Key repeat

* [`key_repeat.h`](features/key_repeat.h)
* [`key_repeat.c`](features/key_repeat.c)

Supports repeating the last keypress sent, including modifiers. Configured to
ignore certain keys.

## License

This code is under an Apache License 2.0, see the [LICENSE file] fore more
detail.

Disclaimer: This is not an officially supported Google product.

[QMK]: http://docs.qmk.fm
[Charybdis Nano]: http://github.com/Bastardkb/Charybdis-Nano
[`getreuer`]: http://github.com/getreuer/qmk-keymap
[LICENSE file]: LICENSE.md
