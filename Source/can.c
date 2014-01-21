#include "MDR32F9Qx_rst_clk.h"
#include "can.h"
#include "1986ve9x_clk.h"
#include "device.h"


TCANMsg 		CANMsgTx;
TCANMsg 		CANMsgRx;
unsigned long	CanAddr;	// рабочий CAN адрес

static void CAN_FilterSet(unsigned char nombuf,unsigned long cmd,unsigned long addr);
static void CAN_FiltersSet(unsigned long addr);
/*--------------------------------------------------
 * ИНИЦИАЛИЗАЦИЯ CAN
 *-------------------------------------------------*/
void CAN_init(void)
{
	CAN_InitTypeDef  sCAN;
	CanAddr=CAN_ADR_UM;	// рабочий CAN адрес усилителя (ГН или ВН)	
	// конфигурация портов
	// разрешение тактовой частоты для CAN1 и используемого порта
		PER_CLOCK_CAN1_INIT;

		PORT_CAN1_DDR_INIT;
		PORT_CAN1_FUNC_INIT;
		PORT_CAN1_DIGEN_INIT;
		PORT_CAN1_PWR_INIT;
		PORT_CAN1_RGND_INIT;	
		PORT_CAN1_RVCC_INIT;

//	// разрешение тактовой частоты для CAN1
//	RST_CLK_PCLKcmd(RST_CLK_PCLK_CANx,ENABLE);


	/* Set the HCLK division factor = 1 for CAN1*/
	CAN_BRGInit(MDR_CANx,CAN_HCLKdiv1);
	// конфигурация CAN1
	CAN_DeInit(MDR_CANx);
    CAN_StructInit (&sCAN);
    sCAN.CAN_ROM  = DISABLE;

    // настройка скорости на 1 МБОД
    sCAN.CAN_PSEG = CAN_PSEG_Mul_4TQ;
    sCAN.CAN_SEG1 = CAN_SEG1_Mul_7TQ;
    sCAN.CAN_SEG2 = CAN_SEG2_Mul_4TQ;
    sCAN.CAN_SJW  = CAN_SJW_Mul_3TQ;
    sCAN.CAN_SB   = CAN_SB_1_SAMPLE;
    sCAN.CAN_BRP  = 4;
	CAN_Init (MDR_CANx,&sCAN);
	CAN_Cmd(MDR_CANx, ENABLE);
	/* Disable all CAN1 interrupt */
	CAN_ITConfig( MDR_CANx, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN |
    		      CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, DISABLE);

	CAN_TxITConfig( MDR_CANx ,1UL<<BUF_TX_NOFLTR, ENABLE);

	//Заполнение структуры отправляемого по CAN информационного сообщения
	CANMsgTx.RTR = 0;
	CANMsgTx.nbyte = 0x05;
	CANMsgTx.SID = (0x090|CAN_ADR_UM)<<18;
	//CANMsgTx.SID = 0x092<<18;	//Усилитель ГН
	//CANMsgTx.SID = 0x091<<18;	//Усилитель ВН
	CANMsgTx.Data.ulong[0] = 0;
	CANMsgTx.Data.ulong[1] = 0;
		
	CAN_FiltersSet(CanAddr);	
}
/*--------------------------------------------------------------------------
 * инициализация CAN2 на порты PF2, PF3
 *-------------------------------------------------------------------------*/

void CAN2_init(void)
{
	CAN_InitTypeDef  sCAN;
	CanAddr=CAN_ADR_UM;	// рабочий CAN адрес усилителя (ГН или ВН)	
	// конфигурация портов
	// разрешение тактовой частоты для CAN1 и используемого порта
		PER_CLOCK_CAN2_INIT;

		PORT_CAN2_DDR_INIT;
		PORT_CAN2_FUNC_INIT;
		PORT_CAN2_DIGEN_INIT;
		PORT_CAN2_PWR_INIT;
		PORT_CAN2_RGND_INIT;	
		PORT_CAN2_RVCC_INIT;

//	// разрешение тактовой частоты для CAN1
//	RST_CLK_PCLKcmd(RST_CLK_PCLK_CANx,ENABLE);


	/* Set the HCLK division factor = 1 for CAN1*/
	CAN_BRGInit(MDR_CANx,CAN_HCLKdiv1);
	// конфигурация CAN1
	CAN_DeInit(MDR_CANx);
    CAN_StructInit (&sCAN);
    sCAN.CAN_ROM  = DISABLE;

    // настройка скорости на 1 МБОД
    sCAN.CAN_PSEG = CAN_PSEG_Mul_4TQ;
    sCAN.CAN_SEG1 = CAN_SEG1_Mul_7TQ;
    sCAN.CAN_SEG2 = CAN_SEG2_Mul_4TQ;
    sCAN.CAN_SJW  = CAN_SJW_Mul_3TQ;
    sCAN.CAN_SB   = CAN_SB_1_SAMPLE;
    sCAN.CAN_BRP  = 4;
	CAN_Init (MDR_CANx,&sCAN);
	
	/* Disable all CAN1 interrupt */
	CAN_ITConfig( MDR_CANx, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN |
    		      CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, DISABLE);

	CAN_TxITConfig( MDR_CANx ,1UL<<BUF_TX_NOFLTR, ENABLE);
	MDR_CANx->BUF_CON[BUF_TX_NOFLTR] |= CAN_BUF_CON_EN;

	//Заполнение структуры отправляемого по CAN информационного сообщения
	CANMsgTx.RTR = 0;
	CANMsgTx.nbyte = 0x05;
	CANMsgTx.SID = (0x090|CAN_ADR_UM)<<18;
	//CANMsgTx.SID = 0x092<<18;	//Усилитель ГН
	//CANMsgTx.SID = 0x091<<18;	//Усилитель ВН
	CANMsgTx.Data.ulong[0] = 0;
	CANMsgTx.Data.ulong[1] = 0;
		
	CAN_FiltersSet(CanAddr);	
	
	CAN_Cmd(MDR_CANx, ENABLE);
	
	if(MDR_CANx->BUF_CON[31] == 0)
		__nop();
}


/*--------------------------------------------------
 * НАСТРОЙКА ФИЛЬТРА приемника
 *--------------------------------------------------*/
static void CAN_FilterSet(unsigned char nombuf,unsigned long cmd,unsigned long addr)
{
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
//	CAN_FilterInitStruct.Mask_ID = 0xFFFFFFFF;
	CAN_FilterInitStruct.Mask_ID = 0x7FF<<18;
	CAN_FilterInitStruct.Filter_ID = SID(cmd,addr);
	CAN_FilterInit(MDR_CANx,nombuf,&CAN_FilterInitStruct);
	CAN_RxITConfig( MDR_CANx ,1UL<<nombuf, ENABLE);
	CAN_Receive(MDR_CANx, nombuf, ENABLE);
	MDR_CANx->BUF_CON[nombuf] &= ~4;
}


/*---------------------------------------------------------
 * настройка CAN фильтров усилителя мощности
 *---------------------------------------------------------*/
static void CAN_FiltersSet(unsigned long addr)
{
//	CAN_FilterSet(BUF_CMD_UM_STOP	 ,CMD_UM_STOP	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_STOPALL ,CMD_UM_STOPALL ,1);
//	CAN_FilterSet(BUF_CMD_UM_RTR	 ,CMD_UM_RTR	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_RTRALL	 ,CMD_UM_RTRALL	 ,1);
	CAN_FilterSet(BUF_CMD_UM_CTRL	 ,CMD_UM_CTRL	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_CTRL2	 ,CMD_UM_CTRL	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_CYCLE	 ,CMD_UM_CYCLE	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_CYCLEALL,CMD_UM_CYCLEALL,1);
//	CAN_FilterSet(BUF_CMD_UM_ANSW	 ,CMD_UM_ANSW	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_SYNCALL ,CMD_UM_SYNCALL ,0);
//	CAN_FilterSet(BUF_CMD_UM_PARAM	 ,CMD_UM_PARAM	 ,CanAddr);
//	CAN_FilterSet(BUF_CMD_UM_PRMALL	 ,CMD_UM_PRMALL	 ,1);
//	CAN_FilterSet(BUF_CMD_UM_PRMANSW ,CMD_UM_PRMANSW ,CanAddr);
//	CAN_FilterSet(BUF_RX_NOFLTR		 ,0		 ,0);
//	CAN_FilterSet(BUF_TX_NOFLTR		 ,0		 ,0);
}
