#ifndef __COMMON1_H
#define __COMMON1_H

typedef unsigned long  u32;
typedef unsigned short u16;
typedef signed short   s16;
typedef unsigned char  u8;

typedef volatile unsigned long	vu32;
typedef volatile signed long	vs32;
typedef volatile unsigned short vu16;
typedef volatile signed short 	vs16;
typedef volatile unsigned char	vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */


#ifndef _BOOL
typedef enum { false, true } bool;
#endif

#ifdef __cplusplus
#define     __I     volatile 
#else
#define     __I     volatile const
#endif
#define     __O     volatile
//#define     __IO    volatile

#define PERIPH_BASE    ((u32)0x40000000) // Базовый адрес регистров периферии
//#define SCS_BASE       ((u32)0xE000E000) //  System Control Space Base Address  

#define BIT(n) (1 << (n))
#define REGBIT(x, b)  \
  (*(vu32*)(((u32)(x) & 0xF0000000) | 0x02000000 | (((u32)(x) & 0x000FFFFF) << 5) | ((b) << 2)))

#define PORT_SET(port,bit)		REGBIT(&port->RXTX,bit)= 1
#define PORT_CLR(port,bit)		REGBIT(&port->RXTX,bit)= 0
#define PORT_FLP(port,bit)		REGBIT(&port->RXTX,bit)=!REGBIT(&port->RXTX,bit)
#define PORT_IN(port,bit)		(REGBIT(&port->RXTX,bit))

#define __IO_REG(NAME, TYPE, ADDRESS)\
                       __IO TYPE NAME __attribute__ ((at(ADDRESS)))

#define __RO_REG(NAME, TYPE, ADDRESS)\
                       __I TYPE NAME __attribute__ ((at(ADDRESS)))

#define __IO_REG_BIT(NAME, BIT, ADDRESS)  \
__IO  u32 NAME[BIT] __attribute__ ((at((((u32)(ADDRESS) & 0xF0000000) | 0x02000000 | (((u32)(ADDRESS) & 0x000FFFFF) << 5)))))

#define __IO_REG_ARRAY(NAME, SIZE, TYPE, ADDRESS)\
__IO TYPE NAME[SIZE] __attribute__ ((at(ADDRESS)))

#endif
