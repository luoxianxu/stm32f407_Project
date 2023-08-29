#ifndef _I2C_H
#define _I2C_H

#include "stm32f4xx.h"

#define RCC_I2CPeriphCLockCmd			RCC_APB1PeriphClockCmd
#define RCC_I2CPeriph					RCC_APB1Periph_I2C1
#define RCC_I2CGPIOPeriphClockCmd		RCC_AHB1PeriphClockCmd
#define RCC_I2CGPIOPeriph				RCC_AHB1Periph_GPIOB

#define I2C								I2C1
#define I2C_GPIO						GPIOB
#define I2C_SCLGPIO_PinSource			GPIO_PinSource6
#define I2C_SDAGPIO_PinSource			GPIO_PinSource7
#define I2C_SCLGPIOPin					GPIO_Pin_6
#define I2C_SDAGPIOPin					GPIO_Pin_7

void myI2C_Init(void);
void myI2C_SendData(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num);
void myI2C_ReadData(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num);

void IIC_Config(void);

uint8_t I2C_ReadOneByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t Reg_addr);
void I2C_WriteOneByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t Reg_addr,uint8_t value);

#endif /*_I2C_H*/
