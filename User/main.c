
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "bee.h"
#include "key.h"
#include "led.h"
#include "USART.h"
#include "mytask.h"
#include "timer.h"
#include "Delay.h"
#include "adc.h"
#include "I2C.h"
#include "software_i2c.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "dma.h"
#include "Gy901.h"
#include <stdio.h>

void GetClickRrec(void){
	RCC_ClocksTypeDef RCC_Clock;
	RCC_GetClocksFreq(&RCC_Clock);
	printf("SYSCLK_Frequency = %d\r\n",RCC_Clock.SYSCLK_Frequency);
	printf("HCLK_Frequency = %d\r\n",RCC_Clock.HCLK_Frequency);
	printf("PCLK1_Frequency = %d\r\n",RCC_Clock.PCLK1_Frequency);
	printf("PCLK2_Frequency = %d\r\n",RCC_Clock.PCLK2_Frequency);
}

//#define MPU6050
#define Usart
#define Pwm
//#define Encorder
//#define Oled
//#define Adc
//#define Freertos
#define Time
//#define I2c
//#define DMA
//#define GY901



int main(void)
{
//	Bee_Init();
//	Key_Init();
//	led_Init();
#ifdef Usart
	Usart_Init();		//ok
	USART2_Config();
#endif
//	GetClickRrec();
#ifdef Pwm
	Pwm_Config();		//ok			PB0-->TIM3->CCR3		PB3-->TIM3->CCR4	
	TIM3->CCR3 = 150;
	TIM3->CCR4 = 150;
#endif
	
#ifdef Oled
	InitOLed();
	OLed_ShowLight();
#endif

#ifdef Time
	Timer_Init();		//ok
#endif

	while(1){
	}
}

