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
注意：
	1：第一个扇区存储用户参数，第二个扇区以及后续扇区存储内置歌曲，第二块存储用户录入的歌曲
	2：第一块第一个扇区中，第一个字节是音量，第二、三个字节是音调偏移量， 第4个字节是内置歌曲数量， 第5个字节是用户歌曲数量，后续7*2个字节是音调基准
*/

typedef struct{
	u8 delay;// 播放这个音调前的延时，单位：(10ms)
	u8 tone;
//	u8 timbre;	// B、C调，1为
}Tones;

typedef struct{
	u8 name[MUSIC_NAME_LEN];
	u8 state;			// 0为已经删除，1为未删除
	u8 len;
	Tones tones[MUSIC_LEN];
}Song;

typedef struct{
	u8 name[MUSIC_NAME_LEN];
	Addr addr;
}SongList;

typedef struct{
	u8 musicSound;			// 音量
	s16 melody;			// 基调音色偏移
	u8 buildInTracks;	// 内置歌曲数量， 内置最多16首
	u8 userTracks;		// 用户歌曲数量， 用户最多16首歌
	u8 state;					// 固件刷写状态		// 1为已经被刷写，否则未被刷写
	u16 hightTones[7];		// 音乐基本调
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
void admin_music_config_init(void);// 恢复出厂设置
void music_add_melody(void);
void music_sub_melody(void);
s16 music_get_melody(void);
void admin_music_config_init(void);

void music_set_basic_tone(u8 tone, u8 op);

void music_play(Song *song);
void music_read_song(Addr addr, Song *song);
u8 music_record(Song *song);

// 暂时不实现，有空再写
// void music_delet(void);

#endif

