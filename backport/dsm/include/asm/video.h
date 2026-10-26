/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_ASM_VIDEO_H
#define _BACKPORT_DSM_ASM_VIDEO_H

#include <linux/pci.h>
#include <linux/vgaarb.h>

static inline bool video_is_primary_device(struct device *dev)
{
	struct pci_dev *pdev;

	if (!dev_is_pci(dev))
		return false;
	pdev = to_pci_dev(dev);
	return pdev == vga_default_device() ||
		(pdev->resource[PCI_ROM_RESOURCE].flags & IORESOURCE_ROM_SHADOW);
}

#endif
