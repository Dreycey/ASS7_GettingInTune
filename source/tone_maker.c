/*
 * tone_maker.c
 *
 *  Created on: Apr 19, 2022
 *      Author: dreyceyalbin
 */

#include "DAC.h"
#include "fp_sin.h"
#include "tone_maker.h"




// Defined in header
int tone_to_samples(int freq, uint16_t *buffer, uint32_t buffer_size) {
	// get sample count and number of iterations
	int samples_per_period = ((SAMPLE_FREQ + (freq/2)) / freq); // from lecture
	int period_count = buffer_size / samples_per_period;
	int sample_count = samples_per_period * period_count;

	// fill buffer
	uint32_t temp_cos;
	for (int i=0; i<sample_count; i++) {
		temp_cos = fp_sin(i * TWO_PI / samples_per_period); // stepsize = TWO_PI / samples_per_period
		buffer[i] = temp_cos + TRIG_SCALE_FACTOR; // add scale factor so y domain is above 0
	}
	PRINTF("Generated %d samples at %d Hz; computed period=%d samples \r\n", sample_count, freq, samples_per_period);

	return sample_count;
}
