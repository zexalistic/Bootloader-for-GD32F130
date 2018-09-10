#include "iap.h" 
#include "protocol.h"

uint8_t   Update_STA = IDLE;
uint16_t USARTx_RX_STA;		//receive flag of rx buffer
uint8_t   USARTx_RX_BUF[DATA_SIZE];
iapfun		jump2app; 

 __asm void MSR_MSP(uint32_t addr) 
{
	MSR MSP, r0 
	BX r14
}//set Main Stack value

void Reset_MCU(void)
{  
	NVIC_SystemReset();
} 



void Update_Init(void){
	Update_STA = IDLE;
	USARTx_RX_STA = RX_STA_SOF;
	UpdateSuccess = 0;
	frm_recv[0].frmFlag = 0;
	frm_recv[1].frmFlag = 0;
	looplist_init();
}


void iap_load_app(uint32_t appxaddr)
{
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)//check if it is legal
	{ 
		jump2app=(iapfun)*(__IO uint32_t*)(appxaddr+4);		//the second address of app code is the program
		MSR_MSP(*(__IO uint32_t*)appxaddr);					//initialize app pointer
		jump2app();											//jump to app
	}
}		 

