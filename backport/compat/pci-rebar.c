// SPDX-License-Identifier: GPL-2.0
/* Adapted from linux-intel-lts drivers/pci/rebar.c for the 5.10 PCI ABI. */
#include <linux/bitfield.h>
#include <linux/module.h>
#include <linux/pci.h>

u32 pci_rebar_get_possible_sizes(struct pci_dev *pdev, int bar)
{
	u32 ctrl, cap;
	unsigned int pos, nbars, i;

	/* The target PCI core cannot resize a VF BAR. */
	if (bar >= PCI_STD_NUM_BARS)
		return 0;
	pos = pci_find_ext_capability(pdev, PCI_EXT_CAP_ID_REBAR);
	if (!pos)
		return 0;
	pci_read_config_dword(pdev, pos + PCI_REBAR_CTRL, &ctrl);
	nbars = FIELD_GET(PCI_REBAR_CTRL_NBAR_MASK, ctrl);
	for (i = 0; i < nbars; i++, pos += 8) {
		pci_read_config_dword(pdev, pos + PCI_REBAR_CTRL, &ctrl);
		if (FIELD_GET(PCI_REBAR_CTRL_BAR_IDX, ctrl) != bar)
			continue;
		pci_read_config_dword(pdev, pos + PCI_REBAR_CAP, &cap);
		cap = FIELD_GET(PCI_REBAR_CAP_SIZES, cap);
		if (pdev->vendor == PCI_VENDOR_ID_ATI && pdev->device == 0x731f &&
		    bar == 0 && cap == 0x700)
			return 0x3f00;
		return cap;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(pci_rebar_get_possible_sizes);
MODULE_LICENSE("GPL");
