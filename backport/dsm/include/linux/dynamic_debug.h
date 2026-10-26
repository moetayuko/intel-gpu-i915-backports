/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_DYNAMIC_DEBUG_H
#define _BACKPORT_DSM_DYNAMIC_DEBUG_H

#include_next <linux/dynamic_debug.h>

/* 5.10 supports per-callsite debugging, but not the optional class filter. */
#define DECLARE_DYNDBG_CLASSMAP(name, type, base, ...)

#endif
