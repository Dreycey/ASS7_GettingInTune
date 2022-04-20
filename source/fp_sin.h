/*
 * fp_sin.h
 *
 *  Created on: Apr 15, 2022
 *      Author: dreyceyalbin
 */

#ifndef FP_SIN_H_
#define FP_SIN_H_


#include <stdint.h>
#define TRIG_SCALE_FACTOR (2037) // perfect scaling factor
#define HALF_PI (3200)  // pi * TRIG_SCALE_FACTOR / 2
#define PI (6399) // pi * TRIG_SCALE_FACTOR / 2
#define TWO_PI (12799) // 2 * pi TRIG_SCALE_FACTOR

/*
 * Function: fp_sin()
 * ---------------
 *
 * Description:
 *     Compute the sin of a scaled input value
 * Input:
 *     1. int32 - input scaled value
 * Returns (non-Error):
 *     1. int32 - scaled sin value
 * Returns (Error Codes):
 *     1. N/A
 */
int32_t fp_sin(int32_t x);


#endif /* FP_SIN_H_ */
