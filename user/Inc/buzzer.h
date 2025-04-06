#ifndef __PASSIVE_BUZZER_H__
#define __PASSIVE_BUZZER_H__

#include "STM32F10x.h"
#include "Delay.h"
#include "music.h"
#include "LED.h"

void buzzer_init(void);
void buzzer_change(u16 circumflex);
void buzzer_tone(u16 music);

#endif
