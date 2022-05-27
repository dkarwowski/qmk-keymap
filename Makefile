# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

.PHONY: build flash
build: bastardkb/charybdis/3x5.hex assets/bastardkb_charybdis_3x5.svg
flash: bastardkb/charybdis/3x5

KEYBOARDS := bastardkb/charybdis/3x5

.PHONY: assets
assets: assets/$(subst /,_,$(KEYBOARDS)).svg

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
	goimports -w svg/**.go

build/generate: svg/generate.go
	cd svg; go build -o ../$@ ../$^

assets/$(subst /,_,$(KEYBOARDS)).svg: $(KEYBOARDS)/keymap.c $(KEYBOARDS)/combos.def build/generate
	if [ ! -d "assets" ]; then mkdir assets; fi
	build/generate --combos $(KEYBOARDS)/combos.def $(KEYBOARDS)/keymap.c > $@.tmp || ($(RM) $@.tmp)
	(cmp -s $@.tmp $@ && $(RM) $@.tmp) || mv -f $@.tmp $@

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
