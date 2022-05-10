# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

.PHONY: clean
clean:
	rm *.hex

.PHONY: cleanall
cleanall: clean
	git submodule deinit --force --all
	rm -r qmk-firmware

.PHONY: qmk-firmware
qmk-firmware:
	git submodule sync --recursive
	git submodule update --init --recursive --progress

.PHONY: qmk-update
qmk-update: qmk-firmware
	git submodule update --remote qmk-firmware

qmk-firmware/users/dkarwowski: qmk-firmware
	ln -f -s -T $(realpath ./)/users/dkarwowski $@

KEYBOARDS := bastardkb/charybdis/3x5

qmk-firmware/keyboards/$(KEYBOARDS)/keymaps/dkarwowski: \
	qmk-firmware qmk-firmware/users/dkarwowski
	ln -f -s -T $(realpath ./)/keyboards/$(subst qmk-firmware/keyboards/,,$@) $@

.SECONDEXPANSION:
.PHONY: $(KEYBOARDS)
$(KEYBOARDS): \
	qmk-firmware/keyboards/$$@/keymaps/dkarwowski
	$(MAKE) -C qmk-firmware $@:dkarwowski:flash

.SECONDEXPANSION:
.PHONY: $(KEYBOARDS).hex
$(KEYBOARDS).hex: \
	qmk-firmware/keyboards/$$(basename $$@)/keymaps/dkarwowski
	$(MAKE) -C qmk-firmware $(basename $@):dkarwowski
