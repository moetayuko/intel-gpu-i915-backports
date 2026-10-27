ccflags-y += -I$(src) -Wframe-larger-than=1280
ifeq ($(CONFIG_BACKPORT_INTEGRATE),)
obj-m += i915-compat.o
else
obj-y += i915-compat.o
endif
i915-compat-y += main.o hrtimer.o bitmap.o string_helpers.o
ifneq ($(CPTCFG_DRM_BACKPORT),y)
i915-compat-y += drm_drv.o drm_managed.o dma-fence-array.o drm_print.o drm_dumb_buffers.o dma-resv.o
endif
ifeq ($(CPTCFG_DRM_BACKPORT),y)
i915-compat-y += pci-rebar.o mm-file.o vmap-pfn.o int_log.o fb_io_fops.o debugfs-string.o
endif
