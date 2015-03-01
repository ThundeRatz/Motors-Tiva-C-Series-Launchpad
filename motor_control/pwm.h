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

#ifndef __PWM_H__

static inline void pwm_init() {
	// Set the clock divider. Specific configurations for PWM modules, banks and
	// pins are left for other files.
#if PWM_CLOCK_DIV == 1
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
#elif PWM_CLOCK_DIV == 2
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_2);
#elif PWM_CLOCK_DIV == 4
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
#elif PWM_CLOCK_DIV == 8
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
#elif PWM_CLOCK_DIV == 16
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_16);
#elif PWM_CLOCK_DIV == 32
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_32);
#elif PWM_CLOCK_DIV == 64
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
#else
#error Invalid PWM clock divider
#endif
}

#endif
