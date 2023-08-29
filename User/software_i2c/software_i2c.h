
#ifndef _SOFTWARE_I2C_H
#define _SOFTWARE_I2C_H

#include "stm32f4xx.h"
#include "sys.h"

#define I2C_SCL_GPIOPeriph			RCC_AHB1Periph_GPIOB
#define I2C_SDA_GPIOPeriph			RCC_AHB1Periph_GPIOB

#define I2C_SCL_Pin					GPIO_Pin_10
#define I2C_SDA_Pin					GPIO_Pin_9

#define I2C_SCL_GPIO				GPIOB
#define I2C_SDA_GPIO				GPIOB

#define I2C_SCL_HIGH()				GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_Pin)
#define I2C_SCL_LOW()				GPIO_ResetBits(I2C_SCL_GPIO, I2C_SCL_Pin)
#define I2C_SDA_HIGH()				GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_Pin)
#define I2C_SDA_LOW()				GPIO_ResetBits(I2C_SDA_GPIO, I2C_SDA_Pin)

#define I2C_Read_SDA()				GPIO_ReadInputDataBit(I2C_SDA_GPIO, I2C_SDA_Pin)

#define I2C_Write					0x00
#define I2C_Read					0x01


static void I2c_SCL_OUT(void);
static void I2c_SDA_OUT(void);
static void I2c_SDA_IN(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Ack(void);
static void I2C_Nack(void);
static uint8_t I2C_WaitAck(void);
void I2C_SoftWare_Init(void);
void I2C_SoftWare_WriteByte(uint8_t data);
uint8_t I2C_SoftWare_ReadByte(uint8_t ack);
void I2C_SoftWare_WriteArray(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num);
void I2C_SoftWare_ReadArray(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t num);


#endif /*_SOFTWARE_I2C_H */
