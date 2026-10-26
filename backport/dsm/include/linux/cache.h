/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_CACHE_H
#define _BACKPORT_DSM_CACHE_H

#include_next <linux/cache.h>

#ifndef ARCH_DMA_MINALIGN
#define ARCH_DMA_MINALIGN 8
#endif

#endif
