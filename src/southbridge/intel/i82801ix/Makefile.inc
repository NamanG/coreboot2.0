##
## This file is part of the coreboot project.
##
## Copyright (C) 2008-2009 coresystems GmbH
##               2012 secunet Security Networks AG
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc.
##

ifeq ($(CONFIG_SOUTHBRIDGE_INTEL_I82801IX),y)

ramstage-y += i82801ix.c
ramstage-y += pci.c
ramstage-y += lpc.c
ramstage-y += pcie.c
ramstage-y += usb_ehci.c
ramstage-y += sata.c
ramstage-y += hdaudio.c
ramstage-y += thermal.c
ramstage-y += smbus.c
ramstage-y += ../common/pciehp.c

ramstage-srcs += src/mainboard/$(MAINBOARDDIR)/hda_verb.c

ramstage-y += ../i82801gx/reset.c
ramstage-y += ../i82801gx/watchdog.c

ramstage-$(CONFIG_HAVE_SMI_HANDLER) += smi.c
smm-$(CONFIG_HAVE_SMI_HANDLER) += smihandler.c

romstage-y += early_init.c
romstage-y += early_smbus.c
romstage-y += dmi_setup.c

endif
