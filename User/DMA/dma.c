#include "dma.h"

uint8_t CameraUsartDmaTxBuffer[15]={0};
void CameraUsartDmaConfig(void){
	DMA_InitTypeDef DMA_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//时钟使能
	CameraUsartDmaPeriphClockCmd(CameraUsartDmaPeriph,ENABLE);
	DMA_InitStruct.DMA_Channel = DMA_CHANNEL;								//通道0
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);			//外设地址
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)CameraUsartDmaTxBuffer;	//目的地址
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;					//方向为外设到内存
	DMA_InitStruct.DMA_BufferSize = DMA_BuFFerSize;							//dma传输数据量
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址不增加
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;					//内存地址增加
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据单位
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//内存数据单位
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;							//正常模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;					//DMA优先级
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init(DMA_DMAy_Streamx, &DMA_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_ITConfig(DMA_DMAy_Streamx, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA_DMAy_Streamx, DMA_IT_TC);
	DMA_Cmd(DMA_DMAy_Streamx, ENABLE);
}


