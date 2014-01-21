#ifndef __CAN_H
#define __CAN_H
#include "MDR32F9Qx_can.h"
#include "defports.h"

#define CAN_BUF_DLC_STD   ((uint32_t)0x00000A00)    /* Standard format */
#define CAN_BUF_DLC_EXT   ((uint32_t)0x00001A00)    /* Extended format */

// MDR_CANx - ������������ � defports.h
//******************************************************************************************************************
// ------------ ������������� ����� ��� CAN ----------------------------------

//CAN1
//**********************************
#define PORT_CAN1	 GPIOD			 //A	 //B	 //C	  //D	  //E
#define PORT_CAN2	 GPIOF			 //A	 //B	 //C	  //D	  //E
#define CAN1_TX	 13				 	 //6	 //2	 //8	  //13	  //1
#define CAN1_RX	 14				     //7	 //3	 //9	  //14	  //0
#define CAN2_TX	 3				 	 //6	 //2	 //8	  //13	  //1
#define CAN2_RX	 2				     //7	 //3	 //9	  //14	  //0
#define	PERNUM_nPORT_CAN1	 nPORTD	 //A	 //B	 //C	  //D	  //E
#define	PERNUM_nPORT_CAN2	 nPORTF	 //A	 //B	 //C	  //D	  //E
#define	PERNUM_nCAN1	 	 nCAN1	 //		 //		 //		  //	  //
#define	PERNUM_nCAN2	 	 nCAN2	 //		 //		 //		  //	  //
//����� ������ ������ �����			 //		 //		 //		  //	  //
//0 � ����							 //		 //		 //		  //	  //
//1 � �������� �������				 //		 //		 //		  //	  //
//2 � �������������� �������		 //		 //		 //		  //	  //
//3 � ���������������� �������		 //		 //		 //		  //	  //
#define	MODE_WORK_CAN1	 3			 //2	 //3	 //1	  //3	  //3
#define	MODE_WORK_CAN2	 3			 //2	 //3	 //1	  //3	  //3

//**********************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ����� ������������ ���������� ������
#define MASK_PORT_CAN1	(((unsigned long)1<<CAN1_TX)|\
						 ((unsigned long)1<<CAN1_RX))
						 
#define MASK_PORT_CAN2	(((unsigned long)1<<CAN2_TX)|\
						 ((unsigned long)1<<CAN2_RX))						 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ��������� �������� �������
#define PER_CLOCK_CAN1_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT_CAN1;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nCAN1;}

#define PER_CLOCK_CAN2_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT_CAN2;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nCAN2;}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������ ������ �����												 
#define PORT_CAN1_FUNC_INIT {PORT_CAN1->FUNC &= ~(((unsigned long)3<<(CAN1_TX*2))|\
					      						((unsigned long)3<<(CAN1_RX*2)));\
												PORT_CAN1->FUNC |=   ((unsigned long)MODE_WORK_CAN1<<(CAN1_TX*2))|\
						  						((unsigned long)MODE_WORK_CAN1<<(CAN1_RX*2));}                                            

#define PORT_CAN2_FUNC_INIT {PORT_CAN2->FUNC &= ~(((unsigned long)3<<(CAN2_TX*2))|\
					      						((unsigned long)3<<(CAN2_RX*2)));\
												PORT_CAN2->FUNC |=   ((unsigned long)MODE_WORK_CAN2<<(CAN2_TX*2))|\
						  						((unsigned long)MODE_WORK_CAN2<<(CAN2_RX*2));}                                            
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++													


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
// ����� ������ �����������
//0 � ����������
//1 � ��������
#define	PORT_CAN1_DIGEN_INIT PORT_CAN1->DIGEN |= MASK_PORT_CAN1
#define	PORT_CAN2_DIGEN_INIT PORT_CAN2->DIGEN |= MASK_PORT_CAN2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ������� ���������� �������� ������ � GND (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_CAN1_RGND_INIT PORT_CAN1->RGND &= ~MASK_PORT_CAN1
#define	PORT_CAN2_RGND_INIT PORT_CAN2->RGND &= ~MASK_PORT_CAN2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ������� ���������� �������� ������ � VCC (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_CAN1_RVCC_INIT PORT_CAN1->RVCC &= ~MASK_PORT_CAN1
#define	PORT_CAN2_RVCC_INIT PORT_CAN2->RVCC &= ~MASK_PORT_CAN2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_CAN1_DDR_INIT PORT_CAN1->DDR |= MASK_PORT_CAN1
#define	PORT_CAN2_DDR_INIT PORT_CAN2->DDR |= MASK_PORT_CAN2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_CAN1_PWR_INIT {PORT_CAN1->PWR &= ~(((unsigned long)3<<(CAN1_TX*2))|\
											  ((unsigned long)3<<(CAN1_RX*2)));\
							PORT_CAN1->PWR |=  (((unsigned long)2<<(CAN1_TX*2))|\
						   					  ((unsigned long)2<<(CAN1_RX*2)));}
#define	PORT_CAN2_PWR_INIT {PORT_CAN2->PWR &= ~(((unsigned long)3<<(CAN2_TX*2))|\
											  ((unsigned long)3<<(CAN2_RX*2)));\
							PORT_CAN2->PWR |=  (((unsigned long)2<<(CAN2_TX*2))|\
						   					  ((unsigned long)2<<(CAN2_RX*2)));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*******************************************************************************************************************

typedef struct
{
	unsigned long SID;		// ����������� �������������
	unsigned char RTR;		// ������� ���������� �������
	unsigned char nbyte;	// ���������� ����
	union
	{
		unsigned char  ubyte[8]; 	// ����������� �����
		unsigned long  ulong[2]; 	// ����������� �����
		__packed struct				// ��������� ��������������� ����
		{
			unsigned char status;	// ��������� ����
			signed short I;			// �������� ����
			signed short V;			// �������� ��������
		}UM_INF5;
		__packed struct				// ��������� ������� ����������
		{
			unsigned char status;	// ��������� ����
			signed short ctrl;		// �������� ������� ����������
		}UM_CTRL3;

	}Data;
}TCANMsg;





// ���������� ������� ������� CAN

#define BUF_CMD_UM_STOP		1 	//  1.  ����� ������ ��� �������� ������� ����� ��������
#define BUF_CMD_UM_STOPALL	2	//  2.  ����� ������ ��� ����������������� ������� ����� ��������
#define BUF_CMD_UM_RTR		3	//  3.  ����� ������ ��� �������� ������� ������������ �����
#define BUF_CMD_UM_RTRALL	4	//  4.  ����� ������ ��� �����������������  ������� ������������ �����
#define BUF_CMD_UM_CTRL		5	//  5.  ����� ������ ��� �������� ������� ����������� (���������� ���������������)
#define BUF_CMD_UM_CYCLE	6	//  6.  ����� ������ ��� �������� ������� ������������ �����(�0 � � ���� DATA, ��� = 0,05 ��)
#define BUF_CMD_UM_CYCLEALL	7	//  7.  ����� ������ ��� ����������������� ������� ������������ �����(�0 � � ���� DATA, ��� = 0,05 ��)
#define BUF_CMD_UM_ANSW		8	//  8.  ����� ������ ��� ��������� ����� ��  �������  �.3,4,5,6,7
#define BUF_CMD_UM_SYNCALL	9	//  9.  ����� ������ ��� �����������������  ������� �������������
#define BUF_CMD_UM_PARAM	10	//  10. ����� ������ ��� �������� ��������������� �������
#define BUF_CMD_UM_PRMALL	11	//  11. ����� ������ ��� ����������������� ��������������� �������
#define BUF_CMD_UM_PRMANSW	12	//  12. ����� ������ ��� ����� �� ��������������� ������� �.9,10

#define BUF_CMD_UM_CTRL2	13	// ����� 2-�� ������ ��� ������ ������� ���������� 
#define BUF_RX_NOFLTR		30	//  30. ����� ����� ��� ������ ������� ���������
#define BUF_TX_NOFLTR		31	//  31. ����� ����� ��� �������� ���������



// ������ ���������� ��������

#define ADDR_UM0	0x00									// 		��������� �������� 0
#define ADDR_UM1	0x01		// ��������� ��							// 		��������� �������� 1
#define ADDR_UM2	0x02		// ��������� ��							// 		��������� �������� 2
#define ADDR_UM3	0x03									// 		��������� �������� 3
#define ADDR_UM4	0x04									// 		��������� �������� 4
#define ADDR_UM5	0x05									// 		��������� �������� 5
#define ADDR_UM6	0x06									// 		��������� �������� 6
#define ADDR_UM7	0x07									// 		��������� �������� 7





// ������� ��� ���������� ��������

//						DSCR    TYPECMD	  ANSW   ADR_TYPE
#define CMD_UM_STOP		(0x0<<9 | 0<<8 	| 0<<7 | 0x2<<3) 	//  1.  �������� ������� ����� �������� 
#define CMD_UM_STOPALL	(0x0<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  2.  ����������������� ������� ����� �������� 
#define CMD_UM_RTR		(0x1<<9 | 0<<8 	| 0<<7 | 0x2<<3)	//  3.  �������� ������� ������������ ����� 
#define CMD_UM_RTRALL	(0x1<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  4.  �����������������  ������� ������������ ����� 
#define CMD_UM_CTRL		(0x3<<9 | 0<<8 	| 0<<7 | 0x2<<3)	//  5.  �������� ������� ����������� (���������� ���������������) 
#define CMD_UM_CYCLE	(0x2<<9 | 0<<8 	| 0<<7 | 0x2<<3)	//  6.  �������� ������� ������������ �����(�0 � � ���� DATA, ��� = 0,05 ��)  
#define CMD_UM_CYCLEALL	(0x2<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  7.  ����������������� ������� ������������ �����(�0 � � ���� DATA, ��� = 0,05 ��)
#define CMD_UM_ANSW		(0x0<<9 | 0<<8 	| 1<<7 | 0x2<<3)	//  8.  ��������� ����� ��  �������  �.3,4,5,6,7 
#define CMD_UM_SYNCALL	(0x0<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  9.  �����������������  ������� �������������
#define CMD_UM_PARAM	(0x0<<9 | 1<<8 	| 0<<7 | 0x2<<3)	//  10. �������� ��������������� ������� 
#define CMD_UM_PRMALL	(0x0<<9 | 1<<8 	| 0<<7 | 0x0<<3)	//  11. ����������������� ��������������� ������� 
#define CMD_UM_PRMANSW	(0x0<<9 | 1<<8 	| 1<<7 | 0x2<<3)	//  12. ����� �� ��������������� ������� �.9,10 





// ������������ ������������ ��������������

#define SID(adr,cmd)	((adr|cmd)<<18)





// �������������� ��� ���������

#define SID_UM_STOP		 SID(CMD_UM_STOP	,CanAddr)
#define SID_UM_STOPALL	 SID(CMD_UM_STOPALL	,1      )
#define SID_UM_RTR		 SID(CMD_UM_RTR		,CanAddr)
#define SID_UM_RTRALL	 SID(CMD_UM_RTRALL	,1      )
#define SID_UM_CTRL		 SID(CMD_UM_CTRL	,CanAddr)
#define SID_UM_CYCLE	 SID(CMD_UM_CYCLE	,CanAddr)
#define SID_UM_CYCLEALL	 SID(CMD_UM_CYCLEALL,1      )
#define SID_UM_ANSW		 SID(CMD_UM_ANSW	,CanAddr)
#define SID_UM_SYNCALL	 SID(CMD_UM_SYNC	,0      )
#define SID_UM_PARAM	 SID(CMD_UM_PARAM	,CanAddr)
#define SID_UM_PRMALL	 SID(CMD_UM_PRMALL	,1      )
#define SID_UM_PRMANSW	 SID(CMD_UM_PRMANSW	,CanAddr)



// ������� ������������ � ��������������� ����������

#define PRM_CMD_RST		0xFF	// �����
#define PRM_CMD_ERR		0x40	// ��������� ���
#define PRM_CMD_INF_RD	0x80	// ������ ������ �� ������������
#define PRM_CMD_UID		0x82	// ���������� ID
#define PRM_CMD_INF_WR	0x84	// ������ ������ �� ������������
#define PRM_CMD_INF_RST	0x88	// ����� ����� �������  ���������� ������ �� ������������

// ������� ����������
extern TCANMsg CANMsgTx;		// ���������� ��� ������������ ���������
extern TCANMsg CANMsgRx;		// ���������� ��� ����������� ���������
extern unsigned long CanAddr;	// ������� CAN �����


// ������� �������
extern void 			CAN_init (void);
extern void 			CAN2_init (void);




// INLINE �������
/*-----------------------------------------------------------------------
 * �������� ��������� � CAN �� ����������� ���������������
 * pCANMsg - ��������� �� ������������ CAN ���������
 * 0 - ������� ��������
 * 1 - ����� ����������� �����
 *----------------------------------------------------------------------*/
__forceinline unsigned char CAN_Put(TCANMsg * pCANMsg)
{
//	if(MDR_CANx->BUF_CON[BUF_TX_NOFLTR]&CAN_BUF_CON_TX_REQ)
//		return 1;
//	else

	if(MDR_CANx->TX & ((unsigned long)1<<BUF_TX_NOFLTR) )
	{
		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DLC = CAN_BUF_DLC_STD;
		// �������� RTR �������
		if(pCANMsg->RTR)
			MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DLC += CAN_RTR_REMOTE;
		else
			MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DLC += pCANMsg->nbyte;

		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].ID  = pCANMsg->SID;
		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DATAL = pCANMsg->Data.ulong[0];
		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DATAH = pCANMsg->Data.ulong[1];
//		MDR_CANx->BUF_CON[BUF_TX_NOFLTR] = CAN_BUF_CON_TX_REQ|CAN_BUF_CON_EN;
		MDR_CANx->BUF_CON[BUF_TX_NOFLTR] |= CAN_BUF_CON_TX_REQ;
		return 0;
	}
	return 1;
}


/*-----------------------------------------------------------------------
 * ��������� �������� ��������� �� ����������� ������
 * pCANMsg - ��������� �� ����������� CAN ���������
 * numbuffer - ����� ������
 * 0 - ��������� �������
 * 1 - ��� ���������
 *----------------------------------------------------------------------*/
__forceinline unsigned char CAN_GetBuffer(TCANMsg * pCANMsg,unsigned char numbuffer)
{
//	if(MDR_CANx->BUF_CON[numbuffer]&CAN_BUF_CON_RX_FULL)
//	{
		pCANMsg->nbyte = MDR_CANx->CAN_BUF[numbuffer].DLC & CAN_DLC_DATA_LENGTH;
		pCANMsg->SID = MDR_CANx->CAN_BUF[numbuffer].ID;
		pCANMsg->Data.ulong[0] = MDR_CANx->CAN_BUF[numbuffer].DATAL;
		pCANMsg->Data.ulong[1] = MDR_CANx->CAN_BUF[numbuffer].DATAH;
//		CAN_ITClearRxTxPendingBit(MDR_CANx, numbuffer, CAN_STATUS_RX_READY);
	  MDR_CANx->BUF_CON[numbuffer] &= ~CAN_BUF_CON_RX_FULL; // �������� ���� ��������� ��������� 
		return 0;
//	}
//	else
//		return 1;
}


/*-----------------------------------------------------------------------
 * ��������� �������� ��������� �� ���� ��������� �������
 * pCANMsg - - ��������� �� ����������� CAN ���������
 * i - ����� ������ ��������� ���������
 * -1 - ��� ���������
 *----------------------------------------------------------------------*/
__forceinline signed char CAN_Get(TCANMsg * pCANMsg)
{

	signed char i=32;
	if(MDR_CANx->STATUS&CAN_STATUS_RX_READY)
	{
		while(i)
			if(!CAN_GetBuffer(pCANMsg,--i))
			{
				
				return i;
			}
	} 
	return -1;
}




#endif
