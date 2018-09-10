/*******************************************************************************
 * Copyright:      
 * File Name:      util.h
 * Description:    Í¨ÓÃ¹¤¾ßº¯ÊýºÍÍ¨ÓÃºê¶¨Òå
 * Author:         HL
 * Version:        V1.0
 * Date:           2016-10-12
 * History:        µ÷ÊÔÊä³öºê¡¢CRC8Ð£Ñé¡¢CRC16Ð£Ñé
 *                 
 ******************************************************************************/

#ifndef USER_MODULE_UTIL
#define USER_MODULE_UTIL


#include "gd32f1x0.h"


/*******************************************************************************
 * Description:    Êä³öµ÷ÊÔÐÅÏ¢
 *                 type = 1 -- Êä³öµ÷ÊÔÐÅÏ¢
 *                 type = 0 -- ²»Êä³öµ÷ÊÔÐÅÏ¢
 *                 type - ¸÷¸öÄ£¿éµ¥¶À¶¨Òå£¬ÓÃÓÚ¿ØÖÆµ¥¸öÄ£¿éµ÷ÊÔÐÅÏ¢ÊÇ·ñÊä³ö
 ******************************************************************************/

#define USER_DEBUG_INFO    //ÊÇ·ñ±àÒëµ÷ÊÔÐÅÏ¢£ºdefine--±àÒë / undef--²»±àÒë

#ifdef USER_DEBUG_INFO
	#define DB_MSG(type, fmt, arg...) \
		do{ \
			if(type) \
				printf(fmt, ##arg); \
		}while(0)

	//ÏÂÃæ \t ÊÇÎªÁËÊ¹"\t"ºóÃæµÄÊä³ö¶ÔÆë£¬³¬³¤Ê±²»½Ø¶Ï£¬×ó¶ÔÆë
	#define DB_BUG(type, fmt, args...) \
		do{ \
			if(type) \
				printf("%-20s\t(%d):\t " fmt, __func__, __LINE__, ##args); \
		}while (0)
#else
	#define DB_MSG(type, fmt, arg...)
	#define DB_BUG(type, fmt, args...)
#endif
		
extern uint16_t CRC_16C(uint8_t *pbuf, uint16_t len, uint16_t crc16);
		
extern uint16_t CRC_16C_32(uint32_t *pbuf, uint16_t len, uint16_t crc16);
	
extern uint16_t CRC_16S(uint8_t *pbuf, uint16_t len);

extern uint8_t CRC_8(uint8_t *pbuf, uint8_t len);

extern uint8_t sum8_gen(uint8_t *pbuf,uint8_t len);

extern uint8_t byte_finder(uint8_t byte, uint8_t *byte_array, uint8_t len);

extern uint8_t range_finder(uint8_t No, uint8_t *range_array, uint8_t len);

#endif
