#ifndef _LED_H_
#define _LED_H_

#include <stm32f10x.h>

#define RGB_R_Pin GPIO_Pin_12
#define RGB_R_Port GPIOB
#define RGB_G_Pin GPIO_Pin_13
#define RGB_G_Port GPIOB
#define RGB_B_Pin GPIO_Pin_14
#define RGB_B_Port GPIOB

#define RGB_COL_R 0
#define RGB_COL_G 1
#define RGB_COL_B 2

#define GRB_reversal(x) GPIOB->ODR ^= x 

#define SET(port, pin) GPIO_WriteBit(port, pin, Bit_SET)
#define RESET(port, pin) GPIO_WriteBit(port, pin, Bit_RESET)

void rgb_reversal(u8 state);
void rgb_led_init(void);
void rgb_set_color(u8 color, u8 bit);
void rgb_reset_color(u8 state);


#endif

