#ifndef __SSP_H
#define __SSP_H
#include "common1.h"

#define SSP1_BASE	(PERIPH_BASE + 0x40000)
#define SSP2_BASE	(PERIPH_BASE + 0xA0000)

typedef struct {
	__IO u16	CR0;
	__IO u16	CR1		__attribute__ ((aligned(4)));
	__IO u16	DR		__attribute__ ((aligned(4)));
	__I	 u16	SR		__attribute__ ((aligned(4)));
	__IO u16	CPSR	__attribute__ ((aligned(4)));
	__IO u16	IMSC	__attribute__ ((aligned(4)));
	__I	 u16	RIS		__attribute__ ((aligned(4)));
	__I	 u16	MIS		__attribute__ ((aligned(4)));
	__O	 u16	ICR		__attribute__ ((aligned(4)));
	__IO u16	DMACR	__attribute__ ((aligned(4)));
} SSP_TypeDef;

#define SSP1	((SSP_TypeDef*) SSP1_BASE)
#define SSP2	((SSP_TypeDef*) SSP2_BASE)




typedef struct {
	u16	DSS		: 4;
	u16	FRF		: 2;
	u16	CPLH	: 2;	// формат сигнала CLK в режиме SPI
	u16	SCR		: 8;
} SSP_CR0_TypeDef;

typedef struct {
	u16	LBM	: 1;
	u16	SSE	: 1;
	u16	SLV	: 1;
	u16	SOD	: 1;
	u16	__0	: 12; // при записи устанавливать в 0
} SSP_CR1_TypeDef;

typedef struct {
	u16	TFE	: 1;
	u16	TNF	: 1;
	u16	RNE	: 1;
	u16	RRF	: 1;
	u16	BSY	: 1;
	u16	__0	: 11;
} SSP_SR_TypeDef;

typedef struct {
	u16	ROVF	: 1;	// переполнение FIFO приемника
	u16	RTOF	: 1;	// таймаут приема данных
	u16	RXF		: 1;	// запрос на обслуживание FIFO приемника
	u16	TXF		: 1;	// запрос на обслуживание FIFO передатчика
	u16	__0		: 12;
} SSP_INTF_TypeDef;

typedef struct {
	u16	ROVF	: 1;
	u16	RTOF	: 1;
	u16	__0		: 14;
} SSP_INTC_TypeDef;

typedef struct {
	u16	RXD	: 1;
	u16	TXD	: 1;
	u16	__0	: 14;
} SSP_DMAE_TypeDef;

/*
	SSP1


__IO_REG(SSP1_CR0,	SSP_CR0_TypeDef,	SSP1_BASE + 0x00);
__IO_REG(SSP1_CR1,	SSP_CR1_TypeDef,	SSP1_BASE + 0x04);
__IO_REG(SSP1_DR,	u16,				SSP1_BASE + 0x08);
__RO_REG(SSP1_SR,	SSP_SR_TypeDef,		SSP1_BASE +	0x0C);
__IO_REG(SSP1_CPSR,	u16,				SSP1_BASE + 0x10);
__IO_REG(SSP1_IMSC,	SSP_INTF_TypeDef,	SSP1_BASE + 0x14);
__RO_REG(SSP1_IFR,	SSP_INTF_TypeDef,	SSP1_BASE + 0x18);
__RO_REG(SSP1_IMFR,	SSP_INTF_TypeDef,	SSP1_BASE + 0x1C);
__IO_REG(SSP1_ICR,	SSP_INTC_TypeDef,	SSP1_BASE + 0x20);
__IO_REG(SSP1_DMAE,	SSP_DMAE_TypeDef,	SSP1_BASE + 0x24);
*/
/*
	SSP2

__IO_REG(SSP2_CR0,	SSP_CR0_TypeDef,	SSP2_BASE + 0x00);
__IO_REG(SSP2_CR1,	SSP_CR1_TypeDef,	SSP2_BASE + 0x04);
__IO_REG(SSP2_DR,	u16,				SSP2_BASE + 0x08);
__RO_REG(SSP2_SR,	SSP_SR_TypeDef,		SSP2_BASE +	0x0C);
__IO_REG(SSP2_CPSR,	u16,				SSP2_BASE + 0x10);
__IO_REG(SSP2_IMSC,	SSP_INTF_TypeDef,	SSP2_BASE + 0x14);
__RO_REG(SSP2_IFR,	SSP_INTF_TypeDef,	SSP2_BASE + 0x18);
__RO_REG(SSP2_IMFR,	SSP_INTF_TypeDef,	SSP2_BASE + 0x1C);
__IO_REG(SSP2_ICR,	SSP_INTC_TypeDef,	SSP2_BASE + 0x20);
__IO_REG(SSP2_DMAE,	SSP_DMAE_TypeDef,	SSP2_BASE + 0x24);
*/
#endif
