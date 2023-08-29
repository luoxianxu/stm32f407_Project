#include "stm32f4xx.h"                  // Device header
#include "Delay.h"

#define OUT 1
#define IN 0

unsigned char time=0;
void DHT_rcc()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
}

void DHT_mode(unsigned char mode)
{
	GPIO_InitTypeDef GPIO_initStructure;
	if(mode==OUT)
	{
		GPIO_initStructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIO_initStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_initStructure.GPIO_Pin=GPIO_Pin_3;
		GPIO_initStructure.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_initStructure.GPIO_Speed=GPIO_High_Speed;
	}
	if(mode==IN)
	{
		GPIO_initStructure.GPIO_Mode=GPIO_Mode_IN;
//		GPIO_initStructure.GPIO_OType=GPIO_OType_OD;
		GPIO_initStructure.GPIO_Pin=GPIO_Pin_3;
		GPIO_initStructure.GPIO_PuPd=GPIO_PuPd_UP ;
		GPIO_initStructure.GPIO_Speed=GPIO_High_Speed;
	}
	GPIO_Init(GPIOA,&GPIO_initStructure);
}

void DHT_start()
{
	DHT_mode(OUT);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	Delay_ms(20);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	Delay_us(40);
}

void DHT_Resend()
{
	DHT_mode(IN);
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)&&(time<100))
	{
		time++;
		Delay_us(1);
	}
	time=0;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)&&(time<100))
	{
		time++;
		Delay_us(1);
	}
	time=0;
}

unsigned char DHT_ReadData_Byte()
{
	unsigned char data=0;
	DHT_mode(IN);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//	Delay_us(14);
	for(uint8_t i=0;i<8;i++)
	{
		
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)&&(time<100))
		{
			time++;
			Delay_us(1);
		}
		time=0;
		data<<=1;
		Delay_us(60);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)
		{
			data|=0x01;
			while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)&&(time<100))
			{
				time++;
				Delay_us(1);
			}
			time=0;
			
		}
	}
	return data;
	
}

void DHT_Recevice_Data(char *humi,char *temp,char *humi_xiao,char *temp_xiao)
{
	DHT_start();
	DHT_Resend();
	char DATA[5]={0,0,0,0,0};
	for(uint8_t i=0;i<5;i++)
	{
		DATA[i]=DHT_ReadData_Byte();
	}
	Delay_ms(1);
	if((DATA[0]+DATA[1]+DATA[2]+DATA[3])==DATA[4])
	{
		*humi=DATA[0];
		*humi_xiao=DATA[1];
		*temp=DATA[2];
		*temp_xiao=DATA[3];
	}
	else
	{
		for(uint8_t i=0;i<5;i++)
		{
			DATA[i]=0;
		}
	}
}
