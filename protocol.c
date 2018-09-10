#include "protocol.h"
#include "iap.h"
#include "util.h"

PROT_FRAME frm_recv[2];			//two frame buffers
uint8_t data_recv_1[DATA_SIZE]; //data buffers
uint8_t data_recv_2[DATA_SIZE];
uint8_t frm_index = 0; 		//index for frm_recv
uint8_t  bit[4];

uint16_t RecLength = 0;			//count for data written to flash 
uint16_t TotalLength = 0;		
uint16_t TotalCrc ;

uint32_t PrimVersion = 0x00000000;
uint32_t CurVersion = 0x00000000;
uint8_t UpdateSuccess = 0;

	__IO uint32_t addr1 = FLASH_APP1_ADDR;//APP1 address, 26k in all 
	__IO uint32_t addr2 = FLASH_APP2_ADDR;//APP2 address, 26k in all . APP2 is a backup. 
	__IO uint32_t version_addr = VERSION_ID_ADDR;//the address of flash that stores version id 

struct version_str original_version, tmp_version;

//This function is used to add the message in the receive buffer to our frame buffer and data buffer.
//It also change the state of state machine in main loop
//frm_recv[index].frmFlag shows the state of frame. When the frame is refreshed but not read, it will in state
//PROT_FRAME_STATE_PEND; if it is being used by the processing function, the flag will be set PROT_FRAME_STATE_EXEC;
//if it is used, then it will be set to PROT_FRAME_STATE_IDLE.
void PROT_CreateRecvFrm(uint8_t *rxMsg, uint8_t *SaverxMsg, uint8_t index)
{
  frm_recv[index].sof = rxMsg[0];
  frm_recv[index].cmd = rxMsg[1];
  frm_recv[index].frmLen = ((uint16_t)rxMsg[2] << 8) + rxMsg[3];
	frm_recv[index].data_seq = rxMsg[4];
	if (frm_recv[index].frmLen ==7)
		frm_recv[index].data_ptr = NULL;
	else{
		frm_recv[index].data_ptr = SaverxMsg;
		memcpy(SaverxMsg, rxMsg + 5, frm_recv[index].frmLen -7);
	}	
	frm_recv[index].crc16 = (uint16_t)(rxMsg[frm_recv[index].frmLen-2] << 8) + rxMsg[frm_recv[index].frmLen-1];	
	frm_recv[index].frmFlag = PROT_FRAME_STATE_PEND;
	
	Update_STA = frm_recv[index].cmd;// change the state of state machine	

}

void send_ack(uint8_t ch){
	usart_send_isr(USART1,ch);
}

void IdleHandler(void){
	USART1_it();
}

void CheckVersion(void){  //check version id
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_EXEC;
	
	original_version.AA = *(__IO uint32_t*)VERSION_ID_ADDR&0x000000FF;
	original_version.BB = (*(__IO uint32_t*)VERSION_ID_ADDR&0x0000FF00) >> 8;
	original_version.CC = (*(__IO uint32_t*)VERSION_ID_ADDR&0x00FF0000) >> 16;
	original_version.DD = (*(__IO uint32_t*)VERSION_ID_ADDR&0xFF000000) >> 24;

	send_ack(OK_01);
	usart_send_isr(USART1,original_version.DD);
	usart_send_isr(USART1,original_version.CC);
	usart_send_isr(USART1,original_version.BB);
	usart_send_isr(USART1,original_version.AA);
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_IDLE;
	Update_STA = IDLE;
}

uint8_t check_version (struct version_str tmp_version, struct version_str original_version){
	if (tmp_version.AA == original_version.AA && tmp_version.BB == original_version.BB){
		if (tmp_version.CC == original_version.CC && tmp_version.DD == original_version.DD)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}


void InitUpdate(void){			//Judge if the current version needs updation
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_EXEC;
		
	tmp_version.DD = *frm_recv[frm_index].data_ptr;
	tmp_version.CC = *(frm_recv[frm_index].data_ptr + 1);
	tmp_version.BB = *(frm_recv[frm_index].data_ptr + 2);
	tmp_version.AA = *(frm_recv[frm_index].data_ptr + 3);
	original_version.AA = *(__IO uint32_t*)VERSION_ID_ADDR&0x000000FF;
	original_version.BB = (*(__IO uint32_t*)VERSION_ID_ADDR&0x0000FF00) >> 8;
	original_version.CC = (*(__IO uint32_t*)VERSION_ID_ADDR&0x00FF0000) >> 16;
	original_version.DD = (*(__IO uint32_t*)VERSION_ID_ADDR&0xFF000000) >> 24;
	
	if(check_version (tmp_version, original_version) != 0){		//The same version number
		send_ack(SAME_VERSION);
	}
	else{
		//reset the version id : 00.00.00.00
		FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );		//erase the user code flash
		FMC_ErasePage(VERSION_ID_ADDR);
		fmc_write_single(VERSION_ID_ADDR,&PrimVersion);				//write the user flag	
	
		//removing the original firmware	
		if(fmc_erase(FLASH_APP1_ADDR,APP_PAGE_SIZE) == 0)
			send_ack(OK_02);
		else
			send_ack(ERASE_FLASH_ERROR);		
	}
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_IDLE;
	Update_STA = IDLE;
}

void PreTrans(void){			//prepare for updation
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_EXEC;
	
	TotalLength = ((uint16_t)*(frm_recv[frm_index].data_ptr) << 8) + *(frm_recv[frm_index].data_ptr + 1);
	TotalCrc = frm_recv[frm_index].crc16;
	
	
	send_ack(OK_03);
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_IDLE;
	Update_STA = IDLE;
}

void TransFile(void){			//Start to transmit data
	static uint8_t CurNum = 0;
	uint16_t dataLen;
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_EXEC;
		
	dataLen = frm_recv[frm_index].frmLen - 7;
	RecLength += dataLen;
	
  if (frm_recv[frm_index].data_seq - CurNum != 1){
		send_ack (TRANS_ORDER_ERROR);
	}
	else if ( fmc_write_word(addr1,frm_recv[frm_index].data_ptr,dataLen) != 0){
		send_ack(WRITE_FLASH_ERROR);		      
	}
	else {
		addr1 += dataLen;
		CurNum = frm_recv[frm_index].data_seq;
		frm_recv[frm_index].frmLen = 0;
		send_ack(OK_04);
		usart_send_isr(USART1,CurNum);
		usart_send_isr(USART1,RecLength>>8);
		usart_send_isr(USART1,RecLength);
	}	
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_IDLE;
	Update_STA = IDLE;	
}

void FinishTrans(void){			//receive success
	uint16_t tmp;	
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_EXEC;		
	
	tmp_version.DD = *frm_recv[frm_index].data_ptr;
	tmp_version.CC = *(frm_recv[frm_index].data_ptr + 1);
	tmp_version.BB = *(frm_recv[frm_index].data_ptr + 2);
	tmp_version.AA = *(frm_recv[frm_index].data_ptr + 3);
	
	addr1 = FLASH_APP1_ADDR;
	CurVersion = ((uint32_t)tmp_version.DD << 24) + ((uint32_t)tmp_version.CC << 16) + ((uint32_t)tmp_version.BB <<8) + (uint32_t)tmp_version.AA;

	fmc_erase_sector(VERSION_ID_ADDR);
	fmc_write_single(VERSION_ID_ADDR,&CurVersion);				//update version id
	
	tmp = 0;
	tmp = CRC_16C_32((uint32_t*)addr1, TotalLength/4, tmp);
	
	if (tmp == TotalCrc && RecLength == TotalLength){
		send_ack(OK_05);
		UpdateSuccess = 1;
	}
		
	else{
		send_ack(SEND_ERROR);
	}		
	
	frm_recv[frm_index].frmFlag = PROT_FRAME_STATE_IDLE;
	Update_STA = IDLE;
}


