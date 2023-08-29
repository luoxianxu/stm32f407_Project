#ifndef _GY901_H
#define _GY901_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

#define Gy901UsartDmaPeriphClockCmd		RCC_AHB1PeriphClockCmd
#define Gy901UsartDmaPeriph				RCC_AHB1Periph_DMA1
#define Gy901DmaChannel					DMA_Channel_4
#define Gy901DmaBufferSize				11
#define Gy901DmaDMAy_Streamx			DMA1_Stream5
#define Gy901DMA_IRQn					DMA1_Stream5_IRQn

#define Gy901_USART_PeripheralClockCmd	RCC_APB1PeriphClockCmd
#define Gy901_USART_Peripheral			RCC_APB1Periph_USART2
#define Gy901_USARTGPIO_PeriphClockCmd	RCC_AHB1PeriphClockCmd
#define Gy901_USART_GPIOPeripheral		RCC_AHB1Periph_GPIOA
#define Gy901_USART_GPIO				GPIOA
#define Gy901_Tx_GPIOPin				GPIO_Pin_2
#define Gy901_Rx_GPIOPin				GPIO_Pin_3
#define Gy901_Tx_PinSource				GPIO_PinSource2
#define Gy901_Rx_PinSource				GPIO_PinSource3
#define Gy901_AF_USART					GPIO_AF_USART2
#define Gy901_USART						USART2
#define Gy901_USART_IRQn				USART2_IRQn

uint16_t CalcCRCSUM(int16_t *arr,uint8_t len);
void USART3_IRQHandler(void);
static void Gy901UsartInit(void);
void Gy901USARTSendData(uint8_t *data,uint8_t Len);
void Gy901WriteData(uint8_t type,uint8_t dataL,uint8_t dataH);
void Gy901_Init(void);


#endif /*_GY901_H*/
