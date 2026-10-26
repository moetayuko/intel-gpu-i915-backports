/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_HIGHMEM_H
#define _BACKPORT_DSM_HIGHMEM_H

#include_next <linux/highmem.h>

#if defined(CONFIG_HIGHMEM) || !defined(CONFIG_X86_64)
#error "DSM local mappings require the x86-64 direct map"
#endif

static inline void *kmap_local_page(struct page *page)
{
	return page_address(page);
}

static inline void *kmap_local_page_prot(struct page *page, pgprot_t prot)
{
	return page_address(page);
}

static inline void kunmap_local(const void *addr) { }

static inline void memcpy_from_page(void *to, struct page *page,
				    size_t offset, size_t len)
{
	void *map = kmap_local_page(page);

	VM_BUG_ON(offset + len > PAGE_SIZE);
	memcpy(to, map + offset, len);
	kunmap_local(map);
}

static inline void memcpy_to_page(struct page *page, size_t offset,
				  const void *from, size_t len)
{
	void *map = kmap_local_page(page);

	VM_BUG_ON(offset + len > PAGE_SIZE);
	memcpy(map + offset, from, len);
	flush_dcache_page(page);
	kunmap_local(map);
}

#endif
