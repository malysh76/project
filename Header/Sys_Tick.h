#define __HCLK	32000000UL

/*------------------------ SystemTick ----------------------------------------*/
typedef struct
{
  vu32 CTRL;
  vu32 LOAD;
  vu32 VAL;
  vuc32 CALIB;
} SysTick_TypeDef;

#define SCS_BASE              ((u32)0xE000E000)
#define SysTick_BASE          (SCS_BASE + 0x0010)

#define SysTick             ((SysTick_TypeDef *) SysTick_BASE)

typedef struct {
    u32 ms_1   : 1;
    u32 ms_10  : 1;
    u32 ms_100 : 1;
	u32 s_1    : 1;
} MS_FLAGS __attribute__((bitband));

extern void SysTickSetup(void);

#ifndef _SYS_TICK
extern volatile MS_FLAGS Ms_Flags __attribute__((at(0x20001000)));
#endif

