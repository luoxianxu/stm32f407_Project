#ifndef _DMA_H
#define _DMA_H

#include "stm32f4xx.h"

#define CameraUsartDmaPeriphClockCmd				RCC_AHB1PeriphClockCmd
#define CameraUsartDmaPeriph						RCC_AHB1Periph_DMA1
#define CameraUsaerBaseAddr							(USART2->DR)
#define DMA_CHANNEL									DMA_Channel_4
#define DMA_DMAy_Streamx							DMA1_Stream5
#define DMA_BuFFerSize								11
#define DMA_IRQn									DMA1_Stream5_IRQn
void CameraUsartDmaConfig(void);

#endif /*_DMA_H*/
