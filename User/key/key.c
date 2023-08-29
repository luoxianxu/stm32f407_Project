
#include "key.h"

void Key_Nvic_Config(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = KEY1_IRQChannel;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
//	NVIC_InitStruct.NVIC_IRQChannel = KEY2_IRQChannel;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStruct);
	
}

void Key_Exit_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);			//
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, KEY1_EXTI_PinSource);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, KEY2_EXTI_PinSource);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, KEY3_EXTI_PinSource);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = KEY_EXTI_Line;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
}

void Key_Config(void){
	KEY_PeriphCLockCmd(KEY_RCC_AHB1Periph_GPIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;			//输入模式配置
	GPIO_InitStruct.GPIO_Pin = KEY1GPIO_Pin;			//GPIO_Pin
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;		//高速
	GPIO_Init(KEY_GPIO, &GPIO_InitStruct);
	
}

void Key_Init(void){
	Key_Config();
	Key_Nvic_Config();
	Key_Exit_Config();
}

