
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

//��ʼ�ź�   ��SCLΪ�ߵ�ƽ��ʱ��SDA�ɸ߱�Ϊ��
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

//ֹͣ�ź�  ��SCLΪ�ߵ�ƽ��ʱ��SDA�ɵͱ�Ϊ��
static void I2C_Stop(void){
	I2c_SDA_OUT();
	I2C_SCL_HIGH();
	I2C_SDA_LOW();
	Delay_us(10);
	I2C_SDA_HIGH();
	Delay_us(10);
}

//Ӧ���ź�  �ڵھŸ�ʱ������  ��SCLΪ�ߵ�ƽ��ʱ��SDAΪ��ΪӦ���ź�  �����ͷ�����
static void I2C_Ack(void){		//?
	I2C_SCL_LOW();
	I2c_SDA_OUT();	//��֪��Ϊʲô��Ҫ������  ���ܷ�ǰ�� ��ʼ�������
	I2C_SDA_LOW();
	Delay_us(10);
	I2C_SCL_HIGH();
	Delay_us(10);
	I2C_SCL_LOW();
	Delay_us(10);
	
}

//��Ӧ���ź�  �ڵھŸ�ʱ������  ��SCLΪ�ߵ�ƽ��ʱ��SDA�ø�Ϊ��Ӧ���ź�  �����ͷ�����
static void I2C_Nack(void){
	I2C_SCL_LOW();
	I2c_SDA_OUT();		//��֪��Ϊʲô��Ҫ������  ���ܷ�ǰ��
	I2C_SDA_HIGH();
	Delay_us(10);
	I2C_SCL_HIGH();
	Delay_us(10);
	I2C_SCL_LOW();
	Delay_us(10);
	
}

//�ȴ�Ӧ���ź�
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
 
//����һ���ֽ�   ��SCL ��Ϊ�͵�ƽ��ʱ��SDA�߲ŷ����ı�  ���ݸ�λ����  �����ͷ�����
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

//��ȡһ���ֽڵ�����
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

//���Ͷ���ֽ�
uint8_t OutTime = 20;
void I2C_SoftWare_WriteArray(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){
	I2C_Start();
	I2C_SoftWare_WriteByte((addr<<1)|I2C_Write);		//���ʹӻ���ַ
	if(I2C_WaitAck()){	//Ӧ��ȴ���ʱ
		printf("1 addr error\r\n");
		I2C_Stop();
		return;
	}
	
	I2C_SoftWare_WriteByte(reg);						//���ͼĴ�����ַ
	if(I2C_WaitAck()){	//Ӧ��ȴ���ʱ
		printf("2 reg error\r\n");
		I2C_Stop();
		return;
	}
	
	for(uint8_t j=0;j<num;j++){
		I2C_SoftWare_WriteByte(data[j]);				//��������
		if(I2C_WaitAck()){	//Ӧ��ȴ���ʱ
			printf("3 data %d error\r\n",j);
			I2C_Stop();
			return;
		}
	}
	I2C_Stop();
}

void I2C_SoftWare_ReadArray(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){

	I2C_Start();		//��ʼ�ź�
	I2C_SoftWare_WriteByte((addr<<1)|I2C_Write);//��ַ  д
	if(I2C_WaitAck()){	//Ӧ��ȴ���ʱ
		printf("4 addr error\r\n");
		I2C_Stop();
		return;
	}
	
	I2C_SoftWare_WriteByte(reg);//�Ĵ���
	if(I2C_WaitAck()){	//Ӧ��ȴ���ʱ
		printf("5 reg error\r\n");
		I2C_Stop();
		return;
	}
	
	I2C_Start();
	I2C_SoftWare_WriteByte((addr<<1)|I2C_Read);//��ַ  ��
	if(I2C_WaitAck()){	//Ӧ��ȴ���ʱ
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



