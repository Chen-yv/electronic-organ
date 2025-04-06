#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "STM32F10x.h"
#include "buzzer.h"
#include "flash.h" 
#include "OLED.h"
#include "string.h"
#include "matrixKey.h" 
#include "timer.h"


#define MUSIC_LEN 110
#define ALL_TRACKS 40
#define MUSIC_NAME_LEN 16

/*
ע�⣺
	1����һ�������洢�û��������ڶ��������Լ����������洢���ø������ڶ���洢�û�¼��ĸ���
	2����һ���һ�������У���һ���ֽ����������ڶ��������ֽ�������ƫ������ ��4���ֽ������ø��������� ��5���ֽ����û���������������7*2���ֽ���������׼
*/

typedef struct{
	u8 delay;// �����������ǰ����ʱ����λ��(10ms)
	u8 tone;
//	u8 timbre;	// B��C����1Ϊ
}Tones;

typedef struct{
	u8 name[MUSIC_NAME_LEN];
	u8 state;			// 0Ϊ�Ѿ�ɾ����1Ϊδɾ��
	u8 len;
	Tones tones[MUSIC_LEN];
}Song;

typedef struct{
	u8 name[MUSIC_NAME_LEN];
	Addr addr;
}SongList;

typedef struct{
	u8 musicSound;			// ����
	s16 melody;			// ������ɫƫ��
	u8 buildInTracks;	// ���ø��������� �������16��
	u8 userTracks;		// �û����������� �û����16�׸�
	u8 state;					// �̼�ˢд״̬		// 1Ϊ�Ѿ���ˢд������δ��ˢд
	u16 hightTones[7];		// ���ֻ�����
}MusicConfig;

extern SongList songList[ALL_TRACKS];
extern MusicConfig Config;
extern u8 musicTracks;


u8 music_init(void);
void music_print_config(void);
void music_print(Song *song);


void music_update_config(void);
void music_add_voice(void);
void music_sub_voice(void);
u8 music_get_voice(void);
void music_save(Song *song);
void admin_music_config_init(void);// �ָ���������
void music_add_melody(void);
void music_sub_melody(void);
s16 music_get_melody(void);
void admin_music_config_init(void);

void music_set_basic_tone(u8 tone, u8 op);

void music_play(Song *song);
void music_read_song(Addr addr, Song *song);
u8 music_record(Song *song);

// ��ʱ��ʵ�֣��п���д
// void music_delet(void);

#endif

