#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# -- Luis Soltero <lsoltero@globalmarinenet.com> --
#

include $(TOPDIR)/rules.mk

PKG_NAME    := X5-Battery
PKG_RELEASE := 1.0

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk

define Package/X5-Battery
	SECTION  := Poray 
	CATEGORY := Poray
	TITLE    := Poray X5 and X6 battery level support 
	DEPENDS  := +kmod-cs1168
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/ 
endef

define Package/X5-Battery/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/batlvl $(1)/usr/bin/batlvl
endef

$(eval $(call BuildPackage,X5-Battery))
