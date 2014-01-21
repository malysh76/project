#ifndef __CRC_H__
#define __CRC_H__

	#define TYPE_CRC8	0
	#define TYPE_CRC16	1
	#define TYPE_CRC32	2

	#define CRC8P		0x8C    // основание для расчета CRC8

	extern unsigned char 	CalcArrayCRC8	(unsigned char CRC8, unsigned char* Data, unsigned short len);
	extern unsigned char 	CalcByteCRC8	(unsigned char CRC8, unsigned char Data);
	extern unsigned char 	CalcCRC8		(unsigned char CRC8, unsigned char Data);
	extern unsigned short 	CalcArrayCRC16 	(unsigned short CRC16, unsigned char* Data, unsigned short len);
	extern unsigned short 	CalcByteCRC16 	(unsigned short CRC16, unsigned char Data);
	extern unsigned long 	CalcArrayCRC32 	(unsigned long CRC32, unsigned char* Data, unsigned short len);
	extern unsigned long 	CalcByteCRC32 	(unsigned long CRC32, unsigned char Data);

#endif
