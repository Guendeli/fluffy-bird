#include "sound.h"

void setNote(Note *n){
    switch((*n).i){
            case MELODY:
                    NR10_REG = 0x00U; //pitch sweep
                    NR11_REG = 0x84U; //wave duty
                    NR12_REG = (*n).env; //envelope
                    NR13_REG = (UBYTE)frequencies[(*n).p]; //low bits of frequency
                    NR14_REG = 0x80U | ((UWORD)frequencies[(*n).p]>>8); //high bits of frequency (and sound reset)
            break;
            case HARMONY:
                    NR10_REG = 0x00U;
                    NR11_REG = 0x00U; //wave duty for harmony is different
                    NR12_REG = (*n).env;
                    NR13_REG = (UBYTE)frequencies[(*n).p];
                    NR14_REG = 0x80U | ((UWORD)frequencies[(*n).p]>>8);
            break;
            case SNARE:
                    NR21_REG = 0x00U;
                    NR22_REG = (*n).env;
                    NR23_REG = (UBYTE)frequencies[(*n).p];
                    NR24_REG = 0x80U | ((UWORD)frequencies[(*n).p]>>8);
                    NR51_REG |= 0x22;
            break;
            case WAVE:

            break;
            case CYMBAL:
                NR41_REG = 30;
                NR42_REG = 55;     
                NR43_REG = 50;         
                NR44_REG = 0xC0U;
                NR51_REG |= 0x88;
            break;
    }
}

void playMusicMenu(){
    setNote(&song_menu_ch1[currentBeat]);
    setNote(&song_menu_ch2[currentBeat]);
    setNote(&song_menu_ch4[currentBeat]);
    NR51_REG |= 0x11U;
}

void playMusicGameover(){
    setNote(&song_gameover_ch1[currentBeat]);
    setNote(&song_gameover_ch2[currentBeat]);
    NR51_REG |= 0x11U;
}

void playMusicGameplay(){
    if (muteChannel1 == 0)
        setNote(&song_gameplay_ch1[currentBeat]);
    setNote(&song_gameplay_ch2[currentBeat]);
    if (muteChannel4 == 0)
        setNote(&song_gameplay_ch4[currentBeat]);
    NR51_REG |= 0x11U;
}

void updateMusicMenu(){
    if (timerCounter >= 7){
            timerCounter = 0;
            currentBeat = currentBeat == 101 ? 0 : currentBeat+1;
            playMusicMenu();
    }
    timerCounter++;
}

void updateMusicGameover(){
    if (timerCounter >= 7 && currentBeat <= 25){
            timerCounter = 0;
            currentBeat = currentBeat == 25 ? 0 : currentBeat+1;
            playMusicGameover();
    }
    timerCounter++;
}

void updateMusicGameplay(){
    if (timerCounter >= 14 - moreSpeed){
            timerCounter = 0;
            if (currentBeat < 101)
            {
                currentBeat++;
            }
            else
            {
                currentBeat = 0;
                /*if (moreSpeed < 5)
                {
                    moreSpeed++;
                }*/
            }
            playMusicGameplay();
    }
    timerCounter++;
    if (muteChannel1 != 0)
    {
        muteChannel1++;
        if (muteChannel1 > 10)
        {
            muteChannel1 = 0;
        }
    }
    if (muteChannel4 != 0)
    {
        muteChannel4++;
        if (muteChannel4 > 10)
        {
            muteChannel4 = 0;
        }
    }
}

void soundFlap()
{
	NR10_REG = 0x34U; // 4th bit indicates if freq increases or decreases
			  // bits 5th-7th indicate sweep delay
	NR11_REG = 0x80U;
	NR12_REG = 0xF0U;
	NR13_REG = 0x0AU; //lowest 8 frequency bits
	NR14_REG = 0xC6U; // first 3 bits are the 3 highest bits of freq
	NR51_REG |= 0x11;
}

void soundYouDie()
{
    NR41_REG = 0x00;//0x1FU;
    NR42_REG = 0xE1;//0xF7U;
    NR43_REG = 0x22;//0x24U;
    NR44_REG = 0xC3;//0x80U;
    NR51_REG = 0x88U;
}

void soundPipe()
{

    NR21_REG = 0x80U;
    NR22_REG = 0x73U;
    NR23_REG = 0x9EU;
    NR24_REG = 0xC7U;
    NR51_REG |= 0x22;	// play frst note
    NR21_REG = 0xAEU;
    NR22_REG = 0x68U;
    NR23_REG = 0xDBU;
    NR24_REG = 0xC6U;
    NR51_REG |= 0x22;


}
