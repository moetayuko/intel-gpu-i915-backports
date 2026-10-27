// SPDX-License-Identifier: GPL-2.0-only
/* Map device PFNs on DSM kernels without CONFIG_VMAP_PFN. */
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/vmalloc.h>
#include <asm/tlbflush.h>

#ifndef CONFIG_VMAP_PFN
static void vmap_pfn_flush_tlb(void *unused)
{
	__flush_tlb_all();
}

void *vmap_pfn(unsigned long *pfns, unsigned int count, pgprot_t prot)
{
	struct page **pages, *page;
	unsigned int i;
	void *addr;
	int ret = 0;

	if (!count)
		return NULL;
	for (i = 0; i < count; i++)
		if (WARN_ON_ONCE(pfn_valid(pfns[i])))
			return NULL;

	pages = kvmalloc_array(count, sizeof(*pages), GFP_KERNEL);
	if (!pages)
		return NULL;
	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page) {
		kvfree(pages);
		return NULL;
	}
	for (i = 0; i < count; i++)
		pages[i] = page;

	/*
	 * get_vm_area_caller() and init_mm are not exported. Reserve the
	 * address space and populate its page tables through vmap instead.
	 * The temporary RAM mappings are never exposed to callers.
	 */
	addr = vmap(pages, count, VM_IOREMAP, PAGE_KERNEL);
	kvfree(pages);
	if (!addr)
		goto out_free_page;

	for (i = 0; i < count; i++) {
		unsigned long address = (unsigned long)addr + ((unsigned long)i << PAGE_SHIFT);
		unsigned int level;
		pte_t *pte = lookup_address(address, &level);

		if (WARN_ON_ONCE(!pte || level != PG_LEVEL_4K)) {
			ret = -EINVAL;
			break;
		}
		/* Only this new, unpublished vmalloc range is being modified. */
		set_pte(pte, pte_mkspecial(pfn_pte(pfns[i], pgprot_nx(prot))));
	}

	/* Retire all temporary RAM translations before freeing the page. */
	on_each_cpu(vmap_pfn_flush_tlb, NULL, 1);
	if (ret) {
		vunmap(addr);
		addr = NULL;
	}

out_free_page:
	__free_page(page);
	return addr;
}
EXPORT_SYMBOL_GPL(vmap_pfn);
#endif
