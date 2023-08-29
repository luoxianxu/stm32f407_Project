
#include "timer.h"

void Timer_NVIC_Config(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;				//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//中断通道使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;		//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//子优先级
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(Timer, TIM_IT_Update, ENABLE);					//中断配置
}
void Timer_Config(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//TIM2时钟使能
	TIM_TimeBaseInitTypeDef Timer_InitStruct;
	Timer_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			//时钟分频
	Timer_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;		//计数模式
	Timer_InitStruct.TIM_Period = Period;						//周期
	Timer_InitStruct.TIM_Prescaler = Prescaler;					//分频
	TIM_TimeBaseInit(Timer, &Timer_InitStruct);
	
	Timer_NVIC_Config();
	
	TIM_Cmd(Timer, ENABLE);
}

void Timer_Init(void){
	Timer_Config();
	Timer_NVIC_Config();
}

void Pwm_Config(void)      //定时器4配置为PWM波配置
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//修改TIM
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;        //修改IO口              
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	       
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   
	GPIO_Init(GPIOB,&GPIO_InitStructure);                      
	
	TIM_TimeBaseStructure.TIM_Prescaler=Pwm_Timer_Prescaler;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=Pwm_Timer_Period;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure); //修改定时器通道
	TIM_OC4Init(TIM3, &TIM_OCInitStructure); //修改定时器通道
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
}

void Timer_Encorder1_Config(void)  //左边  定时器3配置为编码器模式（只有ch1和ch2能作为编码器引脚）
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//使能TIM3和GPIOA
	RCC_TimerEncorder1_PeriphClockCmd(RCC_TimerEncorder1_Periph_TIM,ENABLE);
	RCC_TimerEncorder1_GPIO_PeriphClockCmd(RCC_TimerEncorder1_Periph_GPIO,ENABLE);
	
	//GPIO复用
	GPIO_PinAFConfig(TimerEncorder1_GPIO, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(TimerEncorder1_GPIO, GPIO_PinSource7, GPIO_AF_TIM3);

	//GPIO口配置
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;   			//浮空输入
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_InitStruct.GPIO_Pin=TimerEncorder1_GPIO_Pin;
	GPIO_Init(TimerEncorder1_GPIO,&GPIO_InitStruct);
	
	//TIM3配置		时基结构体配置
	TIM_TimeBaseStructInit(&TIM_InitStruct);	
	TIM_InitStruct.TIM_Prescaler=TimerEncorder1_Prescaler;		//定时器分频
	TIM_InitStruct.TIM_Period=TimerEncorder1_Period;			//周期
	TIM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;				//时钟分频
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;			//向上计数
	TIM_TimeBaseInit(TimerEncorder1,&TIM_InitStruct);
	
	//4倍频
	TIM_EncoderInterfaceConfig(TimerEncorder1,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_ICFilter=0;
	TIM_ICInit(TimerEncorder1,&TIM_ICInitStruct);
	
	TimerEncorder1->CNT=0;
	TIM_Cmd(TimerEncorder1,ENABLE);
}

void Timer_Encorder2_Config(void)  //右边  定时器1配置为编码器模式（只有ch1和ch2能作为编码器引脚）
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//使能TIM1和GPIOA
	RCC_TimerEncorder2_PeriphClockCmd(RCC_TimerEncorder2_Periph_TIM,ENABLE);
	RCC_TimerEncorder2_GPIO_PeriphClockCmd(RCC_TimerEncorder2_Periph_GPIO,ENABLE);
	
	//GPIO复用功能
	GPIO_PinAFConfig(TimerEncorder2_GPIO, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(TimerEncorder2_GPIO, GPIO_PinSource7, GPIO_AF_TIM4);
	
	//GPIO口配置
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;   //输入
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Pin=TimerEncorder2_GPIO_Pin;
	GPIO_Init(TimerEncorder2_GPIO,&GPIO_InitStruct);
	
	//TIM1配置
	TIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Prescaler=TimerEncorder2_Prescaler;
	TIM_InitStruct.TIM_Period=TimerEncorder2_Period;
	TIM_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TimerEncorder2,&TIM_InitStruct);
	
	TIM_EncoderInterfaceConfig(TimerEncorder2,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_ICFilter=0;
	TIM_ICInit(TimerEncorder2,&TIM_ICInitStruct);
	
	TimerEncorder2->CNT=0;
	TIM_Cmd(TimerEncorder2,ENABLE);
}

