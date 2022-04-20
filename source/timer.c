/*
 * timer.c
 *
 *  Created on: Feb 26, 2022
 *      Author: dreyceyalbin
 */
#define F_SYS_CLK (48000000L)
#include "timer.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#define LOAD_TICKS ((F_SYS_CLK / 16) / 16) //Alternative clock, 1/16 seconds



/*
 * Global time variables used for
 * measuring the time. The methods
 * now() and get_timer() retrieve
 * these values.
 */
volatile ticktime_t temp_time = 0;
volatile ticktime_t startup_time = 0;

// Defined in header file.
void init_systick(void) {
	// SysTick is defined in core_cm0.h
	SysTick->LOAD = (LOAD_TICKS) - 1; //ISR every 1/16 seconds
	// Set interrupt priority
	NVIC_SetPriority(SysTick_IRQn, 1);
	// Force load of reload value
	SysTick->VAL = 0;
	// Enable interrupt, enable SysTick timer
	SysTick->CTRL =  SysTick_CTRL_TICKINT_Msk |
			         SysTick_CTRL_ENABLE_Msk;
}

/* Based off Dean Listing 7.2 */
// Defined in header file.
void SysTick_Handler(void) {
	startup_time++;
	temp_time++;
}

// Defined in header file.
ticktime_t now() {
	return startup_time;
}

// Defined in header file.
void reset_timer() {
	temp_time = 0;
}

// Defined in header file.
ticktime_t get_timer() {
	return temp_time;
}

// Defined in header file.
void fractional_delay(uint16_t second_fractions)
{
	reset_timer();
	while(temp_time <= (ticktime_t)second_fractions)
	{
	}
}
