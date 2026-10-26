/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_UACCESS_H
#define _BACKPORT_DSM_UACCESS_H

#include_next <linux/uaccess.h>
#include <linux/overflow.h>

static inline void *memdup_array_user(const void __user *src,
				     size_t n, size_t size)
{
	size_t bytes;

	if (check_mul_overflow(n, size, &bytes))
		return ERR_PTR(-EOVERFLOW);
	return memdup_user(src, bytes);
}

#endif
