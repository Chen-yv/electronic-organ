#include "IIC.h"

#include "printf.h"

void IIC_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	SDA_SET_OUT;
	SCL_SET_OUT;
	SDA_SET;
	SCL_SET;
}

void IIC_start(void){
	SDA_SET;
	SCL_SET;
	SDA_RESET;
}
void IIC_end(void){
	SDA_RESET;
	SCL_SET;
	SDA_SET;
}

u8 IIC_sendByte(u8 Byte){
	SCL_RESET;

	for(int i = 0; i < 8; i ++){
		if(Byte & (0x01<<(7-i)))SDA_SET;
		else SDA_RESET;
		SCL_SET;
		SCL_RESET;
	}
//	printf("0x%x\r\n", Byte);
	SDA_RESET;
	SCL_SET;
	SCL_RESET;
	return 0;
}

u8 IIC_write_data(u8 addr, u8 data[], u8 len){
	
	IIC_start();
	IIC_sendByte(addr);
	for(u8 i = 0; i < len; i ++){
		IIC_sendByte(data[i]);
	}
	IIC_end();
	return 1;
}

