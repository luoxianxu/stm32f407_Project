
#include "bee.h"

void Bee_Init(void){
	BEE_PeriphCLockCmd(BEE_RCC_AHB1Periph_GPIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = Bee_GPIO_Pin;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(Bee_GPIO, &GPIO_InitStruct);
}
