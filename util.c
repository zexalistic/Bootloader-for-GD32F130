/*******************************************************************************
 * Copyright:      
 * File Name:      util.c
 * Description:    通用工具函数和通用宏定义
 * Author:         HL
 * Version:        V1.0
 * Date:           2016-10-12
 * History:        CRC8校验、CRC16校验、
 ******************************************************************************/


#include "util.h"
#include "protocol.h"

//CRC16校验表: X16+X12+X5+1
uint16_t crc16Table[256] = 
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,  
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,  
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,  
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,  
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,  
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,  
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,  
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,  
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,  
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,  
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,  
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,  
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,  
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,  
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,  
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,  
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,  
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,  
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,  
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,  
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,  
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,  
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,  
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,  
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,  
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,  
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,  
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,  
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,  
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,  
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,  
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 
};

//CRC8校验表
uint8_t crc8Table[256] =
{
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20,
	0xa3, 0xfd, 0x1f, 0x41, 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
    0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 0x23, 0x7d, 0x9f, 0xc1,
	0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e,
	0x1d, 0x43, 0xa1, 0xff, 0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
    0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 0xdb, 0x85, 0x67, 0x39,
	0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45,
	0xc6, 0x98, 0x7a, 0x24, 0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
    0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 0x8c, 0xd2, 0x30, 0x6e,
	0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31,
	0xb2, 0xec, 0x0e, 0x50, 0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
    0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 0x32, 0x6c, 0x8e, 0xd0,
	0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea,
	0x69, 0x37, 0xd5, 0x8b, 0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
    0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 0xe9, 0xb7, 0x55, 0x0b,
	0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54,
	0xd7, 0x89, 0x6b, 0x35,
};

/*******************************************************************************
 * Function:       CRC_16S
 * Description:    CRC16校验算法
 * Input:          uint8_t *pbuf: 待校验的数据指针
 *                 uint16_t len:  待检验的数据长度
 * Return:         uint16_t:      校验结果 
 * Note:           多项式: X16+X12+X5+1
 ******************************************************************************/
uint16_t CRC_16S(uint8_t *pbuf, uint16_t len)
{
	return CRC_16C(pbuf, len, 0);
}

/*******************************************************************************
 * Function:       CRC_16C
 * Description:    CRC16校验算法
 * Input:          uint8_t *pbuf: 待校验的数据指针
 *                 uint16_t len:  待检验的数据长度
 * Return:         uint16_t:      校验结果 
 * Note:           多项式: X16+X12+X5+1
 ******************************************************************************/
uint16_t CRC_16C(uint8_t *pbuf, uint16_t len, uint16_t crc16)
{
	uint8_t  crc16Hi;
	
	while(len--)
	{
		crc16Hi = (uint8_t)(crc16 >> 8);         //È¡crc16µÄ¸ß8Î»
		crc16 = crc16 << 8;                 //crc16×óÒÆ8Î»
		crc16 = crc16 ^ crc16Table[crc16Hi ^ (*pbuf)];
		pbuf++;
	}
	
	return crc16;
}

uint16_t CRC_16C_32(uint32_t *pbuf, uint16_t len, uint16_t crc16)
{
	uint8_t pduf0,pduf1,pduf2,pduf3;
	uint8_t  crc16Hi;
	

	
	while(len--)
	{
	  pduf0 = *pbuf & 0x000000ff ;
	  pduf1 = (*pbuf & 0x0000ff00) >>8;
	  pduf2 = (*pbuf & 0x00ff0000) >>16;
	  pduf3 = (*pbuf & 0xff000000) >>24;
		
		crc16Hi = (uint8_t)(crc16 >> 8);         //取crc16的高8位
		crc16 = crc16 << 8;                 //crc16左移8位
		crc16 = crc16 ^ crc16Table[crc16Hi ^ (pduf0)];
		crc16Hi = (uint8_t)(crc16 >> 8);         //取crc16的高8位
		crc16 = crc16 << 8;                 //crc16左移8位
		crc16 = crc16 ^ crc16Table[crc16Hi ^ (pduf1)];
		crc16Hi = (uint8_t)(crc16 >> 8);         //取crc16的高8位
		crc16 = crc16 << 8;                 //crc16左移8位
		crc16 = crc16 ^ crc16Table[crc16Hi ^ (pduf2)];
		crc16Hi = (uint8_t)(crc16 >> 8);         //取crc16的高8位
		crc16 = crc16 << 8;                 //crc16左移8位
		crc16 = crc16 ^ crc16Table[crc16Hi ^ (pduf3)];
		pbuf++;
	}
	
	return crc16;
}


/*******************************************************************************
 * Function:       CRC_8
 * Description:    CRC8校验算法
 * Input:          uint8_t *pbuf: 待校验的数据指针
 *                 uint8_t len:   待检验的数据长度
 * Return:         uint8_t:       校验结果 
 * Note:           多项式: 
 ******************************************************************************/
uint8_t CRC_8(uint8_t *pbuf, uint8_t len)
{
	uint8_t crc8 = 0;

	for(; len > 0; len--)
	{
		crc8 = crc8Table[crc8 ^ (*pbuf)];
		pbuf++;
	}

	return crc8;
}


//SUM8和校验计算
uint8_t sum8_gen(uint8_t *pbuf,uint8_t len)
{
	uint8_t sum8=0;
	uint8_t t=0;
	for(t=0;t<len;t++)
	{
		sum8+=pbuf[t];
	}
	return sum8;
}

//字节查找
//byte-待查找的字节
//byte_array-待查找的字节序列
//len-查找范围
//返回-查找到，则返回第一个字节序列中的序号，否则返回0xFF（未找到）
uint8_t byte_finder(uint8_t byte, uint8_t *byte_array, uint8_t len)
{
	uint8_t t=0;
	for(;len>0;len--)
	{
		if(byte == *(byte_array++))
		{
			return t;              //返回该命令的序号
		}
		t++;
	}
	return 0xFF;                      //没有找到该命令，返回0xFF
}

//范围查找，查找表中第一个不大于输入编号的元素的序号
//No-查找编号
//range_array-查找序列
//len-查找次数
//返回值-0xFF-比列表中的值都要小，0x0-不小于列表中的任何值， 其他-区间指针
uint8_t range_finder(uint8_t No, uint8_t *range_array, uint8_t len)
{
	uint8_t t=0;
	for(;len>0;len--)
	{
		if(No>=*(range_array++))
		{
			return t;
		}
		t++;
	}
	return 0xFF;     
}

//饱和运算
float sauout(float x, float min, float max)
{
	if(x<min) return min;
	else if(x>max) return max;
	else return x;
}



