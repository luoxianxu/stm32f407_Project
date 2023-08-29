
#include "software_i2c.h"
#include "delay.h"
#include <stdio.h>

static void I2c_SCL_OUT(void){
	RCC_AHB1PeriphClockCmd(I2C_SCL_GPIOPeriph,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(I2C_SCL_GPIO, &GPIO_InitStruct);
}

static void I2c_SDA_OUT(void){
	RCC_AHB1PeriphClockCmd(I2C_SDA_GPIOPeriph,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_Pin;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStruct);
}

static void I2c_SDA_IN(void){
	RCC_AHB1PeriphClockCmd(I2C_SDA_GPIOPeriph,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStruct);
}

//起始信号   在SCL为高电平的时候，SDA由高变为低
static void I2C_Start(void){
	I2c_SDA_OUT();
	I2C_SCL_HIGH();
	I2C_SDA_HIGH();
	Delay_us(10);
	I2C_SDA_LOW();
	Delay_us(10);
	I2C_SCL_LOW();
	Delay_us(10);
}

//停止信号  在SCL为高电平的时候，SDA由低变为高
static void I2C_Stop(void){
	I2c_SDA_OUT();
	I2C_SCL_HIGH();
	I2C_SDA_LOW();
	Delay_us(10);
	I2C_SDA_HIGH();
	Delay_us(10);
}

//应答信号  在第九个时钟周期  当SCL为高电平的时候，SDA为低为应答信号  无需释放总线
static void I2C_Ack(void){		//?
	I2C_SCL_LOW();
	I2c_SDA_OUT();	//不知道为什么需要放这里  不能放前面 初始化会出错
	I2C_SDA_LOW();
	Delay_us(10);
	I2C_SCL_HIGH();
	Delay_us(10);
	I2C_SCL_LOW();
	Delay_us(10);
	
}

//非应答信号  在第九个时钟周期  当SCL为高电平的时候，SDA置高为非应答信号  无需释放总线
static void I2C_Nack(void){
	I2C_SCL_LOW();
	I2c_SDA_OUT();		//不知道为什么需要放这里  不能放前面
	I2C_SDA_HIGH();
	Delay_us(10);
	I2C_SCL_HIGH();
	Delay_us(10);
	I2C_SCL_LOW();
	Delay_us(10);
	
}

//等待应答信号
static uint8_t I2C_WaitAck(void){
	uint8_t ack = 0;
//	uint8_t OutTime = 120;
	I2c_SDA_IN();
	I2C_SCL_LOW();
	Delay_us(10);
	I2C_SCL_HIGH();
	Delay_us(10);
	if(I2C_Read_SDA())
		ack = 1;
	I2C_SCL_LOW();
	Delay_us(10);
	return ack;
}

void I2C_SoftWare_Init(void){
	I2c_SCL_OUT();
	I2c_SDA_OUT();
	
	I2C_SCL_HIGH();
	I2C_SDA_HIGH();
}
 
//发送一个字节   在SCL 线为低电平的时候，SDA线才发生改变  数据高位先行  无需释放总线
void I2C_SoftWare_WriteByte(uint8_t data){
	I2c_SDA_OUT();
	
	I2C_SCL_LOW();
//	Delay_us(5);
	for(uint8_t i=0;i<8;i++){
		if(data&0x80)
			I2C_SDA_HIGH();
		else I2C_SDA_LOW();
		data<<=1;
		Delay_us(10);
		I2C_SCL_HIGH();
		Delay_us(10);
		I2C_SCL_LOW();
		Delay_us(10);
	}
}

//读取一个字节的数据
uint8_t I2C_SoftWare_ReadByte(uint8_t ack){
	I2c_SDA_IN();
	uint8_t rec = 0x00;
	for(uint8_t i=0;i<8;i++){
		I2C_SCL_LOW();
		Delay_us(10);
		I2C_SCL_HIGH();
		rec<<=1;
		if(I2C_Read_SDA()){
			rec++;
		}
		Delay_us(10);
//		I2C_SCL_LOW();
	}
	if(ack)
		I2C_Ack();
	else I2C_Nack();
	return rec;
}

//发送多个字节
uint8_t OutTime = 20;
void I2C_SoftWare_WriteArray(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){
	I2C_Start();
	I2C_SoftWare_WriteByte((addr<<1)|I2C_Write);		//发送从机地址
	if(I2C_WaitAck()){	//应答等待超时
		printf("1 addr error\r\n");
		I2C_Stop();
		return;
	}
	
	I2C_SoftWare_WriteByte(reg);						//发送寄存器地址
	if(I2C_WaitAck()){	//应答等待超时
		printf("2 reg error\r\n");
		I2C_Stop();
		return;
	}
	
	for(uint8_t j=0;j<num;j++){
		I2C_SoftWare_WriteByte(data[j]);				//发送数据
		if(I2C_WaitAck()){	//应答等待超时
			printf("3 data %d error\r\n",j);
			I2C_Stop();
			return;
		}
	}
	I2C_Stop();
}

void I2C_SoftWare_ReadArray(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){

	I2C_Start();		//起始信号
	I2C_SoftWare_WriteByte((addr<<1)|I2C_Write);//地址  写
	if(I2C_WaitAck()){	//应答等待超时
		printf("4 addr error\r\n");
		I2C_Stop();
		return;
	}
	
	I2C_SoftWare_WriteByte(reg);//寄存器
	if(I2C_WaitAck()){	//应答等待超时
		printf("5 reg error\r\n");
		I2C_Stop();
		return;
	}
	
	I2C_Start();
	I2C_SoftWare_WriteByte((addr<<1)|I2C_Read);//地址  读
	if(I2C_WaitAck()){	//应答等待超时
		printf("6 addr error\r\n");
		I2C_Stop();
		return;
	}
	
	for(uint8_t j=0;j<num;j++){
		if(j==num-1)
			data[j]=I2C_SoftWare_ReadByte(0);
		else data[j]=I2C_SoftWare_ReadByte(1);
	}
	I2C_Stop();
}



