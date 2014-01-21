#ifndef __USB_H
#define __USB_H
#include "MDR32F9Qx_usb_handlers.h"
typedef __packed struct
{
	unsigned char ind_w;	// ������ ��������� ������ ��� ������ �����
	unsigned char ind_r;	// ������ ��������� ������ ��� ������ �����
	unsigned char Buffer[256];
}TFifo;

#define BUFFER_LENGTH                        100

// �������� ������� ������ � FIFO ������
#define CDC_EMPTY()	(RxBuf.ind_r == RxBuf.ind_w)

//------������� ���������� -----------------------------------------------------
extern uint8_t Buffer[BUFFER_LENGTH];
extern TFifo	 RxBuf;

//------������� ������� --------------------------------------------------------
extern  void 					CDC_Interface_Init(void);
extern unsigned char			CDC_GetChar(void);

extern 	void 					VCom_Configuration(void);
extern 	void 					Setup_CPU_Clock(void);
extern 	void 					Setup_USB(void);


// INLINE FUNCTIONS
/*---------------------------------------------------------------------
 * ������� ������
 *--------------------------------------------------------------------*/
__forceinline unsigned char	CDC_GetChar(void)
{
	if(CDC_EMPTY())
		return 0;
	else
		return RxBuf.Buffer[RxBuf.ind_r++];
}

/*---------------------------------------------------------------------
 * ��������� ������
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
