/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ASS7_GettingInTune.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fp_sin.h"
#include "test_sin.h"
#include "DAC.h"
#include "ADC.h"
#include "timer.h"
#include "tone_maker.h"
/* TODO: insert other definitions and declarations here. */
#define ONE_SECOND (16) // systick is set up to use 1/16 second fractions
#define A4_FREQ (440)
#define D5_FREQ (587)
#define E5_FREQ (659)
#define A5_FREQ (880)



/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    // init in/out buffers, frequency array
    struct TuneBuffer out_buffer = {.buff_count = 0};
    struct TuneBuffer in_buffer = {.buff_count = 0};
    int freq_arr[4] = {A4_FREQ, D5_FREQ, E5_FREQ, A5_FREQ};

    // set up timer
    init_systick();


    // Set up output (DAC, DMA, TMP0)
    Init_DAC();
    Init_DMA();
    TPM0_Init();

    // Set up ADC
    TPM1_Init();
    Init_ADC();

    // testing sin
    test_sin();


    /* Force the counter to be placed into memory. */
    volatile static int tune_index = 0;
    /* Enter an infinite loop, just incrementing a counter. */


    reset_timer(); //reset timer before beggining.
    while(1) {
        // update tune frequency
        if (ONE_SECOND < get_timer()) {
        	tune_index++;
        	if (tune_index == 4) {tune_index = 0;}
        	reset_timer();
        }

        // turn tone into samples
        out_buffer.buff_count = tone_to_samples(freq_arr[tune_index], out_buffer.buffer, BUFFER_SIZE);

        // update DMA/DAC buffer
        DAC_data_transfer(out_buffer.buffer, out_buffer.buff_count);
        DAC_transfer();

        //ADC
        ADC_data_transfer(in_buffer.buffer, BUFFER_SIZE);
        ADC_buffer_analysis(in_buffer.buffer, BUFFER_SIZE);
    }
    return 0 ;
}
