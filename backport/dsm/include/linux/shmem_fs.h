/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_SHMEM_FS_H
#define _BACKPORT_DSM_SHMEM_FS_H

#include_next <linux/shmem_fs.h>

#undef shmem_read_folio_gfp
#undef shmem_read_folio

static inline struct folio *shmem_read_folio_gfp(struct address_space *mapping,
					       pgoff_t index, gfp_t gfp)
{
	struct page *page = shmem_read_mapping_page_gfp(mapping, index, gfp);

	return IS_ERR(page) ? ERR_CAST(page) : page_folio(page);
}

#define shmem_read_folio(mapping, index) \
	shmem_read_folio_gfp(mapping, index, mapping_gfp_mask(mapping))

#endif
