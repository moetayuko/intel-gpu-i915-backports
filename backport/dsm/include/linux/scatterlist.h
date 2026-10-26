/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_SCATTERLIST_H
#define _BACKPORT_DSM_SCATTERLIST_H

#include_next <linux/scatterlist.h>

static inline int sg_alloc_table_from_pages_segment(struct sg_table *sgt,
	struct page **pages, unsigned int n_pages, unsigned int offset,
	unsigned long size, unsigned int max_segment, gfp_t gfp)
{
	struct scatterlist *last;

	last = __sg_alloc_table_from_pages(sgt, pages, n_pages, offset, size,
					  max_segment, NULL, 0, gfp);
	return IS_ERR(last) ? PTR_ERR(last) : 0;
}

struct folio;

static inline void sg_set_folio(struct scatterlist *sg, struct folio *folio,
                                unsigned int len, unsigned int offset)
{
 sg_set_page(sg, (struct page *)folio, len, offset);
}

#endif
