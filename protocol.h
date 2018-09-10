#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "include.h"
#include "iap.h"
#include "crc.h"
#include "protocol.h"

//state machine of protocol transmittion process
#define RX_STA_SOF 0x0000
#define RX_STA_CMD 0x0401
#define RX_STA_LEN_1 	0x0402
#define RX_STA_LEN_2 	0x0403
#define RX_STA_SERIAL	0x0404

//Protocol frame
#define PROT_FRAME_SOF                0xAA      //head of frame
#define PROT_FRAME_BUF_NUM 			      2					

//frame state
#define PROT_FRAME_STATE_IDLE    00  //idle
#define PROT_FRAME_STATE_RECV    11  //receiving				1
#define PROT_FRAME_STATE_PEND    12  //finish receiving					2
#define PROT_FRAME_STATE_EXEC    13  //in execution					  3


extern uint8_t UpdateSuccess;
extern uint8_t frm_index;
extern uint8_t  bit[4];

extern uint8_t data_recv_1[DATA_SIZE]; //data buffer
extern uint8_t data_recv_2[DATA_SIZE];


struct version_str{		//version number; AA is higher than DD
	uint8_t DD;
	uint8_t CC;
	uint8_t BB;
	uint8_t AA;	
};			

#pragma pack (1)			//Byte alignment
typedef struct
{
	uint8_t           sof;                   //head
	uint8_t           cmd;               		//command number
	uint16_t          frmLen;                //frame length
	uint8_t						data_seq;							//data sequence
	uint8_t 					*data_ptr;							//pointer of data storage
	uint16_t          crc16;                  //crc parity
	
	uint8_t           frmFlag;               //state of frame
	
} PROT_FRAME;

#pragma pack ()			//reset alignment

extern struct version_str original_version, tmp_version;
extern void send_ack(uint8_t ch);

extern void IdleHandler(void);
extern void CheckVersion(void);
extern void InitUpdate(void);
extern void PreTrans(void);
extern void TransFile(void);
extern void FinishTrans(void);

extern void PROT_CreateRecvFrm(uint8_t *rxMsg, uint8_t *SaverxMsg, uint8_t index);

extern PROT_FRAME frm_recv[2];

#endif

