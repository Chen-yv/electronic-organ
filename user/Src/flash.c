#include "flash.h"

void flash_init(void){
	SPI_Init();
}


void flash_addr_set_page(Addr * addr, u8 value){
	value &= 0x0F;
	addr->sec_page &= 0xF0;
	addr->sec_page |= value;
}


void flash_addr_set_section(Addr *addr, u8 value){
	
	// 将value高4位置0，防止影响块地址
	value <<= 4;
	value &= 0xF0;
	// 将扇区地址清零
	addr->sec_page &= 0x0F;
	// 将扇区地址设置为value的值
	addr->sec_page |= value;
}

void flash_erasure_section(Addr addr, u8 op){
	// 需要内存对齐，所以需要指向扇区第一个字节
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	
	unsigned char temp[10] = {0};
	//发送写启用指令
	temp[0] = 0x06;
	SPI_Read_Write(temp, 1, NULL, 0);
	
	//发送擦除指令
	temp[0] = op;  
	memcpy(temp+1, (u8*)&addr, 3);
	SPI_Read_Write(temp, 4, NULL, 0);
	
	unsigned char statu = 0;
	//等待擦除完成
	temp[0] = 0x05;
	do
	{
		statu = 0;
		SPI_Read_Write(temp, 1, &statu, 1);
	}while((statu&0x01) == 1);
}



void flash_writeData(Addr addr, void *data, int len){
	
	unsigned char temp[260] = {0};
	//发送写启用指令
	temp[0] = 0x06;
	SPI_Read_Write(temp, 1, NULL, 0);
	
	//写入数据
	temp[0] = 0x02;
	memcpy(temp+1, (u8*)&addr, 3);
	memcpy(temp+4, (u8*)data, len);
	SPI_Read_Write(temp, len+4, NULL, 0);
	unsigned char statu = 0;
	//等待写入完成
	temp[0] = 0x05;
	do
	{
		statu = 0;
		SPI_Read_Write(temp, 1, &statu, 1);
	}while((statu&0x01) == 1);
}




void flash_ReadData(Addr addr, void *data, int len){
	unsigned char temp[260] = {0};
	//发送读取指令
	temp[0] = 0x03;
	memcpy(temp+1, (u8*)&addr, 3);
	SPI_Read_Write(temp, 4, (u8*)data, len);
}

