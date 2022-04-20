/*
 * ADC.h
 *
 *  Created on: Apr 20, 2022
 *      Author: dreyceyalbin
 */

#ifndef ADC_H_
#define ADC_H_
#include "fsl_debug_console.h"
#define SYST_CLOCK (48E6)
#define ACDSAMPLE_FREQ (96E3)
#include "MKL25Z4.h"
#include "autocorrelate.h"




/*
 * Function: TPM1_Init()
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
void TPM1_Init();

/*
 * Function: Init_ADC(void)
 * ---------------
 *
 * Description:
 *     Initializes the ADC
 *
 *     NOTE: This function borrows heavily from (PES 24 slides)
 * Input:
 *     1. void - N/A
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void Init_ADC(void);

/*
 * Function: ADC_data_transfer()
 * ---------------
 *
 * Description:
 *     Stores value from ADC register into the in_buffer
 * Input:
 *     1. in buffer
 *     2. number of samples to transfer
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void ADC_data_transfer(uint16_t *in_buffer, uint32_t sample_count);

/*
 * Function: ADC_buffer_analysis()
 * ---------------
 *
 * Description:
 *     Prints (using PRINTF) simple buffer stats
 * Input:
 *     1. in buffer
 *     2. number of samples to transfer
 * Returns (non-Error):
 *     1. void - N/A
 * Returns (Error Codes):
 *     1. N/A
 */
void ADC_buffer_analysis(uint16_t *in_buffer, uint32_t sample_count);

#endif /* ADC_H_ */
