/*
 * dac.c
 *
 *  Created on: 11.04.2011
 *      Author: 129
 */
#include "common1.h"
#include "1986ve9x_irq.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_dac.h"
#include "1986ve9x_gpio.h"
#include "1986ve9x_tim.h"
#include "defports.h"
#include "dac.h"

void DAC_Init (void)
{
	// конфигурация портов
		PER_CLOCK_DAC_INIT;
		PORT_DAC_FUNC_INIT;
		PORT_DAC_DIGEN_INIT;
		PORT_DAC_RGND_INIT;
		PORT_DAC_RVCC_INIT;
	// конфигурация DAC->CONFIG
		DAC->CONFIG = DAC_CFG_INIT;
}
