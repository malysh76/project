#ifndef __UART_H
#define __UART_H
#include "common1.h"
#include "MDR32Fx.h"

#define UART1_BASE	(PERIPH_BASE + 0x30000)
#define UART2_BASE	(PERIPH_BASE + 0x38000)

typedef	struct {
	__IO u16	DR;		// регистр данных
	__IO u8		SR		__attribute__ ((aligned(4)));	// регистр состояния
	 	 u32 	__0[4]	__attribute__ ((aligned(4)));
	__I  u16	FR;									   	// регистр флагов событий
		 u32	__1 	__attribute__ ((aligned(4)));	
	__IO u8		ILPR;									// регистр управления ИК
	__IO u16	IBRD	__attribute__ ((aligned(4)));	// целая часть делителя скорости
	__IO u8		FBRD	__attribute__ ((aligned(4)));	// дробная часть делителя скорости
	__IO u8		LCR		__attribute__ ((aligned(4)));	// регистр управления линией
	__IO u16	CR		__attribute__ ((aligned(4)));	// регистр управления приемопередатчиком
	__IO u8		IFLS	__attribute__ ((aligned(4)));	// регистр порога прерывания по заполнению буфера
	__IO u16	IMR		__attribute__ ((aligned(4)));	// регистр маски прерываний
	__I	 u16	IFR		__attribute__ ((aligned(4)));	// регистр флагов прерываний
	__I	 u16	IMFR	__attribute__ ((aligned(4)));	// регистр флагов прерываний с учетом маски
	__O	 u16	ICR		__attribute__ ((aligned(4)));	// регистр сброса прерываний
	__IO u8		DMAE	__attribute__ ((aligned(4)));	// регистр разрешения запроса DMA
} UART_TypeDef;

typedef struct {
	u16	DATA	: 8;
	u16	FE		: 1;
	u16	PE		: 1;
	u16	BE		: 1;
	u16	OE		: 1;
	u16	__0		: 4;
} UART_DR_TypeDef;

typedef struct {
	u8	FE	: 1;
	u8	PE	: 1;
	u8	BE	: 1;
	u8	OE	: 1;
	u8	__0	: 4;
} UART_SR_TypeDef;

typedef struct {
	u16	CTS		: 1;
	u16	DSR		: 1;
	u16	DCD		: 1;
	u16	BUSY	: 1;
	u16	RXFE	: 1;
	u16	TXFF	: 1;
	u16	RXFF	: 1;
	u16	TXFE	: 1;
	u16	RI		: 1;
	u16	__0		: 7;
} UART_FR_TypeDef;

typedef struct {
	u8	BRK		: 1;
	u8	PEN		: 1;
	u8	EPS		: 1;
	u8	SBS		: 1;
	u8	FEN		: 1;
	u8	WLEN	: 2;
	u8	SBP		: 1;
} UART_LCR_TypeDef;

typedef struct {
	u16	RTEN	: 1;
	u16	IREN	: 1;
	u16	IRLP	: 1;
	u16	__0		: 4;
	u16	LBEN	: 1;
	u16	TXEN	: 1;
	u16	RXEN	: 1;
	u16	DTR		: 1;
	u16	RTS		: 1;
	u16	DCD		: 1;
	u16	RI		: 1;
	u16	RTSEN	: 1;
	u16	CTSEN	: 1;
} UART_CR_TypeDef;

#define UART1	((UART_TypeDef*) UART1_BASE)
#define UART2	((UART_TypeDef*) UART2_BASE)

/*
	UART1
*/
/*
__IO_REG(UART1_DR,		UART_DR_TypeDef,	UART1_BASE + 0x00);
__IO_REG(UART1_SR,		UART_SR_TypeDef,	UART1_BASE + 0x04);
__RO_REG(UART1_FR,		UART_FR_TypeDef,	UART1_BASE + 0x18);
__IO_REG(UART1_ILPR,	u8,					UART1_BASE + 0x20);
__IO_REG(UART1_IBRD,	u16,				UART1_BASE + 0x24);
__IO_REG(UART1_FBRD,	u8,					UART1_BASE + 0x28);
__IO_REG(UART1_LCR,		UART_LCR_TypeDef,	UART1_BASE + 0x2C);
__IO_REG(UART1_CR,		UART_CR_TypeDef,	UART1_BASE + 0x30);
*/
#endif
