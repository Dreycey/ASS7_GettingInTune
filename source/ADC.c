/*
 * ADC.c
 *
 *  Created on: Apr 20, 2022
 *      Author: dreyceyalbin
 */

#include "ADC.h"
#include "fsl_debug_console.h"

// Defined in header
void TPM1_Init() {

	//turn on clock to TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

	// disable tpm
	TPM1->SC = 0;

	// Load the mod and counter
	TPM1->MOD = TPM_MOD_MOD(SYST_CLOCK / ACDSAMPLE_FREQ); // set sampling frequency
	TPM1->CNT = 0;

	// No prescaler, no PWM, include CMOD
	TPM1->SC = TPM_SC_PS(0) | TPM_SC_CPWMS(0);
}

// Defined in Header
void Init_ADC(void) {

	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

	/*
	 * Clock configuration (PES 24 slides)
	 *
	 * Clock=ADICLK, Not dividing down ADCK
	 */
	ADC0->CFG1 = ADC_CFG1_ADICLK(0) | ADC_CFG1_ADLPC(0) | ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP(0) | ADC_CFG1_MODE(3) ;

	//Clear the configuration_2 register
	ADC0->CFG2 = 0;

	/*
	 * Hardware trigger is enabled, compare function is disabled, DMA is disabled, external pins
	 * are used to voltage reference
	 */
	ADC0->SC2 = ADC_SC2_ADTRG(1) | ADC_SC2_ACFE(0) | ADC_SC2_DMAEN(0) | ADC_SC2_REFSEL(0);

	/*
	 * Input channel set as 23 for DAC loopback
	 */
	ADC0->SC1[0] = ADC_SC1_AIEN(0) | ADC_SC1_DIFF(0) | ADC_SC1_ADCH(23);

	/*
	 * ADC0TRGSEL == 9 for TMP1 overflow
	 * ADC0ALTTRGEN == 1 for Alternate trigger enable
	 * ADC0PRETRGSEL == 0 ADC pre-trigger select
	 */
	SIM->SOPT7 = SIM_SOPT7_ADC0ALTTRGEN(1) | SIM_SOPT7_ADC0TRGSEL(9); // | SIM_SOPT7_ADC0PRETRGSEL(0);
}

// Defined in header
void ADC_data_transfer(uint16_t *buffer, uint32_t sample_count) {
	//Start TPM
	TPM1->SC |= TPM_SC_CMOD(1);

	//Loop for filling the ADC buffer
	for (int i=0; i < sample_count; i++) {
		// wait for read to complete (PES 24 lecture)
		while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
		      ;
		// store A-D result in buffer.
		buffer[i] = ADC0->R[0];
	}

	//Stop TPM
	TPM1->SC &= ~TPM_SC_CMOD_MASK;
}

void ADC_buffer_analysis(uint16_t *buffer, uint32_t sample_count) {
	uint16_t min_val = 65535; // 65536 = 1ULL << 16
	uint16_t max_val = 0;
	uint32_t running_total = 0;
	int i=0;
	while (i<sample_count) {
		uint16_t curr_val = buffer[i];
		if (curr_val > max_val) {
			max_val = curr_val;
		}
		else if (curr_val < min_val) {
			min_val = curr_val;
		}
		running_total += curr_val;
		i++;
	}
	uint16_t running_avg = running_total / sample_count;
	int period = autocorrelate_detect_period(buffer, sample_count, 1);
	int frequency = ACDSAMPLE_FREQ / period;
	PRINTF("min=%d max=%d avg=%d period=%d samples frequency=%d Hz \r\n", min_val, max_val, running_avg, period, frequency);
}
