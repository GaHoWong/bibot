#ifndef __USRATX_H
#define __USRATX_H 
#include "sys.h"	  	

extern u8 uart_receive;
void uartx_init(u32 pclk2,u32 bound);
int USARTX_IRQHandler(void);
void uart2_init(u32 pclk2,u32 bound);
#endif

