/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_FILE_H
#define _BACKPORT_DSM_FILE_H

#include_next <linux/file.h>
#include <linux/cleanup.h>

DEFINE_CLASS(fd, struct fd, fdput(_T), fdget(fd), int fd)
DEFINE_CLASS(fd_raw, struct fd, fdput(_T), fdget_raw(fd), int fd)
DEFINE_CLASS(fd_pos, struct fd, fdput_pos(_T), fdget_pos(fd), int fd)
DEFINE_CLASS(get_unused_fd, int, if (_T >= 0) put_unused_fd(_T),
	get_unused_fd_flags(flags), unsigned int flags)
DEFINE_FREE(fput, struct file *, if (!IS_ERR_OR_NULL(_T)) fput(_T))

#endif
