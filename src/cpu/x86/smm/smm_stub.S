/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 ChromeOS Authors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

/*
 * The stub is a generic wrapper for bootstrapping a C-based SMM handler. Its
 * primary purpose is to put the CPU into protected mode with a stack and call
 * into the C handler.
 *
 * The stub_entry_params structure needs to correspond to the C structure
 * found in smm.h.
 */

.code32
.section ".module_parameters", "aw", @progbits
stub_entry_params:
stack_size:
.long 0
stack_top:
.long 0
c_handler:
.long 0
c_handler_arg:
.long 0
/* struct smm_runtime begins here. */
smm_runtime:
smbase:
.long 0
save_state_size:
.long 0
/* apic_to_cpu_num is a table mapping the default APIC id to cpu num. If the
 * APIC id is found at the given index, the contiguous cpu number is index
 * into the table. */
apic_to_cpu_num:
.fill CONFIG_MAX_CPUS,1,0xff
/* end struct smm_runtime */

.data
/* Provide fallback stack to use when a valid cpu number cannot be found. */
fallback_stack_bottom:
.skip 128
fallback_stack_top:

.text
.code16
.global smm_handler_start
.global __rmodule_entry
__rmodule_entry:
smm_handler_start:
	movl	$(smm_relocate_gdt), %ebx
	data32	lgdt (%ebx)

	movl	%cr0, %eax
	andl	$0x1FFAFFD1, %eax /* CD,NW,PG,AM,WP,NE,TS,EM,MP = 0 */
	orl	$0x1, %eax        /* PE = 1 */
	movl	%eax, %cr0

	/* Enable protected mode */
	data32	ljmp $0x8, $smm_trampoline32

.align 4
smm_relocate_gdt:
	/* The first GDT entry is used for the lgdt instruction. */
	.word	smm_relocate_gdt_end - smm_relocate_gdt - 1
	.long	smm_relocate_gdt
	.word	0x0000

	/* gdt selector 0x08, flat code segment */
	.word	0xffff, 0x0000
	.byte	0x00, 0x9b, 0xcf, 0x00 /* G=1 and 0x0f, 4GB limit */

	/* gdt selector 0x10, flat data segment */
	.word	0xffff, 0x0000
	.byte	0x00, 0x93, 0xcf, 0x00
smm_relocate_gdt_end:

.align 4
.code32
.global smm_trampoline32
smm_trampoline32:
	/* Use flat data segment */
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	movw	%ax, %fs
	movw	%ax, %gs

	/* The CPU number is calculated by reading the initial APIC id. Since
	 * the OS can maniuplate the APIC id use the non-changing cpuid result
	 * for APIC id (ebx[31:24]). A table is used to handle a discontiguous
	 * APIC id space.  */
	mov	$1, %eax
	cpuid
	bswap	%ebx	/* Default APIC id in bl. */
	mov	$(apic_to_cpu_num), %eax
	xor	%ecx, %ecx

1:
	cmp	(%eax, %ecx, 1), %bl
	je	1f
	inc	%ecx
	cmp	$CONFIG_MAX_CPUS, %ecx
	jne	1b
	/* This is bad. One cannot find a stack entry because a cpu num could
	 * not be assigned. Use the fallback stack and check this condition in
	 * C handler. */
	movl	$(fallback_stack_top), %esp
	jmp	2f
1:
	movl	stack_size, %eax
	mul	%ecx
	movl	stack_top, %edx
	subl	%eax, %edx
	mov	%edx, %esp

2:
	/* Call into the c-based SMM relocation function with the platform
	 * parameters. Equivalent to:
	 *   struct arg = { c_handler_params, cpu_num, smm_runtime {;
	 *   c_handler(&arg)
	 */
	push	$(smm_runtime)
	push	%ecx
	push	c_handler_arg
	push	%esp
	mov	c_handler, %eax
	call	*%eax

	/* Exit from SM mode. */
	rsm

