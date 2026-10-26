/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_SEQ_BUF_H
#define _BACKPORT_DSM_SEQ_BUF_H

#include_next <linux/seq_buf.h>

#define DECLARE_SEQ_BUF(NAME, SIZE) \
	struct seq_buf NAME = { .buffer = (char[SIZE]) { 0 }, .size = SIZE }

static inline const char *seq_buf_str(struct seq_buf *s)
{
	if (WARN_ON(s->size == 0))
		return "";

	if (seq_buf_buffer_left(s))
		s->buffer[s->len] = 0;
	else
		s->buffer[s->size - 1] = 0;

	return s->buffer;
}

#endif
