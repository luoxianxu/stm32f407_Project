
#include "led.h"

void led_Init(void){
	Led_RCC_AHB1PeriphClockCmd(Led_AHB1PeriphGPIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//GPIO���ģʽ����
	GPIO_InitStruct.GPIO_Pin = Led_GPIO_Pin;		//GPIO_Pin
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		//�������ģ#ʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//û����������
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;	//����
	GPIO_Init(Led_GPIO, &GPIO_InitStruct);
	
	Led1_ON;
	Led2_ON;
}
