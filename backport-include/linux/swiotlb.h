/*
 * Copyright © 2015 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *
 */

#ifndef _BACKPORT_LINUX_SWIOTLB_H
#define _BACKPORT_LINUX_SWIOTLB_H
#include <linux/version.h>
#include_next <linux/swiotlb.h>

#ifdef BPM_SWIOTLB_MAX_SEGMENT_NOT_PRESENT
static inline unsigned int swiotlb_max_segment(void) { return 0; }
#endif

#ifdef BPM_SWIOTLB_NR_TBL_NO_ARG_PRESENT
#define is_swiotlb_active(X) swiotlb_nr_tbl()
#elif defined(BPM_IS_SWIOTLB_ACTIVE_ARG_DEV_NOT_PRESENT)
#define is_swiotlb_active(X) is_swiotlb_active()
#endif

#endif
