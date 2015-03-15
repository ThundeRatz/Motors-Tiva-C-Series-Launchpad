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

#define MOTOR_RIGHT	(1 << 7)
#define MOTOR_REV	(1 << 6)

///@defgroup motors Motors
///@{

/// Left speed command received from UART0
uint8_t speed_left;

/// Right speed command received from UART0
uint8_t speed_right;

/**
 * Initialize motors PWM output.
 * Configure M0PWM2, M0PWM3, M0PWM7 and M0PWM8 at PB4, PB5, PC4 and PC5,
 * respectively. Notice that PWMs are limited to 64/max_duty_cicle operation
 * (in our case, 2/3 or 1/2, because we use 16V or 12V motors with a 24V battery)
 * The max. duty cicle can be configured at the source file (see
 * ROM_PWMGenPeriodSet at motors.c). The motor controller board has pull-down
 * resistors at FET drivers input to drive outputs low while the Tiva is off/
 * initializing.
 */
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

/**
 * Parse UART0 command.
 * Parse command received from UART0 and set speed_left and speed_right
 * accordingly.
 * @param[in] command
 * @parblock
 * command received. Bit 7 chooses the motor (0 = left, 1 = right). Bit 6
 * reverses direction. Bits 0..5 choose speed.
 * @endparblock
 */
void motors(uint8_t command) {
	if (command & MOTOR_RIGHT)
		if (command & MOTOR_REV)
			speed_right = -(command & 0x3f);
		else
			speed_right = command & 0x3f;
	else
		if (command & MOTOR_REV)
			speed_left = -(command & 0x3f);
		else
			speed_left = command;
}

/**
 * Set left motor PWM.
 * Set PWM duty cicle.
 * @param[in] pwm_value new value
 */
void motor_left(int8_t pwm_value) {
	if (pwm_value < 0) {
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, -pwm_value);
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
	} else {
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, pwm_value);
	}
}

/**
 * Set right motor PWM.
 * Set PWM duty cicle.
 * @param[in] pwm_value new value
 */
void motor_right(int8_t pwm_value) {
	if (pwm_value < 0) {
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, -pwm_value);
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);
	} else {
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 0);
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, pwm_value);
	}
}

///@}
