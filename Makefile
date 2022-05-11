# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

KEYBOARDS := bastardkb/charybdis/3x5

.PHONY: build flash
build: bastardkb/charybdis/3x5.hex
flash: bastardkb/charybdis/3x5

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

.PHONY: clean local-clean qmk-clean
clean: local-clean qmk-clean
local-clean:
	rm -rf ./build
qmk-clean:
	$(MAKE) -C qmk-firmware clean

.PHONY: distclean local-distclean qmk-distclean
distclean: local-clean local-distclean qmk-distclean
local-distclean:
	rm -f *.hex *.bin *.uf2
qmk-distclean:
	$(MAKE) -C qmk-firmware distclean

.PHONY: deinit
deinit: clean
	git submodule deinit --force --all
	rm -r qmk-firmware
