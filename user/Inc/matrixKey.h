#ifndef _MATRIXKEY_H_
#define _MATRIXKEY_H_

#include <stm32f10x.h>

#define LEFT 
#define RIGHT
#define UP 11
#define DOWN 15
#define CONFIRM 12
#define CANSCEL 16


void key_board_init(void);
u8 key_scan(void);

#endif
