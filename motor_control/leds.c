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

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"

#include "system_clock.h"
#include "leds.h"

void leds_init() {
	// Configure PF1, PF2, PF3 (RGB LED), PB6 (STATUS) and PB7 (L. BAT) as PWM
	ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
	ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
	ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	
	// PWM_GEN_2 covers M1PWM4 and M1PWM5
	// PWM_GEN_3 covers M1PWM6 and M1PWM7
    // PWM_GEN_MODE_DBG_RUN configures the PWM to keep sending signals when the
    // board is stopped by a debugger. Use for LEDs and other safe stuff, but
    // BE CAREFUL with motors (we don't want the robot to keep running if
    // accidentally interrupted by ICDI!)
    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
		PWM_GEN_MODE_DBG_RUN | PWM_GEN_MODE_DB_NO_SYNC| PWM_GEN_MODE_FAULT_UNLATCHED |
		PWM_GEN_MODE_FAULT_NO_MINPER);
    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
		PWM_GEN_MODE_DBG_RUN | PWM_GEN_MODE_DB_NO_SYNC | PWM_GEN_MODE_FAULT_UNLATCHED |
		PWM_GEN_MODE_FAULT_NO_MINPER);
	
	// PWM_GEN_0 covers M0PWM0 and M0PWM1
    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
		PWM_GEN_MODE_DBG_RUN | PWM_GEN_MODE_DB_NO_SYNC| PWM_GEN_MODE_FAULT_UNLATCHED |
		PWM_GEN_MODE_FAULT_NO_MINPER);
	
	// Set the Period (in clock ticks)
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 4096);
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 4096);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 4096);
	
	// Enable the PWM generator
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	
	// Enable PWM
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);
}
