/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_IO_MAPPING_H
#define _BACKPORT_DSM_IO_MAPPING_H

#include_next <linux/io-mapping.h>

#ifdef CONFIG_HAVE_ATOMIC_IOMAP
#error "DSM local IO mappings require the x86-64 direct mapping implementation"
#endif

static inline void __iomem *io_mapping_map_local_wc(struct io_mapping *map,
						   unsigned long offset)
{
	return io_mapping_map_wc(map, offset, PAGE_SIZE);
}

static inline void io_mapping_unmap_local(void __iomem *addr)
{
	io_mapping_unmap(addr);
}

#endif
