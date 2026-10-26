/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_LINUX_OVERFLOW_H
#define __BACKPORT_LINUX_OVERFLOW_H

#include_next <linux/overflow.h>

#ifdef BPM_SIZE_MUL_NOT_PRESENT
static inline size_t size_mul(size_t factor1, size_t factor2)
{
	size_t bytes;

	if (check_mul_overflow(factor1, factor2, &bytes))
		return SIZE_MAX;
	return bytes;
}
#endif

/* typeof accepts either a type or an expression, as the upstream API does. */
#ifndef overflows_type
#define overflows_type(n, T) ({ \
	typeof(T) __val; \
	__builtin_add_overflow((n), 0, &__val); \
})
#endif

/*
 * struct_size_t() - Calculate size of struct with trailing variable array
 * @TYPE: struct type name
 * @member: name of the array member
 * @count: number of elements in the array
 *
 * Like struct_size() but takes a type instead of a pointer.
 * Added in newer kernels for type-safe flexible array allocation.
 */
#ifndef struct_size_t
#define struct_size_t(TYPE, member, count)  \
        struct_size((TYPE *)NULL, member, count)
#endif

#endif /* __BACKPORT_LINUX_OVERFLOW_H */
