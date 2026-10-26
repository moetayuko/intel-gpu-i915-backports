/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_VIDEO_CMDLINE_H
#define _BACKPORT_DSM_VIDEO_CMDLINE_H

#include <linux/fb.h>

static inline const char *video_get_options(const char *name)
{
	char *options = NULL;

	fb_get_options(name, &options);
	return options;
}

#endif
