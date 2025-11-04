# Copyright (C) 2015-2025 CE Programming
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

BUILD_SRC := $(patsubst %,build/%.src,$(wildcard *.c *.cpp))

EZCFLAGS := -S -ffreestanding -Wall -Wextra -Wimplicit-float-conversion -Wimplicit-int-float-conversion -Oz
EZCFLAGS += -D_EZ80 -isystem ../libc/include -isystem ../ce/include -isystem ../fileioc -mllvm -profile-guided-section-prefix=false
EZCXXFLAGS := $(EZCFLAGS) -fno-exceptions -fno-rtti -std=c++20 -D_LIBCPP_BUILDING_LIBRARY
EZCXXFLAGS += -isystem ../libcxx/include

WILDCARD_SRC = $(wildcard *.src) $(BUILD_SRC)
LIBCXX_H := include/

ALGORITHM_H := include/__algorithm
BIT_H := include/__bit
CHARCONV_H := include/__charconv
CHRONO_H := wildcard include/__chrono
COMPARE_H := wildcard include/__compare
CONCEPTS_H := include/__concepts
DEBUG_UTILS_H := include/__debug_utils
FORMAT_H := include/__format
FUNCTIONAL_H := include/__functional
FWD_H := include/__fwd
IOS_H := include/__ios
ITERATOR_H := include/__iterator
MEMORY_H := include/__memory
NUMERIC_H := include/__numeric
RANDOM_H := include/__random
RANGES_H := include/__ranges
STRING_H := include/__string
SUPPORT_H := include/__support
TYPE_TRAITS_H := include/__type_traits
UTILITY_H := include/__utility
VARIANT_H := include/__variant

all: $(BUILD_SRC)

build/%.c.src: %.c
	$(Q)$(call MKDIR,build)
	$(Q)$(EZCC) $(EZCFLAGS) $< -o $@

build/%.cpp.src: %.cpp
	$(Q)$(call MKDIR,build)
	$(Q)$(EZCC) $(EZCXXFLAGS) $< -o $@

clean:
	$(Q)$(call RMDIR,build)

install: all
	$(Q)$(call MKDIR,$(INSTALL_LIBCXX))
	$(Q)$(call MKDIR,$(INSTALL_H))
	$(Q)$(call MKDIR,$(INSTALL_CXX_H))
	$(Q)$(call MKDIR,$(INSTALL_ALGORITHM))
	$(Q)$(call MKDIR,$(INSTALL_BIT))
	$(Q)$(call MKDIR,$(INSTALL_CHARCONV))
	$(Q)$(call MKDIR,$(INSTALL_CHRONO))
	$(Q)$(call MKDIR,$(INSTALL_COMPARE))
	$(Q)$(call MKDIR,$(INSTALL_CONCEPTS))
	$(Q)$(call MKDIR,$(INSTALL_DEBUG_UTILS))
	$(Q)$(call MKDIR,$(INSTALL_FORMAT))
	$(Q)$(call MKDIR,$(INSTALL_FUNCTIONAL))
	$(Q)$(call MKDIR,$(INSTALL_FWD))
	$(Q)$(call MKDIR,$(INSTALL_IOS))
	$(Q)$(call MKDIR,$(INSTALL_ITERATOR))
	$(Q)$(call MKDIR,$(INSTALL_MEMORY))
	$(Q)$(call MKDIR,$(INSTALL_NUMERIC))
	$(Q)$(call MKDIR,$(INSTALL_RANDOM))
	$(Q)$(call MKDIR,$(INSTALL_RANGES))
	$(Q)$(call MKDIR,$(INSTALL_STRING))
	$(Q)$(call MKDIR,$(INSTALL_SUPPORT))
	$(Q)$(call MKDIR,$(INSTALL_TYPE_TRAITS))
	$(Q)$(call MKDIR,$(INSTALL_UTILITY))
	$(Q)$(call MKDIR,$(INSTALL_VARIANT))
	$(Q)$(call COPY,$(call NATIVEPATH,$(WILDCARD_SRC)),$(INSTALL_LIBCXX))
	$(Q)$(call COPY,$(call NATIVEPATH,$(LIBCXX_H)),$(INSTALL_CXX_H))
	$(Q)$(call COPY,$(call NATIVEPATH,$(ALGORITHM_H)),$(INSTALL_ALGORITHM))
	$(Q)$(call COPY,$(call NATIVEPATH,$(BIT_H)),$(INSTALL_BIT))
	$(Q)$(call COPY,$(call NATIVEPATH,$(CHARCONV_H)),$(INSTALL_CHARCONV))
	$(Q)$(call COPY,$(call NATIVEPATH,$(CHRONO_H)),$(INSTALL_CHRONO))
	$(Q)$(call COPY,$(call NATIVEPATH,$(COMPARE_H)),$(INSTALL_COMPARE))
	$(Q)$(call COPY,$(call NATIVEPATH,$(CONCEPTS_H)),$(INSTALL_CONCEPTS))
	$(Q)$(call COPY,$(call NATIVEPATH,$(DEBUG_UTILS_H)),$(INSTALL_DEBUG_UTILS))
	$(Q)$(call COPY,$(call NATIVEPATH,$(FORMAT_H)),$(INSTALL_FORMAT))
	$(Q)$(call COPY,$(call NATIVEPATH,$(FUNCTIONAL_H)),$(INSTALL_FUNCTIONAL))
	$(Q)$(call COPY,$(call NATIVEPATH,$(FWD_H)),$(INSTALL_FWD))
	$(Q)$(call COPY,$(call NATIVEPATH,$(IOS_H)),$(INSTALL_IOS))
	$(Q)$(call COPY,$(call NATIVEPATH,$(ITERATOR_H)),$(INSTALL_ITERATOR))
	$(Q)$(call COPY,$(call NATIVEPATH,$(MEMORY_H)),$(INSTALL_MEMORY))
	$(Q)$(call COPY,$(call NATIVEPATH,$(NUMERIC_H)),$(INSTALL_NUMERIC))
	$(Q)$(call COPY,$(call NATIVEPATH,$(RANDOM_H)),$(INSTALL_RANDOM))
	$(Q)$(call COPY,$(call NATIVEPATH,$(RANGES_H)),$(INSTALL_RANGES))
	$(Q)$(call COPY,$(call NATIVEPATH,$(STRING_H)),$(INSTALL_STRING))
	$(Q)$(call COPY,$(call NATIVEPATH,$(SUPPORT_H)),$(INSTALL_SUPPORT))
	$(Q)$(call COPY,$(call NATIVEPATH,$(TYPE_TRAITS_H)),$(INSTALL_TYPE_TRAITS))
	$(Q)$(call COPY,$(call NATIVEPATH,$(UTILITY_H)),$(INSTALL_UTILITY))
	$(Q)$(call COPY,$(call NATIVEPATH,$(VARIANT_H)),$(INSTALL_VARIANT))

.PHONY: all clean
