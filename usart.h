#ifndef __USART_H
#define __USART_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f1x0.h"
#include <stdio.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////////////
#define DATA_SIZE	280	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊý 0.25K

/* usart1 */
#define COMx						COM1

#define COM1                        USART1
#define COM1_AF                  	GPIO_AF_0
#define COM1_CLK                    RCC_APB2PERIPH_USART1

#define COM1_TX_PIN                 GPIO_PIN_6
#define COM1_TX_GPIO_PORT           GPIOB
#define COM1_TX_GPIO_CLK            RCC_AHBPERIPH_GPIOB
#define COM1_TX_SOURCE              GPIO_PINSOURCE6

#define COM1_RX_PIN                 GPIO_PIN_7
#define COM1_RX_GPIO_PORT           GPIOB
#define COM1_RX_GPIO_CLK            RCC_AHBPERIPH_GPIOB
#define COM1_RX_SOURCE              GPIO_PINSOURCE7

#define COM1_IRQn                   USART1_IRQn
//////////////////////////////////////////////////////////////////////////////////////
#define PROT_UART_EN
#define LP_SIZE 64			//The value o LP_SIZE is important. In the debugging stage, I have sent a package of 519 Bytes, 
//How ever the MCU only receive 516Bytes.(LP_SIZE = 20) The problem will be solved if I only sent 256Bytes.

struct looplist{
	uint8_t data[LP_SIZE];
	uint16_t head ;
	uint16_t tail ;
	uint16_t size;
	uint16_t count;
};

extern struct looplist lp;

extern void USART1_Init(uint32_t bdr);
extern void USART1_it(void);
void usart_send_isr (USART_TypeDef* USARTx,uint16_t Data);
extern void looplist_push( uint8_t ch);
extern void looplist_init(void);
extern uint8_t looplist_pop( void);

#endif /* __USART_H */
