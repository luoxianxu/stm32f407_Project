
#include "USART.h"
#include "stdio.h"

void USART_NVIC_Config(void){
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ITConfig(USART, USART_IT_RXNE, ENABLE);
	
	
}
void USART1_Config(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);			//GPIO时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);			//串口时钟使能
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);		//GPIO复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 230400;					//115200波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStruct.USART_Parity = USART_Parity_No;			//没有校验位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//8位数据
	USART_Init(USART1, &USART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);

}

void USART2_Config(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);			//GPIO时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);			//串口时钟使能
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);		//GPIO复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 230400;					//115200波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStruct.USART_Parity = USART_Parity_No;			//没有校验位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//8位数据
	USART_Init(USART2, &USART_InitStruct);
	
	USART_Cmd(USART2, ENABLE);

}

void Usart_Init(void){
	USART1_Config();
	USART_NVIC_Config();
}


int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);
    USART2->DR = (u8) ch;      
	return ch;
}

