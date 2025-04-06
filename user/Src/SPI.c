#include "SPI.h"

void SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_CS | SPI_SCLK | SPI_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);
	
	//模式1的CS和SCLK空闲状态电平
	GPIO_WriteBit(SPI_PORT, SPI_CS, Bit_SET);
	GPIO_WriteBit(SPI_PORT, SPI_SCLK, Bit_RESET);
}

void CS_OUT(int x)
{
	if(x == 1)
		GPIO_WriteBit(SPI_PORT, SPI_CS, Bit_SET);
	else
		GPIO_WriteBit(SPI_PORT, SPI_CS, Bit_RESET);
}

void SCLK_OUT(int x)
{
	if(x == 1)
		GPIO_WriteBit(SPI_PORT, SPI_SCLK, Bit_SET);
	else
		GPIO_WriteBit(SPI_PORT, SPI_SCLK, Bit_RESET);
}


void MOSI_OUT(int x)
{
	if(x == 1)
		GPIO_WriteBit(SPI_PORT, SPI_MOSI, Bit_SET);
	else
		GPIO_WriteBit(SPI_PORT, SPI_MOSI, Bit_RESET);
}

int MISO_IN(void)
{
	return GPIO_ReadInputDataBit(SPI_PORT, SPI_MISO);
}

//模式1
void SPI_Read_Write(unsigned char w_data[], int w_len, unsigned char r_data[], int r_len)
{
	//选中设备，CS拉低
	CS_OUT(0);
	//发送数据 --- 移出
	for(int i = 0;i < w_len; i++)
	{
		for(int j = 0;j < 8; j++)
		{
			SCLK_OUT(0);
			if((w_data[i]&(0x1 << (7-j))) == 0)
				MOSI_OUT(0);
			else
				MOSI_OUT(1);
			SCLK_OUT(1);
		}
	}
	
	//接收数据 --- 采样
	for(int i = 0; i < r_len; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			SCLK_OUT(1);
			SCLK_OUT(0);
			if(MISO_IN() == 1)
				r_data[i] |= 1 << (7-j);
			else
				r_data[i] &= ~(0x1 << (7-j));
		}
	}
	
	//取消选中
	CS_OUT(1);
}


