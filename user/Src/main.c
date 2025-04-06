#include <STM32F10x.h>

#include "OLED.h"
#include "Delay.h"
#include "matrixKey.h"
#include "printf.h"
#include "buzzer.h"
#include "flash.h"
#include "timer.h"
#include "music.h"
#include "LED.h"

u8 selectMusic = 0;

void Refrash_screen(){
	OLED_SetScorll(0);
	OLED_ShowString(3, 20, (u8*)"->", 2);

	if (selectMusic==0)OLED_ShowString(2, 32, (u8*)"            ", 12);
	else OLED_ShowString(2, 32, songList[selectMusic-1].name, strlen((char*)songList[selectMusic-1].name));
	OLED_ShowString(3, 32, (u8*)"            ", 12);
	OLED_ShowString(3, 32, songList[selectMusic].name, strlen((char*)songList[selectMusic].name));
	if(selectMusic==musicTracks-1)OLED_ShowString(4, 32, (u8*)"            ", 12);
	else OLED_ShowString(4, 32, songList[selectMusic+1].name, strlen((char*)songList[selectMusic+1].name));

	OLED_ShowString(7,0,(u8*)"voice:", sizeof("voice:")-1);
	OLED_ShowNum(7, sizeof("voice:")*6-3, music_get_voice());
	OLED_ShowString(7,64,(u8*)"melody:", sizeof("melody:")-1);
	OLED_ShowNum(7, 64+sizeof("melody:")*6-3, music_get_melody());
	OLED_ShowString(0,0,(u8*)"music list:", sizeof("music list:")-1);
	OLED_ShowNum(0, (sizeof("music list:")+1)*6, (int)selectMusic+1);
	OLED_ShowString(0, (sizeof("music list:")+3)*6, (u8*)"/", 1);
	OLED_ShowNum(0, (sizeof("music list:")+4)*6, (int)musicTracks);
	OLED_SetScorll(1);
}

void key_deal(u8 key){

	Song s1;
	switch(key){
		
		case 8:	// 录歌
			TIM_Cmd(TIM4, DISABLE);
			OLED_SetScorll(0);
			if(music_record(&s1) == 1){
				music_play(&s1);
				music_save(&s1);
				music_print(&s1);
			}
			OLED_SetScorll(1);
			TIM_Cmd(TIM4, ENABLE);
			break;
		case 9: 	music_add_voice();break;//加声音
		case 13:	music_sub_voice();break;//减声音
		case 10:	music_add_melody();break;//高音
		case 14:	music_sub_melody();break;//低音
		case 11:	selectMusic = (selectMusic-1+musicTracks)%musicTracks; break;/* up */
		case 15:	selectMusic = (selectMusic+1) % musicTracks; break;/* down */ 
		case 12:	music_read_song(songList[selectMusic].addr, &s1); music_play(&s1); break;/* CONFIRM */
		case 16:	break;/* CONFIRM */
		default:	// 练习
			buzzer_tone(key); break;
	}
}

char infos[10][20]={
{"1-7 to practice"},
{"8 to record"},
{"9 to up voice"},
{"13 to down voice"},
{"10 to up melody"},
{"14 to down melody"},
{"11 to up"},
{"15 to down"},
{"12 to confirm"},
{"16 to cancel"}
};
u8 tips;
void TIM4_IRQHandler(){
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET){
		OLED_SetScorll(0);
		oled_info(infos[tips]);
		Refrash_screen();
		tips = (tips+1) % 10;
		OLED_SetScorll(1);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void music_init_act(){
	while(music_init() == 0){
		printf("music init error");
		admin_music_config_init();
	}
}

int main(){
	
	printf_init(9600);

	OLED_Init();
	OLED_Clear();
	rgb_led_init();

	buzzer_init();
	key_board_init();
	flash_init();
	counter_time_init();
//	OLED_SetScorll();
	music_init_act();
	
	time_init(TIM4, 5000);
	


	
	
	Refrash_screen();

	u8 resetState = 0;
//	u8 i = 1;
	while(1){
		/* 
		1、按1234567练习模式
		2、11上，15下， 14左，16右， 10取消，12确认
		3、8录歌，9音量调节，13音色调节
		*/ 
		u8 key = key_scan();
		if (key == 0)continue;
		key_deal(key);
		
		if(resetState >= 10 && key == CANSCEL){
			admin_music_config_init();
			music_init_act();
		}
		if(key == 1)resetState ++;
		else resetState = 0;
		
		Refrash_screen();

	}
}
