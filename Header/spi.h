/*
   spi.h
   Created on: 26.06.2012
*/
#ifndef SPI_H_
#define SPI_H_
#include "1986ve9x_ssp.h"
#include "ports.h"

// ------------ ИНИЦИАЛИЗАЦИЯ ПОРТА ДЛЯ SPI ----------------------------------

//SPI1
//**********************************
#define PORT_SPI1	 GPIOD			 //B	 //D	 //E	  //F
#define SSP1_FSS	 9				 //12	 //9	 //13	  //2
#define SSP1_CLK	 10				 //13	 //10	 //-	  //1
#define SSP1_RXD	 11				 //14	 //11	 //12	  //3
#define SSP1_TXD	 12				 //15	 //12	 //-	  //0
#define	PERNUM_nPORT1	 	 nPORTD	 //B	 //D	 //E	  //F
#define	PERNUM_nSPI1	 	 nSPI1	 //		 //		 //		  //
//Режим работы вывода порта			 //		 //		 //		  //
//0 – порт							 //		 //		 //		  //
//1 – основная функция				 //		 //		 //		  //
//2 – альтернативная функция		 //		 //		 //		  //
//3 – переопределенная функция		 //		 //		 //		  //
#define	MODE_WORK1	 3				 //2	 //3	 //2	  //2

//**********************************


//SPI2
//**********************************
#define PORT_SPI2	 GPIOC           //B       //C    //C    //F      //D
#define SSP2_FSS	 0               //12???   //0    //14   //12     //3
#define SSP2_CLK	 1               //13      //1    //-    //13     //5
#define SSP2_RXD	 2               //14???   //2    //15   //14???  //2
#define SSP2_TXD	 3               //15      //3    //-    //15     //6
#define	PERNUM_nPORT2	 	 nPORTC  //B       //C    //C    //F      //D
#define	PERNUM_nSPI2	 	 nSPI2	 //        //     //     //       //
//Режим работы вывода порта			 //        //     //     //       //
//0 – порт							 //        //     //     //       //
//1 – основная функция				 //        //     //     //       //
//2 – альтернативная функция		 //        //     //     //       //
//3 – переопределенная функция		 //        //     //     //       //
#define	MODE_WORK2	 3               //3       //3    //2    //3      //2

//**********************************


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// маска используемых аналоговых портов
#define MASK_PORT_SPI1	(((unsigned long)1<<SSP1_TXD)|\
						 ((unsigned long)1<<SSP1_RXD)|\
						 ((unsigned long)1<<SSP1_CLK)|\
						 ((unsigned long)1<<SSP1_FSS))
#define MASK_PORT_SPI2	(((unsigned long)1<<SSP2_TXD)|\
						 ((unsigned long)1<<SSP2_RXD)|\
						 ((unsigned long)1<<SSP2_CLK)|\
						 ((unsigned long)1<<SSP2_FSS))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// включение тактовой частоты
#define PER_CLOCK_SPI1_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT1;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nSPI1;}
#define PER_CLOCK_SPI2_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT2;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nSPI2;}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Режим работы вывода порта												 
#define PORT_SPI1_FUNC_INIT {PORT_SPI1->FUNC &= ~(((unsigned long)3<<(SSP1_TXD*2))|\
					      						((unsigned long)3<<(SSP1_RXD*2))|\
                                                ((unsigned long)3<<(SSP1_CLK*2))|\
												((unsigned long)3<<(SSP1_FSS*2)));\
							PORT_SPI1->FUNC |=   ((unsigned long)MODE_WORK1<<(SSP1_TXD*2))|\
						  						((unsigned long)MODE_WORK1<<(SSP1_RXD*2))|\
                                                ((unsigned long)MODE_WORK1<<(SSP1_CLK*2))|\
												((unsigned long)MODE_WORK1<<(SSP1_FSS*2));}
#define PORT_SPI2_FUNC_INIT {PORT_SPI2->FUNC &= ~(((unsigned long)3<<(SSP2_TXD*2))|\
					      						((unsigned long)3<<(SSP2_RXD*2))|\
                                                ((unsigned long)3<<(SSP2_CLK*2))|\
												((unsigned long)3<<(SSP2_FSS*2)));\
							PORT_SPI2->FUNC |=   ((unsigned long)MODE_WORK2<<(SSP2_TXD*2))|\
						  						((unsigned long)MODE_WORK2<<(SSP2_RXD*2))|\
                                                ((unsigned long)MODE_WORK2<<(SSP2_CLK*2))|\
												((unsigned long)MODE_WORK2<<(SSP2_FSS*2));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++													


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
// Режим работы контроллера
//0 – аналоговый
//1 – цифровой
#define	PORT_SPI1_DIGEN_INIT PORT_SPI1->DIGEN |= MASK_PORT_SPI1
#define	PORT_SPI2_DIGEN_INIT PORT_SPI2->DIGEN |= MASK_PORT_SPI2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// регистр разрешения подтяжки вывода к GND (100K)
//0 – подтяжка в питание выключена
//1 – подтяжка в питание включена (есть подтяжка ~100К)
#define	PORT_SPI1_RGND_INIT PORT_SPI1->RGND &= ~MASK_PORT_SPI1
#define	PORT_SPI2_RGND_INIT PORT_SPI2->RGND &= ~MASK_PORT_SPI2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// регистр разрешения подтяжки вывода к VCC (100K)
//0 – подтяжка в питание выключена
//1 – подтяжка в питание включена (есть подтяжка ~100К)
#define	PORT_SPI1_RVCC_INIT PORT_SPI1->RVCC &= ~MASK_PORT_SPI1
#define	PORT_SPI2_RVCC_INIT PORT_SPI2->RVCC &= ~MASK_PORT_SPI2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_SPI1_DDR_INIT PORT_SPI1->DDR |= MASK_PORT_SPI1
#define	PORT_SPI2_DDR_INIT PORT_SPI2->DDR |= MASK_PORT_SPI2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_SPI1_PWR_INIT {PORT_SPI1->PWR &= ~(((unsigned long)3<<(SSP1_FSS*2))|\
											  ((unsigned long)3<<(SSP1_CLK*2))|\
											  ((unsigned long)3<<(SSP1_RXD*2))|\
											  ((unsigned long)3<<(SSP1_TXD*2)));\
						   PORT_SPI1->PWR |=  (((unsigned long)2<<(SSP1_FSS*2))|\
						   					  ((unsigned long)2<<(SSP1_CLK*2))|\
											  ((unsigned long)2<<(SSP1_RXD*2))|\
						   					  ((unsigned long)2<<(SSP1_TXD*2)));}
#define	PORT_SPI2_PWR_INIT {PORT_SPI2->PWR &= ~(((unsigned long)3<<(SSP2_FSS*2))|\
											  ((unsigned long)3<<(SSP2_CLK*2))|\
											  ((unsigned long)3<<(SSP2_RXD*2))|\
											  ((unsigned long)3<<(SSP2_TXD*2)));\
						   PORT_SPI2->PWR |=  (((unsigned long)2<<(SSP2_FSS*2))|\
						   					  ((unsigned long)2<<(SSP2_CLK*2))|\
											  ((unsigned long)2<<(SSP2_RXD*2))|\
						   					  ((unsigned long)2<<(SSP2_TXD*2)));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				

//-----------------Инициализация SPI-----------------------------------
//Информационная скорость
//F_SSPCLK / ( CPSDVR * (1 + SCR)   
//---------------------------------
//200 кБит/сек (два слова по 16 бит -  160мкС < 250мкС (Период цикла таймера)) 
//80МГц/ (4 * (1 + 19) = 1 МБит/сек

//---------------- Регистр CR0------------------------------
//битовое поле SCR 15...8  от 0 до 255
#define CR0_SCR_1	 99	   //SPI1
#define CR0_SCR_2	 9	   //SPI2

//бит SPH 7 - фаза сигнала CLKOUT (используется только в режиме SPI Motorolla)
//"0" - по переднему фронту R\W, "1" - по заднему фронту. 
#define CR0_SPH_1	 1	   //SPI1
#define CR0_SPH_2	 1	   //SPI2

//бит SPO 6 - полярность сигнала CLKOUT (используется только в режиме SPI Motorolla)
//"0" - в режиме ожидания "0", "1" - в режиме ожидания "1".
#define CR0_SPO_1	 0	   //SPI1
#define CR0_SPO_2	 0	   //SPI2

//битовое поле FRF 5...4 - Формат информационного кадра
//00 – протокол SPI фирмы Motorola;
//01 – протокол SSI фирмы Texas Instruments;
//10 – протокол Microwire фирмы National Semiconductor;
//11 – резерв
#define CR0_FRF_1	 0	  //SPI1
#define CR0_FRF_2	 0	  //SPI2

//битовое поле DSS 3...0 - Размер слова данных
//0000 – резерв
//0001 – резерв
//0010 – резерв
//0011 – 4 бита
//|||| - ||||||
//1111 – 16 бит
#define CR0_DSS_1	 15	  //SPI1
#define CR0_DSS_2	 15	  //SPI2

#define SPI1_CR0_INIT (unsigned long)CR0_SCR_1<<8|\
					  (unsigned long)CR0_SPH_1<<7|\
					  (unsigned long)CR0_SPO_1<<6|\
					  (unsigned long)CR0_FRF_1<<4|\
					  (unsigned long)CR0_DSS_1
#define SPI2_CR0_INIT (unsigned long)CR0_SCR_2<<8|\
					 (unsigned long)CR0_SPH_2<<7|\
					 (unsigned long)CR0_SPO_2<<6|\
					 (unsigned long)CR0_FRF_2<<4|\
					 (unsigned long)CR0_DSS_2
//------------------------------------------------------------

//---------------Регистр CR1----------------------------------
//бит SOD 3 - Запрет выходных линий в режиме ведомого устройства
//"0" – управление линией SSP_TXD в ведомом режиме разрешена
//"1" – управление линией SSP_TXD в ведомом режиме запрещена
#define CR1_SOD_1	 0		//SPI1
#define CR1_SOD_2	 0		//SPI2

//бит MS 2 - Выбор ведущего или ведомого режима работы
//"0" – ведущий модуль (устанавливается по умолчанию)
//"1" – ведомый модуль
#define CR1_MS_1	 1		//SPI1
#define CR1_MS_2	 1		//SPI2

//бит SSE 1 - Разрешение работы приемопередатчика
//"0" – работа запрещена, "1" – работа разрешена
#define CR1_SSE_1	 1	    //SPI1
#define CR1_SSE_2	 1	    //SPI2

//бит LBM 0 - Тестирование по шлейфу
//"0" – нормальный режим работы приемопередатчика
//"1" – выход регистра сдвига передатчика соединен со входом регистра сдвига приемника
#define CR1_LBM_1	 0	    //SPI1
#define CR1_LBM_2	 0	    //SPI2

#define SPI1_CR1_INIT (unsigned long)CR1_SOD_1<<3|\
					 (unsigned long)CR1_MS_1<<2|\
					 (unsigned long)CR1_SSE_1<<1|\
					 (unsigned long)CR1_LBM_1
#define SPI2_CR1_INIT (unsigned long)CR1_SOD_2<<3|\
					 (unsigned long)CR1_MS_2<<2|\
					 (unsigned long)CR1_SSE_2<<1|\
					 (unsigned long)CR1_LBM_2
//---------------------------------------------------------------

//----------------Регистр CPSR-----------------------------------
//битовое поле CPSDVSR 7...0 от 2 до 254
//Коэффициент деления тактовой частоты
//число должно быть четным (младший бит автоматом устанавливается в "0"
#define CPSR_CPSDVSR_1	 4		//SPI1
#define CPSR_CPSDVSR_2	 4		//SPI2

#define SPI1_CPSR_INIT (unsigned long)CPSR_CPSDVSR_1   //SPI1
#define SPI2_CPSR_INIT (unsigned long)CPSR_CPSDVSR_2   //SPI2
//----------------------------------------------------------------

//----------------Регистр IMSC------------------------------------
//Регистр маски прерывания
//"1" – не маскирована
//"0" – маскирована

//бит TXIM 3 - Маска прерывания по заполнению на 50% и менее буфера FIFO передатчика
#define IMSC_TXIM_1	0	 //SPI1
#define IMSC_TXIM_2	0	 //SPI2

//бит RXIM 2 - Маска прерывания по заполнению на 50% и менее буфера FIFO приемника
#define IMSC_RXIM_1	0	//SPI1
#define IMSC_RXIM_2	0	//SPI2

//бит RTIM 1 - Маска прерывания по таймауту приемника 
//(буфер FIFO приемника не пуст и не было попыток его чтения в течение времени таймаута)
#define IMSC_RTIM_1	0	//SPI1
#define IMSC_RTIM_2	0	//SPI2

//бит RORIM 0 -	Маска прерывания по переполнению буфера приемника
#define IMSC_RORIM_1	0	//SPI1
#define IMSC_RORIM_2	0	//SPI2

#define SPI1_IMSC_INIT (unsigned long)IMSC_TXIM_1<<3|\
					  (unsigned long)IMSC_RXIM_1<<2|\
					  (unsigned long)IMSC_RTIM_1<<1|\
					  (unsigned long)IMSC_RORIM_1
#define SPI2_IMSC_INIT (unsigned long)IMSC_TXIM_2<<3|\
					  (unsigned long)IMSC_RXIM_2<<2|\
					  (unsigned long)IMSC_RTIM_2<<1|\
					  (unsigned long)IMSC_RORIM_2
//-----------------------------------------------------------------

//-----------------Регистр DMACR-----------------------------------
//Регистр управления прямым доступом к памяти
//"1" - разрешено, "0" - запрещено
//бит TXDMAE 1 - Использование DMA при передаче
#define DMACR_TXDMAE_1	0	 //SPI1
#define DMACR_TXDMAE_2	0	 //SPI2

//бит RXDMAE 0 - Использование DMA при приеме
#define DMACR_RXDMAE_1	0 	 //SPI1
#define DMACR_RXDMAE_2	0 	 //SPI2

#define SPI1_DMACR_INIT (unsigned long)DMACR_TXDMAE_1<<1|\
					   (unsigned long)DMACR_RXDMAE_1
#define SPI2_DMACR_INIT (unsigned long)DMACR_TXDMAE_2<<1|\
					   (unsigned long)DMACR_RXDMAE_2					   				
//------------------------------------------------------------------

//запись в буфер SPI
#define			SPI1_WR(data)		SSP1->DR = ((unsigned long)data)	//SPI1
#define			SPI2_WR(data)		SSP2->DR = ((unsigned long)data)	//SPI2

//чтение из буфера SPI
#define			SPI1_RD				SSP1->DR   //SPI1
#define			SPI2_RD				SSP2->DR   //SPI2

//---------------------------------------------
//чтение флага пустоты буфера FIFO приемника
//"0" - буфер FIFO приемника пуст;
//"1" - буфер FIFO приемника не пуст.
//регистр SR
//бит 2 - RNE
#define			SPI1_SR_RNE				(SSP1->SR & 0x04)  //SPI1
#define			SPI2_SR_RNE				(SSP2->SR & 0x04)  //SPI2
//---------------------------------------------

// экспорт функций
extern void SPI1_Init (void);
extern void SPI2_Init (void);

#endif /* SPI_H_ */
