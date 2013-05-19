#
# Copyright (C) 2013 Felix Kaechele <felix@fetzig.org>
#                    Luis Soltero <lsoltero@globalmarinenet.com>
#                    Michel Stempin <michel.stempin@wanadoo.fr>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/MPRA1
	NAME:=Hame MPR-A1
	PACKAGES:=kmod-usb-core kmod-usb2 kmod-ledtrig-netdev kmod-ledtrig-timer
endef

define Profile/MPRA1/Description
	Package set for Hame MPR-A1 board
endef

$(eval $(call Profile,MPRA1))
