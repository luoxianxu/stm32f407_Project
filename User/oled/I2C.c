
#include"stm32f4xx.h"
#include "Delay.h" 

//iic接口层  //底层接口文件
//oled函数层 //中层控制文件
//main函数层 //上层主调文件

//1.IIC初始化函数
void IIC_Init(void)
{			
		//时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		//6 7 引脚.
		GPIO_InitTypeDef iicstruct;
		iicstruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		//输出模式
		iicstruct.GPIO_Mode = GPIO_Mode_OUT;
		//推挽输出-----
		iicstruct.GPIO_OType = GPIO_OType_OD;
		//低速输出
		iicstruct.GPIO_Speed = GPIO_Low_Speed;
		iicstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		//引脚初始化。
		GPIO_Init(GPIOB,&iicstruct);
}

//2.iic输出模式函数
void iic_outputmode()
{
		GPIO_InitTypeDef iicstruct;
		iicstruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		//输出模式
		iicstruct.GPIO_Mode = GPIO_Mode_OUT;
		//推挽输出
		iicstruct.GPIO_OType = GPIO_OType_PP;
		//低速输出
		iicstruct.GPIO_Speed = GPIO_Speed_100MHz;
		iicstruct.GPIO_PuPd = GPIO_PuPd_UP;
		//引脚初始化。
		GPIO_Init(GPIOB,&iicstruct);
}


//3IIC开始信号函数  ok
void IIC_Start(void)
{	
		//两个都输出高电平 SCL
		GPIO_SetBits(GPIOB,GPIO_Pin_6 );
		GPIO_SetBits(GPIOB,GPIO_Pin_7 );
		//延时2us进行缓冲时间。
		Delay_us(2);
		
		//SCL为高电平的时候，SDA为高变低
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		Delay_us(2);
		
		//时钟线变为低电平。
		//SCL为低电平的时候允许数据线进行数据的改变
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		//函数退出的时候，两个都为低
}	

//4IIC停止信号函数
void IIC_Stop(void)
{
		//iic输出模式
		iic_outputmode();
		//停止信号制作
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		Delay_us(2);
	
		//时钟线变成高电平，
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Delay_us(2);
		//数据线由低到高
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		Delay_us(2);
}
//5.iic获取应答信号函数
unsigned char iic_getack()
{
			//时钟线为高电平的时候，数据线上的数据是有效的。
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
		//数据线时钟变高
		Delay_us(1);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		Delay_us(1);
	
		//时钟线变为高电平 //这个时候读取数据线上的数据
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Delay_us(1);
	
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
				return 1;
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			Delay_us(1);
			return 0; //低电平才算应答
		}
}

//6IIC发送字节函数
void iic_send_byte_data(unsigned char data)
{
		iic_outputmode();
		unsigned char i = 0 ;
		iic_outputmode();
		//时钟线高电平，数据有效 时钟线低电平，数据可以变化
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		Delay_us(2);
		//串行数据，一位一位发送
		for(i = 0 ; i <8 ; i++)
		{		
				//我们这个是先发送低位 再发送高位
				//字节低位是1就输出高电平，是0就输出低电平
			
				if((data>>(7-i))& 0x01)  //高电平就输出高电平 先发送高位再发送低位
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
				else		//低电平就输出低电平
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
				Delay_us(2);//一位一位传输
				
				//时钟线为高电平的时候，数据有效
				GPIO_SetBits(GPIOB,GPIO_Pin_6);
				Delay_us(2);//一位一位传输	
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				Delay_us(2);
		}
		//所有数据输出结束后，SCL为低电平，因为时钟线低电平数据无效
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		Delay_us(1);
}


