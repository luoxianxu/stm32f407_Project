
#ifndef _USART_H
#define _USART_H

#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

#define USART		USART1
#define NVIC_PriorityGroup				NVIC_PriorityGroup_2
#define USART_IRQn						USART1_IRQn


void USART1_Config(void);
void USART2_Config(void);
void Usart_Init(void);



#endif /*_USART_H*/
