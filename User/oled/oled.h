#ifndef __oled_H
#define __oled_H

void InitOLed(void);
void Oled_print(uint8_t x, uint8_t y,char *str);
void OLed_ShowChina(uint8_t x,uint8_t y,uint8_t *buf);
void OLed_ShowASCII(uint8_t x, uint8_t y,char *str);
void OLed_ShowLight(void);
#endif
