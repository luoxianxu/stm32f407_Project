/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stdio.h"
#include "Delay.h"
#include "bee.h"
#include "adc.h"
#include "led.h"
#include "Gy901.h"
#include <string.h>

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the InterrupTIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);t Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

uint8_t RecData[5] = {0};
uint8_t cnt = 0;
uint8_t state = 0;
uint16_t Pos_x = 0,Pos_y = 0;
void USART1_IRQHandler(void){										//is ok
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET){
		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			
			uint8_t data = USART_ReceiveData(USART1);
			if(state == 0 && data == 0x11){
				RecData[cnt++] = data;
				state = 1;
			}
			else if(state == 1){
				RecData[cnt++] = data;
				if(cnt == 5)
					state = 2;
			}
			else if(state == 2 && data == 0x99){
				RecData[cnt++] = data;
				state = 3;
			}
			else if(state == 3){
				RecData[cnt++] = data;
				Pos_x = (uint16_t)((RecData[1]<<8)|(RecData[2]));
				Pos_y = (uint16_t)((RecData[3]<<8)|(RecData[4]));
				state = 0;
				cnt = 0;
				memset(RecData,0,sizeof(RecData));
			}
			else{
				cnt = 0;
				state = 0;
				memset(RecData,0,sizeof(RecData));
			}
		}
	
		if(USART_GetITStatus(USART1,USART_IT_ORE)!=RESET){
			uint8_t data = USART_ReceiveData(USART1);
			USART_ClearITPendingBit(USART1,USART_IT_ORE);
		}
	}
}

extern float Acc_x,Acc_y,Acc_z;
extern float Gyro_x,Gyro_y,Gyro_z;
extern float Roll,Pitch,Yaw;
void TIM2_IRQHandler(void){
	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ){
		printf("Pos_x = %d\r\n",Pos_x);
		printf("Pos_y = %d\r\n",Pos_y);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

int i = 2000000;
void EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){
		Delay_ms(30);
		while(i--);
		i=2000000;
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

//GY901接收函数
extern uint8_t Gy901DmaUsartRxBuffer0[11];
extern uint8_t Gy901DmaUsartRxBuffer1[11];
uint8_t Gy901UsartRxFlag = 0;
void DMA1_Stream5_IRQHandler(void){
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5)!=RESET){
		if(Gy901DmaUsartRxBuffer0[0] == 0x55 && Gy901DmaUsartRxBuffer0[10] == (CalcCRCSUM((int16_t *)Gy901DmaUsartRxBuffer0,10)&0x00FF)){
			if(Gy901DmaUsartRxBuffer0[1] == 0x51){		//加速度帧
				short data_x = (short)((short)Gy901DmaUsartRxBuffer0[3]<<8|Gy901DmaUsartRxBuffer0[2]);
				short data_y = (short)((short)Gy901DmaUsartRxBuffer0[5]<<8|Gy901DmaUsartRxBuffer0[4]);
				short data_z = (short)((short)Gy901DmaUsartRxBuffer0[7]<<8|Gy901DmaUsartRxBuffer0[6]);
				Acc_x = (float)data_x/32768*16*9.8f;
				Acc_y = (float)data_y/32768*16*9.8f;
				Acc_z = (float)data_z/32768*16*9.8f;
			}
			else if(Gy901DmaUsartRxBuffer0[1] == 0x52){		//角速度帧
				short data_x = (short)((short)Gy901DmaUsartRxBuffer0[3]<<8|Gy901DmaUsartRxBuffer0[2]);
				short data_y = (short)((short)Gy901DmaUsartRxBuffer0[5]<<8|Gy901DmaUsartRxBuffer0[4]);
				short data_z = (short)((short)Gy901DmaUsartRxBuffer0[7]<<8|Gy901DmaUsartRxBuffer0[6]);
				Gyro_x = (float)data_x/32768*2000;
				Gyro_y = (float)data_y/32768*2000;
				Gyro_z = (float)data_z/32768*2000;
			}
			else if(Gy901DmaUsartRxBuffer0[1] == 0x53){		//角度帧
				short data_x = (short)((short)Gy901DmaUsartRxBuffer0[3]<<8|Gy901DmaUsartRxBuffer0[2]);
				short data_y = (short)((short)Gy901DmaUsartRxBuffer0[5]<<8|Gy901DmaUsartRxBuffer0[4]);
				short data_z = (short)((short)Gy901DmaUsartRxBuffer0[7]<<8|Gy901DmaUsartRxBuffer0[6]);
				Roll  = (float)data_x/32768*180;
				Pitch = (float)data_y/32768*180;
				Yaw   = (float)data_z/32768*180;
			}
		}
		memset(Gy901DmaUsartRxBuffer0,0,sizeof(Gy901DmaUsartRxBuffer0));
	}
}


