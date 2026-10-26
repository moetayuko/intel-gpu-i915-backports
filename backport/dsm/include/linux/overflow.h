/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_OVERFLOW_H
#define _BACKPORT_DSM_OVERFLOW_H

#include_next <linux/overflow.h>

#ifndef range_overflows
#define range_overflows(start, size, max) ({ \
	typeof(start) start__ = (start); \
	typeof(size) size__ = (size); \
	typeof(max) max__ = (max); \
	(void)(&start__ == &size__); \
	(void)(&start__ == &max__); \
	start__ >= max__ || size__ > max__ - start__; \
})
#define range_overflows_t(type, start, size, max) \
	range_overflows((type)(start), (type)(size), (type)(max))
#endif

#define range_end_overflows(start, size, max) ({ \
	typeof(start) start__ = (start); \
	typeof(size) size__ = (size); \
	typeof(max) max__ = (max); \
	(void)(&start__ == &size__); \
	(void)(&start__ == &max__); \
	start__ > max__ || size__ > max__ - start__; \
})
#define range_end_overflows_t(type, start, size, max) \
	range_end_overflows((type)(start), (type)(size), (type)(max))

#define __bp_overflows_constexpr(x, T) ( \
	!is_signed_type(typeof(x)) ? (x) > type_max(typeof(T)) : \
	!is_signed_type(typeof(T)) ? (x) < 0 || (x) > type_max(typeof(T)) : \
	(x) < type_min(typeof(T)) || (x) > type_max(typeof(T)))
#define castable_to_type(n, T) \
	__builtin_choose_expr(__is_constexpr(n), \
		!__bp_overflows_constexpr(n, T), __same_type(n, T))

#endif
