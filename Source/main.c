
#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_board.h"
#include "MDR32F9Qx_can.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_wwdg.h"

#include "common1.h"
#include "1986ve9x_wdg.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_tim.h"
#include "1986ve9x_gpio.h"
#include "1986ve9x_uart.h"
#include "defports.h"
#include "pll.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "ctrlpwm.h"
#include "wake.h"
#include "protocol.h"
//#include "eeprom.h"
#include "boot.h"
#include "adr.h"
#include "exchange.h"
#include "ports.h"
#include "position.h"
#include "1986ve9x_adc.h"
#include "dac.h"
#include "Debug.h"
#include "spi.h"
#include "can.h"
#include "device.h"
#include "MDR32F9Qx_eeprom.h"


#define VERSION		8
#define ADDR_LOT    0x08000000    


typedef __packed struct
{
	unsigned char type;
	unsigned char begPt;
	unsigned char endPt;
	unsigned char freq;
}TDebug ; 

TDebug Debug;
TInfo	Info;
unsigned char lotbuf[6];



/* Private function prototypes -----------------------------------------------*/

// функции
//void work(void);
// объявление указателя на функцию work для сохранения постоянного адреса
//const TWorkFunc workfunc __attribute__((at(ADDR_WORK_ENTRY))) = work ;

//void(*TWorkFunc)(void)
//const void(*)(void) workfunc __attribute__((at(ADDR_WORK_ENTRY))) = work ;
// инициализация портов в режиме микроконтроллера
void Ports_Init_MK		(void);

// калибровка АЦП
void ADC_Calibr 		(void);

// обработка обмена по WAKE
void ProcessWake		(UART_TypeDef * pUART);
//void ProcessWake(void);


// главный цикл в режиме микроконтроллера
//void Process_MK			(void);

// главный цикл в режиме енкодера
void Process_ENC		(void);

// обработка линий управления CTRL и RDY
//void ProcessCtrl		(void);

// тестирование FLASH
//unsigned long TestFlash(TTypeMem TypeMem, unsigned long testadr,unsigned char *buf, unsigned short len);

// инициализация глобальных переменных
void Variable_Init		(void);


//------------------------------------------------------------------------------
/*
void work(void)
{
	// тактирование включили все порты
	PER_CLOCK |= 	(unsigned long)1<<nPORTA|
					(unsigned long)1<<nPORTB|
					(unsigned long)1<<nPORTC|
					(unsigned long)1<<nPORTD|
					(unsigned long)1<<nPORTE|
					(unsigned long)1<<nPORTF;
	Process_MK();

}

*/

void EEPROM_Init(void)
{
    unsigned long i;
    PER_CLOCK |= (unsigned long)1<<nEEPROM_CNTRL;
    *(__packed unsigned long *)lotbuf = EEPROM_ReadWord(ADDR_LOT,EEPROM_Info_Bank_Select);
    for(i=0;i<10000;i++)
    *(__packed unsigned short *)&lotbuf[4] = EEPROM_ReadHalfWord(ADDR_LOT+4,EEPROM_Info_Bank_Select);    
    
}
/*--------------------------------------------------------------------------
 * РАБОТА МИКРОКОНТРОЛЛЕРА
 *-------------------------------------------------------------------------*/
int main(void) 
{
	MDR_EEPROM->CMD |= (0x3<<EEPROM_CMD_DELAY_Pos); //
	PLL_Init();
	PER_CLOCK |= 	(unsigned long)1<<nPORTA|
					(unsigned long)1<<nPORTB|
					(unsigned long)1<<nPORTC|
					(unsigned long)1<<nPORTD|
					(unsigned long)1<<nPORTE|
					(unsigned long)1<<nPORTF;
    
    
    EEPROM_Init();
//	CAN2_init();	
	
//	Ports_Init_MK();	
//	ADC_Init();			
//	DAC_Init();
//  SPI1_Init();
//	SPI2_Init();			
	UART1_Init(115200);	
//	UART2_Init(115200);	
	WAKE_Init(255);
	TIM_pwmInit(1);		
	CtrlPwm[CPWM_VERSOFT]  =  VERSION;
	Variable_Init();
//	if(MDR_CANx->BUF_CON[31] == 0)
//		__nop();
	NVIC_EnableIRQ(Timer1_IRQn);
//	PORT_CLR(PORT_UM_VKL,BIT_UM_VKL); //Сигнал "УМ ВКЛ" - включен
	while(1)
	{  
		ProcessWake(UART1);
	}

}



//------------------------------------------------------------------------------
void Ports_Init_MK(void)
{
//Входы:
//++++++++++++++++++++++++++++++++++++++++++++
	//Аппаратная защита по току с триггера
	PORT_InitDI(PORT_ALARM_TRIG,BIT_ALARM_TRIG,0,0,0);

	//Коммутатор 1
	PORT_InitDI(PORT_SWITH_1,BIT_SWITH_1,0,0,0);

	//Коммутатор 2
	PORT_InitDI(PORT_SWITH_2,BIT_SWITH_2,0,0,0);

	//Коммутатор 3
	PORT_InitDI(PORT_SWITH_3,BIT_SWITH_3,0,0,0);


	//Команда включения УМ "Вкл. Привода ГН"
//	PORT_InitDI(PORT_VKL_UM,BIT_VKL_UM,0,0,0);

	//Сигнал на запуск ВИП
//	PORT_InitDI(PORT_VIP_START,BIT_VIP_START,0,0,0);

	//Корпус "Люки БМ закрыты"
//	PORT_InitDI(PORT_LYUKI_BM,BIT_LYUKI_BM,0,0,0);

	//Корпус "Люки шасси закрыты"
//	PORT_InitDI(PORT_LYUKI_SCH,BIT_LYUKI_SCH,0,0,0);

    //+15В от концевика "РСВП"
//	PORT_InitDI(PORT_RSVP,BIT_RSVP,0,0,0);

	//Команда "Аварийный поворот" +27В
//	PORT_InitDI(PORT_ALARM_ROTATE,BIT_ALARM_ROTATE,0,0,0);

    //Корпус "ММПБ Вкл." (-27В тормозная муфта вкл.)
//    PORT_InitDI(PORT_BRAKE_MUFF,BIT_BRAKE_MUFF,0,0,0);

	//Готовность преобразователя напряжения.	 "0" - преобр. готов
//	PORT_InitDI(PORT_GOTOV_PREOBR,BIT_GOTOV_PREOBR,0,0,0);
	
	//Резерв  DIG3_IN
//	PORT_InitDI(PORT_DIG3_IN,BIT_DIG3_IN,0,0,0); 
//++++++++++++++++++++++++++++++++++++++++++++

//Выходы:
//--------------------------------------------	
	PORT_InitDO(PORT_RESET_TRIG,BIT_RESET_TRIG,2,0);			//Сброс триггера аппаратной защиты	
	PORT_InitDO(PORT_EXT_INT1,BIT_EXT_INT1,2,0);				//Запрос на выдачу по SPI IA и IB
	PORT_InitDO(PORT_REGISTR_PWM_OUT,BIT_REGISTR_PWM_OUT,2,0);	//Включение ШИМ выходов на регистре 1594АП5
	PORT_InitDO(PORT_UM_VKL,BIT_UM_VKL,2,0);					//Dig21_out	 Сигнал УМ_ВКЛ
	PORT_SET(PORT_UM_VKL,BIT_UM_VKL); 							//Снять сигнал "УМ ВКЛ"
	PORT_InitDO(PORT_POWER_RELAY,BIT_POWER_RELAY,2,0);			//Dig12_out Сигнал на вкл. силовых реле
	PORT_SET(PORT_POWER_RELAY,BIT_POWER_RELAY);					//Снять сигнал ВКЛ. сил.реле
	PORT_InitDO(PORT_OUT_SWITH1,BIT_OUT_SWITH1,2,0);			//Dig13_out Выход на коммутатор 1 ВН
	PORT_SET(PORT_OUT_SWITH1,BIT_OUT_SWITH1);					//Снять сигнал на коммутатор 1
	PORT_InitDO(PORT_OUT_SWITH2,BIT_OUT_SWITH2,2,0);			//Dig14_out Выход на коммутатор 2 ПУЛ
	PORT_SET(PORT_OUT_SWITH2,BIT_OUT_SWITH2);					//Снять сигнал на коммутатор 2
	PORT_InitDO(PORT_OUT_SWITH3,BIT_OUT_SWITH3,2,0);			//Dig15_out Выход на коммутатор 3 ПУП
	PORT_SET(PORT_OUT_SWITH3,BIT_OUT_SWITH3);					//Снять сигнал на коммутатор 3
	PORT_InitDO(PORT_DIG19,BIT_DIG19,2,0);						//Dig19 Меандр 20 кГц
	PORT_InitDO(PORT_RSTX_DE,BIT_RSTX_DE,2,0);					//Dig10 Сигнал включения передатчика
	PORT_InitDO(PORT_dig18,BIT_dig18,2,0);
//---------------------------------------------

//Спец. функции:
//+++++++++++++++++++++++++++++++++++++++++++++
	// инициализация выходов PWM
	PORT_InitFN(PORT_PWM_OUT,BIT_PWM_OUT1,2,2);	// PWM1
	PORT_InitFN(PORT_PWM_OUT,BIT_PWM_OUT2,2,2);	// PWM2
	PORT_InitFN(PORT_PWM_OUT,BIT_PWM_OUT3,2,2);	// PWM3
	PORT_InitFN(PORT_PWM_OUT,BIT_PWM_OUT4,2,2);	// PWM4
	PORT_InitFN(PORT_PWM_OUT,BIT_PWM_OUT5,2,2);	// PWM5
	PORT_InitFN(PORT_PWM_OUT,BIT_PWM_OUT6,2,2);	// PWM6
//+++++++++++++++++++++++++++++++++++++++++++++

}


/*------------------------------------------------------------------------------
 *  ОБРАБОТКА WAKE
 *----------------------------------------------------------------------------*/

void ProcessWake(UART_TypeDef * pUART)
{
	unsigned char cmd;
	unsigned char RxMsgLen;
	unsigned char len;
	unsigned long i;
	// обработка wake
	if(!WAKE_GetMsg (pUART,&cmd, RxMsg, &RxMsgLen))
	{
		switch(cmd)
		{
		case APC_ECHO:	//	Запрос возврата переданного пакета
			WAKE_PutMsg(pUART,cmd,RxMsg,RxMsgLen);
			break;
		case APC_INFO:	//	Запрос информации об ЭОК
			
			Info.DevCode = DEV_CODE_UM100;
			Info.Version = VERSION;
			WAKE_PutMsg(pUART,APC_INFO,(__packed unsigned char*)&Info,sizeof(TInfo));
		//WAKE_PutMsg(pUART,APC_INFO,RxMsg,RxMsgLen);
			
			break;
		case APC_POS:	//	Позиционирование на заданный угол.
	//		if(*(unsigned long*)RxMsg < (1<<ENC_MAX_DIGIT))
			{
				// для отладки !!!
				CtrlPwm[CPWM_BETTA] = *(unsigned long*)RxMsg/32;
				TxMsg[0] = 0;	// успешно
			}
//			else
//				TxMsg[1] = 1; 	// ошибка
			WAKE_PutMsg(pUART,APC_POS,TxMsg,1);
			break;

		case APC_BUSY:	//	Проверка  готовности ЭОК

			break;


		case APC_WRITE:	//	Запись значения углов соответствующих позиций во ОЗУ ЭОК
			if(RxMsgLen>sizeof(TCFG))
				TxMsg[0] = 0x01;
			else
			{
				for(i = 0; i<RxMsgLen;i++)
					((unsigned char*)&Config.AngPnt)[i] = RxMsg[i];
				TxMsg[0] = 0x00;
			}
			len = 1;
			WAKE_PutMsg(pUART,APC_WRITE,TxMsg,len);
			break;

//		case APC_READ:	//	Чтение значения углов соответствующих позиций из ОЗУ ЭОК
//			WAKE_PutMsg(pUART,APC_READ,(unsigned char*)Config.AngPnt,MAX_POINT*4);
//			break;

		case APC_STORE:	//	сохранения указанных данных из ОЗУ в энергонезависимой памяти
			if(RxMsg[0] == 0x00)
			{
				Config.AngleD = CtrlPwm[CPWM_DANGLE]; // сохранение поправка в стуктуру
				Config.AngInit = CtrlPwm[CPWM_ANGLINIT]; // сохранение поправка в стуктуру
//				EEPROM_ErasePage(MEM_INF,ADDR_CONFIG);
//				EEPROM_WriteData(MEM_INF,ADDR_CONFIG,(unsigned char*)&Config,sizeof(TCFG));
				TxMsg[0] = 0x00;
			}
			else
				TxMsg[0] = 0x01;
			len = 1;
			WAKE_PutMsg(pUART,APC_STORE,TxMsg,len);
			break;

		case APC_RESTORE:	//	восстановление указанных данных из энергонезависимой памяти в ОЗУ
			if(RxMsg[0] == 0x00)
			{
//				EEPROM_ReadData(MEM_INF,ADDR_CONFIG,(unsigned char*)&Config,sizeof(TCFG));
				CtrlPwm[CPWM_DANGLE] = Config.AngleD; // восстановили поправку в рабочую переменую
				CtrlPwm[CPWM_ANGLINIT] = Config.AngInit; // восстановили базовый угол
				TxMsg[0] = 0x00;
			}
			else
				TxMsg[0] = 0x01;
			len = 1;
			WAKE_PutMsg(pUART,APC_RESTORE,TxMsg,len);
			break;

		case APC_VAR_READ:	//	Чтение сервисных переменных системой верхнего уровня
			len = RxMsgLen*4;
			while(RxMsgLen--)
				*(long*)&TxMsg[RxMsgLen*4] = CtrlPwm[RxMsg[RxMsgLen]];
			WAKE_PutMsg(pUART,APC_VAR_READ,TxMsg,len);
			break;

		case APC_VAR_WRITE:	//	запись сервисных переменных системой верхнего уровня
			len = RxMsgLen/5;
			i = 0;			// позиция индекса
			TxMsg[0] = 0;
			while(len)
			{
				if(RxMsg[i] >= SIZE_CTRL_PWM)
				{
					TxMsg[0] = 1;
					break;
				}
				CtrlPwm[RxMsg[i]] = *(unsigned long*)&RxMsg[i+1];
// проверка записи заданного угла в текущую точку
				if(RxMsg[i] == CPWM_ANGLTASK)
				{
					Config.AngPnt[CtrlPwm[CPWM_POINT]] = CtrlPwm[CPWM_ANGLTASK];
					Pos.change_task = 1;
				}

				len--;
				i += 5;
			}

			WAKE_PutMsg(pUART,APC_VAR_WRITE,TxMsg,1);
			break;

		case APC_MODE: // режим работы
			if(RxMsg[0]>3)
				TxMsg[0] = 0x01;
			else
			{
				workMode = (TMODE)RxMsg[0];
				TxMsg[0] = 0x00;
			}

			WAKE_PutMsg(pUART,APC_MODE,TxMsg,1);
			break;

		case APC_DEBUG: // отработка серии
			Debug = *(TDebug*)RxMsg;
			TxMsg[0] = 0x00;
			WAKE_PutMsg(pUART,APC_DEBUG,TxMsg,1);
			break;
		
		
		case APC_BOOT:	//	Переход в загрузчик
			// отправляем ответ
			TxMsg[0] = 0;	// успешно
			WAKE_PutMsg(pUART,APC_BOOT,TxMsg,1);
			for(i = 0; i<50000; i++);
			// уйти на перезагрузку
			RST_CLK_PCLKcmd(RST_CLK_PCLK_WWDG,ENABLE);
			MDR_BKP->REG_0E &= ~BKP_REG_0E_BKP_REG_Msk;
			MDR_BKP->REG_0E  |= 0x6<<BKP_REG_0E_BKP_REG_Pos;
			MDR_WWDG->CFR = 0x00;
			MDR_WWDG->CR = 0xFF;
			MDR_WWDG->CR = 0xFF;
			WWDG_Enable(0x7F);
			while (1);
		
		
		case APC_OSC_START:
			oscil_ind[0] = RxMsg[0];
			oscil_ind[1] = RxMsg[1];
			oscil_ind[2] = RxMsg[2];
			oscil_ind[3] = RxMsg[3];
			oscil_en = 1;
			break;
		
        case APC_OSC_STOP:	
			oscil_en = 0;
			break;
		
		case APC_OSC_CTRL:	
			ctrlbuf.var = RxMsg[0];                                 // индекс переменной
            ctrlbuf.frm = RxMsg[1];                                 // форма сигнала
            ctrlbuf.amp = *((__packed unsigned short*)&RxMsg[2]);   // амплитуда
            ctrlbuf.prd = *((__packed unsigned long*)&RxMsg[4]);    // период в мс
			break;
        
        case APC_LOT:
            __disable_irq();
            if(RxMsg[0])
            {
                EEPROM_ErasePage (ADDR_LOT, EEPROM_Info_Bank_Select);
                for(i=0;i<10000;i++)
                EEPROM_ProgramWord(ADDR_LOT,EEPROM_Info_Bank_Select,*(__packed unsigned long *)&RxMsg[1]);
                for(i=0;i<10000;i++)
                EEPROM_ProgramHalfWord(ADDR_LOT+4,EEPROM_Info_Bank_Select,*(__packed unsigned short *)&RxMsg[5]);
                WAKE_PutMsg(pUART,APC_LOT,TxMsg,1);
            }
            else
            {
                *(__packed unsigned long *)TxMsg = EEPROM_ReadWord(ADDR_LOT,EEPROM_Info_Bank_Select);
                for(i=0;i<10000;i++)
                *(__packed unsigned short *)&TxMsg[4] = EEPROM_ReadHalfWord(ADDR_LOT+4,EEPROM_Info_Bank_Select);
                WAKE_PutMsg(pUART,APC_LOT,TxMsg,6);
            }
            __enable_irq();
        break;
		

		}
	}
}


/*--------------------------------------------------------------------------------------
 * Инициализация глобальных переменных
 *--------------------------------------------------------------------------------------*/
void Variable_Init(void)
{
 	CtrlPwm[CPWM_VERSOFT]  =  VERSION + VER_OFFSET;
	CtrlPwm[CPWM_ANGL] = 0;	  //gamma
	CtrlPwm[CPWM_VLC] = 0;	  //скорость
	CtrlPwm[CPWM_IQIN_] = 0;  //заданное управление
	CtrlPwm[CPWM_IQINN] = 0;  //текущее управление	

	CtrlPwm[CPWM_K1]=KPI1;
	CtrlPwm[CPWM_K2]=KPI2;
	//CtrlPwm[CPWM_DANGLE]=5850;	  //поправка угла
	CtrlPwm[CPWM_DANGLE]= DANGLE;	  //поправка угла
	CtrlPwm[CPWM_COUNT] = 0;
	CtrlPwm[CPWM_CAN_COUNT] = 0;
	CtrlPwm[CPWM_CAN_KOMAND] = 0;  //Текущая Команда по CAN

	CtrlPwm[CPWM_CAN_ID] = 0;
	CtrlPwm[CPWM_CAN_DLC] = 0;
	CtrlPwm[CPWM_CAN_DATAH] = 0;
	CtrlPwm[CPWM_CAN_DATAL] = 0;
	CtrlPwm[CPWM_IQIN] = 0;
	
	
	CtrlPwm[CPWM_DEBUG1]  =  0;
	CtrlPwm[CPWM_DEBUG2]  =  0;
	CtrlPwm[CPWM_DEBUG3]  =  0;
	CtrlPwm[CPWM_DEBUG4]  =  0;
	CtrlPwm[CPWM_DEBUG7]  =  0;
	CtrlPwm[CPWM_DEBUG8]  =  0;
	CtrlPwm[CPWM_DEBUG9]   =  1000;
	CtrlPwm[CPWM_DEBUG10]  =  0;
	
	CtrlPwm[CPWM_INTIQ] = 0;
	CtrlPwm[CPWM_INTID] = 0;
	
	CtrlPwm[CPWM_KV]  = KV;
	CtrlPwm[CPWM_KINTV] = KINTV;
	CtrlPwm[CPWM_INTVLC] = 0;
	Count_Contrl=1000;
	
	CtrlPwm[CPWM_TIMCNT] = 0;
}
