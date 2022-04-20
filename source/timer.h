/*
 * timer.h
 *
 *  Created on: Feb 26, 2022
 *      Author: dreyceyalbin
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>

typedef uint32_t ticktime_t;  // time since boot, in sixteenths of a second


/*
 * Function:
 *     void init_systick(void);
 * Description:
 *     This function initializes the timer for the
 *     SysTick timer.
 * INPUT:
 *     1. N/A - void
 * OUTPUT:
 *     1. N/A - void
 * ERROR CODE:
 *     1. N/A
 * NOTEs:
 */
void init_systick(void);


/*
 * Function:
 *     ticktime_t now();
 * Description:
 *     This function resets returns the global
 *     time in 1/16ths of a second
 * INPUT:
 *     1. N/A - void
 * OUTPUT:
 *     1. time in 1/16ths of a second since startup
 * ERROR CODE:
 *     1. N/A
 * NOTEs:
 * 		- returns value for the global variable 'startup_time'
 */
ticktime_t now(); // returns time since startup, in sixteenths of a second

/*
 * Function:
 *     void reset_timer();
 * Description:
 *     This function resets a temporary
 *     timer using SysTick.
 * INPUT:
 *     1. N/A - void
 * OUTPUT:
 *     1. N/A - void
 * ERROR CODE:
 *     1. N/A
 * NOTEs:
 * 		- resets the global variable 'temp_time'
 */
void reset_timer(); // resets timer to 0; doesn't affect now() values

/*
 * Function:
 *     ticktime_t get_timer();
 * Description:
 *     This function gets the value for a temporary/resetable
 *     timer using SysTick.
 * INPUT:
 *     1. N/A - void
 * OUTPUT:
 *     1. time in 1/16 of a second since the timer has started.
 * ERROR CODE:
 *      1. N/A
 * NOTEs:
 * 		- returns the global variable 'temp_time'
 */
ticktime_t get_timer();  // returns ticks since the last call to reset_timer()

/*
 * Function:
 *     fractional_delay(uint16_t second_fractions);
 * Description:
 *     This function delays real time for 1/16 of a second
 *     multiplied by the input value. Therfore, an input of 16 is
 *     equivilent to one second, N*16 is N seconds, and 4 is 250 ms.
 * INPUT:
 *     1. number of 1/16s of a second to delay
 * OUTPUT:
 *     1. N/A - void
 *     - delays time
 * ERROR CODE:
 *      1. N/A
 * NOTEs:
 */
void fractional_delay(uint16_t second_fractions);

#endif /* TIMER_H_ */
