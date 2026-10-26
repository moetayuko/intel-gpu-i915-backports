/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_COMPONENT_H
#define _BACKPORT_DSM_COMPONENT_H

#include_next <linux/component.h>
#include <linux/of.h>

static inline void component_release_of(struct device *dev, void *data)
{
 of_node_put(data);
}

#endif
