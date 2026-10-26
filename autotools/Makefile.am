#
# Backports Makefile
#

#------------------------------------------------------------------------------
#
# GLOBAL VARIABLES
#
##------------------------------------------------------------------------------

#
# KERNEL VARIABLES
#

#
# BACKPORT VARIABLES
#

#
# OSV GENERIC VARIABLES
#

# Kernel build directory - defaults to running kernel
KLIB ?= /lib/modules/@DOLLAR_SIGN@(shell uname -r)
KLIB_BUILD ?= @DOLLAR_SIGN@(KLIB)/build

RELEASE = $(if $(BUILD_VERSION),$(BUILD_VERSION),1)

# Extract kernel version from utsrelease.h
# Uses precise '^#define UTS_RELEASE ' pattern to avoid matching UTS_UBUNTU_RELEASE_ABI.
# Falls back to uname -r if headers are not available.
TARGET_KERN_VER=@DOLLAR_SIGN@(shell cat @DOLLAR_SIGN@(KLIB_BUILD)/include/generated/utsrelease.h 2>/dev/null | grep '^#define UTS_RELEASE ' | cut -d '"' -f2 || uname -r)
FULL_KERN_STR=@DOLLAR_SIGN@(shell echo "@DOLLAR_SIGN@(TARGET_KERN_VER)" | sed 's/\.el[0-9]*_[0-9]*\.x86_64//' | sed 's/\.x86_64//' | sed 's/-[a-zA-Z].*//' | tr -d '+')
KER_VER=@DOLLAR_SIGN@(subst -,.,@DOLLAR_SIGN@(FULL_KERN_STR))

# BKPT_VER consist of backported release version.
# Expected input: 'BACKPORTS_RELEASE_TAG="xeb_v6.17.13.49_260409.4"'
BKPT_VER=@DOLLAR_SIGN@(shell cat src/versions | grep BACKPORTS_RELEASE_TAG | cut -d '=' -f 2 | tr -d '"' | cut -d '_' -f 3 | cut -d '-' -f 1 2>/dev/null || echo 1)

# DEV_TAG is extracted from BASE_KERNEL_TAG.
# Expected format: "xe-v6.17.13.49" -> extracts "v6.17.13.49" and removes 'v' prefix
DEV_TAG_RAW=@DOLLAR_SIGN@(shell cat src/versions | grep BASE_KERNEL_TAG | cut -f 2 -d "\"" | cut -d "-" -f 2 2>/dev/null || echo 1)
DEV_TAG=@DOLLAR_SIGN@(shell echo @DOLLAR_SIGN@(DEV_TAG_RAW) | sed 's/^v//')

# Debian package version: 0.<BKPT_VER> (e.g., 0.260409.5)
# RPM package version: kernel version (e.g., 6.12.0.124.8.1)
# Note: VERSION is reserved by automake (set via AC_INIT in configure.ac), use KERN_PKG_VERSION for RPM
KERN_PKG_VERSION=$(KER_VER)
RELEASE_VERSION=$(DEV_TAG).$(BKPT_VER)

# Distro suffix (e.g., el10_1) for RPM packaging - supports RHEL 9 and 10 only
DISTRO_SUFFIX=@DOLLAR_SIGN@(shell cat $(KLIB_BUILD)/include/generated/utsrelease.h 2>/dev/null | grep UTS_RELEASE | cut -d '"' -f2 | grep -oE 'el(9|10)_[0-9]+' | head -1)

#------------------------------------------------------------------------------
#
# DKMS Package Targets
#
#------------------------------------------------------------------------------

I915_PKG_NAME_BASENAME=intel-i915-dkms
I915_PKG_NAME=$(I915_PKG_NAME_BASENAME)

# Debian version format: <KER_VER>-<RELEASE_VERSION> e.g. intel-i915-dkms_6.12.0.124.8.1-6.17.13.49.260409.4+i1-1_all.deb
I915_PKG_VERSION=$(KER_VER)-$(RELEASE_VERSION)
I915_PKG_RELEASE=$(RELEASE)

# RPM version format: <kernel-ver>-<dev_tag>.<bkpt_ver>.<release> e.g. intel-i915-dkms-6.12.0.124.8.1-6.17.13.54.260409.5.1.el10_1.x86_64.rpm
I915_RPM_PKG_VERSION=$(KERN_PKG_VERSION)
I915_RPM_PKG_RELEASE=$(RELEASE_VERSION).$(RELEASE)

#------------------------------------------------------------------------------
#
# Debian Package Targets
#
#------------------------------------------------------------------------------
BACKPORT_DIR = src

#read default version from changelog.in and replace it with proper version info during package generation
DEF_VER=@DOLLAR_SIGN@(shell cat debian/changelog.in | head -1 | cut -d '(' -f 2 | cut -d ')' -f 1)

#------------------------------------------------------------------------------
# Debian DKMS Package Targets
#------------------------------------------------------------------------------

# i915-dkms-deb-pkggg
# Creates the i915 backport infrastructure DKMS Debian package
#------------------------------------------------------------------------------


export KBUILD_ONLYI915DIRS := src/drivers/gpu src/include src/scripts src/drivers/platform
export KBUILD_AUTOTOOLS := m4 AUTHORS  ChangeLog  configure.ac  COPYING  INSTALL Makefile  Makefile.am  NEWS  README.md compile.sh
I915_TAR_CONTENT=$(KBUILD_ONLYI915DIRS) $(KBUILD_AUTOTOOLS) src/Makefile* src/local-symbols src/MAINTAINERS src/local-symbols-autoconf \
	src/Kconfig src/Kconfig.sources src/Kconfig.package.hacks src/COPYING src/versions src/defconfigs src/backport-include src/kconf src/compat

I915_DKMS_MK_CONTROL=$(BACKPORT_DIR)/scripts/backport-mkdebcontrol
I915_DKMS_MK_RULES=$(BACKPORT_DIR)/scripts/backport-mkdebrules
I915_DKMS_MK_DKMS=$(BACKPORT_DIR)/scripts/backport-mki915debdkms
I915_DKMS_MK_README=$(BACKPORT_DIR)/scripts/backport-mkdebreadme
I915_DKMS_MK_COPYRIGHT=$(BACKPORT_DIR)/scripts/backport-mkdebcopyright

#------------------------------------------------------------------------------
#
# RPM Package Targets
#
#------------------------------------------------------------------------------

I915_RPM_MK_SPEC=$(BACKPORT_DIR)/scripts/backport-mkdrmdkmsspec
I915_RPM_MK_DKMS=$(BACKPORT_DIR)/scripts/backport-mkdrmdkmsconf

.PHONY: i915-dkms-deb-pkg
i915-dkms-deb-pkg: clean
	$(CONFIG_SHELL) $(I915_DKMS_MK_CONTROL) -n $(I915_PKG_NAME) -v $(I915_PKG_VERSION) -r $(I915_PKG_RELEASE) -z dkms > debian/control
	$(CONFIG_SHELL) $(I915_DKMS_MK_RULES) -n $(I915_PKG_NAME) -v $(I915_PKG_VERSION) -r $(I915_PKG_RELEASE) -z dkms > debian/rules
	cp debian/changelog.in debian/changelog
	sed -i 's/pkg-name/$(I915_PKG_NAME)/g' debian/changelog
	sed -i 's/$(DEF_VER)/$(I915_PKG_VERSION)/g' debian/changelog
	for i in $(I915_TAR_CONTENT); do  	\
		z=$$(expr index $$i "=")	;\
		if [ $$z = 0 ]; then		\
			x=$$(expr index $$i "/");\
			if [ $$x != 0 ]; then	\
				y=`dirname $$i`	;\
				echo "$$i /usr/src/@BACKPORT_PACKAGE_NAME@-@BACKPORT_PACKAGE_VERSION@/$$y";\
			else 	\
				echo "$$i /usr/src/@BACKPORT_PACKAGE_NAME@-@BACKPORT_PACKAGE_VERSION@";\
			fi		\
		fi 		\
	done > debian/$(I915_PKG_NAME).install.in
	$(CONFIG_SHELL) $(I915_DKMS_MK_DKMS) -n $(I915_PKG_NAME) -v $(I915_PKG_VERSION) -r $(I915_PKG_RELEASE) > debian/$(I915_PKG_NAME).dkms.in
	$(CONFIG_SHELL) $(I915_DKMS_MK_README) -n $(I915_PKG_NAME) -v $(I915_PKG_VERSION) -r $(I915_PKG_RELEASE) > debian/README.Debian
	$(CONFIG_SHELL) $(I915_DKMS_MK_COPYRIGHT) -n $(I915_PKG_NAME) -v $(I915_PKG_VERSION) -r $(I915_PKG_RELEASE) > debian/copyright
	+dch -l "+i${I915_PKG_RELEASE}-" -m "build ${I915_PKG_RELEASE}"
	+dpkg-buildpackage -j`nproc --all` -us -uc -b -rfakeroot

.PHONY: i915-dkms-rpm-pkg
i915-dkms-rpm-pkg: clean
	mkdir -p rpm
	mkdir -p ~/rpmbuild/SOURCES
	@DOLLAR_SIGN@(CONFIG_SHELL) @DOLLAR_SIGN@(I915_RPM_MK_SPEC) -n @DOLLAR_SIGN@(I915_PKG_NAME) -v @DOLLAR_SIGN@(I915_RPM_PKG_VERSION) -r @DOLLAR_SIGN@(I915_RPM_PKG_RELEASE) @DOLLAR_SIGN@(if @DOLLAR_SIGN@(DISTRO_SUFFIX),-d @DOLLAR_SIGN@(DISTRO_SUFFIX)) -z dkms > rpm/@DOLLAR_SIGN@(I915_PKG_NAME).spec
	@DOLLAR_SIGN@(CONFIG_SHELL) @DOLLAR_SIGN@(I915_RPM_MK_DKMS) -n @DOLLAR_SIGN@(I915_PKG_NAME) -v @DOLLAR_SIGN@(I915_RPM_PKG_VERSION) -r @DOLLAR_SIGN@(I915_RPM_PKG_RELEASE) > rpm/@DOLLAR_SIGN@(I915_PKG_NAME).dkms.conf
	cp -r src m4 debian Makefile* configure* aclocal.m4 AUTHORS ChangeLog COPYING INSTALL NEWS README.md compile.sh ~/rpmbuild/SOURCES/ 2>/dev/null || true
	+rpmbuild -bb --define "_sourcedir @DOLLAR_SIGN@(PWD)" rpm/@DOLLAR_SIGN@(I915_PKG_NAME).spec

all clean:
	$(MAKE) -C src $@

olddefconfig menuconfig savedefconfig:
	$(MAKE) -C src KLIB=$(LINUX_OBJ) KLIB_BUILD=$(LINUX_OBJ) $@


.PHONY: modules
modules: clean
	$(shell ./src/scripts/create_automacro_h.sh -h $(LINUX_OBJ) -b $(BACKPORT_DIR))
	$(MAKE) -C src KLIB=$(LINUX_OBJ) KLIB_BUILD=$(LINUX_OBJ) $@

#------------------------------------------------------------------------------
#
# Make Targets
#
# Add all make targets for generative, packaging, build Targets.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#
# Make build clean Targets
#
#------------------------------------------------------------------------------

# Make Mr proper to clean-up all generated files.
# Versions file will not be cleaned-up to maintain built kernel versions.
#------------------------------------------------------------------------------
.PHONY: mrproper
mrproper:
	@rm -f debian/README.Debian
	@rm -f debian/changelog
	@rm -f debian/control
	@rm -f debian/copyright
	@rm -f debian/$(I915_PKG_NAME_BASENAME)*.dkms.in
	@rm -f debian/$(I915_PKG_NAME_BASENAME)*.install.in
	@rm -rf debian/$(I915_PKG_NAME_BASENAME)*
	@rm -f debian/rules
	@rm -f src/dkms.conf
	@test -f .config && $(MAKE) clean || true


PHONY = FORCE
FORCE:

#------------------------------------------------------------------------------
#
# Help Target
#
#------------------------------------------------------------------------------
.PHONY: help
help:
	@echo "Available make targets:"
	@echo ""
	@echo "  Build targets:"
	@echo "    all                 - Build all kernel modules"
	@echo "    modules             - Build kernel modules (requires LINUX_OBJ)"
	@echo "    clean               - Remove build artifacts"
	@echo "    mrproper            - Deep clean all generated files (debian, rpm, config)"
	@echo ""
	@echo "  Packaging targets:"
	@echo "    i915-dkms-deb-pkg     - Build i915 infrastructure DKMS Debian package (.deb)"
	@echo "    i915-dkms-rpm-pkg     - Build i915 infrastructure DKMS RPM package (.rpm)"
	@echo ""
	@echo "  Kernel config targets:"
	@echo "    olddefconfig        - Update .config using default symbol values"
	@echo "    menuconfig          - Interactive kernel configuration menu"
	@echo "    savedefconfig       - Save minimal .config as defconfig"
	@echo ""
	@echo "  Variables (can be overridden on command line):"
	@echo "    KLIB                - Kernel modules directory (default: /lib/modules/\$$(uname -r))"
	@echo "    KLIB_BUILD          - Kernel build directory (default: \$$(KLIB)/build)"
	@echo "    LINUX_OBJ           - Kernel object directory (required for modules/config targets)"
	@echo "    BUILD_VERSION       - Override RELEASE version (default: 1)"
