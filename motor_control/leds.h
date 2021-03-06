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

#ifndef __LEDS_H__
#define __LEDS_H__

#include "driverlib/pwm.h"
#include "driverlib/rom.h"

void leds_init();

///@addtogroup leds
///@{

/**
 * Set red LED.
 * @param[in] brightness red value
 */
static inline void leds_r(uint32_t brightness) {
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, brightness);
}

/**
 * Set blue LED.
 * @param[in] brightness blue value
 */
static inline void leds_b(uint32_t brightness) {
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, brightness);
}

/**
 * Set green LED.
 * @param[in] brightness green value
 */
static inline void leds_g(uint32_t brightness) {
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, brightness);
}

/**
 * Set status LED.
 * @param[in] brightness status brightness value
 */
static inline void leds_status(uint32_t brightness) {
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, brightness);
}

/**
 * Set low battery LED.
 * @param[in] brightness low battery brightness value
 */
static inline void leds_lbatt(uint32_t brightness) {
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, brightness);
}

#endif

///@}
