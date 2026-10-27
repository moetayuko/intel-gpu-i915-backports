/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_VMALLOC_H
#define _BACKPORT_DSM_VMALLOC_H

#include_next <linux/vmalloc.h>

#ifndef CONFIG_VMAP_PFN
#define vmap_pfn LINUX_BACKPORT(vmap_pfn)
void *vmap_pfn(unsigned long *pfns, unsigned int count, pgprot_t prot);
#endif

#endif
