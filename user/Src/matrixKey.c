#include "matrixKey.h"
#include "printf.h"
#include "Delay.h"

void key_board_init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
u8 key_scan(void){
	GPIO_Write(GPIOA, 0x00);
	for (u8 i = 0; i < 4; i++){
		GPIO_Write(GPIOA, (u16)(1<<i));
		if((GPIO_ReadInputData(GPIOA)>>4)&0xf){
			Delay_ms(5);// Ïû¶¶
			u8 row = (GPIO_ReadInputData(GPIOA)>>4)&0x0f;
			if(row){
				if((row >> 0)&0x01)row = 1;
				else if((row >> 1)&0x01)row = 2;
				else if((row >> 2)&0x01)row = 3;
				else if((row >> 3)&0x01)row = 4;
//				printf("ÐÐ£º%d, ÁÐ:%d, result:%d\r\n", row, 4-i, 4*row-i);
				while((GPIO_ReadInputData(GPIOA)>>4)&0xf);
				return (4*row-i);
			}
		}
	}
	return 0;
}

