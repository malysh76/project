#ifndef __CLOCK1_H
#define __CLOCK1_H
#include "common1.h"
#include "MDR32Fx.h"




typedef struct {
	__I  u8		rCLOCK_STATUS;	// регистр состояния блока управления частотой
	__IO u16	rPLL_CONTROL	__attribute__ ((aligned(4)));	// регистр управления PLL
	__IO u8		rHS_CONTROL		__attribute__ ((aligned(4)));
	__IO u16	rCPU_CLOCK		__attribute__ ((aligned(4)));
	__IO u16	rUSB_CLOCK		__attribute__ ((aligned(4)));
	__IO u16	rADC_CLOCK		__attribute__ ((aligned(4)));
	__IO u16	rRTC_CLOCK		__attribute__ ((aligned(4)));
	__IO u32	rPER_CLOCK		__attribute__ ((aligned(4)));
	__IO u32	rCAN_CLOCK;
	__IO u32	rTIM_CLOCK;
	__IO u32	rUART_CLOCK;
	__IO u32	rSSP_CLOCK;
} RST_CLK_TypeDef;



typedef struct {
	u8	USB_RDY : 1;	// флаг готовности PLL USB
	u8	CPU_RDY	: 1;	// флаг готовности PLL CPU
	u8	HSE_RDY	: 1;	// флаг готовности генератора HSE
	u8	__0		: 5;
} CLOCK_STATUS_TypeDef;

typedef   struct {
	u16	USB_ON	: 1;	// бит включения PLL USB
	u16	USB_RLD	: 1;	// бит перезапуска PLL USB (установить после изменения USB_MUL)
	u16	CPU_ON	: 1;	// бит включения PLL CPU
	u16	CPU_RLD	: 1;	// бит перезапуска PLL CPU (установить после изменения CPU_MUL)
	u16	USB_MUL	: 4;	// коэффициент умножения для PLL USB
	u16	CPU_MUL	: 4;	// коэффициент умножения для PLL CPU
	u16	__0		: 4;
} PLL_CONTROL_TypeDef;

typedef struct {
	u8	HSE_ON	: 1;	// бит влючения HSE
	u8	HSE_OSC	: 1;	// режим HSE : 0 - XTAL, 1 - XGEN
	u8	__0		: 6;
} HS_CONTROL_TypeDef;

#define	CPU_C1(n)			((u16)(n) & 0x03)
#define CPU_C2(n)			(((u16)(n) & 0x03) << 2)
#define	CPU_C3(n)			(((u16)(n) & 0x0F) << 4)
#define HCLK_SEL(n)			(((u16)(n) & 0x03) << 8)

typedef struct {
	u16	C1_SEL		: 2;
	u16	C2_SEL		: 1;
	u16	__0			: 1;
	u16	C3_SEL		: 4;
	u16	HCLK_SEL	: 2;
	u16	__1			: 6;
} CPU_CLOCK_TypeDef;

typedef struct {
	u16	C1_SEL	: 2;
	u16	C2_SEL	: 1;
	u16	__0		: 1;
	u16	C3_SEL	: 1;
	u16	__1		: 3;
	u16	CLK_EN	: 1;
	u16	__2		: 7;
} USB_CLOCK_TypeDef;

typedef struct {
	u16	C1_SEL	: 2;
	u16	__0		: 2;
	u16	C2_SEL	: 2;
	u16	__1		: 2;
	u16	C3_SEL	: 4;
	u16	__2		: 1;
	u16	CLK_EN	: 1;
	u16	__3		: 2;
} ADC_CLOCK_TypeDef;

typedef struct {
	u16	HSE_SEL		: 4;
	u16	HSI_SEL		: 4;
	u16	HSE_RTC_EN	: 1;
	u16	HSI_RTC_EN	: 1;
	u16	__0	: 6;
} RTC_CLOCK_TypeDef;

typedef struct {
	u32	CAN1_BRG	: 8;
	u32	CAN2_BRG	: 8;
	u32	__0	: 8;
	u32	CAN1_CLK_EN	: 1;
	u32	CAN2_CLK_EN	: 1;
	u32	__1	: 6;
} CAN_CLOCK_TypeDef;

typedef struct {
	u32	TIM1_BRG	: 8;
	u32	TIM2_BRG	: 8;
	u32	TIM3_BRG	: 8;
	u32	TIM1_CLK_EN	: 1;
	u32	TIM2_CLK_EN	: 1;
	u32	TIM3_CLK_EN	: 1;
	u32	__0	: 5;
} TIM_CLOCK_TypeDef;

typedef struct {
	u32	UART1_BRG		: 8;
	u32	UART2_BRG		: 8;
	u32	__0		: 8;
	u32	UART1_CLK_EN	: 1;
	u32	UART2_CLK_EN	: 1;
	u32	__1		: 5;
} UART_CLOCK_TypeDef;

typedef struct {
	u32	SSP1_BRG		: 8;
	u32	SSP2_BRG		: 8;
	u32	__0		: 8;
	u32	SSP1_CLK_EN		: 1;
	u32	SSP2_CLK_EN		: 1;
	u32	__1		: 5;
} SSP_CLOCK_TypeDef;
/*
typedef struct {
	u32 nCAN1		  :1;   // 0
	u32 nCAN2		  :1;   // 1
	u32 nUSB		  :1;   // 2
	u32 nEEPROM_CNTRL :1;   // 3
	u32 nRST_CLK      :1;   // 4
	u32 nDMA	      :1;   // 5
	u32 nUART1	      :1;   // 6
	u32 nUART2	      :1;   // 7
	u32 nSPI1	      :1;   // 8
	u32	__1			  :1;   // 9
	u32 nI2C1	      :1;   // 10
	u32 nPOWER	      :1;   // 11
	u32 nWWDT	      :1;   // 12
	u32 nIWDT	      :1;   // 13
	u32 nTIMER1	      :1;   // 14
	u32 nTIMER2	      :1;   // 15
	u32 nTIMER3	      :1;   // 16
	u32 nADC	      :1;   // 17
	u32 nDAC	      :1;   // 18
	u32 nCOMP	      :1;   // 19
	u32 nSPI2	      :1;   // 20
	u32 nPORTA	      :1;   // 21
	u32 nPORTB	      :1;   // 22
	u32 nPORTC	      :1;   // 23
	u32 nPORTD	      :1;   // 24
	u32 nPORTE	      :1;   // 25
	u32 nBKP	      :1;   // 26
	u32	__2			  :1;   // 27
	u32 nPORTF	      :1;   // 28
	u32	__3			  :1;   // 29
	u32 nEXT_BUS_CTRL :1;   // 30
	u32	__4			  :1;   // 31
} PER_CLOCK_TypeDef;	 */

typedef enum {
	nCAN1	= 0,
	nCAN2	= 1,
	nUSB	= 2,
	nEEPROM_CNTRL = 3,
	nRST_CLK	= 4,
	nDMA	= 5,
	nUART1	= 6,
	nUART2	= 7,
	nSPI1	= 8,
	nI2C1	= 10,
	nPOWER	= 11,
	nWWDT	= 12,
	nIWDT	= 13,
	nTIMER1	= 14,
	nTIMER2	= 15,
	nTIMER3	= 16,
	nADC	= 17,
	nDAC	= 18,
	nCOMP	= 19,
	nSPI2	= 20,
	nPORTA	= 21,
	nPORTB	= 22,
	nPORTC	= 23,
	nPORTD	= 24,
	nPORTE	= 25,
	nBKP	= 27,
	nPORTF	= 29,
	nEXT_BUS_CTRL	= 30
} PERNUM_TypeDef;

/*		 ОПРЕДЕЛЕНИЕ АДРЕСОВ РЕГИСТРОВ			*/

#define  RST_CLOCK_BASE	(PERIPH_BASE + 0x020000)												   
#define  CLOCK_STATUS_BASE	RST_CLOCK_BASE + 0x00
#define  PLL_CONTROL_BASE	RST_CLOCK_BASE + 0x04
#define  HS_CONTROL_BASE	RST_CLOCK_BASE + 0x08
#define  CPU_CLOCK_BASE		RST_CLOCK_BASE + 0x0C
#define  USB_CLOCK_BASE		RST_CLOCK_BASE + 0x10
#define  ADC_CLOCK_BASE		RST_CLOCK_BASE + 0x14
#define  RTC_CLOCK_BASE		RST_CLOCK_BASE + 0x18
#define  PER_CLOCK_BASE		RST_CLOCK_BASE + 0x1C
#define  CAN_CLOCK_BASE		RST_CLOCK_BASE + 0x20
#define  TIM_CLOCK_BASE		RST_CLOCK_BASE + 0x24
#define  UART_CLOCK_BASE	RST_CLOCK_BASE + 0x28
#define  SSP_CLOCK_BASE		RST_CLOCK_BASE + 0x2C

/*		ПРИВЯЗКА СТРУКТУР К АДРЕСАМ									*/
/*
#define RST_CLOCK 		((RST_CLK_TypeDef*) 		RST_CLOCK_BASE)
#define CLOCK_STATUS	((CLOCK_STATUS_TypeDef*)	CLOCK_STATUS_BASE)
#define PLL_CONTROL		((PLL_CONTROL_TypeDef*)    PLL_CONTROL_BASE)
#define HS_CONTROL		((HS_CONTROL_TypeDef*)	    HS_CONTROL_BASE)
#define CPU_CLOCK		((CPU_CLOCK_TypeDef*)	    CPU_CLOCK_BASE)
#define USB_CLOCK		((USB_CLOCK_TypeDef*)	    USB_CLOCK_BASE)
#define ADC_CLOCK		((ADC_CLOCK_TypeDef*)	    ADC_CLOCK_BASE)
#define RTC_CLOCK		((RTC_CLOCK_TypeDef*)	    RTC_CLOCK_BASE)
#define PER_CLOCK		((PER_CLOCK_TypeDef*)	    PER_CLOCK_BASE)
#define CAN_CLOCK		((CAN_CLOCK_TypeDef*)	    CAN_CLOCK_BASE)
#define TIM_CLOCK		((TIM_CLOCK_TypeDef*)	    TIM_CLOCK_BASE)
#define UART_CLOCK		((UART_CLOCK_TypeDef*)	    UART_CLOCK_BASE)
#define SSP_CLOCK		((SSP_CLOCK_TypeDef*)	    SSP_CLOCK_BASE)
*/
#define RST_CLOCK 		(*(RST_CLK_TypeDef*) 		 (RST_CLOCK_BASE))
#define CLOCK_STATUS	(*(volatile unsigned long*)	 (CLOCK_STATUS_BASE))
#define PLL_CONTROL		(*(volatile unsigned long*)  (PLL_CONTROL_BASE))
#define HS_CONTROL		(*(volatile unsigned long*)  (HS_CONTROL_BASE))
#define CPU_CLOCK		(*(volatile unsigned long*)  (CPU_CLOCK_BASE))
#define USB_CLOCK		(*(volatile unsigned long*)  (USB_CLOCK_BASE))
#define ADC_CLOCK		(*(volatile unsigned long*)  (ADC_CLOCK_BASE))
#define RTC_CLOCK		(*(volatile unsigned long*)  (RTC_CLOCK_BASE))
#define PER_CLOCK		(*(volatile unsigned long*)	 (PER_CLOCK_BASE))
#define CAN_CLOCK		(*(volatile unsigned long*)  (CAN_CLOCK_BASE))
#define TIM_CLOCK		(*(volatile unsigned long*)  (TIM_CLOCK_BASE))
#define UART_CLOCK		(*(volatile unsigned long*)  (UART_CLOCK_BASE))
#define SSP_CLOCK		(*(volatile unsigned long*)  (SSP_CLOCK_BASE))

#endif
