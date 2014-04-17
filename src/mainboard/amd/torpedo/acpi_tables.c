/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <console/console.h>
#include <string.h>
#include <arch/acpi.h>
#include <arch/ioapic.h>
#include <arch/io.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <cpu/x86/msr.h>
#include "agesawrapper.h"
#include <cpu/amd/mtrr.h>
#include <cpu/amd/amdfam12.h>

extern u32 apicid_sb900;

extern const unsigned char AmlCode[];
extern const unsigned char AmlCode_ssdt[];


unsigned long acpi_fill_mcfg(unsigned long current)
{
  /* Just a dummy */
  return current;
}

unsigned long acpi_fill_madt(unsigned long current)
{

  /* create all subtables for processors */
  current += acpi_create_madt_lapic((acpi_madt_lapic_t *)current, 0, 0);
  current += acpi_create_madt_lapic((acpi_madt_lapic_t *)current, 1, 1);
  current += acpi_create_madt_lapic((acpi_madt_lapic_t *)current, 2, 2);
  current += acpi_create_madt_lapic((acpi_madt_lapic_t *)current, 3, 3);

  /* Write SB900 IOAPIC, only one */
  current += acpi_create_madt_ioapic((acpi_madt_ioapic_t *) current, apicid_sb900,
             IO_APIC_ADDR, 0);

  current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
            current, 0, 0, 2, 0);
  current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
            current, 0, 9, 9, 0xF);

  /* 0: mean bus 0--->ISA */
  /* 0: PIC 0 */
  /* 2: APIC 2 */
  /* 5 mean: 0101 --> Edige-triggered, Active high */

  /* create all subtables for processors */
  current += acpi_create_madt_lapic_nmi((acpi_madt_lapic_nmi_t *)current, 0, 5, 1);
  current += acpi_create_madt_lapic_nmi((acpi_madt_lapic_nmi_t *)current, 1, 5, 1);
  current += acpi_create_madt_lapic_nmi((acpi_madt_lapic_nmi_t *)current, 2, 5, 1);
  current += acpi_create_madt_lapic_nmi((acpi_madt_lapic_nmi_t *)current, 3, 5, 1);
  /* 1: LINT1 connect to NMI */

  return current;
}

unsigned long acpi_fill_hest(acpi_hest_t *hest)
{
	void *addr, *current;

	/* Skip the HEST header. */
	current = (void *)(hest + 1);

	addr = agesawrapper_getlateinitptr(PICK_WHEA_MCE);
	if (addr != NULL)
		current += acpi_create_hest_error_source(hest, current, 0, (void *)((u32)addr + 2), *(UINT16 *)addr - 2);

	addr = agesawrapper_getlateinitptr(PICK_WHEA_CMC);
	if (addr != NULL)
		current += acpi_create_hest_error_source(hest, current, 1, (void *)((u32)addr + 2), *(UINT16 *)addr - 2);

	return (unsigned long)current;
}

unsigned long acpi_fill_slit(unsigned long current)
{
  // Not implemented
  return current;
}

unsigned long acpi_fill_srat(unsigned long current)
{
  /* No NUMA, no SRAT */
  return current;
}

unsigned long write_acpi_tables(unsigned long start)
{
  unsigned long current;
  acpi_rsdp_t *rsdp;
  acpi_rsdt_t *rsdt;
#if 0 // Don't need HPET table.
  acpi_hpet_t *hpet;
#endif
  acpi_madt_t *madt;
  acpi_srat_t *srat;
  acpi_slit_t *slit;
  acpi_fadt_t *fadt;
  acpi_facs_t *facs;
  acpi_header_t *dsdt;
  acpi_header_t *ssdt;
  acpi_hest_t *hest;

  get_bus_conf(); /* it will get sblk, pci1234, hcdn, and sbdn */

  /* Align ACPI tables to 16 bytes */
  start = ALIGN(start, 16);
  current = start;

  printk(BIOS_INFO, "ACPI: Writing ACPI tables at %lx...\n", start);

  /* We need at least an RSDP and an RSDT Table */
  rsdp = (acpi_rsdp_t *) current;
  current += sizeof(acpi_rsdp_t);
  rsdt = (acpi_rsdt_t *) current;
  current += sizeof(acpi_rsdt_t);

  /* clear all table memory */
  memset((void *)start, 0, current - start);

  acpi_write_rsdp(rsdp, rsdt, NULL);
  acpi_write_rsdt(rsdt);

  /*
   * We explicitly add these tables later on:
   */
#if 0 // Don't need HPET table.
  current = (current + 0x07) & -0x08;
  printk(BIOS_DEBUG, "ACPI:    * HPET at %lx\n", current);
  hpet = (acpi_hpet_t *) current;
  current += sizeof(acpi_hpet_t);
  acpi_create_hpet(hpet);
  acpi_add_table(rsdp, hpet);
#endif

  /* If we want to use HPET Timers Linux wants an MADT */
  current = ALIGN(current, 8);
  printk(BIOS_DEBUG, "ACPI:    * MADT at %lx\n",current);
  madt = (acpi_madt_t *) current;
  acpi_create_madt(madt);
  current += madt->header.length;
  acpi_add_table(rsdp, madt);

  /* HEST */
  current = ALIGN(current, 8);
  hest = (acpi_hest_t *)current;
  acpi_write_hest((void *)current);
  acpi_add_table(rsdp, (void *)current);
  current += ((acpi_header_t *)current)->length;

  /* SRAT */
  current = ALIGN(current, 8);
  printk(BIOS_DEBUG, "ACPI:    * SRAT at %lx\n", current);
  srat = (acpi_srat_t *) agesawrapper_getlateinitptr (PICK_SRAT);
  if (srat != NULL) {
    memcpy((void *)current, srat, srat->header.length);
    srat = (acpi_srat_t *) current;
    //acpi_create_srat(srat);
    current += srat->header.length;
    acpi_add_table(rsdp, srat);
  }

  /* SLIT */
  current = ALIGN(current, 8);
  printk(BIOS_DEBUG, "ACPI:   * SLIT at %lx\n", current);
  slit = (acpi_slit_t *) agesawrapper_getlateinitptr (PICK_SLIT);
  if (slit != NULL) {
    memcpy((void *)current, slit, slit->header.length);
    slit = (acpi_slit_t *) current;
    //acpi_create_slit(slit);
    current += slit->header.length;
    acpi_add_table(rsdp, slit);
  }

  /* SSDT */
  current = ALIGN(current, 16);
  printk(BIOS_DEBUG, "ACPI:    * SSDT at %lx\n", current);
  ssdt = (acpi_header_t *)agesawrapper_getlateinitptr (PICK_PSTATE);
  if (ssdt != NULL) {
    memcpy((void *)current, ssdt, ssdt->length);
    ssdt = (acpi_header_t *) current;
    current += ssdt->length;
  }
  else {
    ssdt = (acpi_header_t *) current;
    memcpy(ssdt, &AmlCode_ssdt, sizeof(acpi_header_t));
    current += ssdt->length;
    memcpy(ssdt, &AmlCode_ssdt, ssdt->length);
   /* recalculate checksum */
    ssdt->checksum = 0;
    ssdt->checksum = acpi_checksum((unsigned char *)ssdt,ssdt->length);
  }
  acpi_add_table(rsdp,ssdt);

  printk(BIOS_DEBUG, "ACPI:    * SSDT for PState at %lx\n", current);

  /* DSDT */
  current = ALIGN(current, 8);
  printk(BIOS_DEBUG, "ACPI:    * DSDT at %lx\n", current);
  dsdt = (acpi_header_t *)current; // it will used by fadt
  memcpy(dsdt, &AmlCode, sizeof(acpi_header_t));
  current += dsdt->length;
  memcpy(dsdt, &AmlCode, dsdt->length);
  printk(BIOS_DEBUG, "ACPI:    * DSDT @ %p Length %x\n",dsdt,dsdt->length);

  /* FACS */ // it needs 64 bit alignment
  current = ALIGN(current, 8);
  printk(BIOS_DEBUG, "ACPI: * FACS at %lx\n", current);
  facs = (acpi_facs_t *) current; // it will be used by fadt
  current += sizeof(acpi_facs_t);
  acpi_create_facs(facs);

  /* FADT */
  current = ALIGN(current, 8);
  printk(BIOS_DEBUG, "ACPI:    * FADT at %lx\n", current);
  fadt = (acpi_fadt_t *) current;
  current += sizeof(acpi_fadt_t);

  acpi_create_fadt(fadt, facs, dsdt);
  acpi_add_table(rsdp, fadt);

  printk(BIOS_INFO, "ACPI: done.\n");
  return current;
}
