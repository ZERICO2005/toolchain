# Copyright (C) 2015-2026 CE Programming
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

include $(CURDIR)/../common.mk

EZCFLAGS += -Iinclude
EZCFLAGS += -Werror-implicit-function-declaration
EZCFLAGS += -Wno-implicit-int-conversion
EZCFLAGS += -Wno-unused-label
# EZCFLAGS += -DINLINE_LEVEL=5
EZCFLAGS += -DSOFTFLOAT_FAST_INT64
EZCFLAGS += -DSOFTFLOAT_FAST_DIV32TO16
EZCFLAGS += -DSOFTFLOAT_FAST_DIV64TO32
EZCFLAGS += -DSOFTFLOAT_ROUND_ODD
EZCFLAGS += -Oz

WILDCARD_H := $(wildcard include/*.h)

LIB_SOFTFLOAT := build/libsoftfloat.a

.SECONDARY:

all: $(LIB_SOFTFLOAT)

$(LIB_SOFTFLOAT): build/libsoftfloat.o
	$(Q)$(call MKDIR,build)
	$(Q)$(EZAR) rcs $@ $^

build/libsoftfloat.o: build/libsoftfloat.c.src
	$(Q)$(call MKDIR,build)
	$(Q)$(EZAS) $(EZASFLAGS) $< -o $@

build/libsoftfloat.c.src: libsoftfloat.c
	$(Q)$(call MKDIR,build)
	$(Q)$(EZCC) $(EZCFLAGS) $< -o $@

clean:
	$(Q)$(call RMDIR,build)

install: all
	$(Q)$(call MKDIR,$(INSTALL_SOFTFLOAT))
	$(Q)$(call COPY,$(call NATIVEPATH,$(LIB_SOFTFLOAT)),$(INSTALL_SOFTFLOAT))

.PHONY: all clean install
