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
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"

#include "leds.h"

///@defgroup voltwatch Voltwatch
///@{

/**
 * Initialize voltwatch.
 * Initialize ADC0 and use SS3 to sample battery voltage and issue an interrupt.
 * Has lowest priority (7).
 */
void voltwatch_init() {
	ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	
	ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_EIGHTH, 1);
	ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 32);
	
	///@todo Change to ADC_TRIGGER_TIMER and configure a timer
	ROM_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_ALWAYS, 0);
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_END |
		ADC_CTL_IE);
	
	ROM_ADCSequenceEnable(ADC0_BASE, 3);
	ROM_IntPrioritySet(INT_ADC0SS3, 7 << 5);
	ROM_IntEnable(INT_ADC0SS3);
	ROM_ADCIntEnable(ADC0_BASE, 3);
}

/**
 * Voltwatch Interrupt Service Routine.
 * Called when the ADC has data for us. Sets red and green led according to
 * battery voltage.
 */
void voltwatch_ISR() {
	uint32_t voltage;
	
	ROM_ADCIntClear(ADC0_BASE, 3);
	// If a FIFO buffer is needed (eg. you need a series of measurements with
	// small periods), sampler 0 has a 8 sample buffer and 1 and 2 have a
	// 4 sample buffer
	ROM_ADCSequenceDataGet(ADC0_BASE, 3, &voltage);
	
	leds_r(4095 - voltage);
	leds_g(voltage);
}

///@}
