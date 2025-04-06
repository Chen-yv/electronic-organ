#ifndef _LIGHTSEN_H_
#define _LIGHTSEN_H_

#include <STM32F10x.h>

#define LIGHT_SEN_Port GPIOB
#define LIGHT_SEN_Pin GPIO_Pin_9


void light_sensor_init(void);
void light_sensor_EXIT_init(void);
u8 light_sensor_get_value(void);


#endif
