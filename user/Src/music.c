#include "music.h"

#include "stdio.h"

MusicConfig Config;
SongList songList[ALL_TRACKS] = {0};	// 歌曲列表 包含歌曲名和内存映射
u8 musicTracks = 0;				// 列表中读取到的歌曲数量
#define firmware_write	0		// 固件刷写， 1为恢复出厂设置


/*
注意：
	1：第一个块存储用户参数，第二个块存储内置歌曲，第三个块存用户录入的歌曲
	2：第一块第一个扇区中，第一个字节是音量(偏移量0)，第二、三个字节是音调偏移量， 第4个字节是内置歌曲数量， 
					第5个字节是用户歌曲数量，第6个字节是第一次启动标志位，最低位为1为第一次启动，后续7*2个字节是音调基准
	3：每一个扇区存一首歌，一块最多存16首
	4：每首歌的第一个字节存的是这首歌的长度
*/
void music_read_config(void){
	Addr addr;
	addr.block = 0;
	addr.offset = 0;
	addr.sec_page = 0;
	flash_ReadData(addr, &Config, sizeof(Config));
}
// 恢复出厂设置
void admin_music_config_init(){
	// 初始化配置
	Config.buildInTracks = 2;
	u16 _tones[7] = {261, 293, 329, 349, 392, 440, 493};
	memcpy(Config.hightTones, _tones, 7*2);
	Config.melody = 50;
	Config.musicSound = 10;
	Config.userTracks = 0;
	Config.state = 1;
	musicTracks = 0;
	
	printf("--------------------\r\nRestore start...\r\n");
	Addr addr;
	addr.block = 0;
	flash_addr_set_section(&addr, 0);
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	
	// 擦除相关块
	for(u8 i = 0; i < 4; i ++){
		addr.block = i;
		flash_erasure_section(addr, EAR_64BLOCK);
	}
	printf("erase firmware successful!\r\n");
	
	addr.block = 0;
	flash_addr_set_section(&addr, 0);
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	
	
	// 写入配置文件
	printf("restore settings...\r\n");
	music_update_config();
	printf("restore settings successful!\r\n");
	
	
	printf("Restore built-in songs... ");
	addr.block = 1;
	flash_addr_set_section(&addr, 0);
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	// 写入第一首内置歌曲
	Song song1;
	memcpy(song1.name, "two tigger", 10);
	song1.state = 1;
	song1.len = 32;
	u8 melody[MUSIC_LEN] = {
		50, 1, 50, 2, 50, 3, 50, 1, 
		50, 1, 50, 2, 50, 3, 50, 1, 
		50, 4, 50, 5, 50, 6, 60, 4, 50, 5, 50, 6, 
		60, 5, 20, 6, 20, 5, 30, 4, 40, 3, 50, 1, 
		60, 5, 20, 6, 20, 5, 30, 4, 40, 3, 50, 1, 
		60, 3, 50, 2, 50, 1, 80, 3, 50, 2, 50, 1 
	};
	memcpy(song1.tones, melody, MUSIC_LEN);
	flash_writeData(addr, &song1, sizeof(Song));
	
	printf("1 ");
	
	
		// 写入第一首内置歌曲
	memcpy(song1.name, "happy birth", MUSIC_LEN);
	song1.state = 1;
	song1.len = 12;
	flash_addr_set_section(&addr, 1);
	u8 melody1[MUSIC_LEN] = {56, 5, 17, 5, 25, 6, 42, 5, 59, 7, 35, 6, 70, 5, 14, 5, 22, 6, 39, 5, 36, 7, 38, 6};
	memcpy(song1.tones, melody1, MUSIC_LEN);
	flash_writeData(addr, &song1, sizeof(Song));
	
	printf("2 ");
	
	// 内置歌曲恢复完毕
	printf("\r\n");
	// 初始化结束
	printf("Restore finished!\r\n--------------------\r\n");
	
}

u8 music_init(void){
	if(firmware_write == 1)
		admin_music_config_init();
	// 启动初始化
	printf("music start..\r\n");
	Addr addr;
	addr.block = 0;
	flash_addr_set_section(&addr, 0);
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	
	printf("reading music config...\r\n");
	flash_ReadData(addr, &Config, sizeof(MusicConfig));
	
	if(Config.state != 1){
		printf("firmware data error, please redo\r\n");
		return 0;
	}
	music_print_config();
	printf("reading music config successful!\r\n");
	
	printf("reading music list...\r\n");
	Song song1;
	addr.block = 1;
	flash_addr_set_section(&addr, 0);
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	// 读取内置歌曲信息, 最多15首
	for(u8 i = 0; i < Config.buildInTracks; i ++){
		flash_addr_set_section(&addr, i);
		flash_ReadData(addr, &song1, sizeof(Song));
		memcpy(songList[musicTracks].name, song1.name, strlen((char*)song1.name));
		memcpy(&songList[musicTracks].addr, &addr, sizeof(Addr)); 
		musicTracks++;
	}
	
	addr.block = 2;
	flash_addr_set_section(&addr, 0);
	flash_addr_set_page(&addr, 0);
	addr.offset = 0;
	// 读取用户歌曲信息， 最多16首
	for(u8 i = 0; i < Config.userTracks; i ++){
		flash_addr_set_section(&addr, i);
		flash_ReadData(addr, &song1, sizeof(Song));
		memcpy(songList[musicTracks].name, song1.name, strlen((char*)song1.name));
		memcpy(&songList[musicTracks].addr, &addr, sizeof(Addr)); 
		musicTracks++;
	}
	printf("reading music list successful!, total: %d\r\n", musicTracks);
	// 打印读取到的歌曲
	for(u8 i = 0; i < musicTracks; i ++){
		Song s1;
		flash_ReadData(songList[i].addr, &s1, sizeof(Song));
		printf("loc: block=0x%2x, sec==0x%2x, page==0x%2x, offect==0x%2x\r\n", songList[i].addr.block, songList[i].addr.sec_page>>4, songList[i].addr.sec_page&0x0F, songList[i].addr.offset);
		printf("music%d--", i);
		music_print(&s1);
		printf("\r\n");
	}
	return 1;
}

void music_print_config(){
	music_read_config();
	printf("sound=%d, melody=%d, buildIntracks=%d, userTracks=%d, state=%d\r\n", Config.musicSound, Config.melody, Config.buildInTracks, Config.userTracks, Config.state);
	printf("base tones: ");
	for(u8 i = 0; i < 7; i ++)printf("%d ", Config.hightTones[i]);
	printf("\r\n");

}

void music_print(Song *song){
	printf("name:%s, state:%d\r\ntones: ", (char*)song->name, song->state);
	for(u8 i = 0; i < song->len; i ++){
		printf("%d, %d ", song->tones[i].delay, song->tones[i].tone);
	}
	printf("\r\n");
}

void music_update_config(void){
	Addr addr;
	addr.block = 0;
	addr.offset = 0;
	addr.sec_page = 0;
	flash_erasure_section(addr, EAR_32BLOCK);
	flash_writeData(addr, &Config, sizeof(MusicConfig));
	
	music_print_config();
}

void music_set_basic_tone(u8 tone, u8 op){
	
	music_update_config();
}
void music_save(Song *song){
	if (Config.userTracks == 16){
		printf("save error, music max 16\r\n");
		return;
	}
	oled_info("saving...");
	Addr addr;
	addr.block = 2;
	addr.sec_page = 0;
	flash_addr_set_section(&addr, Config.userTracks);
	addr.offset = 0;
	flash_writeData(addr, song, sizeof(Song));
	memcpy(songList[musicTracks].name, song->name, MUSIC_NAME_LEN);
	memcpy(&songList[musicTracks].addr, &addr, sizeof(Addr));
	musicTracks+=1;
	Config.userTracks+=1;
	printf("userTrack: %d \r\n", Config.userTracks);
	music_update_config();
	
	music_read_song(addr, song);
	oled_info(" ");
	return;
}

u8 music_record(Song *song){
	oled_info("12:save,16:cancel");
	song->len = 0;
	u8 info[MUSIC_NAME_LEN] = "user music";
	info[strlen((char*)info)] = '0'+(Config.userTracks+1);
	memcpy(song->name, info, MUSIC_NAME_LEN);
	song->state = 1;
	empty_counter();
	while(1){
		u8 key = key_scan();
			if(0 != key){
				if(key < 8){
					song->tones[song->len].tone = key;
					u16 time = get_counter();
					song->tones[song->len].delay = (u16)time;
					printf("tone=%d, delay=%d\r\n", song->tones[song->len].tone, song->tones[song->len].delay);
					buzzer_tone((u16)key);
					song->len++;
					empty_counter();
				}
				if(key == CONFIRM){
					return 1;}
				if(key == CANSCEL){
					return 0;}
			}
	}
	
}

void music_read_song(Addr addr, Song *song){
	flash_ReadData(addr, song, sizeof(Song));
}
void music_play(Song *song){
	oled_info("any key to exit...");
	for(u8 i = 0; i < song->len; i ++){
		u8 key = key_scan();
		if(key != 0)return;
		Delay_ms((u32)(song->tones[i].delay-1) * 10);
		buzzer_tone((u16)song->tones[i].tone);
		printf("tones=%d delay=%d\r\n", song->tones[i].tone, (song->tones[i].delay));
	}
	printf("\r\n");
}


void music_add_voice(void){
	if(Config.musicSound>=20)return;
	Config.musicSound++;
	music_update_config();
}

void music_sub_voice(void){
	if(Config.musicSound<=0)return;
	Config.musicSound--;
	music_update_config();
}
u8 music_get_voice(void){
	return Config.musicSound;
}

void music_add_melody(void){
	if(Config.melody>900)return;
	Config.melody+=10;
	music_update_config();
}
void music_sub_melody(void){
	if(Config.melody<-100)return;
	Config.melody-=10;
	music_update_config();
}
s16 music_get_melody(void){
	return Config.melody;
}


