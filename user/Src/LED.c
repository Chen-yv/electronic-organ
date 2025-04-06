#include "LED.h"

void rgb_led_init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Pin = RGB_R_Pin;
	GPIO_Init(RGB_R_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = RGB_G_Pin;
	GPIO_Init(RGB_G_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = RGB_B_Pin;
	GPIO_Init(RGB_B_Port, &GPIO_InitStruct);
}
void rgb_set_color(u8 color, u8 bit){
	if(color == RGB_COL_R || color == RGB_COL_G || color == RGB_COL_B)
		switch(color){
			case RGB_COL_R: 
				bit >= 1 ? SET(RGB_R_Port, RGB_R_Pin): RESET(RGB_R_Port, RGB_R_Pin);
			break;
			case RGB_COL_G: 
				bit >= 1 ? SET(RGB_G_Port, RGB_G_Pin): RESET(RGB_G_Port, RGB_G_Pin);
			break;
			case RGB_COL_B: 
				bit >= 1 ? SET(RGB_B_Port, RGB_B_Pin): RESET(RGB_B_Port, RGB_B_Pin);
			break;
		}
}

void rgb_reversal(u8 state){
	state %= 8;
	GRB_reversal(state<<12);
}
void rgb_reset_color(u8 state){
	if(state == 1){
		SET(RGB_R_Port, RGB_R_Pin);
		RESET(RGB_G_Port, RGB_G_Pin);
		RESET(RGB_B_Port, RGB_B_Pin);
	}else if(state == 2){
		RESET(RGB_R_Port, RGB_R_Pin);
		SET(RGB_G_Port, RGB_G_Pin);
		RESET(RGB_B_Port, RGB_B_Pin);
	}else if(state == 3){
		RESET(RGB_R_Port, RGB_R_Pin);
		RESET(RGB_G_Port, RGB_G_Pin);
		SET(RGB_B_Port, RGB_B_Pin);
	}
}
