/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_ASM_UACCESS_H
#define _BACKPORT_DSM_ASM_UACCESS_H

#include_next <asm/uaccess.h>

/* The native x86 nocache helper provides the same inatomic WC copy. */
#define copy_from_user_inatomic_nontemporal __copy_from_user_inatomic_nocache

#endif
