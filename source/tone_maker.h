/*
 * tone_maker.h
 *
 *  Created on: Apr 19, 2022
 *      Author: dreyceyalbin
 */

#ifndef TONE_MAKER_H_
#define TONE_MAKER_H_
#include <stdint.h>
#include "fsl_debug_console.h"


int tone_to_samples(int freq, uint16_t *buffer, uint32_t buffer_size);

#endif /* TONE_MAKER_H_ */
