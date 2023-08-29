
#ifndef _BEE_H
#define _BEE_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define BEE_PeriphCLockCmd			RCC_AHB1PeriphClockCmd
#define BEE_RCC_AHB1Periph_GPIO		RCC_AHB1Periph_GPIOB
#define Bee_GPIO					GPIOB
#define Bee_GPIO_Pin				GPIO_Pin_10

#define BEE_ON						GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define BEE_OFF						GPIO_ResetBits(GPIOB, GPIO_Pin_10)

void Bee_Init(void);

#endif/*_BEE_H*/
