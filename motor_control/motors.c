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

#define MOTOR_RIGHT	(1 << 7)
#define MOTOR_REV	(1 << 6)

void motors_init() {
    // Configure PB4, PB5, PC4, PC5 as PWM
    ROM_GPIOPinConfigure(GPIO_PB4_M0PWM2);
    ROM_GPIOPinConfigure(GPIO_PB5_M0PWM3);
    ROM_GPIOPinConfigure(GPIO_PC4_M0PWM6);
    ROM_GPIOPinConfigure(GPIO_PC5_M0PWM7);
    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    ROM_GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    
    // PWM_GEN_1 covers M0PWM2 and M0PWM3
    // PWM_GEN_3 covers M0PWM6 and M0PWM7
    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
	PWM_GEN_MODE_DB_NO_SYNC| PWM_GEN_MODE_FAULT_UNLATCHED | PWM_GEN_MODE_FAULT_NO_MINPER);
    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
	PWM_GEN_MODE_DB_NO_SYNC | PWM_GEN_MODE_FAULT_UNLATCHED | PWM_GEN_MODE_FAULT_NO_MINPER);
    
    // Set the Period (in clock ticks)
    // Frequency = (SYSTEM_CLOCK / PWM_CLOCK_DIV) / (ui32Gen)
    // Period (seconds) = PWM_CLOCK_DIV * ui32Gen / SYSTEM_CLOCK
    // Here, frequency = 1953.125
    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 128);
    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 128);
    
    // Enable the PWM generator
    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    
    // Enable PWM
    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
}

void motors(int8_t command) {
    if (command & MOTOR_RIGHT)
	if (command & MOTOR_REV) {
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, command & 0x3f);
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);
	} else {
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 0);
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, command & 0x3f);
	}
    else
	if (command & MOTOR_REV) {
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, command & 0x3f);
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
	} else {
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
	    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, command);
	}
}
