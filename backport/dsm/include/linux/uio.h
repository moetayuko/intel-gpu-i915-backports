/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_UIO_H
#define _BACKPORT_DSM_UIO_H

#include_next <linux/uio.h>

#define ITER_SOURCE WRITE
#define ITER_DEST READ
#define iov_iter_ubuf(iter, direction, base, len) \
	iov_iter_init(iter, direction, \
		&(const struct iovec){ .iov_base = (base), .iov_len = (len) }, 1, len)
/* The single iovec must remain alive as long as the iterator. */

#endif
