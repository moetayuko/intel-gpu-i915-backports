/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_PAGEVEC_H
#define _BACKPORT_DSM_PAGEVEC_H

#include_next <linux/pagevec.h>
#include <linux/mm.h>
#include <linux/swap.h>

struct folio_batch {
	unsigned int nr;
	struct folio *folios[PAGEVEC_SIZE];
};

static inline void folio_batch_init(struct folio_batch *batch) { batch->nr = 0; }

static inline unsigned int folio_batch_count(struct folio_batch *batch)
{
	return batch->nr;
}

static inline unsigned int folio_batch_add(struct folio_batch *batch,
					  struct folio *folio)
{
	batch->folios[batch->nr++] = folio;
	return PAGEVEC_SIZE - batch->nr;
}

static inline void __folio_batch_release(struct folio_batch *batch)
{
	struct pagevec pages;
	unsigned int n;

	pagevec_init(&pages);
	for (n = 0; n < batch->nr; n++)
		pagevec_add(&pages, (struct page *)batch->folios[n]);
	__pagevec_release(&pages);
	batch->nr = 0;
}

static inline void check_move_unevictable_folios(struct folio_batch *batch)
{
	struct pagevec pages;
	unsigned int n;

	pagevec_init(&pages);
	for (n = 0; n < batch->nr; n++)
		pagevec_add(&pages, (struct page *)batch->folios[n]);
	check_move_unevictable_pages(&pages);
}

static inline unsigned int folio_batch_space(struct folio_batch *batch)
{
 return PAGEVEC_SIZE - batch->nr;
}

static inline void folio_batch_release(struct folio_batch *batch)
{
 /* Native __pagevec_release drains the per-CPU pagevec itself. */
 __folio_batch_release(batch);
}

#endif
