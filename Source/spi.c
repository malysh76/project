/*
 * spi.c
 *
 *  Created on: 26.06.2012
*/

 #include "common1.h"
 #include "1986ve9x_irq.h"
 #include "1986ve9x_clk.h"
 #include "1986ve9x_gpio.h"
 #include "1986ve9x_ssp.h"
 #include "spi.h"
// #include "ugol.h"

//Инициализация SPI1
//-----------------------------------------------------------------------
void SPI1_Init (void)
{
	    // конфигурация портов
		PER_CLOCK_SPI1_INIT;
		PORT_SPI1_DDR_INIT;
		PORT_SPI1_FUNC_INIT;
		PORT_SPI1_DIGEN_INIT;
		PORT_SPI1_PWR_INIT;
		PORT_SPI1_RGND_INIT;	
		PORT_SPI1_RVCC_INIT;

		// разрешение тактовой частоты SSP1
	    SSP_CLOCK |= (unsigned long)1<<24;
		    
		// делитель тактовой частоты  SSP1
	    SSP_CLOCK &= 0xFFFFFF00; // SSPCLK = 1 * PLLCPUOUT = 80 МГц

		// конфигурация SSP1
		SSP1->CR0   = SPI1_CR0_INIT;
		SSP1->CR1   = SPI1_CR1_INIT;
		SSP1->CPSR  = SPI1_CPSR_INIT;
		SSP1->IMSC  = SPI1_IMSC_INIT;
		SSP1->DMACR = SPI1_DMACR_INIT; 	 
}
//------------------------------------------------------------------------

//Инициализация SPI2
//------------------------------------------------------------------------
void SPI2_Init (void)
{
	    // конфигурация портов
		PER_CLOCK_SPI2_INIT;
		PORT_SPI2_DDR_INIT;
		PORT_SPI2_FUNC_INIT;
		PORT_SPI2_DIGEN_INIT;
		PORT_SPI2_PWR_INIT;
		PORT_SPI2_RGND_INIT;	
		PORT_SPI2_RVCC_INIT;
	
		// разрешение тактовой частоты SSP2
	    SSP_CLOCK |= (unsigned long)1<<25;
        
		// делитель тактовой частоты  SSP2
	    SSP_CLOCK &= 0xFFFF00FF; // SSPCLK = 1 * PLLCPUOUT = 80 МГц

		// конфигурация SSP2
		SSP2->CR0   = SPI2_CR0_INIT;
		SSP2->CR1   = SPI2_CR1_INIT;
		SSP2->CPSR  = SPI2_CPSR_INIT;
		SSP2->IMSC  = SPI2_IMSC_INIT;
		SSP2->DMACR = SPI2_DMACR_INIT; 	 
}
//---------------------------------------------------------------------------
