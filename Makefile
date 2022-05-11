# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

KEYBOARDS := bastardkb/charybdis/3x5

.SECONDEXPANSION:
.PHONY: $(KEYBOARDS).hex
$(KEYBOARDS).hex: \
	qmk-firmware/keyboards/$$(basename $$@)/keymaps/dkarwowski \
	qmk-firmware/users/dkarwowski
	$(MAKE) -C qmk-firmware $(basename $@):dkarwowski

.SECONDEXPANSION:
.PHONY: $(KEYBOARDS)
$(KEYBOARDS): \
	qmk-firmware/keyboards/$$@/keymaps/dkarwowski \
	qmk-firmware/users/dkarwowski
	$(MAKE) -C qmk-firmware $@:dkarwowski:dfu

qmk-firmware/Makefile:
	git submodule sync --recursive
	git submodule update --init --recursive --progress

.PHONY: qmk-update
qmk-update: qmk-firmware/Makefile
	git submodule update --remote qmk-firmware

qmk-firmware/users/dkarwowski: qmk-firmware/Makefile
	ln -f -s -T $(realpath ./)/shared $@

qmk-firmware/keyboards/$(KEYBOARDS)/keymaps/dkarwowski: qmk-firmware/Makefile
	ln -f -s -T $(realpath ./)/$(subst qmk-firmware/keyboards/,,$(subst /keymaps/dkarwowski,,$@)) $@

.PHONY: format
format: \
	qmk-firmware/keyboards/$(KEYBOARDS)/keymaps/dkarwowski \
	qmk-firmware/users/dkarwowski
	cd qmk-firmware; qmk format-c **/dkarwowski/*.{c,h}

.PHONY: clean
clean:
	$(MAKE) -C qmk-firmware clean
	rm -rf .build/

.PHONY: distclean
distclean:
	$(MAKE) -C qmk-firmware distclean
	rm -f *.hex

.PHONY: deinit
deinit: clean
	git submodule deinit --force --all
	rm -r qmk-firmware
