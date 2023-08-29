#include "I2C.h"
#include <stdio.h>

// I2C1		SDA	PB7|PB9				SCL	PB6|PB10
// I2C2		SDA PH5|PF0|PB11		SCL PH4|PF1|PB10
// I2C3		SDA PH8|PC9				SCL PH7|PA8

void myI2C_Init(void){
	
	RCC_I2CPeriphCLockCmd(RCC_I2CPeriph,ENABLE);				//I2C时钟使能
	RCC_I2CGPIOPeriphClockCmd(RCC_I2CGPIOPeriph,ENABLE);		//GPIO B使能
	//GPIO复用
	GPIO_PinAFConfig(I2C_GPIO, I2C_SCLGPIO_PinSource, GPIO_AF_I2C1);
	GPIO_PinAFConfig(I2C_GPIO, I2C_SDAGPIO_PinSource, GPIO_AF_I2C1);
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = I2C_SCLGPIOPin | I2C_SDAGPIOPin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
	//I2C初始化
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;					//I2C1模式
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;				//启用应答信号
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		//7位地址
	I2C_InitStruct.I2C_ClockSpeed = 400000;				//400KHz
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;	//自己的地址

	I2C_Init(I2C, &I2C_InitStruct);
	I2C_Cmd(I2C, ENABLE);		//I2C使能
	
}

uint8_t outtime = 20;
void myI2C_SendData(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){
	uint8_t i = outtime;
	
    while((i--)&&I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));//等待总线空闲
    if(i==0){
		printf("busy\r\n");
		return;
	}
	
	i = outtime;
	I2C_GenerateSTART(I2C, ENABLE);		//产生起始信号
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);//
	if(i==0){
		printf("start busy\r\n");
		return;
	}
	
	//发送从机地址		写模式
	i = outtime;
	I2C_Send7bitAddress(I2C, addr, I2C_Direction_Transmitter);
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);//7位地址，主机发送，从机产生的应答信号
	if(i==0){
		printf("address error\r\n");
		return;
	}
	
	i = outtime;
	I2C_SendData(I2C, reg);
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);//数据写入寄存器，并正在被移出信号，
	if(i==0){
		printf("send reg error\r\n");
		return;
	}
	
	while(num--){
		I2C_SendData(I2C, data[i++]);
		//EV8  I2C_EVENT_MASTER_BYTE_TRANSMITTING  数据写入寄存器，并正在被移出
		//EV8_2  I2C_EVENT_MASTER_BYTE_TRANSMITTED  数据已经被传输到总线上了
		//其中，对于一般的程序来说，EV8足矣，EV8_2会更慢，但是更可靠
		i = outtime;
		while(i-- && I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);
		if(i==0){
			printf("send error\r\n");
			return;
		}
	}
	I2C_GenerateSTOP(I2C, ENABLE);	//通信结束信号
}

void myI2C_ReadData(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num){
	uint8_t i = outtime;
	uint8_t cnt = 0;
	while(i-- && I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));//等待总线不忙
	if(i==0){
		printf("busy\r\n");
		return;
	}
	
	i = outtime;
	I2C_GenerateSTART(I2C, ENABLE);		//产生起始信号
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	if(i==0){
		printf("start busy\r\n");
		return;
	}
	
	//发送从机地址		写模式
	I2C_Send7bitAddress(I2C, addr, I2C_Direction_Transmitter);
	i = outtime;
	//7位地址，主机发送，从机产生的应答信号
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
	if(i==0){
		printf("address error\r\n");
		return;
	}
	
	I2C_SendData(I2C, reg);
	i = outtime;
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);//数据写入寄存器，并正在被移出信号，
	if(i==0){
		printf("send reg error\r\n");
		return;
	}

	i = outtime;
	I2C_GenerateSTART(I2C, ENABLE);		//产生起始信号
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	if(i==0){
		printf("busy\r\n");
		return;
	}

	i = outtime;
	I2C_Send7bitAddress(I2C, addr, I2C_Direction_Receiver);	//接收模式
	while(i-- && I2C_CheckEvent(I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);//7位地址，主机接收，从机产生的应答信号
	if(i==0){
		printf("address error\r\n");
		return;
	}

	while(num--){
		i = outtime;
		while(i-- && I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);//主模式接收必须等待该事件产生，才能读取数据
		if(i==0){
			printf("rec error\r\n");
			return;
		}
		
		data[cnt++] = I2C_ReceiveData(I2C);
		if(num == 1){
			I2C_AcknowledgeConfig(I2C, DISABLE);	//关闭应答
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
//    //发送I2C的START信号，接口自动从从设备编程主设备
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
