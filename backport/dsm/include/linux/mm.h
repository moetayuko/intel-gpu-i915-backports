/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_MM_H
#define _BACKPORT_DSM_MM_H

#include_next <linux/mm.h>

static inline bool is_cow_mapping(vm_flags_t flags)
{
	return (flags & (VM_SHARED | VM_MAYWRITE)) == VM_MAYWRITE;
}

/* Folios use the compound head and retain the native page reference. */
#undef page_folio
#define page_folio(page) ((struct folio *)compound_head(page))
#undef folio_file_page
#define folio_file_page(folio, file_index_) \
	nth_page((struct page *)(folio), (file_index_) - ((struct page *)(folio))->index)
#define folio_nr_pages(folio) compound_nr((struct page *)(folio))
#define folio_pfn(folio) page_to_pfn((struct page *)(folio))

#define vma_set_file LINUX_BACKPORT(vma_set_file)
void vma_set_file(struct vm_area_struct *vma, struct file *file);

#define folio_address(folio) page_address((struct page *)(folio))
#define virt_to_folio(addr) page_folio(virt_to_page(addr))

#endif
