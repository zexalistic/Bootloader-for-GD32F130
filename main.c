#include "include.h"
#include "systick.h"
#include "timer.h"
#include "usart.h"
#include "iap.h"
#include "protocol.h"

/*
	Function name: main
	Description: This is a bootloader with IAP function of GD32F130C8. The flash is divided into five parts:
						bootloader(0x8000000~0x8001FFF), APP1(0x8002000~0x80087FF), APP2(0x8008800~0x800EFFF) 
						and USER_DATA(0x800F000~0x800FFFF). The bootloader first check the update-flag in 
						0x800F000. If the data in 0x800F000 equals to LoaderKey, then it begins to prepare for updating
						the firmware, or it will jump to the main app. If the updation is successful, it will jump to main app and wait 
						until every process is alright, then in the main app we will set the LoaderKey to 0. If the updation 
						fails, it will restart the bootloader and do the above again.	
						The version ID will be set when all the codes were written to flash. But it can not guarantee the correction of 
						the codes. It is kind of dilemma
	ATTENTION: Flash shall be erased first, then to be written.
*/


int main(void)
{
	__IO uint32_t UserDataAddr = FLASH_DATA_ADDR;//user data address, 4k in all	
	
	uint32_t LoaderKey = 0xAA55DD99;		//进入用户程序的标志
		__IO uint32_t UpdateFlag = *(__IO uint32_t*)UserDataAddr;		//update flag
	
//system init
	systick_init(72);
	USART1_Init(115200);		//Baud rate is 115200


//BL
 if(UpdateFlag != LoaderKey && ((*(__IO uint32_t*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)
	{
		iap_load_app(FLASH_APP1_ADDR);//run FLASH APP
	}		

	Update_Init();
	while(1)
	{	
	
	 switch (Update_STA){
		 case IDLE: 		IdleHandler();break;
		 case CHECK: 		CheckVersion();break;
		 case INIT:			InitUpdate();break;
		 case PRETANS:	PreTrans();break;
		 case TRANS:		TransFile();break;
		 case FINISH:		FinishTrans();break;
		 default:				Update_STA = IDLE;break;
	 }			
		
		if (UpdateSuccess == 1)
			break;
		else 
			continue;	
	}			//end of while(1)
	
	systick_dly_ms(40000);	//hang for a while
	send_ack(OK_00);
	
	
	 if(UpdateSuccess == 1)		//all the file blocks for updation are received
	 {
		 // It's better to disable IRQs before jump
				if(((*(__IO uint32_t*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)		//judge if the app code is legal
				{
					iap_load_app(FLASH_APP1_ADDR);//run FLASH APP
				}
				else
				{
					send_ack(NO_STD_FIRMWARE);
					Reset_MCU();		
				}
	 }
	

}



