/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_STRING_H
#define _BACKPORT_DSM_STRING_H

#include_next <linux/string.h>

static inline bool mem_is_zero(const void *s, size_t n)
{
	return !memchr_inv(s, 0, n);
}

#define strtomem_pad(dest, src, pad) do { \
	const size_t __size = __builtin_object_size(dest, 1); \
	BUILD_BUG_ON(__size == (size_t)-1); \
	memcpy_and_pad(dest, __size, src, strnlen(src, __size), pad); \
} while (0)

static inline ssize_t backport_strscpy(char *dest, const char *src, size_t size)
{
	return strscpy(dest, src, size);
}

#define __bp_strscpy_2(dest, src) \
	backport_strscpy(dest, src, sizeof(dest) + __must_be_array(dest))
#define __bp_strscpy_3(dest, src, size) backport_strscpy(dest, src, size)
#define __bp_strscpy_pick(_1, _2, _3, func, ...) func
#define strscpy(...) \
	__bp_strscpy_pick(__VA_ARGS__, __bp_strscpy_3, __bp_strscpy_2)(__VA_ARGS__)

#endif
