dnl # v6.12-d140f80f6035
dnl # PCI: Deprecate pcim_iomap_regions() in favor of pcim_iomap_region()
dnl #
AC_DEFUN([AC_PCIM_IOMAP_REGION_NOT_PRESENT], [
       AC_KERNEL_DO_BACKGROUND([
               AC_KERNEL_TRY_COMPILE([
                       #include <linux/pci.h>
               ], [
                       struct pci_dev *pdev = NULL;
                       pcim_iomap_region(pdev, 0, "test");
               ], [
               ], [
                       AC_DEFINE([BPM_PCIM_IOMAP_REGION_NOT_PRESENT], 1,
                               [pcim_iomap_region() is not available])
               ])
       ])
])
dnl #
dnl # v6.6-49f776724e64
dnl # PCI/AER: Export pcie_aer_is_native()
dnl #
AC_DEFUN([AC_PCIE_AER_IS_NATIVE_EXP_SYM_NOT_PRESENT], [
        AC_KERNEL_DO_BACKGROUND([
                AC_KERNEL_CHECK_SYMBOL_EXPORT([pcie_aer_is_native],
                        [drivers/pci/pcie/aer.c], [],
		[
                        AC_DEFINE([BPM_PCIE_AER_IS_NATIVE_EXP_SYM_NOT_PRESENT], 1,
                                [pcie_aer_is_native() exported symbol is not available])
                ])
        ])
])
