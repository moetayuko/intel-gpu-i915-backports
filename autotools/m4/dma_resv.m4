dnl #
dnl # v5.19-7bc80a5462c3 dma-buf: add enum dma_resv_usage v4
dnl #
AC_DEFUN([AC_DMA_RESV_USAGE_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
			AC_KERNEL_TRY_COMPILE([
				#include <linux/dma-resv.h>
			],[
				enum dma_resv_usage usage = DMA_RESV_USAGE_BOOKKEEP;
				(void)usage;
			],[
			],[
				AC_DEFINE(BPM_DMA_RESV_USAGE_NOT_PRESENT, 1,
					  [enum dma_resv_usage is not available in the kernel])
			])
	])
])

dnl #
dnl # v5.16-5baaac3184ab dma-buf: add dma_resv_for_each_fence v3
dnl #
AC_DEFUN([AC_DMA_RESV_FOR_EACH_FENCE_NOT_PRESENT], [
        AC_KERNEL_DO_BACKGROUND([
                        AC_KERNEL_TRY_COMPILE([
                                #include <linux/dma-resv.h>
                        ],[
                                struct dma_resv_iter cursor;
                                struct dma_fence *fence = NULL;
                                dma_resv_for_each_fence(&cursor, NULL, 0, fence)
                                        ;
                        ],[
                        ],[
                                AC_DEFINE(BPM_DMA_RESV_FOR_EACH_FENCE_NOT_PRESENT, 1,
                                          [dma_resv_for_each_fence() is not available in the kernel])
                        ])
        ])
])
