#
# Copyright (C) 2013 Felix Kaechele <felix@fetzig.org>
#                    Luis Soltero <lsoltero@globalmarinenet.com>
#                    Michel Stempin <michel.stempin@wanadoo.fr>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/TOPAP01
	NAME:=TopLink TOP-AP01
	PACKAGES:=kmod-usb-core kmod-usb2 kmod-ledtrig-netdev kmod-ledtrig-timer
endef
define Profile/TOPAP01/Description
	Package set for TopLink TOP-AP01 board
endef

$(eval $(call Profile,TOPAP01))
