//*****************************************************************************
// ThundeRatz Robotics Team 2015. Some rights reserved.
// Code licensed under a Creative Commons Attribution 4.0 International License
// (http://creativecommons.org/licenses/by/4.0/).
//
// You are free to:
// Share — copy and redistribute the material in any medium or format.
// Adapt — remix, transform, and build upon the material for any purpose, even
// commercially.
//
// The licensor cannot revoke these freedoms as long as you follow the license
// terms.
//
// Under the following terms:
// Attribution — You must give appropriate credit, provide a link to the license,
// and indicate if changes were made. You may do so in any reasonable manner,
// but not in any way that suggests the licensor endorses you or your use.
// No additional restrictions — You may not apply legal terms or technological
// measures that legally restrict others from doing anything the license permits.
//
// Notices:
//
// You do not have to comply with the license for elements of the material in
// the public domain or where your use is permitted by an applicable exception
// or limitation.
//
// No warranties are given. The license may not give you all of the permissions
// necessary for your intended use. For example, other rights such as publicity,
// privacy, or moral rights may limit how you use the material.
//
// Full licence: http://creativecommons.org/licenses/by/4.0/legalcode
//*****************************************************************************

//*****************************************************************************
// Code hosted at: https://github.com/ThundeRatz/Motors-Tiva-C-Series-Launchpad.
//
// Uses the Tiva Firmware Development Package ("TivaWare", under BSD licence).
//
// Lost? For help with the development environment:
// https://github.com/ThundeRatz/trekking-magellan-docs
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"

#include "system_clock.h"
#include "motors.h"
#include "watchdog.h"

#include "leds.h"

///@defgroup uart UART0 command receiver
///@{

/**
 * Initialize UART.
 * Initialize UART0 for receiving commands. Uses pins PA0 and PA1 and 2400 baud.
 * Its interrupt has priority 1.
 */
void UART_init() {
	// Configure PA0, PA1 as UART
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	ROM_UARTConfigSetExpClk(UART0_BASE, SYSTEM_CLOCK, 2400,
		(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	ROM_IntPrioritySet(INT_UART0, 1 << 5);
	ROM_IntEnable(INT_UART0);
	ROM_UARTIntEnable(UART0_BASE, UART_INT_OE | UART_INT_BE |
		UART_INT_PE | UART_INT_FE | UART_INT_RT | UART_INT_RX);
}

#ifdef DEBUG
// 0 = UART_INT_RI
// 1 = UART_INT_CTS
// 2 = UART_INT_DCD
// 3 = UART_INT_DSR
// 4 = UART_INT_RX
// 5 = UART_INT_TX
// 6 = UART_INT_RT
// 7 = UART_INT_FE
// 8 = UART_INT_PE
// 9 = UART_INT_BE
// 10 = UART_INT_OE
// 12 = UART_INT_9BIT
// 16 = UART_INT_DMARX
// 17 = UART_INT_DMATX
// Set unused numbers to 0xffffffff to identify them easily when debugging
static unsigned int error_count[32] = {[11] = 0xffffffff, [13] = 0xffffffff,
	[14] = 0xffffffff, [15] = 0xffffffff, [18 ... 31] = 0xffffffff};
#endif

/**
 * UART Interrupt Service Routine.
 * Calls motors() with received commands.
 * @see motors
 */
void UART_ISR() {
	uint32_t int_status;
	leds_b(4000);
	int_status = ROM_UARTIntStatus(UART0_BASE, true);
	ROM_UARTIntClear(UART0_BASE, int_status);
	
	// Iterate over each status bit
	for (; int_status; int_status &= int_status - 1) {
		switch (int_status & -int_status) {
			case UART_INT_RX:
			watchdog_reset();
			while (ROM_UARTCharsAvail(UART0_BASE))
				motors(ROM_UARTCharGet(UART0_BASE));
			break;
			
			case UART_INT_OE: case UART_INT_BE:
			case UART_INT_PE: case UART_INT_FE:
			case UART_INT_RT:
			leds_status(4095);
#ifdef DEBUG
			error_count[__builtin_clz(int_status)]++;
#endif
			break;
		}
	}
}

///@}
