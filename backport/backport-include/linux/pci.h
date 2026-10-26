/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BACKPORT_LINUX_PCI_H
#define _BACKPORT_LINUX_PCI_H
#include <linux/sizes.h>
#include <linux/log2.h>
#include <asm/div64.h>
#include_next <linux/pci.h>

#ifndef PCI_IRQ_INTX
#define PCI_IRQ_INTX PCI_IRQ_LEGACY
#endif

#ifdef BPM_PCI_REBAR_SIZE_SUPPORTED_NOT_PRESENT
/*
 * v7.0 added pci_rebar_size_to_bytes, pci_rebar_size_supported,
 * pci_rebar_get_max_size, and pci_resize_resource gained a 4th arg.
 * Provide compat for 6.6.
 */
static inline u64 pci_rebar_size_to_bytes(int size)
{
	return (u64)1 << (size + 20);
}

static inline bool pci_rebar_size_supported(struct pci_dev *pdev,
					    int bar, int size)
{
	u32 sizes = pci_rebar_get_possible_sizes(pdev, bar);

	return sizes & BIT(size);
}

static inline int pci_rebar_get_max_size(struct pci_dev *pdev, int bar)
{
	u32 sizes = pci_rebar_get_possible_sizes(pdev, bar);

	if (!sizes)
		return -ENOTSUPP;
	return fls(sizes) - 1;
}
#endif /* BPM_PCI_REBAR_SIZE_SUPPORTED_NOT_PRESENT */

#ifndef pci_dev_for_each_resource
#define pci_dev_for_each_resource(pdev, res, i) \
	for ((i) = 0; (i) < PCI_NUM_RESOURCES && \
	     (((res) = &((pdev)->resource[(i)])), 1); (i)++)
#endif

#ifdef BPM_PCIM_IOMAP_REGION_NOT_PRESENT
static inline void __iomem *pcim_iomap_region(struct pci_dev *pdev, int bar, const char *name)
{
        int err;

        err = pcim_iomap_regions(pdev, BIT(bar), name);
        if (err)
                return IOMEM_ERR_PTR(err);

        return pcim_iomap_table(pdev)[bar];
}
#endif

#ifdef BPM_PCIE_AER_IS_NATIVE_EXP_SYM_NOT_PRESENT
/* Requires pcie_ports=native on the kernel cmdline; native_aer is then always set */
static inline int pcie_aer_is_native(struct pci_dev *pdev)
{
	return !pdev->aer_cap ? 0 : 1;
}
#endif

#endif /* _BACKPORT_LINUX_PCI_H */
