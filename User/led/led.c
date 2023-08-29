
#include "led.h"

void led_Init(void){
	Led_RCC_AHB1PeriphClockCmd(Led_AHB1PeriphGPIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//GPIO输出模式配置
	GPIO_InitStruct.GPIO_Pin = Led_GPIO_Pin;		//GPIO_Pin
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		//推挽输出模#式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有配置上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;	//高速
	GPIO_Init(Led_GPIO, &GPIO_InitStruct);
	
	Led1_ON;
	Led2_ON;
}
