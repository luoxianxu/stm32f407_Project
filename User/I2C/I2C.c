#include "I2C.h"
#include <stdio.h>

// I2C1		SDA	PB7|PB9				SCL	PB6|PB10
// I2C2		SDA PH5|PF0|PB11		SCL PH4|PF1|PB10
// I2C3		SDA PH8|PC9				SCL PH7|PA8

void myI2C_Init(void){
	
	RCC_I2CPeriphCLockCmd(RCC_I2CPeriph,ENABLE);				//I2Cʱ��ʹ��
	RCC_I2CGPIOPeriphClockCmd(RCC_I2CGPIOPeriph,ENABLE);		//GPIO Bʹ��
	//GPIO����
	GPIO_PinAFConfig(I2C_GPIO, I2C_SCLGPIO_PinSource, GPIO_AF_I2C1);
	GPIO_PinAFConfig(I2C_GPIO, I2C_SDAGPIO_PinSource, GPIO_AF_I2C1);
	//GPIO��ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = I2C_SCLGPIOPin | I2C_SDAGPIOPin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
	//I2C��ʼ��
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;					//I2C1ģʽ
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;				//����Ӧ���ź�
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		//7λ��ַ
	I2C_InitStruct.I2C_ClockSpeed = 400000;				//400KHz
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;	//�Լ��ĵ�ַ

	I2C_Init(I2C, &I2C_InitStruct);
	I2C_Cmd(I2C, ENABLE);		//I2Cʹ��
	
}

uint8_t outtime = 20;
void myI2C_SendData(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){
	uint8_t i = outtime;
	
    while((i--)&&I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));//�ȴ����߿���
    if(i==0){
		printf("busy\r\n");
		return;
	}
	
	i = outtime;
	I2C_GenerateSTART(I2C, ENABLE);		//������ʼ�ź�
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);//
	if(i==0){
		printf("start busy\r\n");
		return;
	}
	
	//���ʹӻ���ַ		дģʽ
	i = outtime;
	I2C_Send7bitAddress(I2C, addr, I2C_Direction_Transmitter);
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);//7λ��ַ���������ͣ��ӻ�������Ӧ���ź�
	if(i==0){
		printf("address error\r\n");
		return;
	}
	
	i = outtime;
	I2C_SendData(I2C, reg);
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);//����д��Ĵ����������ڱ��Ƴ��źţ�
	if(i==0){
		printf("send reg error\r\n");
		return;
	}
	
	while(num--){
		I2C_SendData(I2C, data[i++]);
		//EV8  I2C_EVENT_MASTER_BYTE_TRANSMITTING  ����д��Ĵ����������ڱ��Ƴ�
		//EV8_2  I2C_EVENT_MASTER_BYTE_TRANSMITTED  �����Ѿ������䵽��������
		//���У�����һ��ĳ�����˵��EV8���ӣ�EV8_2����������Ǹ��ɿ�
		i = outtime;
		while(i-- && I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);
		if(i==0){
			printf("send error\r\n");
			return;
		}
	}
	I2C_GenerateSTOP(I2C, ENABLE);	//ͨ�Ž����ź�
}

void myI2C_ReadData(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){
	uint8_t i = outtime;
	uint8_t cnt = 0;
	while(i-- && I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));//�ȴ����߲�æ
	if(i==0){
		printf("busy\r\n");
		return;
	}
	
	i = outtime;
	I2C_GenerateSTART(I2C, ENABLE);		//������ʼ�ź�
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	if(i==0){
		printf("start busy\r\n");
		return;
	}
	
	//���ʹӻ���ַ		дģʽ
	I2C_Send7bitAddress(I2C, addr, I2C_Direction_Transmitter);
	i = outtime;
	//7λ��ַ���������ͣ��ӻ�������Ӧ���ź�
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
	if(i==0){
		printf("address error\r\n");
		return;
	}
	
	I2C_SendData(I2C, reg);
	i = outtime;
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);//����д��Ĵ����������ڱ��Ƴ��źţ�
	if(i==0){
		printf("send reg error\r\n");
		return;
	}

	i = outtime;
	I2C_GenerateSTART(I2C, ENABLE);		//������ʼ�ź�
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	if(i==0){
		printf("busy\r\n");
		return;
	}

	i = outtime;
	I2C_Send7bitAddress(I2C, addr, I2C_Direction_Receiver);	//����ģʽ
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);//7λ��ַ���������գ��ӻ�������Ӧ���ź�
	if(i==0){
		printf("address error\r\n");
		return;
	}

	while(num--){
		i = outtime;
		while(i-- && I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);//��ģʽ���ձ���ȴ����¼����������ܶ�ȡ����
		if(i==0){
			printf("rec error\r\n");
			return;
		}
		
		data[cnt++] = I2C_ReceiveData(I2C);
		if(num == 1){
			I2C_AcknowledgeConfig(I2C, DISABLE);	//�ر�Ӧ��
		}
	}
}


//unsigned char I2C_Err=0;
//uint8_t I2C_ReadOneByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t Reg_addr)
//{  
//    uint8_t readout;
//    u32 tmr;

//    tmr = outtime;
//    while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
//    if(tmr==0) I2C_Err = 1;

//    I2C_GenerateSTART(I2Cx, ENABLE);
//    //����I2C��START�źţ��ӿ��Զ��Ӵ��豸������豸
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_Send7bitAddress(I2Cx,I2C_Addr,I2C_Direction_Transmitter);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_SendData(I2Cx, Reg_addr);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_GenerateSTART(I2Cx, ENABLE);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
//    if(tmr==0) I2C_Err = 1; 

//    I2C_AcknowledgeConfig(I2Cx, DISABLE);
//    I2C_GenerateSTOP(I2Cx, ENABLE);
//    tmr = outtime;
//    while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))));  /* EV7 */
//    if(tmr==0) I2C_Err = 1;

//    readout = I2C_ReceiveData(I2Cx);

//    I2C_AcknowledgeConfig(I2Cx, ENABLE);

//    return readout;
//}

//void I2C_WriteOneByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t Reg_addr,uint8_t value)
//{
//    u32 tmr;

//    tmr = outtime;
//    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
//    while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
//    if(tmr==0) I2C_Err = 1;

//    I2C_GenerateSTART(I2Cx, ENABLE);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))); 
//    if(tmr==0) I2C_Err = 1;

//    I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_SendData(I2Cx, Reg_addr);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_SendData(I2Cx, value);
//    tmr = outtime;
//    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_GenerateSTOP(I2Cx, ENABLE);
//    //I2C_AcknowledgeConfig(I2Cx, DISABLE);
//}
