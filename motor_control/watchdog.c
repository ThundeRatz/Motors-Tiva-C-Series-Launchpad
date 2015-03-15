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
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/watchdog.h"
#include "driverlib/rom.h"

void watchdog_ISR();

///@defgroup watchdog Watchdog
///@{

/** 
 * Initialize the watchdog.
 * Initialize WATCHDOG0_BASE with reset capability, debug stall and highest
 * priority and lock it.
 */
void watchdog_init() {
	ROM_WatchdogResetEnable(WATCHDOG0_BASE);
	ROM_WatchdogStallEnable(WATCHDOG0_BASE);
	ROM_WatchdogReloadSet(WATCHDOG0_BASE, 30000000);
	ROM_IntPrioritySet(INT_WATCHDOG, 0);
	///@todo Is this even needed?
	WatchdogIntRegister(WATCHDOG0_BASE, watchdog_ISR);
	ROM_WatchdogEnable(WATCHDOG0_BASE);
	ROM_WatchdogIntEnable(WATCHDOG0_BASE);
	ROM_WatchdogLock(WATCHDOG0_BASE);
}

/** 
 * Reset watchdog.
 * Unlock and reset WATCHDOG0_BASE.
 */
void watchdog_reset() {
	ROM_WatchdogUnlock(WATCHDOG0_BASE);
	ROM_WatchdogReloadSet(WATCHDOG0_BASE, 30000000);
	ROM_WatchdogLock(WATCHDOG0_BASE);
}

/** 
 * Watchdog Interrupt Service Routine.
 * Called at watchdog interrupt. Will do a software reset of the processor.
 */
void watchdog_ISR() {
	// Reset on first timeout
	// If the ISR is misconfigured, the interrupt flag will be left set and the
	// watchdog will reset on second timeout
	ROM_SysCtlReset();
}

///@}
