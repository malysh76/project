#include "headers\1986_map.h"

#define _SYS_TICK
#include "Sys_Tick.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//

//=========================================================================== System Timer Configuration
// <h> System Timer Configuration
// <i> Требуется задать значение HCLC в Sys_Tick.h
// <e0> System Timer Enable
//   <o2> SYSTICK period [ms] <1-1000:10>
//   <i> Set the timer period for System Timer.
//   <i> Default: 1  (1ms)
//   <o1.1> System Timer interrupt enabled
// </e>
// </h>
#define __SYSTICK_SETUP           1
#define __SYSTICK_CTRL_VAL        0x00000006
#define __SYSTICK_PERIOD          0x00000032

#define __SYSTICKCLK    (__HCLK)	// в 1986ВЕ1 такт SysTick только от HCLC

#define SYSTICK_CSR_ENABLE    ((unsigned long)0x00000001)
#define SYSTICK_CSR_COUNTFLAG ((unsigned long)0x00010000)

#if __SYSTICK_SETUP
void SysTickSetup(void) 
{
#if ((__SYSTICK_PERIOD*(__SYSTICKCLK/1000)-1) > 0xFFFFFF)       // reload value to large
   #error "Reload Value to large!"
#else
  SysTick->LOAD  = __SYSTICK_PERIOD*(__SYSTICKCLK/1000)-1;      // set reload register
  SysTick->CTRL  = __SYSTICK_CTRL_VAL;                          // set clock source and Interrupt enable

  SysTick->VAL   =  0;                                          // clear  the counter
  SysTick->CTRL |= SYSTICK_CSR_ENABLE;                          // enable the counter
#endif
}
#endif

//===================================================================================
// BitBand регистр флагов 1, 10, 100 мс и 1 сек
//===================================================================================
volatile MS_FLAGS Ms_Flags __attribute__((at(0x20001000)));
//===================================================================================
// Установка флагов 1, 10, 100 мс и 1 сек
//===================================================================================
void __irq SysTick_Handler()
{
static u8 ms_10_cnt;
static u8 ms_100_cnt;
static u8 s_1_cnt;

Ms_Flags.ms_1 = 1;

if(++ms_10_cnt >= 10)
	{
	ms_10_cnt = 0;
	Ms_Flags.ms_10 = 1;

	if(++ms_100_cnt >= 10)
		{
		ms_100_cnt = 0;
		Ms_Flags.ms_100 = 1;

		if(++s_1_cnt >= 10)
			{
			s_1_cnt = 0;
			Ms_Flags.s_1 = 1;
			}
		}
	}
}


