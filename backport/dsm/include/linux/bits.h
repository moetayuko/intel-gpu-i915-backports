/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_BITS_H
#define _BACKPORT_DSM_BITS_H

#include_next <linux/bits.h>

#define BIT_U8(nr) BIT_TYPE(u8, nr)
#define GENMASK_U8(h, l) GENMASK_TYPE(u8, h, l)

#endif
