
#include"stm32f4xx.h"
#include "Delay.h" 

//iic�ӿڲ�  //�ײ�ӿ��ļ�
//oled������ //�в�����ļ�
//main������ //�ϲ������ļ�

//1.IIC��ʼ������
void IIC_Init(void)
{			
		//ʱ��
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		//6 7 ����.
		GPIO_InitTypeDef iicstruct;
		iicstruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		//���ģʽ
		iicstruct.GPIO_Mode = GPIO_Mode_OUT;
		//�������-----
		iicstruct.GPIO_OType = GPIO_OType_OD;
		//�������
		iicstruct.GPIO_Speed = GPIO_Low_Speed;
		iicstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		//���ų�ʼ����
		GPIO_Init(GPIOB,&iicstruct);
}

//2.iic���ģʽ����
void iic_outputmode()
{
		GPIO_InitTypeDef iicstruct;
		iicstruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		//���ģʽ
		iicstruct.GPIO_Mode = GPIO_Mode_OUT;
		//�������
		iicstruct.GPIO_OType = GPIO_OType_PP;
		//�������
		iicstruct.GPIO_Speed = GPIO_Speed_100MHz;
		iicstruct.GPIO_PuPd = GPIO_PuPd_UP;
		//���ų�ʼ����
		GPIO_Init(GPIOB,&iicstruct);
}


//3IIC��ʼ�źź���  ok
void IIC_Start(void)
{	
		//����������ߵ�ƽ SCL
		GPIO_SetBits(GPIOB,GPIO_Pin_6 );
		GPIO_SetBits(GPIOB,GPIO_Pin_7 );
		//��ʱ2us���л���ʱ�䡣
		Delay_us(2);
		
		//SCLΪ�ߵ�ƽ��ʱ��SDAΪ�߱��
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		Delay_us(2);
		
		//ʱ���߱�Ϊ�͵�ƽ��
		//SCLΪ�͵�ƽ��ʱ�����������߽������ݵĸı�
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		//�����˳���ʱ��������Ϊ��
}	

//4IICֹͣ�źź���
void IIC_Stop(void)
{
		//iic���ģʽ
		iic_outputmode();
		//ֹͣ�ź�����
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		Delay_us(2);
	
		//ʱ���߱�ɸߵ�ƽ��
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Delay_us(2);
		//�������ɵ͵���
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		Delay_us(2);
}
//5.iic��ȡӦ���źź���
unsigned char iic_getack()
{
			//ʱ����Ϊ�ߵ�ƽ��ʱ���������ϵ���������Ч�ġ�
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
		//������ʱ�ӱ��
		Delay_us(1);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		Delay_us(1);
	
		//ʱ���߱�Ϊ�ߵ�ƽ //���ʱ���ȡ�������ϵ�����
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Delay_us(1);
	
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
				return 1;
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			Delay_us(1);
			return 0; //�͵�ƽ����Ӧ��
		}
}

//6IIC�����ֽں���
void iic_send_byte_data(unsigned char data)
{
		iic_outputmode();
		unsigned char i = 0 ;
		iic_outputmode();
		//ʱ���߸ߵ�ƽ��������Ч ʱ���ߵ͵�ƽ�����ݿ��Ա仯
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		Delay_us(2);
		//�������ݣ�һλһλ����
		for(i = 0 ; i <8 ; i++)
		{		
				//����������ȷ��͵�λ �ٷ��͸�λ
				//�ֽڵ�λ��1������ߵ�ƽ����0������͵�ƽ
			
				if((data>>(7-i))& 0x01)  //�ߵ�ƽ������ߵ�ƽ �ȷ��͸�λ�ٷ��͵�λ
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
				else		//�͵�ƽ������͵�ƽ
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
				Delay_us(2);//һλһλ����
				
				//ʱ����Ϊ�ߵ�ƽ��ʱ��������Ч
				GPIO_SetBits(GPIOB,GPIO_Pin_6);
				Delay_us(2);//һλһλ����	
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				Delay_us(2);
		}
		//�����������������SCLΪ�͵�ƽ����Ϊʱ���ߵ͵�ƽ������Ч
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		Delay_us(1);
}


