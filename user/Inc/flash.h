#ifndef __FLASH_H__
#define __FLASH_H__

#include "spi.h"
#include <stdio.h>
#include <string.h>

#define EAR_SEC 			0x20 		// ��������
#define EAR_32BLOCK 	0x52		// 32KB�����	
#define EAR_64BLOCK		0xD8		// 64KB�����		һ��������
#define EAR_ALL				0xC7		// ��Ƭ����

typedef struct{
	u8 block;			// 64��
	u8 sec_page;	// 16����*16ҳ, ����������ʹ�ú�������
	u8 offset;		// 256kb
}Addr;

void flash_addr_set_page(Addr * addr, u8 value);// ����ҳ��ַ

void flash_addr_set_section(Addr *addr, u8 value);	// ����������ַ

void flash_erasure_section(Addr addr, u8 op);		// ������ַ���ڵ�����

void flash_init(void);

void flash_writeData(Addr addr, void *data, int len);// ע�⣬û�в���

void flash_ReadData(Addr addr, void *data, int len);

#endif
