dnl # pci_rebar.m4 - check for v7.0 pci rebar helpers
AC_DEFUN([AC_PCI_REBAR_SIZE_SUPPORTED_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
#include <linux/pci.h>
		], [
			bool (*fn)(struct pci_dev *, int, int) = pci_rebar_size_supported;
			(void)fn;
		], [
		], [
			AC_DEFINE(BPM_PCI_REBAR_SIZE_SUPPORTED_NOT_PRESENT, 1,
			[pci_rebar_size_supported and related helpers not available])
		])
	])
])
