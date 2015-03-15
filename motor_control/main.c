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

#define RAMP			400

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

#include "pwm.h"
#include "motors.h"
#include "uart.h"
#include "leds.h"
#include "voltwatch.h"
#include "watchdog.h"

// The clock is left at default internal 16 MHz, but can be changed to higher/
// lower frequencies with SysCtlClockSet. If changed, update the value at
// system_clock.h

// If the FPU is used at main, please enable it at startup_gcc.c first

int main() {
	uint32_t reset_cause = ROM_SysCtlResetCauseGet();
	ROM_SysCtlResetCauseClear(reset_cause);
	
	if (reset_cause == SYSCTL_CAUSE_SW || reset_cause == SYSCTL_CAUSE_WDOG0)
		leds_status(4095);
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	// UART
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// PWM outputs
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// PWM outputs
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	// ADC
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	// board LEDs PWM and switches
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);		// PWM Module 0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		// PWM Module 1
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);	// UART 0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		// ADC0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);	// Watchdog 0
	
	pwm_init();
	motors_init();
	leds_init();
	UART_init();
	voltwatch_init();
	watchdog_init();
	// Enable interrupts
	ROM_IntMasterEnable();
	
	int8_t curr_speed_left = 0, curr_speed_right = 0;
    for (;;) {
		if (curr_speed_left != speed_left) {
			if (curr_speed_left < speed_left)
				curr_speed_left++;
			else
				curr_speed_left--;
			motor_left(curr_speed_left);
		}
		if (curr_speed_right != speed_right) {
			if (curr_speed_right < speed_right)
				curr_speed_right++;
			else
				curr_speed_right--;
			motor_right(curr_speed_right);
		}
		
		if (curr_speed_right == 0 && curr_speed_left == 0)
			watchdog_reset();
		
		ROM_SysCtlDelay(RAMP);
	}
}
