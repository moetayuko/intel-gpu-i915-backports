AC_DEFUN([AC_MIPI_DSI_DEVICE_ATTACHED_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/drm_mipi_dsi.h>
		], [
			struct mipi_dsi_device dev;
			(void)dev.attached;
		], [
			AC_DEFINE([BPM_MIPI_DSI_DEVICE_ATTACHED_PRESENT], 1,
				[mipi_dsi_device has an attached member])
		], [
		])
	])
])

AC_DEFUN([AC_MIPI_DSI_DEVICE_DSC_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/drm_mipi_dsi.h>
		], [
			struct mipi_dsi_device dev;
			(void)dev.dsc;
		], [
			AC_DEFINE([BPM_MIPI_DSI_DEVICE_DSC_PRESENT], 1,
				[mipi_dsi_device has a dsc member])
		], [
		])
	])
])

AC_DEFUN([AC_MIPI_DSI_DRIVER_REMOVE_RETURN_VOID_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/drm_mipi_dsi.h>
		], [
			char check[__builtin_types_compatible_p(
				typeof(((struct mipi_dsi_driver *)0)->remove),
				void (*)(struct mipi_dsi_device *)) ? 1 : -1];
			(void)check;
		], [
		], [
			AC_DEFINE([BPM_MIPI_DSI_DRIVER_REMOVE_RETURN_VOID_NOT_PRESENT], 1,
				[mipi_dsi_driver.remove does not return void])
		])
	])
])
