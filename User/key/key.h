
#ifndef _KEY_H
#define _KEY_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


#define KEY_PeriphCLockCmd			RCC_AHB1PeriphClockCmd
#define KEY_RCC_AHB1Periph_GPIO		RCC_AHB1Periph_GPIOE
#define KEY_GPIO					GPIOE			
#define KEY1GPIO_Pin				GPIO_Pin_4
//#define KEY2GPIO_Pin				GPIO_Pin_5
//#define KEY3GPIO_Pin				GPIO_Pin_6


#define KEY1_IRQChannel				EXTI4_IRQn
//#define KEY2_IRQChannel				EXTI9_5_IRQn
#define KEY_EXTI_Line				EXTI_Line4

#define KEY_EXTI_PortSourceGPIO		EXTI_PortSourceGPIOE
#define KEY1_EXTI_PinSource			EXTI_PinSource4
//#define KEY2_EXTI_PinSource			EXTI_PinSource5
//#define KEY3_EXTI_PinSource			EXTI_PinSource6

void Key_Nvic_Config(void);
void Key_Exit_Config(void);
void Key_Init(void);
void Key_Init(void);


#endif/*_KEY_H*/
