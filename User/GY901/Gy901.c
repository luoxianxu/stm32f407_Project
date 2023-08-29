#include "Gy901.h"
#include "Delay.h"
#include <stdio.h>
#include <string.h>

float Acc_x = 0.0,Acc_y = 0.0,Acc_z = 0.0;
float Gyro_x = 0.0,Gyro_y = 0.0,Gyro_z = 0.0;
float Roll = 0.0,Pitch = 0.0,Yaw = 0.0;

uint16_t CalcCRCSUM(int16_t *arr,uint8_t len){
	uint16_t sum = 0;
	for(int i=0;i<len;i++){
		sum += arr[i];
	}
	return (uint8_t)(sum&0xFF);		//取低八位
}


uint8_t num = 0;
uint8_t Gy901UsartTxCnt = 0;
int16_t Gy901UsartTxBuf[11] = {0};
void USART2_IRQHandler(void){
	uint32_t status = USART_GetITStatus(Gy901_USART, USART_IT_RXNE);
	int32_t rec = 0;
	if(status & USART_IT_RXNE){		//接收中断
		rec = USART_ReceiveData(Gy901_USART);		//获取数据，会自动清除
//		USART_SendData(USART1, rec);
		if(Gy901UsartTxCnt == 0 && rec == 0x55){
			Gy901UsartTxBuf[Gy901UsartTxCnt++] = rec;
		}
		else if(Gy901UsartTxCnt >= 1 && Gy901UsartTxCnt <= 9){
			Gy901UsartTxBuf[Gy901UsartTxCnt++] = rec;
		}
		else if(Gy901UsartTxCnt == 10 && (CalcCRCSUM(Gy901UsartTxBuf,10)&0x00FF) == rec){
			Gy901UsartTxBuf[Gy901UsartTxCnt++] = rec;
			if(Gy901UsartTxBuf[1] == 0x51){		//加速度帧
				short data_x = (short)((short)Gy901UsartTxBuf[3]<<8|Gy901UsartTxBuf[2]);
				short data_y = (short)((short)Gy901UsartTxBuf[5]<<8|Gy901UsartTxBuf[4]);
				short data_z = (short)((short)Gy901UsartTxBuf[7]<<8|Gy901UsartTxBuf[6]);
				Acc_x = (float)data_x/32768*16*9.8f;
				Acc_y = (float)data_y/32768*16*9.8f;
				Acc_z = (float)data_z/32768*16*9.8f;
//				printf("Acc_x = %f   Acc_y = %f  Acc_z = %f\r\n",Acc_x,Acc_y,Acc_z);
			}
			else if(Gy901UsartTxBuf[1] == 0x52){		//角速度帧
				short data_x = (short)((short)Gy901UsartTxBuf[3]<<8|Gy901UsartTxBuf[2]);
				short data_y = (short)((short)Gy901UsartTxBuf[5]<<8|Gy901UsartTxBuf[4]);
				short data_z = (short)((short)Gy901UsartTxBuf[7]<<8|Gy901UsartTxBuf[6]);
				Gyro_x = (float)data_x/32768*2000;
				Gyro_y = (float)data_y/32768*2000;
				Gyro_z = (float)data_z/32768*2000;
//				printf("Gyro_x = %f   Gyro_y = %f  Gyro_z = %f\r\n",Gyro_x,Gyro_y,Gyro_z);
			}
			else if(Gy901UsartTxBuf[1] == 0x53){		//角度帧
				short data_x = (short)((short)Gy901UsartTxBuf[3]<<8|Gy901UsartTxBuf[2]);
				short data_y = (short)((short)Gy901UsartTxBuf[5]<<8|Gy901UsartTxBuf[4]);
				short data_z = (short)((short)Gy901UsartTxBuf[7]<<8|Gy901UsartTxBuf[6]);
				Roll  = (float)data_x/32768*180;
				Pitch = (float)data_y/32768*180;
				Yaw   = (float)data_z/32768*180;
//				printf("Roll = %f   Pitch = %f  Gyro_z = %f\r\n",Roll,Pitch,Yaw);
			}
			memset(Gy901UsartTxBuf,0,sizeof(Gy901UsartTxBuf));
			Gy901UsartTxCnt = 0;
		}
		else{
			memset(Gy901UsartTxBuf,0,sizeof(Gy901UsartTxBuf));
			Gy901UsartTxCnt = 0;
		}
		USART_ClearITPendingBit(Gy901_USART, USART_IT_RXNE);
	}
}


static void Gy901UsartInit(void){
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//时钟使能
	Gy901_USART_PeripheralClockCmd(Gy901_USART_Peripheral,ENABLE);
	Gy901_USARTGPIO_PeriphClockCmd(Gy901_USART_GPIOPeripheral,ENABLE);
	
	GPIO_PinAFConfig(Gy901_USART_GPIO, Gy901_Tx_PinSource, Gy901_AF_USART);		//GPIO复用功能
	GPIO_PinAFConfig(Gy901_USART_GPIO, Gy901_Rx_PinSource, Gy901_AF_USART);
	//串口引脚配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = Gy901_Tx_GPIOPin|Gy901_Rx_GPIOPin;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(Gy901_USART_GPIO, &GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate=230400;   //配置串口波特率
	USART_InitStruct.USART_StopBits=USART_StopBits_1; //一个停止位
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStruct.USART_WordLength=USART_WordLength_8b; //字长为8位数据格式
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; //收发模式
	USART_InitStruct.USART_Parity=USART_Parity_No;  //无校验位
	USART_Init(Gy901_USART,&USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = Gy901_USART_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(Gy901_USART, USART_IT_RXNE, ENABLE);

	USART_DMACmd(Gy901_USART, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(Gy901_USART,ENABLE); //串口使能
}

void Gy901USARTSendData(uint8_t *data,uint8_t Len){		//串口发送数据
	for(uint8_t i=0;i<Len;i++){
		USART_SendData(Gy901_USART, data[i]);
		while(USART_GetFlagStatus(Gy901_USART,USART_FLAG_TXE) == RESET);
	}
}

void Gy901WriteStart(void){
	uint8_t Unlock[5] = {0xFF,0xAA,0x69,0x88,0xB5};
	Gy901USARTSendData(Unlock,5);		//发送解锁指令
}

void Gy901WriteStop(void){
	uint8_t Save[5] = {0xFF,0xAA,0x00,0x00,0x00};
	Gy901USARTSendData(Save,5);			//发送保存指令
}

void Gy901WriteData(uint8_t type,uint8_t dataL,uint8_t dataH){
	uint8_t WirteBuf[5] = {0};
	WirteBuf[0] = 0xFF;
	WirteBuf[1] = 0xAA;
	WirteBuf[2] = type;
	WirteBuf[3] = dataL;
	WirteBuf[4] = dataH;
	Gy901USARTSendData(WirteBuf,5);		//发送数据
}

uint8_t Gy901DmaUsartRxBuffer0[11] = {0};
uint8_t Gy901DmaUsartRxBuffer1[11] = {0};
void Gy901UsartDmaTxConfig(void){
	DMA_InitTypeDef DMA_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//时钟使能
	Gy901UsartDmaPeriphClockCmd(Gy901UsartDmaPeriph,ENABLE);
	DMA_InitStruct.DMA_Channel = Gy901DmaChannel;							//通道0
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(Gy901_USART->DR);	//外设地址
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)Gy901DmaUsartRxBuffer0;	//目的地址   双缓冲之一
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;					//方向为外设到内存
	DMA_InitStruct.DMA_BufferSize = Gy901DmaBufferSize;						//dma传输数据量
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址不增加
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;					//内存地址增加
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据单位
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//内存数据单位
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;							//正常模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;					//DMA优先级
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init(Gy901DmaDMAy_Streamx, &DMA_InitStruct);
	
	//配置双缓冲区  并设置Memory0为当前缓冲区
//	DMA_DoubleBufferModeConfig(Gy901DmaDMAy_Streamx, (uint32_t)Gy901DmaUsartRxBuffer1,DMA_Memory_0);
	NVIC_InitStruct.NVIC_IRQChannel = Gy901DMA_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	DMA_ITConfig(Gy901DmaDMAy_Streamx, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(Gy901DmaDMAy_Streamx, DMA_IT_TC);
//	DMA_DoubleBufferModeCmd(Gy901DmaDMAy_Streamx, ENABLE);		//启动双缓冲区
	DMA_Cmd(Gy901DmaDMAy_Streamx, ENABLE);
}

void Gy901_Init(void){
	//串口接收DMA配置
//	Gy901UsartDmaTxConfig();
	//串口初始化
	Gy901UsartInit();
	Gy901WriteStart();
	Delay_ms(50);
	Gy901WriteData(0x02,0x0E,0x00);		//读取角速度 角加速度  角度
//	Gy901WriteData(0x03,0x07,0x00);		//输出频率为20hz
//	Gy901WriteData(0x04,0x06,0x00);
	Delay_ms(50);
	Gy901WriteStop();
}
