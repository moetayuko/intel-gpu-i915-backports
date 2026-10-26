/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_CMPXCHG_H
#define _BACKPORT_DSM_CMPXCHG_H

#include_next <asm/cmpxchg.h>

#define try_cmpxchg64(ptr, oldp, new) try_cmpxchg(ptr, oldp, new)

#endif
