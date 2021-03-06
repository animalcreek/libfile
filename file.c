/*
 * Copyright (c) 2015-2017 Animal Creek Technologies, Inc.
 *
 * Released under the MIT license.
 * SPDX-License-Identifier: MIT
 */

/**
 * @file libfile/file.c
 * @brief File Helper Library
 * @author Mark Greer <mgreer@animalcreek.com>
 *
 * Set of routines to provide easy-to-use ways to perform common Linux(tm)
 * file operations.
 */

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "file.h"

/**
 * @brief Get size of file in bytes
 * @param [in] filename Pointer to filename string
 * @param [out] size Pointer to where file size is placed
 * @return 0 on success, negative errno on failure
 */
int file_get_size(char *filename, off_t *size)
{
	struct stat stat_data;
	int ret;

	if (!filename || !size)
		return -EINVAL;

	ret = stat(filename, &stat_data);
	if (ret)
		return -errno;

	*size = stat_data.st_size;
	return 0;
}

/**
 * @brief Determine if file is present (i.e., exists)
 * @param [in] filename Pointer to filename string
 * @return true when file exists, false when it doesn't exist
 */
bool file_is_present(char *filename)
{
	struct stat stat_data;

	if (!filename)
		return -EINVAL;

	return stat(filename, &stat_data) == 0;
}

/**
 * @brief Read specified number of bytes from file or socket
 * @param [in] fd File descriptor to read from
 * @param [out] buf Buffer to place incoming data
 * @param [in] len Number of bytes to read
 * @return 0 on success, negative errno on failure
 *
 * Read the specified number of bytes from the file/pipe/socket associated
 * with the passed in file descriptor.  It will block waiting indefinitely
 * for the specified number of bytes to be received or until it is interrupted.
 * If the read() system call returns with a return code of zero, -ECONNRESET
 * will be returned.
 */
int file_read(int fd, void *buf, size_t len)
{
	size_t bytes_left = len;
	ssize_t size;

	if (!buf || !len)
		return -EINVAL;

	while (bytes_left > 0) {
		size = read(fd, buf, bytes_left);
		if (size < 0)
			return -errno;
		else if (size == 0)
			return -ECONNRESET;
		else
			bytes_left -= size;
	}

	return 0;
}

/**
 * @brief Write specified number of bytes to file or socket
 * @param [in] fd File descriptor to write to
 * @param [in] buf Buffer containing data to write
 * @param [in] len Number of bytes to write
 * @return 0 on success, negative errno on failure
 *
 * Write the specified number of bytes to the file/pipe/socket associated
 * with the passed in file descriptor.  It will block indefinitely until
 * the specified number of bytes have been accepted by the kernel or until
 * it is interrupted.
 */
int file_write(int fd, void *buf, size_t len)
{
	ssize_t size;
	int ret = 0;

	if (!buf || !len)
		return -EINVAL;

	size = write(fd, buf, len);
	if (size < 0)
		ret = -errno;
	else if (size != (ssize_t)len)
		ret = -ECONNRESET;

	return ret;
}

/**
 * @brief Open, write to, and close a file
 * @param [in] filename Pointer to filename string
 * @param [in] buf Buffer containing data to write
 * @param [in] len Number of bytes to write
 * @return 0 on success, negative errno on failure
 *
 * Open the specified file, write the specified number of bytes, and close
 * the file.  It may block indefinitely as described by the file_write()
 * documentation.
 */
int file_owc(char *filename, void *buf, size_t len)
{
	int fd, ret, ret_close;

	fd = open(filename, O_WRONLY);
	if (fd < 0)
		return -errno;

	ret = file_write(fd, buf, len);

	ret_close = close(fd);
	if (ret_close)
		return ret_close;

	return ret;
}
