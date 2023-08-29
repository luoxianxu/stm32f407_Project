#ifndef __I2C_H
#define __I2C_H
void IIC_Init(void);
void iic_outputmode(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char iic_getack(void);
void iic_send_byte_data(unsigned char data);

#endif
