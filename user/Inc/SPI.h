#ifndef __SPI_H__
#define __SPI_H__

#include <STM32F10x.h>

#define SPI_PORT 		GPIOB 
#define SPI_CS 			GPIO_Pin_7
#define SPI_SCLK 		GPIO_Pin_8
#define SPI_MOSI 		GPIO_Pin_9
#define SPI_MISO 		GPIO_Pin_10

void SPI_Init(void);

void SPI_Read_Write(unsigned char w_data[], int w_len, unsigned char r_data[], int r_len);

#endif
