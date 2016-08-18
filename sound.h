#ifndef __SOUND_H__
#define __SOUND_H__

#include "songs.h"

UINT16 currentBeat;
UINT8 timerCounter;
UINT8 moreSpeed;
UINT8 muteChannel1;
UINT8 muteChannel4;


void setNote(Note* note);
void playMusicMenu();
void playMusicGameover();
void playMusicGameplay();
void updateMusicMenu();
void updateMusicGameover();
void updateMusicGameplay();

void soundFlap();
void soundYouDie();
void soundPipe();

#endif
