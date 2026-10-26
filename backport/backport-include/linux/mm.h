/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_LINUX_MM_H
#define __BACKPORT_LINUX_MM_H

#include_next <linux/mm.h>

#ifdef BPM_VM_FLAGS_SET_NOT_PRESENT
#define vm_flags_set(vma, flags)	((vma)->vm_flags |= (flags))

#define vm_flags_clear(vma, flags)	((vma)->vm_flags &= ~(flags))

#define vm_flags_mod(vma, set, clear)	\
	do {				\
		vm_flags_set((vma), (set));	\
		vm_flags_clear((vma), (clear));	\
	} while (0)
#endif

#ifdef BPM_FOLIO_PUT_NOT_PRESENT
#define folio_file_page(folio, idx) ((struct page *)(folio))
#define folio_put(folio) put_page((struct page *)(folio))
#define folio_mark_accessed(folio) mark_page_accessed((struct page *)(folio))
#define folio_lock(folio) lock_page((struct page *)(folio))
#define folio_unlock(folio) unlock_page((struct page *)(folio))
#define folio_mark_dirty(folio) set_page_dirty((struct page *)(folio))
#define folio_mapped(folio) page_mapped((struct page *)(folio))
#define folio_clear_dirty_for_io(folio) clear_page_dirty_for_io((struct page *)(folio))
#define folio_set_reclaim(folio) SetPageReclaim((struct page *)(folio))
#define folio_clear_reclaim(folio) ClearPageReclaim((struct page *)(folio))
#define folio_test_writeback(folio) PageWriteback((struct page *)(folio))
#define page_folio(page) ((struct folio *)(page))
#define folio_page(folio, n) (nth_page((struct page *)(folio), n))
#define folio_alloc(gfp, order) ((struct folio *)alloc_pages((gfp), (order)))
#define folio_order(folio) compound_order((struct page *)(folio))
#define folio_trylock(folio) trylock_page((struct page *)(folio))
#define vma_alloc_folio(gfp, order, vma, addr, hugepage) \
        ((struct folio *)((order) == 0 ? \
                alloc_page_vma((gfp), (vma), (addr)) : \
                alloc_pages_vma((gfp), (order), (vma), (addr), numa_node_id(), (hugepage))))
#endif

#endif /* __BACKPORT_LINUX_MM_H */
