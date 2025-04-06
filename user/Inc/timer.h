#ifndef _MYTIMER_H_
#define _MYTIMER_H_

#include <STM32F10x.h>

void time_init(TIM_TypeDef *TIM, u16 ms);
void counter_time_init(void);
void empty_counter(void);
u16 get_counter(void);

#endif
