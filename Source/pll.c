#include "common1.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_tim.h"
#include "1986ve9x_irq.h"
//#include "1986ve9x_gpio.h"
//#include "1986ve9x_i2c.h"
//#include "1986ve9x_ssp.h"
//#include "1986ve9x_uart.h"
//#include "1986ve9x_ext.h"
#include "defports.h"
#include "pll.h"
//------------------------------------------------------------------------------
//		инициализаци€ PLL
//------------------------------------------------------------------------------
void PLL_Init(void)
{
	
	RST_CLOCK.rPLL_CONTROL = 0x00;
//“аблица 82 Ц ќписание битов регистра PLL_CONTROL 
//є         ‘ункциональн     –асшифровка функционального имени бита, краткое описание  
//           ое им€ бита      назначени€ и принимаемых значений. 
// 31..12    -                «арезервировано 
// 11...8    PLL               оэффициент умножени€ дл€ CPU PLL: 
//           CPU              PLLCPUo = PLLCPUi x (PLLCPUMUL+1) 
//           MUL[3:0] 
// 7..4      PLL               оэффициент умножени€ дл€ USB PLL: 
//           USB              PLLUSBo = PLLUSBi x (PLLUSBMUL+1) 
//           MUL[3:0] 
// 3         PLL              Ѕит перезапуска PLL 
//           CPU              ѕри смене коэффициента умножени€ в рабочем режиме  
//           PLD              необходимо задать равным 1 
// 2         PLL              Ѕит включени€ PLL 
//           CPU               0 Ц PLL выключена 
//           ON                1 Ц PLL включена 
// 1         PLL               Ѕит перезапуска PLL 
//           USB               ѕри смене коэффициента умножени€ в рабочем режиме  
//           RLD               необходимо задать равным 1 
// 0         PLL               Ѕит включени€ PLL 
//           USB               0 Ц PLL выключена 
//           ON                1 Ц PLL включена 


// включение HSE осцил€тора
	RST_CLOCK.rHS_CONTROL = 0x01;
//	“аблица 84 Ц ќписание битов регистра HS_CONTROL
//	1	HSE_BYP		Ѕит управлени€ HSE осцилл€тором
//		0 Ц режим осцилл€тора
//		1 Ц режим внешнего генератора
//	0	HSE_ON	Ѕит управлени€ HSE осцилл€тором
//		0 Ц выключен
//		1 Ц включен


// проверка готовности осцил€тора
	while ( !RST_CLOCK.rCLOCK_STATUS&0x04 ) {}
//	“аблица 80 Ц ќписание битов регистра CLOCK_STATUS
//	 є         ‘ункциональн     –асшифровка функционального имени бита, краткое описание
//	           ое им€ бита      назначени€ и принимаемых значений.
//	 31..3     -                «арезервировано
//	 2         HSE_RDY          ‘лаг выхода в рабочий режим осцилл€тора HSE
//	                            0 Ц осцилл€тор не запущен или не стабилен
//	                            1 Ц осцилл€тор запущен и стабилен
//	 1         PLL_CPU_RDY      ‘лаг выхода в рабочий режим CPU PLL
//	                            0 Ц PLL не запущена или не стабильна
//	                            1 Ц PLL запущена и стабильна
//	 0         PLL_USB_RDY      ‘лаг выхода в рабочий режим USB PLL
//	                            0 Ц PLL не запущена или не стабильна
//	                            1 Ц PLL запущена и стабильна

// выбор коэфициента умножени€
	#if ((HSE<2)||(HSE>16))
		#warning HSE should be in the range 2...16 MHz
		#elif (PLLCPUOUT>80)
				#warning PLLCPUOUT should be no more 80 ћHz
			#elif ((PLLCPUOUT/HSE)>16)
				#warning PLLCPUOUT/HSE should be no more 16
	#endif
	RST_CLOCK.rPLL_CONTROL |= (unsigned long)(PLLCPUOUT/HSE-1)<<8;

// включение PLL
	RST_CLOCK.rPLL_CONTROL |= 0x04;

//  ожидание готовности PLL
	while ( !RST_CLOCK.rCLOCK_STATUS&0x02 ) {}

// выбор источника и коммутаци€ тактовой частоты
// CPU_C1 = HSE, CPU_C2 = CPUPLL, CPU_C3 = CPU_C2, HCLK = CPU_C3
	RST_CLOCK.rCPU_CLOCK = CPU_C1(2)|CPU_C2(1)|CPU_C3(0)|HCLK_SEL(1);
}
