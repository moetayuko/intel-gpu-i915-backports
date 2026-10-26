/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_LINUX_OVERFLOW_H
#define __BACKPORT_LINUX_OVERFLOW_H

#include_next <linux/overflow.h>

/*
 * overflows_type() was added in newer kernels.
 * Fallback: value overflows target type T if truncating cast changes it.
 */
#ifndef overflows_type
#define overflows_type(n, T) ({                                 \
        typeof(n) __n = (n);                                    \
        __n != (typeof(n))((T)(__n));                           \
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
