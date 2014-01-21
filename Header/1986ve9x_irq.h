#ifndef __IRQ_H
#define __IRQ_H

//#include "MDR32Fx.h"

//#define __enable_irq()	__asm{"cpsie i"}
//#define __disable_irq()	__asm{"cpsid i"}

//typedef unsigned          int uint32_t;
//typedef unsigned          char uint8_t;
//typedef enum
//{
/*	Системные прерывания	*/
/////	NMI_IRQn = -14, 
/////	MemMamage_IRQn = -12,
///	BusFault_IRQn = -11,
///	UsageFault_IRQn = -10,
///	SVCall_IRQn = -5,
/////	DebugMonitor_IRQn = -4,
///	PendSV_IRQn = -2,
///	SysTick_IRQn = -1,
/*	Прерывания периферии	*/
///	CAN1_IRQn	= 0, /* CAN1 */ 
///	CAN2_IRQn	= 1,
///	USB_IRQn	= 2,
///	DMA_IRQn	= 5,
///	UART1_IRQn	= 6,
///	UART2_IRQn	= 7,
///	SSP1_IRQn	= 8,
///	I2C_IRQn	= 10,
/////	PWR_IRQn	= 11,
///	WWDG_IRQn	= 12,
/////	TIM1_IRQn	= 14, /* TIMER1 */
/////	TIM2_IRQn	= 15,
/////	TIM3_IRQn	= 16,
///	ADC_IRQn	= 17,
/////	COMP_IRQn	= 19,
///	SSP2_IRQn	= 20,
/////	RTC_IRQn	= 27,
/////	EXT1_IRQn	= 28,
/////	EXT2_IRQn	= 29,
/////	EXT3_IRQn	= 30,
/////	EXT4_IRQn	= 31
//} IRQn_Type1;

///#define NVIC_BASE   		(SCS_BASE +  0x0100)
///#define SCB_BASE    		(SCS_BASE +  0x0D00)   /*!< System Control Block Base Address    */
///#define __NVIC_PRIO_BITS    4		               /*!< standard definition for NVIC Priority Bits */
//#define NVIC_AIRCR_VECTKEY  (0x5FA << 16)   	   /*!< AIRCR Key for write access   */

//typedef struct
//{
//  __IO	u32 ISER[8];                      /* регистр флагов разрешения прерываний */
//  		u32	__0[24];
//  __IO	u32 ICER[8];                      /* регистр флагов запрета прерываний          */
//  		u32	__1[24];
//  __IO	u32 ISPR[8];                      /* */
//  		u32	__2[24];
//  __IO	u32	ICPR[8];                      /* */
//  		u32	__3[24];
//  __IO	u32	IABR[8];                      /* */
//  		u32	__4[56];
//  __IO	u8  IP[240];                      /* */
//  		u32	__5[644];
//  __O	u32	STIR;                         /* */
//}  NVIC_TypeDef;

/* memory mapping struct for System Control Block */
///typedef struct
///{
///  __I  uint32_t CPUID;                        /*!< CPU ID Base Register                                     */
///  __IO uint32_t ICSR;                         /*!< Interrupt Control State Register                         */
///  __IO uint32_t VTOR;                         /*!< Vector Table Offset Register                             */
///  __IO uint32_t AIRCR;                        /*!< Application Interrupt / Reset Control Register           */
///  __IO uint32_t SCR;                          /*!< System Control Register                                  */
///  __IO uint32_t CCR;                          /*!< Configuration Control Register                           */
///  __IO uint8_t  SHP[12];                      /*!< System Handlers Priority Registers (4-7, 8-11, 12-15)    */
///  __IO uint32_t SHCSR;                        /*!< System Handler Control and State Register                */
///  __IO uint32_t CFSR;                         /*!< Configurable Fault Status Register                       */
///  __IO uint32_t HFSR;                         /*!< Hard Fault Status Register                               */
///  __IO uint32_t DFSR;                         /*!< Debug Fault Status Register                              */
///  __IO uint32_t MMFAR;                        /*!< Mem Manage Address Register                              */
///  __IO uint32_t BFAR;                         /*!< Bus Fault Address Register                               */
///  __IO uint32_t AFSR;                         /*!< Auxiliary Fault Status Register                          */
///  __I  uint32_t PFR[2];                       /*!< Processor Feature Register                               */
///  __I  uint32_t DFR;                          /*!< Debug Feature Register                                   */
///  __I  uint32_t ADR;                          /*!< Auxiliary Feature Register                               */
///  __I  uint32_t MMFR[4];                      /*!< Memory Model Feature Register                            */
///  __I  uint32_t ISAR[5];                      /*!< ISA Feature Register                                     */
///} SCB_Type;


///#define NVIC  ((NVIC_TypeDef*) NVIC_BASE)       //  NVIC configuration struct
///#define SCB   ((SCB_Type *)    SCB_BASE)         /*!< SCB configuration struct             */

///__forceinline static void NVIC_EnableIRQ(IRQn_Type IRQn)
///{
///  NVIC->ISER[((u32)(IRQn) >> 5)] = (1 << ((u32)(IRQn) & 0x1F)); /* разрешить прерывание */
///}
//------------------------------------------------------------------------------
///__forceinline static void NVIC_DisableIRQ(IRQn_Type IRQn)
///{
///  NVIC->ICER[((u32)(IRQn) >> 5)] = (1 << ((u32)(IRQn) & 0x1F)); /* запретить прерывание */
///}
//------------------------------------------------------------------------------
/**
 * @brief  Set the priority for an interrupt
 *
 * @param  IRQn_Type IRQn is the Number of the interrupt
 * @param  priority is the priority for the interrupt
 * @return none
 *
 * Set the priority for the specified interrupt. The interrupt
 * number can be positive to specify an external (device specific)
 * interrupt, or negative to specify an internal (core) interrupt. \n
 *
 * Note: The priority cannot be set for every core interrupt.
 */
///__forceinline static void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
///{
///  if(IRQn < 0) {
///    SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); } /* set Priority for Cortex-M3 System Interrupts */
///  else {
///    NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);    }        /* set Priority for device specific Interrupts      */
///}
//------------------------------------------------------------------------------
/**
 * @brief  Set the Priority Grouping in NVIC Interrupt Controller
 *
 * @param  uint32_t priority_grouping is priority grouping field
 * @return none
 *
 * Set the priority grouping field using the required unlock sequence.
 * The parameter priority_grouping is assigned to the field
 * SCB->AIRCR [10:8] PRIGROUP field. Only values from 0..7 are used.
 * In case of a conflict between priority grouping and available
 * priority bits (__NVIC_PRIO_BITS) the smallest possible priority group is set.
 */
///__forceinline static void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
///{
///  uint32_t reg_value;
///  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);                         /* only values 0..7 are used          */

///  reg_value  = SCB->AIRCR;                                                    /* read old register configuration    */
///  reg_value &= ~((0xFFFFU << 16) | (0x0F << 8));                              /* clear bits to change               */
///  reg_value  = ((reg_value | NVIC_AIRCR_VECTKEY | (PriorityGroupTmp << 8)));  /* Insert write key and priorty group */
///  SCB->AIRCR = reg_value;
///}
//------------------------------------------------------------------------------

#endif
