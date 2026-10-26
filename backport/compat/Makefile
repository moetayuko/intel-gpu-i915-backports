ccflags-y += -I$(src) -Wframe-larger-than=1280
ifeq ($(CONFIG_BACKPORT_INTEGRATE),)
obj-m += i915-compat.o
else
obj-y += i915-compat.o
endif
i915-compat-y += main.o drm_drv.o drm_managed.o hrtimer.o dma-fence-array.o drm_print.o drm_dumb_buffers.o bitmap.o string_helpers.o dma-resv.o
