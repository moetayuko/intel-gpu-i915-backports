/*
 * Copyright (C) 2016 Red Hat
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 * Rob Clark <robdclark@gmail.com>
 */

#include <drm/drm_print.h>

#ifdef BPM_DRM_PRINT_HEX_DUMP_NOT_PRESENT
/**
 * drm_print_hex_dump - print a hex dump to a &drm_printer stream
 * @p: The &drm_printer
 * @prefix: Prefix for each line, may be NULL for no prefix
 * @buf: Buffer to dump
 * @len: Length of buffer
 *
 * Print hex dump to &drm_printer, with 16 space-separated hex bytes per line,
 * optionally with a prefix on each line. No separator is added after prefix.
 */
void drm_print_hex_dump(struct drm_printer *p, const char *prefix,
                        const u8 *buf, size_t len)
{
        int i;

        for (i = 0; i < len; i += 16) {
                int bytes_per_line = min_t(size_t, 16, len - i);

                drm_printf(p, "%s%*ph\n", prefix ?: "", bytes_per_line, buf + i);
        }
}
EXPORT_SYMBOL(drm_print_hex_dump);
#endif
