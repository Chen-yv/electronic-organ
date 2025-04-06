#ifndef __OLED_H__
#define __OLED_H__

#include "IIC.h"
#include <string.h>

void OLED_Init(void);

void OLED_Clear(void);


void OLED_ShowString(int x, int y, unsigned char str[], int len);

void OLED_ShowNum(int x, int y, int num);
void oled_info(char data[]);

void OLED_Show_Chinese(int x, int y, int num);

void OLED_SetScorll(u8 set);

#endif
