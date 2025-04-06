#include "oled.h"
#include "oledfont.h"

//��������
void OLED_SendData(unsigned char data[], int len)
{
	unsigned char buf[128] = {0};
	//����OLED�ĵ�ַ
	unsigned char addr = 0x78;
	//���÷��Ϳ����ֽ�Ϊ����
	buf[0] = 0x40;
	//��������װ��buf��
	memcpy(buf+1, data, len);
	//���͵�OLED�豸
	IIC_write_data(addr, buf, len+1);
}
//��������
void OLED_SendCmd(unsigned char cmd[], int len)
{
	unsigned char buf[8] = {0};
	//����OLED�ĵ�ַ
	unsigned char addr = 0x78;
	//���÷��Ϳ����ֽ�Ϊ����
	buf[0] = 0x00;
	//��������װ��buf��
	memcpy(buf+1, cmd, len);
	//���͵�OLED�豸
	IIC_write_data(addr, buf, len+1);
}

void OLED_Init(void)
{
	unsigned char cmd[8] = {0};
	IIC_init();
	
	//���͹ر���ʾָ��
	cmd[0] = 0xAE;
	OLED_SendCmd(cmd, 1);
	
	//������ɱ�
	cmd[0] = 0x8D;
	cmd[1] = 0x94;
	OLED_SendCmd(cmd, 2);
	
//	//ˮƽ����ת
//	cmd[0] = 0xA1;
//	OLED_SendCmd(cmd, 1);
//	//��ֱ����ת
//	cmd[0] = 0xC8;
//	OLED_SendCmd(cmd, 1);
	
	//������ʾ
	cmd[0] = 0xAF;
	OLED_SendCmd(cmd, 1);
}

void OLED_Clear(void)
{
	unsigned char data = 0x00;
	unsigned char cmd[8] = {0};
	for(int i = 0; i < 8; i++)
	{
		//������ʾ����ʼ��
		cmd[0] = 0xB0+i;
		OLED_SendCmd(cmd, 1);
		for(int j = 0; j < 128; j++)
		{
			//������ʼ��
			cmd[0] = 0x00 + (j & 0x0F);
			cmd[1] = 0x10 + (j >> 4);
			OLED_SendCmd(cmd, 2);
			//������ʾ��ÿһ������
			OLED_SendData(&data, 1);
		}
	}
}

void OLED_ShowString(int x, int y, unsigned char str[], int len)
{
	unsigned char data = 0x00;
	unsigned char cmd[8] = {0};
	
	//������ʾ����ʼ��
	cmd[0] = 0xB0+x;
	OLED_SendCmd(cmd, 1);
	
	//������ʼ��
	cmd[0] = 0x00 + (y & 0x0F);
	cmd[1] = 0x10 + (y >> 4);
	OLED_SendCmd(cmd, 2);
	
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			//������ʾ��ÿһ������
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
	
	//������ʾ����ʼ��
	cmd[0] = 0xB0+x;
	OLED_SendCmd(cmd, 1);
	
	//������ʼ��
	cmd[0] = 0x00 + (y & 0x0F);
	cmd[1] = 0x10 + (y >> 4);
	OLED_SendCmd(cmd, 2);
	
	for(int i = 0; i < 16; i++)
	{
		//������ʾ��ÿһ������
		data = Hzk[num*2][i];
		OLED_SendData(&data, 1);
	}
	
	//������ʾ����ʼ��
	cmd[0] = 0xB0+x+1;
	OLED_SendCmd(cmd, 1);
	
	//������ʼ��
	cmd[0] = 0x00 + (y & 0x0F);
	cmd[1] = 0x10 + (y >> 4);
	OLED_SendCmd(cmd, 2);
	
	for(int i = 0; i < 16; i++)
	{
		//������ʾ��ÿһ������
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
	
	//ֹͣ����
	cmd[0] = 0x2E;
	OLED_SendCmd(cmd, 1);
	if (set == 0)return;
	//���ù���ģʽ
	cmd[0] = 0x27;     // ˮƽ���ҹ���
	cmd[1] = 0x00;     // dummy
	cmd[2] = 0x06;     // ��ʼҳ��ַ = 6
	cmd[3] = 0x05;     // �����ٶȣ�֡�����
	cmd[4] = 0x06;     // ����ҳ��ַ = 6��ͬһҳ��
	cmd[5] = 0x00;     // dummy
	cmd[6] = 0x7F;     // �з�Χ 0~127
	OLED_SendCmd(cmd, 7);
	
	//��������
	cmd[0] = 0x2F;
	OLED_SendCmd(cmd, 1);
}

