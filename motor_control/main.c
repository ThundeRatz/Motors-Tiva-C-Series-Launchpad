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

#define DELAY			400

#include <stdbool.h>
#include <stdint.h>
//#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

#include "system_clock.h"
#include "pwm.h"
#include "motors.h"
#include "uart.h"
#include "leds.h"
#include "voltwatch.h"

// The clock is left at default internal 16 MHz, but can be changed to higher/
// lower frequencies with SysCtlClockSet. If changed, update the value at
// system_clock.h

// If the FPU is used at main, please enable it at startup_gcc.c first

int main() {
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	// UART
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// PWM outputs
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// PWM outputs
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	// ADC
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	// board LEDs PWM and switches
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);		// PWM Module 0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		// PWM Module 1
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);	// UART 0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		// ADC0
	
	pwm_init();
	motors_init();
	leds_init();
	UART_init();
	voltwatch_init();
	// enable interrupts
	ROM_IntMasterEnable();
	
    for (;;) {
		uint8_t uart_value;
		uart_value = UARTCharGet(UART0_BASE);
		
		//pwm_value = uart_value * (SYSTEM_CLOCK / PWM_FREQ_HZ  / PWM_CLOCK_DIV) / 255;
		//pwm_value = (pwm_value == SYSTEM_CLOCK / PWM_FREQ_HZ  / PWM_CLOCK_DIV) ? SYSTEM_CLOCK / PWM_FREQ_HZ  / PWM_CLOCK_DIV - 1 : pwm_value;
		
		//leds_r(SYSTEM_CLOCK / PWM_FREQ_HZ  / PWM_CLOCK_DIV - 1);
		//leds_g(100);
		leds_b(uart_value);
    }
}
