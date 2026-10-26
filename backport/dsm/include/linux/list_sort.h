/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_LIST_SORT_H
#define _BACKPORT_DSM_LIST_SORT_H

#include_next <linux/list_sort.h>

struct backport_sort_ctx {
	void *priv;
	int (*cmp)(void *, const struct list_head *, const struct list_head *);
};

static int backport_list_cmp(void *data, struct list_head *a, struct list_head *b)
{
	struct backport_sort_ctx *ctx = data;

	return ctx->cmp(ctx->priv, a, b);
}

static inline void backport_list_sort(void *priv, struct list_head *head,
	int (*cmp)(void *, const struct list_head *, const struct list_head *))
{
	struct backport_sort_ctx ctx = { .priv = priv, .cmp = cmp };

	list_sort(&ctx, head, backport_list_cmp);
}

#define list_sort backport_list_sort

#endif
