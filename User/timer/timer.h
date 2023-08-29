
#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

#define Timer					TIM2
#define Period					499
#define Prescaler				83


//定时器
#define  RCC_Timer_PeriphClockCmd		RCC_APB1PeriphClockCmd
#define	 RCC_Timer_Periph_TIM			RCC_APB1Periph_TIM2
#define	 Timer							TIM2
#define  Timer_Period					199//10ms
#define  Timer_Prescaler				7199
#define  Timer_IRQn						TIM2_IRQn

//PWM
#define  RCC_PwmTimer_PeriphClockCmd	RCC_APB1PeriphClockCmd
#define  RCC_PwmGPIO_PeriphClockCmd		RCC_AHB1PeriphClockCmd
#define  RCC_PwmTimer_Periph_TIM		RCC_APB1Periph_TIM3
#define  RCC_PwmGPIO_Periph_GPIO		RCC_AHB1Periph_GPIOA
#define  Pwm_GPIO_Pin					GPIO_Pin_6|GPIO_Pin_7
#define	 Pwm_GPIO						GPIOA
#define  Pwm_Timer						TIM3
#define  Pwm_Timer_Period				199
#define  Pwm_Timer_Prescaler			8399


//编码器模式
#define  RCC_TimerEncorder1_PeriphClockCmd				RCC_APB1PeriphClockCmd
#define  RCC_TimerEncorder1_GPIO_PeriphClockCmd			RCC_AHB1PeriphClockCmd
#define  RCC_TimerEncorder1_Periph_TIM					RCC_APB1Periph_TIM3
#define  RCC_TimerEncorder1_Periph_GPIO					RCC_AHB1Periph_GPIOA
#define  TimerEncorder1									TIM3
#define  TimerEncorder1_GPIO_Pin						GPIO_Pin_6|GPIO_Pin_7
#define  TimerEncorder1_GPIO							GPIOA
#define  TimerEncorder1_Period							65535
#define  TimerEncorder1_Prescaler						0

#define  RCC_TimerEncorder2_PeriphClockCmd				RCC_APB1PeriphClockCmd
#define  RCC_TimerEncorder2_GPIO_PeriphClockCmd			RCC_AHB1PeriphClockCmd
#define  RCC_TimerEncorder2_Periph_TIM					RCC_APB1Periph_TIM4
#define  RCC_TimerEncorder2_Periph_GPIO					RCC_AHB1Periph_GPIOB
#define  TimerEncorder2									TIM4
#define  TimerEncorder2_GPIO_Pin						GPIO_Pin_6|GPIO_Pin_7
#define  TimerEncorder2_GPIO							GPIOB
#define  TimerEncorder2_Period							65535
#define  TimerEncorder2_Prescaler						0


void Timer_NVIC_Config(void);
void Timer_Config(void);
void Timer_Init(void);

void Pwm_Config(void);
void Timer_Encorder1_Config(void);
void Timer_Encorder2_Config(void);

#endif/*_TIMER_H*/
