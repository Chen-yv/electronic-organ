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
	
	// ��value��4λ��0����ֹӰ����ַ
	value <<= 4;
	value &= 0xF0;
	// ��������ַ����
	addr->sec_page &= 0x0F;
	// ��������ַ����Ϊvalue��ֵ
	addr->sec_page |= value;
}

void flash_erasure_section(Addr addr, u8 op){
	// ��Ҫ�ڴ���룬������Ҫָ��������һ���ֽ�
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	
	unsigned char temp[10] = {0};
	//����д����ָ��
	temp[0] = 0x06;
	SPI_Read_Write(temp, 1, NULL, 0);
	
	//���Ͳ���ָ��
	temp[0] = op;  
	memcpy(temp+1, (u8*)&addr, 3);
	SPI_Read_Write(temp, 4, NULL, 0);
	
	unsigned char statu = 0;
	//�ȴ��������
	temp[0] = 0x05;
	do
	{
		statu = 0;
		SPI_Read_Write(temp, 1, &statu, 1);
	}while((statu&0x01) == 1);
}



void flash_writeData(Addr addr, void *data, int len){
	
	unsigned char temp[260] = {0};
	//����д����ָ��
	temp[0] = 0x06;
	SPI_Read_Write(temp, 1, NULL, 0);
	
	//д������
	temp[0] = 0x02;
	memcpy(temp+1, (u8*)&addr, 3);
	memcpy(temp+4, (u8*)data, len);
	SPI_Read_Write(temp, len+4, NULL, 0);
	unsigned char statu = 0;
	//�ȴ�д�����
	temp[0] = 0x05;
	do
	{
		statu = 0;
		SPI_Read_Write(temp, 1, &statu, 1);
	}while((statu&0x01) == 1);
}




void flash_ReadData(Addr addr, void *data, int len){
	unsigned char temp[260] = {0};
	//���Ͷ�ȡָ��
	temp[0] = 0x03;
	memcpy(temp+1, (u8*)&addr, 3);
	SPI_Read_Write(temp, 4, (u8*)data, len);
}

