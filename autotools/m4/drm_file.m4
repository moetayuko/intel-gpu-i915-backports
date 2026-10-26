dnl #
dnl # v6.5-686b21b5f6ca2
dnl # drm: Add fdinfo memory stats
dnl #
AC_DEFUN([AC_DRM_MEMORY_STATS_NOT_PRESENT], [
        AC_KERNEL_DO_BACKGROUND([
                AC_KERNEL_TRY_COMPILE([
                        #include <drm/drm_file.h>
                ],[
			struct drm_memory_stats stats = {};
			(void)stats;
                ],[
                ],[
                        AC_DEFINE(BPM_DRM_MEMORY_STATS_NOT_PRESENT, 1,
                                [drm_memory_stats is not available])
                ])
        ])
])
