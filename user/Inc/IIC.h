#ifndef __IIC_H__
#define __IIC_H__

#include <STM32F10x.h>
#include "Delay.h"

#define IIC_PORT GPIOB
#define SDA_PIN 5
#define SCL_PIN 6			// 都使用小于8的pin脚

#define SDA_SET_OUT IIC_PORT->CRL &= ~(0x0F << (SDA_PIN * 4)); IIC_PORT->CRL |= (0x03 << (SDA_PIN * 4))  // 推挽输出50MHz
#define SDA_SET_IN IIC_PORT->CRL &= ~(0x0F << (SDA_PIN * 4)); IIC_PORT->CRL |= (0x04 << (SDA_PIN * 4))  // 浮空输入

#define SDA_SET IIC_PORT->ODR |= (0x01 << SDA_PIN)
#define SDA_RESET IIC_PORT->ODR &= ~(0x01 << SDA_PIN)
#define SDA_IN ((GPIOC->IDR & ((0x01 << SDA_PIN))) >> SDA_PIN)

#define SCL_SET_OUT IIC_PORT->CRL &= ~(0x0F << (SCL_PIN * 4)); IIC_PORT->CRL |= (0x03 << (SCL_PIN * 4))  // 推挽输出50MHz
#define SCL_SET_IN IIC_PORT->CRL &= ~(0x0F << (SCL_PIN * 4)); IIC_PORT->CRL |= (0x04 << (SCL_PIN * 4))  // 浮空

#define SCL_SET IIC_PORT->ODR |= (0x01 << SCL_PIN)
#define SCL_RESET IIC_PORT->ODR &= ~(0x01 << SCL_PIN)
#define SCL_IN ((GPIOC->IDR & ((0x01 << SCL_PIN))) >> SCL_PIN)

void IIC_init(void);
u8 IIC_write_data(u8 addr, u8 data[], u8 len);

#endif
