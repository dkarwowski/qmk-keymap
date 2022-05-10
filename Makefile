# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

.PHONY: clean
clean:
	rm -f *.hex

.PHONY: cleanall
cleanall: clean
	git submodule deinit --force --all
	rm -r qmk-firmware

qmk-firmware/Makefile:
	git submodule sync --recursive
	git submodule update --init --recursive --progress

.PHONY: qmk-update
qmk-update: qmk-firmware/Makefile
	git submodule update --remote qmk-firmware

qmk-firmware/users/dkarwowski: qmk-firmware/Makefile
	ln -f -s -T $(realpath ./)/users/dkarwowski $@

KEYBOARDS := bastardkb/charybdis/3x5

qmk-firmware/keyboards/$(KEYBOARDS)/keymaps/dkarwowski: qmk-firmware/Makefile
	ln -f -s -T $(realpath ./)/keyboards/$(subst qmk-firmware/keyboards/,,$@) $@

.SECONDEXPANSION:
.PHONY: $(KEYBOARDS)
$(KEYBOARDS): \
	qmk-firmware/keyboards/$$@/keymaps/dkarwowski \
	qmk-firmware/users/dkarwowski
	$(MAKE) -C qmk-firmware $@:dkarwowski:flash

.SECONDEXPANSION:
.PHONY: $(KEYBOARDS).hex
$(KEYBOARDS).hex: \
	qmk-firmware/keyboards/$$(basename $$@)/keymaps/dkarwowski \
	qmk-firmware/users/dkarwowski
	$(MAKE) -C qmk-firmware $(basename $@):dkarwowski
