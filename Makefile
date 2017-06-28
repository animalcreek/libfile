#
# Copyright (c) 2017 Animal Creek Technologies, Inc.
#
# Released under the MIT license.
# SPDX-License-Identifier: MIT
#

#
# Set 'Q' to '@' to quiet the output; otherwise, leave unset
#

TOPDIR ?= $(CURDIR)

libfile_OBJS = file.o

PHONY := all
all: $(libfile_OBJS)

PHONY += clean
clean:
	@echo "  CLEAN   $(subst $(TOPDIR)/, ,$(CURDIR))"
	$(Q)$(RM) $(libfile_OBJS)

%.o: %.c
	@echo "  CC    $(subst $(TOPDIR)/, ,$(realpath $<))"
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

.PHONY: $(PHONY)
