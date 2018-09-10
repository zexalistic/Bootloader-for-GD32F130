#ifndef __IAP_H__
#define __IAP_H__

#include "systick.h"
#include "usart.h"
#include "flash.h"

#define FLASH_APP1_ADDR ADDR_FMC_SECTOR_8			//Address of the first app. 
//0x8000000~0x8001FFF is reserved for bootloader. 0x8002000~0x80087FF is reserved for app1
#define FLASH_APP2_ADDR ADDR_FMC_SECTOR_34		//Address of the second app. 
//0x8008800~0x800EFFF is reserved for storage.
#define FLASH_DATA_ADDR   ADDR_FMC_SECTOR_60  //user storage
#define VERSION_ID_ADDR 	ADDR_FMC_SECTOR_61	//version id
//0x800F000~0x800FFFF is reserved for user data.
#define APP_PAGE_SIZE 26
#define USR_PAGE_SIZE 4

//state machine of firmware updation
#define IDLE 0x00
#define CHECK 0x01
#define INIT 0x02
#define PRETANS 0x03
#define TRANS 0x04
#define FINISH 0x05

typedef  void (*iapfun)(void);					
void MSR_MSP(uint32_t addr);							//set address of stack
void iap_load_app(uint32_t appxaddr);			//jump to app
void Update_Init(void);
void Reset_MCU(void);					

extern uint8_t Update_STA;
extern uint8_t   USARTx_RX_BUF[DATA_SIZE];
extern uint16_t USARTx_RX_STA;


#endif

