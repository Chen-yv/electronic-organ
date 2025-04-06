#include "timer.h"


// TIM可以是2、3、4，倒计时ms毫秒触发中断
void time_init(TIM_TypeDef *TIM, u16 ms){
	TIM_TimeBaseInitTypeDef TIM_InitStruc;
	NVIC_InitTypeDef NVIC_InitStuc;
	
	TIM_InitStruc.TIM_Prescaler = 36000-1;
	TIM_InitStruc.TIM_Period =  (ms * 2)-1;
	TIM_InitStruc.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruc.TIM_ClockDivision = TIM_CKD_DIV1;
	
	NVIC_InitStuc.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStuc.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStuc.NVIC_IRQChannelSubPriority = 2;
		
	
	if(TIM == TIM2){//TIM2_IRQHandler
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		NVIC_InitStuc.NVIC_IRQChannel = TIM2_IRQn;
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}else if(TIM == TIM3){//TIM3_IRQHandler
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		NVIC_InitStuc.NVIC_IRQChannel = TIM3_IRQn;
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
	}else if(TIM == TIM4){//TIM4_IRQHandler
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		NVIC_InitStuc.NVIC_IRQChannel = TIM4_IRQn;
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM4, ENABLE);
	}
	
	TIM_TimeBaseInit(TIM, &TIM_InitStruc);
	NVIC_Init(&NVIC_InitStuc);
	
}

void counter_time_init(){
	TIM_TimeBaseInitTypeDef TIM_InitStruc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_InitStruc.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruc.TIM_Period = 7200;
	TIM_InitStruc.TIM_Prescaler = 10000;
	TIM_InitStruc.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM1, &TIM_InitStruc);
	TIM_Cmd(TIM1, ENABLE);
}

void empty_counter(){
	TIM_SetCounter(TIM1, 0);
}

u16 get_counter(){
	return (u16)(TIM_GetCounter(TIM1) / 100);
}
