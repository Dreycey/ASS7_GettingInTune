/*
 * test_sin.c
 *
 *  Created on: Apr 16, 2022
 *      Author: dreyceyalbin
 */

#include "test_sin.h"


/*	OTHER FILES TO BE INCLUDED	*/
#include "fp_sin.h"
#include "math.h"

// Defined in header
void test_sin() {
	double act_sin;
	double exp_sin;
	double err;
	double sum_sq = 0;
	double max_err = 0;
	for (int i=-TWO_PI; i <= TWO_PI; i++) {
		exp_sin = sin( (double)i / TRIG_SCALE_FACTOR) * TRIG_SCALE_FACTOR;
		act_sin = fp_sin(i);
		err = act_sin - exp_sin;
		if (err < 0)
			err = -err;
		if (err > max_err)
			max_err = err;
		sum_sq += err*err;
	}
	PRINTF("max_err=%d sum_sq=%d \r\n", (int)max_err, (int)sum_sq);
}
