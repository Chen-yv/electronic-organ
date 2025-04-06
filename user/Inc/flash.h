#ifndef __FLASH_H__
#define __FLASH_H__

#include "spi.h"
#include <stdio.h>
#include <string.h>

#define EAR_SEC 			0x20 		// 扇区擦除
#define EAR_32BLOCK 	0x52		// 32KB块擦除	
#define EAR_64BLOCK		0xD8		// 64KB块擦除		一整个扇区
#define EAR_ALL				0xC7		// 整片擦除

typedef struct{
	u8 block;			// 64块
	u8 sec_page;	// 16扇区*16页, 这个参数最好使用函数配置
	u8 offset;		// 256kb
}Addr;

void flash_addr_set_page(Addr * addr, u8 value);// 配置页地址

void flash_addr_set_section(Addr *addr, u8 value);	// 配置扇区地址

void flash_erasure_section(Addr addr, u8 op);		// 擦除地址所在的扇区

void flash_init(void);

void flash_writeData(Addr addr, void *data, int len);// 注意，没有擦除

void flash_ReadData(Addr addr, void *data, int len);

#endif
