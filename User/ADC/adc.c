#include "stm32f4xx.h"
#include "Delay.h"

/*
 使用ADC1
 ADC_IN0 --->光照--->PA0
 ADC_IN10 --->可变电压输入--->PC0
 ADC_IN8--->J15-->PB0
 ADC_IN9--->J14-->PB1 --->酒精传感器
*/
//初始化ADC	
//三个函数：电路配置 + 采集函数 + 求平均值函数
void  myAdcInit(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	//1使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//2ADC1电路使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

	//3先初始化ADC1通道0 IO口 PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//模拟输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//不带上下拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	//初始化 PA0 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//ADC1复位 //复位结束	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	 
 	
	//独立模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	//DMA失能
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	//4初始化
	ADC_CommonInit(&ADC_CommonInitStructure);

	//12位模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//非扫描模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	//关闭连续转换 一次转换
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//禁止触发检测，使用软件触发 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	//5ADC初始化
	ADC_Init(ADC1, &ADC_InitStructure);
	//6开启AD转换器	
	ADC_Cmd(ADC1, ENABLE);
}				  
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
unsigned short int	getAdc(unsigned char ch)   
{	
	//设置规则通道组
	//1ADC1,通道0,一个序列，480个机器周期,提高采样时间可以提高精确度，这是采样时间。		    
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	
	//2开始转换
	ADC_SoftwareStartConv(ADC1);			
	//3等待转换结束
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//4获取转换结果
	return ADC_GetConversionValue(ADC1);	
}

//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
unsigned short int getAdcAverage(unsigned char ch,unsigned char times)
{
	//一个通道采集5次，取和 求平均值。
	unsigned short int temp_val=0;
	unsigned char  t;
	for(t=0;t<times;t++)
	{
		temp_val+=getAdc(ch);
		Delay_ms(5);
	}
	return temp_val/times;
} 

