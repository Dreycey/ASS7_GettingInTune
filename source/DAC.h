/*
 * DAC.h
 *
 *  Created on: Apr 18, 2022
 *      Author: dreyceyalbin
 */

#ifndef DAC_H_
#define DAC_H_

#define SYST_CLOCK (48E6)
#define SAMPLE_FREQ (48E3)
#define BUFFER_SIZE (1024)
#include "MKL25Z4.h"
#include <stdio.h>
#include <string.h>

/*
 * Buffer for holding tunes.
 */
struct TuneBuffer {
	uint16_t buffer[BUFFER_SIZE]; // buffer; from PES 23 lecture pg. 28
	uint32_t buff_count; // number of samples added.
};

/*
 * Function: Init_DAC()
 * ---------------
 *
 * Description:
 *     Initializes the DAC
 * Input:
 *     1. void - N/A
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void Init_DAC(void);

/*
 * Function: TPM0_Init()
 * ---------------
 *
 * Description:
 *     Initializes the TMP module
 *     FROM: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_9/DMA_Examples/Source/timers.c
 * Input:
 *     1. void - N/A
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void TPM0_Init();

/*
 * Function: Init_DMA()
 * ---------------
 *
 * Description:
 *     Initializes the DMA
 *     CREDIT: Dean Alexander
 *     FROM: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_9/DMA_Examples/Source/DMA.c
 * Input:
 *     1. void - N/A
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void Init_DMA();

/*
 * Function: DAC_data_transfer()
 * ---------------
 *
 * Description:
 *     Used to transfer data to to the DAC.
 * Input:
 *     1. Frequency of the tone
 *     2. the buffer count
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void DAC_data_transfer(uint16_t *frequency, uint32_t cnt);

/*
 * Function: DAC_transfer()
 * ---------------
 *
 * Description:
 *     DAC transfer function, used to force transfer
 * Input:
 *     1. void - N/A
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void DAC_transfer(void);

/*
 * Function: DMA0_IRQHandler()
 * ---------------
 *
 * Description:
 *     Handler utilized by the interupt for the DMA
 *
 *     FROM: Dean - https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_9/DMA_Examples/Source/DMA.c
 * Input:
 *     1. void - N/A
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void DMA0_IRQHandler(void);


#endif /* DAC_H_ */
