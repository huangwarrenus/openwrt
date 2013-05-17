#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=cs1168
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/cs1168
  SUBMENU:=Other modules
  TITLE:=Poray CS1168 ADC Battery Voltage
  FILES:=$(PKG_BUILD_DIR)/cs1168.ko
  AUTOLOAD:=$(call AutoLoad,30,cs1168)
  KCONFIG:=
endef

define KernelPackage/cs1168/description
  Kernel module which implements cs1168 ADC battery voltage sampling for Poray routers
endef

EXTRA_KCONFIG:= \
	CONFIG_CS1168=m

EXTRA_CFLAGS:= \
	-DCONFIG_RALINK_RT5350=1 \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,cs1168))
