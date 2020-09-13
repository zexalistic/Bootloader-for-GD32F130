#include "usart.h"
#include "timer.h"
#include "protocol.h"
#include "util.h"
#include "error.h"
#include "iap.h"

uint16_t crc16_tmp = 0;
struct looplist lp;
void looplist_init(void){
	lp.size = LP_SIZE;			
	lp.head = 0;
	lp.tail = 0;
	lp.count = 0;
}

void looplist_push( uint8_t ch){
	lp.data[lp.head] = ch;
	if(lp.head < lp.size -1){
		lp.head ++;
	}
	else 
		lp.head = 0;
	lp.count ++;
}

uint8_t looplist_pop(void){
	uint8_t ch = 0;
		if (lp.count > 0){
		ch = lp.data[lp.tail];
		if(lp.tail < lp.size -1)
			lp.tail ++;
		else
			lp.tail = 0;
		
		lp.count --;
	}
	return ch;
}

void NVIC_Config(USART_TypeDef* USARTx);

/* Private functions */
void USART1_Init(uint32_t bdr)
{
	GPIO_InitPara GPIO_InitStructure;
	USART_InitPara USART_InitStructure;
	
	RCC_AHBPeriphClock_Enable(COM1_TX_GPIO_CLK | COM1_RX_GPIO_CLK, ENABLE);
#if defined(GD32F130_150)
	/* Enable USART clock */
	RCC_APB2PeriphClock_Enable(COM1_CLK, ENABLE);
#elif defined(GD32F170_190)
	/* Enable USART clock */
	RCC_APB1PeriphClock_Enable(COM1_CLK, ENABLE);
#else
	#error "Please define GD32F130_150 or GD32F170_190"			
#endif
	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(COM1_TX_GPIO_PORT, COM1_TX_SOURCE, COM1_AF);
	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(COM1_RX_GPIO_PORT, COM1_RX_SOURCE, COM1_AF);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_10MHZ;
    GPIO_InitStructure.GPIO_OType = GPIO_OTYPE_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = COM1_RX_PIN;
	GPIO_Init(COM1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = COM1_TX_PIN;
	GPIO_Init(COM1_TX_GPIO_PORT, &GPIO_InitStructure);
	
    USART_InitStructure.USART_BRR = bdr;											
    USART_InitStructure.USART_WL = USART_WL_8B;										
    USART_InitStructure.USART_STBits = USART_STBITS_1;								
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;							
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;	
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;			
	
	USART_DeInit( USART1 );
	USART_Init( USART1, &USART_InitStructure );
	
	USART_INT_Set(USART1, USART_INT_TBE, DISABLE);									//disable the TBE interrupt
	USART_ClearBitState(USART1, USART_FLAG_TC);
	
	USART_Enable( USART1, ENABLE );
	
	//Enable USART RBNE interrupt
    USART_INT_Set(USART1, USART_INT_RBNE, ENABLE);
		
	
	NVIC_Config( USART1 );
}

//·¢ËÍµ¥¸ö×Ö·û
void usart_send_isr (USART_TypeDef* USARTx,uint16_t Data){		
	USART_DataSend(USARTx,Data);
	while (USART_GetBitState(USARTx, USART_FLAG_TC) != SET);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Config(USART_TypeDef* USARTx)
{
    NVIC_InitPara NVIC_InitStructure;
	if( USARTx  == USART1)							//ÖÐ¶ÏÍ¨µÀ
	{
		NVIC_InitStructure.NVIC_IRQ = USART1_IRQn;
	}
	else
	{
		NVIC_InitStructure.NVIC_IRQ = USART2_IRQn;
	}
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 3;	//ÇÀÕ¼ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQSubPriority = 3;		//¸±ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;		//Ñ¡ÔñµÄÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void USART_SendMsg(USART_TypeDef* USARTx, uint8_t *msg, uint8_t len)
{	
	uint8_t i;
	
	for(i=0;i<len;i++)
	{
		USART_DataSend(USARTx, msg[i]);         //Ïò´®¿Ú·¢ËÍÊý¾Ý
		while(USART_GetBitState(USARTx,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
	}
}

void USART1_it(void)
{
#ifdef PROT_UART_EN
	static uint16_t len=0;
	uint16_t tmp;
	uint8_t Res;
	if (lp.count > 0){	
		Res = looplist_pop();
		//send_ack(Res);
		
		switch(USARTx_RX_STA)
		{
			case RX_STA_SOF:							//head of a frame, it shall always be PROT_FRAME_SOF
				if(Res == PROT_FRAME_SOF)
				{
					USARTx_RX_STA = RX_STA_CMD;
					USARTx_RX_BUF[0] = PROT_FRAME_SOF ;
				}
				break;
			case RX_STA_CMD:							//command number
				USARTx_RX_STA = RX_STA_LEN_1;
				USARTx_RX_BUF[1] = Res;	
				break;
			case RX_STA_LEN_1:							//frame length
				USARTx_RX_STA = RX_STA_LEN_2;
				USARTx_RX_BUF[2] = Res;
				break;
			case RX_STA_LEN_2 :
				USARTx_RX_STA = RX_STA_SERIAL;
				USARTx_RX_BUF[3] = Res;
				len = ((((uint16_t)USARTx_RX_BUF[2]) << 8) + USARTx_RX_BUF[3]) ; //length of frame
				break;
			case RX_STA_SERIAL:			//serial number for data frame£¬0 means the frame is a command frame. The serial number starts from 1
				USARTx_RX_STA ++;
				USARTx_RX_BUF[4] = Res;
				break;				
			default:
				if(USARTx_RX_STA & 0x400)			
				{
					tmp = USARTx_RX_STA&0x3FF;
					USARTx_RX_STA ++;
					USARTx_RX_BUF[tmp] = Res;
					if(tmp < len-2)				//CRC16 check for all the data
					{
						crc16_tmp = CRC_16C(&USARTx_RX_BUF[tmp], 1, crc16_tmp);
					}
				  if(tmp == len-1)		//finish receiving a frame
					{
						USARTx_RX_STA = RX_STA_SOF;
						//CRC16 check
						if(crc16_tmp == ((uint16_t)(USARTx_RX_BUF[tmp-1] << 8) + USARTx_RX_BUF[tmp]) || USARTx_RX_BUF[4] == 0)
						{
							if (frm_recv[0].frmFlag == PROT_FRAME_STATE_IDLE){
								frm_index = 0;
								PROT_CreateRecvFrm(USARTx_RX_BUF,data_recv_1,0);	
								crc16_tmp = 0;			//set crc check initial value
							}							
							else if (frm_recv[1].frmFlag == PROT_FRAME_STATE_IDLE){
								frm_index = 1;
								PROT_CreateRecvFrm(USARTx_RX_BUF,data_recv_2,1);
								crc16_tmp = 0;		
							}
							else {
								send_ack(BUFFER_OVERFLOW);
							}						
							
						}
						else 
							send_ack(crc16_tmp);
					}
				}
		}
		
	}
#else
	uint8_t ch;
	if(USART_GetBitState(COM1, USART_FLAG_RBNE) != RESET)
	{ 	
		ch = USART_DataReceive(USART1);
	  	printf( "%c", ch );    
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
#pragma import(__use_no_semihosting)             
//support functions needed by std libs               
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//define _sys_exit() to avoid semihosting
void _sys_exit(int x) 
{ 
	x = x; 
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    USART_DataSend( COMx, (uint8_t)ch );
    while( USART_GetBitState( COMx , USART_FLAG_TBE) != SET );
    return ch;
}

/* retarget the C library scanf function to the USART */
int fgetc(FILE *f)
{
	while ( USART_GetBitState(COMx, USART_FLAG_RBNE) != SET );
	return (int)USART_DataReceive( COMx );
}
