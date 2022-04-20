/*
 * DAC.c
 *
 *  Created on: Apr 18, 2022
 *      Author: dreyceyalbin
 */

#include "DAC.h"
#define DAC_PIN (30)





// buffer for DAC
static struct TuneBuffer g_DACBuffer = {.buff_count = 0};

// Defined in header
void Init_DAC(void) {
  // Enable clock to DAC and Port E
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Select analog for pin
	PORTE->PCR[DAC_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[DAC_PIN] |= PORT_PCR_MUX(3);	// set up for TPM0_CH3

	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}

// Defined in header
void TPM0_Init() {

	//turn on clock to TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	// disable tpm
	TPM0->SC = 0;

	// Load the mod and counter
	TPM0->MOD = TPM_MOD_MOD(SYST_CLOCK / (SYST_CLOCK/1000)); // set sampling frequency
	TPM0->CNT = 0;

	// No prescaler, no PWM, include CMOD, include DMA
	TPM0->SC = TPM_SC_PS(0) | TPM_SC_CPWMS(0) | TPM_SC_CMOD(1) | TPM_SC_DMA_MASK;
}

// Defined in header
void Init_DMA() {

	// Send clock init to DMA and DMAMUX
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	// turn off channel for configuration
	DMAMUX0->CHCFG[0] = 0;

	// set interrupt; inc source buff, 16 bit words D&S, use cycle stealing, hardware trigger
	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK | DMA_DCR_SSIZE(2) |
			           DMA_DCR_DSIZE(2) | DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK;

	// Set the NVIC
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	// Set DMA MUX channel to trigger with TPM0 overflow
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}

// Defined in header
void DAC_data_transfer(uint16_t *out_buffer, uint32_t cnt) {
	// Turn timer off
	TPM0->SC &= ~TPM_SC_CMOD_MASK;


	//
	g_DACBuffer.buff_count = cnt; // update the global count; reload value
	// Memory copy to buffer
	memcpy(g_DACBuffer.buffer, out_buffer, cnt*2); // transfer out_buff to DAC buff

	// Restart (or start/enable) the TMP0 timer
	TPM0->SC |= TPM_SC_CMOD(1);

//	/*
//	 * TODO: is the below needed?
//	 */
//	// start the source and destination registers, respectively
//	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t) g_DACBuffer.buffer); //source
//	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0]))); //write to DAC
//
//	// Enable the DMAMUX
//	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(g_DACBuffer.buff_count * 2);
//	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
}

void DAC_transfer(void) {
	// direct source and destination DMA
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t) g_DACBuffer.buffer);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0])));

	// Bytes to transfer
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(g_DACBuffer.buff_count * 2);

	// Turn OFF done flag (pg. 356 of reference manual)
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK; // TODO: is this needed?
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK; // enable channel

}

// Defined in header
void DMA0_IRQHandler(void) {

	// Turn ON done flag (pg. 356 of reference manual)
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
	DAC_transfer();
}
