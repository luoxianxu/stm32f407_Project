#include "stm32f4xx.h"
#include "Delay.h"

/*
 ʹ��ADC1
 ADC_IN0 --->����--->PA0
 ADC_IN10 --->�ɱ��ѹ����--->PC0
 ADC_IN8--->J15-->PB0
 ADC_IN9--->J14-->PB1 --->�ƾ�������
*/
//��ʼ��ADC	
//������������·���� + �ɼ����� + ��ƽ��ֵ����
void  myAdcInit(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	//1ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//2ADC1��·ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

	//3�ȳ�ʼ��ADC1ͨ��0 IO�� PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//ģ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	//��ʼ�� PA0 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//ADC1��λ //��λ����	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	 
 	
	//����ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	//DMAʧ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	//4��ʼ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	//12λģʽ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//��ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	//�ر�����ת�� һ��ת��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//��ֹ������⣬ʹ��������� 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//�Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	//5ADC��ʼ��
	ADC_Init(ADC1, &ADC_InitStructure);
	//6����ADת����	
	ADC_Cmd(ADC1, ENABLE);
}				  
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
unsigned short int	getAdc(unsigned char ch)   
{	
	//���ù���ͨ����
	//1ADC1,ͨ��0,һ�����У�480����������,��߲���ʱ�������߾�ȷ�ȣ����ǲ���ʱ�䡣		    
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	
	//2��ʼת��
	ADC_SoftwareStartConv(ADC1);			
	//3�ȴ�ת������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//4��ȡת�����
	return ADC_GetConversionValue(ADC1);	
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
unsigned short int getAdcAverage(unsigned char ch,unsigned char times)
{
	//һ��ͨ���ɼ�5�Σ�ȡ�� ��ƽ��ֵ��
	unsigned short int temp_val=0;
	unsigned char  t;
	for(t=0;t<times;t++)
	{
		temp_val+=getAdc(ch);
		Delay_ms(5);
	}
	return temp_val/times;
} 

