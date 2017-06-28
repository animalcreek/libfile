/*
 * Copyright (c) 2015-2017 Animal Creek Technologies, Inc.
 *
 * Released under the MIT license.
 * SPDX-License-Identifier: MIT
 */

#ifndef __LIBFILE_H
#define __LIBFILE_H

/**
 * @file libfile/file.h
 * @brief File Helper Library API
 * @author Mark Greer <mgreer@animalcreek.com>
 */

#include <unistd.h>
#include <stdbool.h>

int file_get_size(char *filename, off_t *size);
bool file_is_present(char *filename);
int file_read(int fd, void *buf, size_t len);
int file_write(int fd, void *buf, size_t len);
int file_owc(char *filename, void *buf, size_t len);

#endif
