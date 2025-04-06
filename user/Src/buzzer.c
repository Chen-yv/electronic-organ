#include "buzzer.h"


void buzzer_init(void){
	TIM_TimeBaseInitTypeDef TIM_InitStruc;
	GPIO_InitTypeDef GPIO_InitStruc;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_1;
	
	GPIO_Init(GPIOB, &GPIO_InitStruc);
	

	TIM_InitStruc.TIM_Prescaler = 72-1;
	TIM_InitStruc.TIM_Period =  1;
	TIM_InitStruc.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruc.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3, &TIM_InitStruc);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	
	TIM_OC4Init(TIM3, &TIM_OCInitStruct);
	TIM_OC3Init(TIM3, &TIM_OCInitStruct);
	
	TIM_Cmd(TIM3, ENABLE);
}
void buzzer_tone(u16 music){
	rgb_reversal(music);
	if(music < 8){
		buzzer_change(Config.hightTones[music-1] + Config.melody);
	}
	Delay_ms(30);
	buzzer_change(0);
}
void buzzer_change(u16 circumflex){
	if (circumflex == 0){
		TIM_SetAutoreload(TIM3,1);
		TIM_SetCompare4(TIM3,0);
		TIM_SetCompare3(TIM3,0);
	}else {
		TIM_SetAutoreload(TIM3,(1000000/circumflex));
		TIM_SetCompare4(TIM3,(u16)((1000000/circumflex)/(u16)Config.musicSound));
		TIM_SetCompare3(TIM3,(u16)((1000000/circumflex)/(u16)Config.musicSound));
	}
}
