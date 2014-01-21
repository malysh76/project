#ifndef __FIFO_BUF_H__
	#define __FIFO_BUF_H__
	
// Типы данных
	typedef struct
	{
		volatile unsigned short	wr;		// указатель на запись
		volatile unsigned short	rd;		// указатель на чтение
		unsigned short			size;	// размер буфера (2^n, где n = 1..16)
		unsigned short			mask;	// маска размера буфера
		volatile unsigned char	*data;	// указатель на буфер данных
	} TFIFO_BUF;
	
// Макроопределения
/************************	Очистка буфера	*************************************/
/*	Входные параметры:															*/
/*		fifo - указатель на FIFO буфер											*/
/*	Выходные параметры:	нет														*/
/********************************************************************************/	
	#define FIFO_CLEAR(fifo)	{(fifo)->wr = 0; (fifo)->rd = 0;}
/*************	Определение количества данных в буфере	*************************/
/*	Входные параметры:															*/
/*		fifo - указатель на FIFO буфер											*/
/*	Выходные параметры:	нет														*/
/********************************************************************************/	
	#define FIFO_AVAIL(fifo)	((unsigned short)((fifo)->size + (fifo)->wr - (fifo)->rd) & (fifo)->mask)
/*************	Определение свободного места в буфере	*************************/
/*	Входные параметры:															*/
/*		fifo - указатель на FIFO буфер											*/
/*	Выходные параметры:	нет														*/
/********************************************************************************/	
	#define FIFO_FREE(fifo)		((unsigned short)((fifo)->mask + (fifo)->rd - (fifo)->wr) & (fifo)->mask)
	
// Экспортируемые функции
	extern void 			FIFO_Init 	(TFIFO_BUF* fifo, volatile unsigned char* buf, unsigned short fifo_size);
	extern unsigned short	FIFO_Put 	(TFIFO_BUF* fifo, volatile unsigned char* data, unsigned short len);
	extern unsigned short	FIFO_Get	(TFIFO_BUF* fifo, volatile unsigned char* data, unsigned short len);
	
#endif
