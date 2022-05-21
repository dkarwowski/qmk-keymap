## QMK Keymap

Directory contains my current [QMK] keymap, targeted for a [Charybdis Nano].
The base layer uses a Colemak-DH layout rather than QWERTY and expects the
trackball to exist, thus dropping the 6th thumb cluster key.

## How to build

This repo provides a non-fork pattern for maintaining my keymap (particularly,
this seems most aligned with the requirements for Google's employee copyright
requirements).

The directory is set up such that:

```
qmk-keymap/
  qmk-firmware/            # QMK's firmware in a git submodule.
    ...
  shared/                  # Userspace directory.
    ...                    # Gets linked to qmk-firmware/users/dkarwowski
  bastardkb/charybdis/3x5/ # Keyboard's keymap directory.
    ...                    # Maps to qmk-firmware/keyboards/$KB/keymaps/dkarwowski
```

To build the hex files, you can simply run:

```shell
$ make
```

To flash a specific keyboard with my keymap:

```shell
$ make bastardkb/charybdis/3x5
```

If I ever add other keymaps, those will work identically to above (and the
general `make` call should, by default, build those as well).

## Features

### Sticky Shift

* [`sticky_shift.h`](features/sticky_shift.h)
* [`sticky_shift.c`](features/sticky_shift.c)

Custom one-shot / tap dance handling for shifting keys. Performs the functions
including:

1. One-shot shift is sent when you perform a tap-release without pressing
   anything else in the mean time.
2. Regular shifting is performed if the key is held while other keys are being
   pressed.
3. Caps word (based on [`getreuer`]'s implementation) when double-tapped within
   a specific window. Can be deactivated by tapping shift again (next letter
   will _not_ be shifted), or typing a "word breaking" character.
4. Caps lock is activated when triple-tapped within a window (e.g. rapidly
   after caps word is activated. This is only deactivated after the custom
   shift key is pressed again.

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

## Keyboards

### Bastardkb Charybdis Nano

![bastardkb/charybdis/3x5/keymap.c](./assets/bastardkb_charbydis_3x5.svg)

## License

This code is under an Apache License 2.0, see the [LICENSE file] fore more
detail.

Disclaimer: This is not an officially supported Google product.

[QMK]: http://docs.qmk.fm
[Charybdis Nano]: http://github.com/Bastardkb/Charybdis-Nano
[`getreuer`]: http://github.com/getreuer/qmk-keymap
[LICENSE file]: LICENSE.md
