# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

.PHONY: clean
clean:
	rm qmk-firmware/users/dkarwowski
	rm qmk-firmware/keyboards/$(CHARYBDIS_DIR)/keymaps/dkarwowski

qmk-firmware:
	git submodule sync --recursive
	git submodule update --init --recursive --progress
	ln -s users/dkarwowski qmk-firmware/users/dkarwowski
	cd qmk-firmware; qmk setup

.PHONY: userspace
userspace:
ifeq (,$(wildcard qmk-firmware/users/dkarwowski))
	ln -s $(realpath ./)/users/dkarwowski qmk-firmware/users/dkarwowski
endif

.PHONY: qmk-update
qmk-update: qmk-firmware
	git submodule update --remote qmk-firmware

CHARYBDIS_DIR:= bastardkb/charybdis/3x5

.PHONY: charybdis
charybdis: userspace
ifeq (,$(wildcard qmk-firmware/keyboards/$(CHARYBDIS_DIR)/keymaps/dkarwowski))
	ln -s $(realpath ./)/keyboards/$(CHARYBDIS_DIR)/keymaps/dkarwowski \
		qmk-firmware/keyboards/$(CHARYBDIS_DIR)/keymaps/dkarwowski
endif

charybdis-build: charybdis
	cd qmk-firmware; qmk compile -kb $(CHARYBDIS_DIR) -km dkarwowski

charybdis-flash: charybdis
	cd qmk-firmware; qmk flash -kb $(CHARYBDIS_DIR) -km dkarwowski

