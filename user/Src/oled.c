#include "oled.h"
#include "oledfont.h"

//发送数据
void OLED_SendData(unsigned char data[], int len)
{
	unsigned char buf[128] = {0};
	//设置OLED的地址
	unsigned char addr = 0x78;
	//设置发送控制字节为数据
	buf[0] = 0x40;
	//把数据组装到buf中
	memcpy(buf+1, data, len);
	//发送到OLED设备
	IIC_write_data(addr, buf, len+1);
}
//发送命令
void OLED_SendCmd(unsigned char cmd[], int len)
{
	unsigned char buf[8] = {0};
	//设置OLED的地址
	unsigned char addr = 0x78;
	//设置发送控制字节为命令
	buf[0] = 0x00;
	//把命令组装到buf中
	memcpy(buf+1, cmd, len);
	//发送到OLED设备
	IIC_write_data(addr, buf, len+1);
}

void OLED_Init(void)
{
	unsigned char cmd[8] = {0};
	IIC_init();
	
	//发送关闭显示指令
	cmd[0] = 0xAE;
	OLED_SendCmd(cmd, 1);
	
	//开启电荷泵
	cmd[0] = 0x8D;
	cmd[1] = 0x94;
	OLED_SendCmd(cmd, 2);
	
//	//水平方向翻转
//	cmd[0] = 0xA1;
//	OLED_SendCmd(cmd, 1);
//	//垂直方向翻转
//	cmd[0] = 0xC8;
//	OLED_SendCmd(cmd, 1);
	
	//开启显示
	cmd[0] = 0xAF;
	OLED_SendCmd(cmd, 1);
}

void OLED_Clear(void)
{
	unsigned char data = 0x00;
	unsigned char cmd[8] = {0};
	for(int i = 0; i < 8; i++)
	{
		//设置显示的起始行
		cmd[0] = 0xB0+i;
		OLED_SendCmd(cmd, 1);
		for(int j = 0; j < 128; j++)
		{
			//设置起始列
			cmd[0] = 0x00 + (j & 0x0F);
			cmd[1] = 0x10 + (j >> 4);
			OLED_SendCmd(cmd, 2);
			//设置显示的每一个数据
			OLED_SendData(&data, 1);
		}
	}
}

void OLED_ShowString(int x, int y, unsigned char str[], int len)
{
	unsigned char data = 0x00;
	unsigned char cmd[8] = {0};
	
	//设置显示的起始行
	cmd[0] = 0xB0+x;
	OLED_SendCmd(cmd, 1);
	
	//设置起始列
	cmd[0] = 0x00 + (y & 0x0F);
	cmd[1] = 0x10 + (y >> 4);
	OLED_SendCmd(cmd, 2);
	
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			//设置显示的每一个数据
			data = F6x8[*(str+i)-32][j];
			OLED_SendData(&data, 1);
		}
	}
}

void OLED_ShowNum(int x, int y, int num){
	if(num == 0){
		OLED_ShowString(x, y, (u8*)"0   ", 5);
		return;
	}
	u8 temp[10] = {0}, count = 9, sign = 0;
	if(num<0){
		sign = 1;
		num *= -1;
	}
	while(num){
		temp[count] = num % 10 + '0';
		count --;
		num /= 10;
	}
	OLED_ShowString(x, y, (u8*)"     ", 5);
	if(sign == 1){OLED_ShowString(x, y, (u8*)"-", 1);y+=6;}
	OLED_ShowString(x, y, temp+count+1, 9-count);
	
}

void OLED_Show_Chinese(int x, int y, int num)
{
	unsigned char data = 0x00;
	unsigned char cmd[8] = {0};
	
	//设置显示的起始行
	cmd[0] = 0xB0+x;
	OLED_SendCmd(cmd, 1);
	
	//设置起始列
	cmd[0] = 0x00 + (y & 0x0F);
	cmd[1] = 0x10 + (y >> 4);
	OLED_SendCmd(cmd, 2);
	
	for(int i = 0; i < 16; i++)
	{
		//设置显示的每一个数据
		data = Hzk[num*2][i];
		OLED_SendData(&data, 1);
	}
	
	//设置显示的起始行
	cmd[0] = 0xB0+x+1;
	OLED_SendCmd(cmd, 1);
	
	//设置起始列
	cmd[0] = 0x00 + (y & 0x0F);
	cmd[1] = 0x10 + (y >> 4);
	OLED_SendCmd(cmd, 2);
	
	for(int i = 0; i < 16; i++)
	{
		//设置显示的每一个数据
		data = Hzk[num*2+1][i];
		OLED_SendData(&data, 1);
	}
}

void oled_info(char data[]){
	OLED_ShowString(6, 0, (u8*)"                         ", 25);
	OLED_ShowString(6, 0, (u8*)data, strlen(data));

}


void OLED_SetScorll(u8 set)
{
	unsigned char cmd[8] = {0};
	
	//停止滚动
	cmd[0] = 0x2E;
	OLED_SendCmd(cmd, 1);
	if (set == 0)return;
	//设置滚动模式
	cmd[0] = 0x27;     // 水平向右滚动
	cmd[1] = 0x00;     // dummy
	cmd[2] = 0x06;     // 起始页地址 = 6
	cmd[3] = 0x05;     // 滚动速度（帧间隔）
	cmd[4] = 0x06;     // 结束页地址 = 6（同一页）
	cmd[5] = 0x00;     // dummy
	cmd[6] = 0x7F;     // 列范围 0~127
	OLED_SendCmd(cmd, 7);
	
	//启动滚动
	cmd[0] = 0x2F;
	OLED_SendCmd(cmd, 1);
}

