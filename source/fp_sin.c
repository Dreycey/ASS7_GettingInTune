/*
 * fp_sin.c
 *
 *  Created on: Apr 15, 2022
 *      Author: dreyceyalbin
 */

#include "fp_sin.h"
#define TRIG_TABLE_STEPS     (32)
#define TRIG_TABLE_STEP_SIZE (HALF_PI/TRIG_TABLE_STEPS)
#include "assert.h"


/*
 * Structure used for linear (or any other) interpolation.
 *
 * Ensures all values are used for interpolation
 */
struct interpolate_struct {
	int32_t x_min;
	int32_t x_max;
	int32_t y_min;
	int32_t y_max;
};
typedef struct interpolate_struct interpolate_struct;

/*
 * sin lookup table with scaled value.
 * Corresponds to TRIG_TABLE_STEPS
 */
static const int16_t sin_lookup[TRIG_TABLE_STEPS+1] =
  {0, 100, 200, 299, 397, 495, 591, 686, 780, 871, 960, 1047,
   1132, 1214, 1292, 1368, 1440, 1509, 1575, 1636, 1694, 1747,
   1797, 1842, 1882, 1918, 1949, 1976, 1998, 2015, 2027, 2035,
   2037};

/*
 * Function: linear_interpolation()
 * ---------------
 *
 * Description:
 *     Computes a linear interpolation using a interpolate_struct
 * Input:
 *     1. int32 - input scaled value
 *     2. interpolate_struct - struvct needed for linear interpolation (y1,y2,x1,x2)
 * Returns (non-Error):
 *     1. int32 - scaled sin value
 * Returns (Error Codes):
 *     1. N/A
 */
int32_t linear_interpolation(int32_t x, interpolate_struct * interp_struct) {
	uint32_t x_diff = (interp_struct->x_max - interp_struct->x_min);
	uint32_t y_diff = (interp_struct->y_max - interp_struct->y_min);
	/* Half denominator used, suggested by Howdy */
	uint32_t interpolation = ((x_diff/2) + (x - interp_struct->x_min) * y_diff)/x_diff + interp_struct->y_min;
	return interpolation;
}

//Defined in header
int32_t fp_sin(int32_t x) {
	int val_sign = 1;
	// shift values till in correct domain [0, HALF_PI]
	while (x < -PI) {
		x += TWO_PI;
	}
	while (x > PI) {
		x -= TWO_PI;
	}
	assert(-PI <= x && x <= PI);
	if (x < 0){ // move negatives to positive domain
		val_sign = -1;
		x = -x;
	}
	if (x > HALF_PI) { // if above HALF_PI, find corresponding points in [0, HALF_PI]
		x = PI - x;
	}
	assert(0 <= x && x <= HALF_PI);

	// Find index
	int32_t idx = TRIG_TABLE_STEPS * x / HALF_PI;
	int32_t x_min = TRIG_TABLE_STEP_SIZE * idx;

	// if in table, use val, else interpolate
	if (x == x_min) {
		return val_sign * sin_lookup[idx];
	}
	else {
		interpolate_struct interp_struct;
		interp_struct.x_min = TRIG_TABLE_STEP_SIZE * idx;
		interp_struct.x_max = TRIG_TABLE_STEP_SIZE * (idx + 1);
		interp_struct.y_min = sin_lookup[idx];
		interp_struct.y_max = sin_lookup[idx+1];
		return val_sign * linear_interpolation(x, &interp_struct);
	}
}
