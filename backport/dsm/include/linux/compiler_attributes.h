/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_COMPILER_ATTRIBUTES_H
#define _BACKPORT_DSM_COMPILER_ATTRIBUTES_H

#include_next <linux/compiler_attributes.h>

#ifndef __cleanup
#define __cleanup(func) __attribute__((__cleanup__(func)))
#endif

#endif
