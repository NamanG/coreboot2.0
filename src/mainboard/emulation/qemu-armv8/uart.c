/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Naman Govil, <namangov@gmail.com> 
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <console/console.h>
#include <uart.h>

#define UART0_IO_ADDRESS	(0x1C090000)

static void pl011_init_dev(void) {
	/*Do we need to make the pins 0 and 8 high?
	 * */
}

static void pl011_uart_tx_byte(unsigned char data) {
	static volatile uint32_t  *uart0_address =
		(uint32_t *)UART0_IO_ADDRESS;

	*uart0_address = (uint32_t)data;
}

static void pl011_uart_tx_flush(void) {
}

#if !defined(__PRE_RAM__)
static const struct console_driver pl011_uart_console __console = {
	.init     = pl011_init_dev,
	.tx_byte  = pl011_uart_tx_byte,
	.tx_flush = pl011_uart_tx_flush,
};

uint32_t uartmem_getbaseaddr(void)
{
	return UART0_IO_ADDRESS;
}

#else
void uart_init(void){
	pl011_init_dev();
}

void uart_tx_byte(unsigned char data){
	pl011_uart_tx_byte(data);
}

void uart_tx_flush(void) {
	pl011_uart_tx_flush();
}
#endif
