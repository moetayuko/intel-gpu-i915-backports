/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_PCI_H
#define _BACKPORT_DSM_PCI_H

struct pci_dev;

#define pci_rebar_get_possible_sizes LINUX_I915_BACKPORT(pci_rebar_get_possible_sizes)
unsigned int pci_rebar_get_possible_sizes(struct pci_dev *pdev, int bar);

#include_next <linux/pci.h>

static inline int pci_rebar_bytes_to_size(u64 bytes)
{
	return order_base_2(bytes) - 20;
}

static inline int backport_pci_resize_resource(struct pci_dev *dev, int bar,
					      int size, unsigned long flags)
{
	if (flags)
		return -EOPNOTSUPP;
	return pci_resize_resource(dev, bar, size);
}

#define pci_resize_resource(dev, bar, size, flags) \
	backport_pci_resize_resource(dev, bar, size, flags)

#endif
