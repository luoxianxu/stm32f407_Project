
#ifndef _LED_H
#define _LED_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define Led_RCC_AHB1PeriphClockCmd		RCC_AHB1PeriphClockCmd
#define	Led_AHB1PeriphGPIO				RCC_AHB1Periph_GPIOF
#define	Led_GPIO						GPIOF

#define	Led1_GPIO_Pin					GPIO_Pin_9
#define	Led2_GPIO_Pin					GPIO_Pin_10
#define Led_GPIO_Pin					Led1_GPIO_Pin | Led2_GPIO_Pin

#define Led1_OFF						GPIO_SetBits(Led_GPIO, Led1_GPIO_Pin)
#define	Led1_ON							GPIO_ResetBits(Led_GPIO, Led1_GPIO_Pin)

#define Led2_OFF						GPIO_SetBits(Led_GPIO, Led2_GPIO_Pin)
#define	Led2_ON							GPIO_ResetBits(Led_GPIO, Led2_GPIO_Pin)

#define Led1_Toggle						GPIO_ToggleBits(Led_GPIO, Led1_GPIO_Pin)
#define Led2_Toggle						GPIO_ToggleBits(Led_GPIO, Led2_GPIO_Pin)


void led_Init(void);

#endif/*_LED_H*/
