/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_BITOPS_H
#define _BACKPORT_DSM_BITOPS_H

#include_next <linux/bitops.h>

#ifndef for_each_clear_bitrange
#define for_each_clear_bitrange(b, e, addr, size)		\
	for ((b) = 0;						\
	     (b) = find_next_zero_bit((addr), (size), (b)),	\
	     (e) = find_next_bit((addr), (size), (b) + 1),	\
	     (b) < (size);					\
	     (b) = (e) + 1)
#endif

#endif
