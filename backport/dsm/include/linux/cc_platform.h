/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_CC_PLATFORM_H
#define _BACKPORT_DSM_CC_PLATFORM_H

#include <asm/mem_encrypt.h>

enum cc_attr { CC_ATTR_MEM_ENCRYPT, CC_ATTR_GUEST_MEM_ENCRYPT };

static inline bool cc_platform_has(enum cc_attr attr)
{
	if (attr == CC_ATTR_MEM_ENCRYPT)
		return sme_active() || sev_active();
	return attr == CC_ATTR_GUEST_MEM_ENCRYPT && sev_active();
}

#endif
