#ifndef __TIMER_H
#define __TIMER_H
#include "common1.h"

#define TIMER1_BASE	(PERIPH_BASE + 0x70000)
#define TIMER2_BASE	(PERIPH_BASE + 0x78000)
#define TIMER3_BASE (PERIPH_BASE + 0x80000)

typedef struct {
	__IO u16	CNT;			// регистр счетчика
	__IO u16	PSC		__attribute__ ((aligned(4)));			// регистр предделител€ частоты
	__IO u16	ARR		__attribute__ ((aligned(4)));			// верхн€€ граница счета
	__IO u16	CTRL	__attribute__ ((aligned(4)));			// регистр управлени€
	__IO u32	ICHR[4]	__attribute__ ((aligned(4)));		// регистры сравнени€ и захвата каналов
	__IO u32	CH_CTRL[4];		// регистры є0 управлени€ каналов
	__IO u32	CH_CTRL1[4];	// регистры є1 управлени€ каналов
	__IO u32	CH_DTG[4];		// регистры делител€ частоты каналов
	__IO u8		BRKETR_CNTRL;	// регистр управлени€ входами BRK и ETR
	__IO u32	INTF	__attribute__ ((aligned(4)));			// регистр флагов событий
	__IO u32	INTE;			// регистр флагов разрешени€ событийных прерываний
	__IO u32	DMAE;			// регистр разрешени€ событийных запросов DMA
	__IO u32	CH_CTRL2[4];	// регистры є2 управлени€ каналов
	__IO u32	ICHRn1[4];		// регистры сравнени€ и захвата c первым каналом 
} TIMER_TypeDef;

#define TIMER1	((TIMER_TypeDef*) TIMER1_BASE)
#define TIMER2	((TIMER_TypeDef*) TIMER2_BASE)
#define TIMER3	((TIMER_TypeDef*) TIMER3_BASE)

typedef struct {
	u16	CNT_EN		: 1;	// разрешение работы таймера
	u16	ARRB_EN		: 1;	// разрешение обновлени€ ARR
	u16	WR_CMPL		: 1;	// флаг окончани€ записи в регистры CNT, ARR и PSC
	u16	DIR_DOWN	: 1;	// флаг обратного счета
	u16	FDTS		: 2;	// частота сэмплировани€ данных FDTS
	u16	CNT_MODE	: 2;	// режим счета основного счетчика
	u16	EVENT_SEL	: 4;	// выбор источника событий
	u16	__0			: 4;
} TIM_CTRL_TypeDef;

typedef struct {
	u32	CHFLTR	: 4;
	u32	CHSEL	: 2;
	u32	CHPSC	: 2;
	u32	OCCE	: 1;
	u32	OCCM	: 3;
	u32	BRKEN	: 1;
	u32	ETREN	: 1;
	u32	WRCMPL	: 1;
	u32	CAPnPWM	: 1;
	u32	__0		: 16;
} TIM_CH_CTRL_TypeDef;

typedef struct {
	u32	SELOE	: 2;
	u32	SELOUT	: 2;
	u32	INV		: 1;
	u32	__0		: 3;
	u32	NSELOE	: 2;
	u32	NSELOUT	: 2;
	u32	NINV	: 1;
	u32	__1		: 19;
} TIM_CH_CTRL1_TypeDef;

typedef struct {
	u32	CHSEL1	: 2;
	u32	CCR1_EN	: 1;
	u32	CRRRLD	: 1;
	u32	__0		: 28;
} TIM_CH_CTRL2_TypeDef;

typedef struct {
	u32	DTG		: 4;
	u32	EDTS	: 1;
	u32	__0		: 3;
	u32	DTGx	: 8;
	u32	__1		: 16;
} TIM_CH_DTG_TypeDef;

typedef struct {
	u8	BRK_INV		: 1;
	u8	ETR_INV		: 1;
	u8	ETR_PSC		: 2;
	u8	ETR_FILTER	: 4;
} TIM_BRKETR_CTRL_TypeDef;

typedef struct {
	u32	CNT_ZERO	: 1;
	u32	CNT_ARR		: 1;
	u32	ETR_RE		: 1;
	u32	ETR_FE		: 1;
	u32	BRK_HLD		: 1;
	u32	CCR_CAP		: 4;
	u32	CCR_REF		: 4;
	u32	CCR_CAP1	: 4;
	u32	__0			: 15;
} TIM_EVENT_TypeDef;

#endif
