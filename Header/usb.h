#ifndef __USB_H
#define __USB_H
#include "MDR32F9Qx_usb_handlers.h"
typedef __packed struct
{
	unsigned char ind_w;	// индекс следующей €чейки дл€ записи слова
	unsigned char ind_r;	// индекс следующей €чейки дл€ чтени€ слова
	unsigned char Buffer[256];
}TFifo;

#define BUFFER_LENGTH                        100

// проверка наличи€ данных в FIFO буфере
#define CDC_EMPTY()	(RxBuf.ind_r == RxBuf.ind_w)

//------Ё —ѕќ–“ ѕ≈–≈ћ≈ЌЌџ’ -----------------------------------------------------
extern uint8_t Buffer[BUFFER_LENGTH];
extern TFifo	 RxBuf;

//------Ё —ѕќ–“ ‘”Ќ ÷»… --------------------------------------------------------
extern  void 					CDC_Interface_Init(void);
extern unsigned char			CDC_GetChar(void);

extern 	void 					VCom_Configuration(void);
extern 	void 					Setup_CPU_Clock(void);
extern 	void 					Setup_USB(void);


// INLINE FUNCTIONS
/*---------------------------------------------------------------------
 * прин€ть символ
 *--------------------------------------------------------------------*/
__forceinline unsigned char	CDC_GetChar(void)
{
	if(CDC_EMPTY())
		return 0;
	else
		return RxBuf.Buffer[RxBuf.ind_r++];
}

/*---------------------------------------------------------------------
 * отправить данные
 *--------------------------------------------------------------------*/
__forceinline	CDC_PutData(unsigned char * Buf, uint32_t Len)
{
	while(USB_CDC_SendData(Buf, Len)!=USB_SUCCESS)
	{
		
	}
	
	//USB_CDC_SendData(Buf, Len);
	return ; 
}
#endif
