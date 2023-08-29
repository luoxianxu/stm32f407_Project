#ifndef __DHT11_H
#define __DHT11_H

#define OUT 1
#define IN 0
void DHT_rcc(void);

void DHT_mode(unsigned char mode);

void DHT_start(void);
void DHT_Resend(void);
unsigned char DHT_ReadData_Byte(void);


void DHT_Recevice_Data(char *humi,char *temp,char *humi_xiao,char *temp_xiao);

#endif
