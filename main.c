/*

GAMEBOY SPECIFICATIONS:
- CPU: Custom 8-bit Sharp LR35902 core at 4.19 MHz. 
- RAM: 8 kB internal S-RAM (can be extended up to 32 kB)
- Video RAM: 8 kB internal
- ROM: On-CPU-Die 256-byte bootstrap; 256 kb, 512 kb, 1 Mb, 2 Mb, 4 Mb and 8 Mb cartridges
- Sound: 2 pulse wave generators, 1 PCM 4-bit wave sample (64 4-bit samples played in 1×64 bank or 2×32 bank) channel, 1 noise generator, and one audio input from the cartridge.
- Display: Reflective STN LCD SCREEN_DIMENSION × 144 pixels
- Frame Rate: Approx. 59.7 frames per second on a regular Game Boy, 61.1 on a Super Game Boy
- Vertical Blank Rate: Approx 1.1 ms
- Screen size: 66 mm (2.6 in) diagonal
- Color Palette: 2-bit (4 shades of "gray")
- Power: 6 V, 0.7 W (4 AA batteries provide 15–30+ hours)
- Dimensions: 90 mm (W) × 148 mm (H) × 32 mm (D) / 3.5″ × 5.8″ × 1.3″ 
*/

#include "header.h"
#include "sound.c"

void main(void)
{
	awake();

	while(TRUE)
	{
		update();
		draw();

		wait_vbl_done();	
	}
}

void awake()
{
	DISPLAY_ON; // TURNS ON THE GAMEBOY LCD
	soundInit();
	initrand(DIV_REG); // SEED OUR RANDOMIZER

	position.x = 50;
	position.y = 100;
	position.width = 16;
	position.height = 16;
	position.vX = 0;
	position.vY = 0;

	lowerPlumb.x = SCREEN_DIMENSION;
	lowerPlumb.y = 0;
	lowerPlumb.width = 16;
	lowerPlumb.vX = -1;
	lowerPlumb.vY = 0;
	lowerPlumb.height = SCREEN_DIMENSION;

	upperPlumb.x = SCREEN_DIMENSION;
	upperPlumb.y = 0;
	upperPlumb.width = 16;
	upperPlumb.vX = -1;
	upperPlumb.vY = 0;
	upperPlumb.height = SCREEN_DIMENSION;

	set_sprite_data(0, 20, GameSprites);
	/*BIRD*/
	set_sprite_tile(0, 12); //Parameter 1 = number of the tile for reference from other functions. Parameter 2: The number of the tile of the GBTD
  	set_sprite_tile(1, 13);
  	set_sprite_tile(2, 14);
  	set_sprite_tile(3, 15);
	set_sprite_tile(4, 16); //Parameter 1 = number of the tile for reference from other functions. Parameter 2: The number of the tile of the GBTD
  	set_sprite_tile(5, 17);
  	set_sprite_tile(6, 18);
  	set_sprite_tile(7, 19);

	/*Lower Plumb */
	set_sprite_tile(10, 0);
	set_sprite_tile(11, 1);
	set_sprite_tile(12, 2);
	set_sprite_tile(13, 3);
	

	/*Upper Plumb */
	set_sprite_tile(20, 8);
	set_sprite_tile(21, 9);
	set_sprite_tile(22, 10);
	set_sprite_tile(23, 11);

	initPipes();
	SHOW_SPRITES;

	flag = SPLASH_SCREEN;
}

void start()
{
	position.x = 50;
	position.y = 100;
	position.width = 16;
	position.height = 16;
	position.vX = 0;
	position.vY = 0;

	lowerPlumb.x = SCREEN_DIMENSION;
	lowerPlumb.y = 0;
	lowerPlumb.width = 16;
	lowerPlumb.vX = -1;
	lowerPlumb.vY = 0;
	lowerPlumb.height = 0;

	upperPlumb.x = SCREEN_DIMENSION;
	upperPlumb.y = 0;
	upperPlumb.width = 16;
	upperPlumb.vX = -1;
	upperPlumb.vY = 0;
	upperPlumb.height = 0;

	initRandomizer();

	points = 0;
	time = 0x00;
	painted = 0;
	isFirstTime = TRUE;
	hasPassedThePlumb = FALSE;
	firstJumpDone = FALSE;
	flag = GAME;

	gotoxy(0, 0);
	printf("POINTS: %d         ", upperPlumb.y);

	gotoxy(2, 7);
	printf("                                                                ");

	gotoxy(3, 9);
	printf("                                                                ");
}

void update()
{
	input = joypad();
	if(input & J_SELECT)
	{
		if(musicOn == TRUE)
			musicOn = FALSE;
		else
			musicOn = TRUE;
	}	

	switch(flag)
	{
		default:
		case SPLASH_SCREEN:	
			if(musicOn)
				updateMusicMenu(); //Play the music.
			if(input & J_START)
				start();
			break;
		case GAME:
				updatePlayer();
				//firstJumpDone = TRUE;
				updatePlumbs();

				time++;
				if(time >= 0xFF)  
					time = 0x00;
				if(musicOn)
					updateMusicGameplay();
				break;
			break;
		case GAME_OVER:
			/*if(musicOn)
				updateMusicMenu();*/
			if(input & J_START)
				start();	
			break;
	}
}

void draw()
{
	switch(flag)
	{
		default:
		case SPLASH_SCREEN:
			gotoxy(0, 0);
			printf("                                     ");
			gotoxy(2, 7);
			printf("FLAPPY FLUFFY");
			gotoxy(3, 9);
			printf("Omar\n    (@HomerDalors)");				
			break;
		case GAME:
			animatePlayer();
			paintPlumbs();
			
			
			break;
		case GAME_OVER:
			gotoxy(4, 8);
			printf("GAME OVER");
			break;
	}
}

void soundInit()
{
    NR52_REG = 0xFFU;
    NR51_REG = 0x00U;
    NR50_REG = 0x77U;
}

void soundCleanNoise()
{
	NR41_REG = 1;//0x0FU; //Duration
    NR42_REG = 1;         //Volumen
    NR43_REG = 1;         //Tone
    NR44_REG = 0xC0U;
    NR51_REG |= 0x88;
}

void initRandomizer()
{
	UWORD seed = DIV_REG;
	seed |= (UWORD)DIV_REG << 8;
	initarand(seed);
}

void updatePlayer()
{	
	

	if((input & J_A) && (pressedA == 0)) 
	{
		if(!firstJumpDone)
		{
			firstJumpDone = TRUE;
			return;
		}	
		position.vY = -4;
		//sound_flap();
		pressedA = 1;
		soundFlap();
		
	} 
	else if((input & J_A) == 0) 
	{
		pressedA = 0;
	}

	if(position.y < (SCREEN_DIMENSION - position.height)) 
	{
		if(firstJumpDone)
		{
			UBYTE tm = time % 0x05;
			position.vY += tm < 1;
			
			
			if((position.y + position.vY) < SPACE_POINTS_BAR) {
				//sound_you_die();
				//trans_gameplay_gameover();
				position.vY = 0; // I just found out the ceiling doesn't kill you in the original
			}
		}

		//hit the ceiling
		
	}
	else
	{
		soundYouDie();
		position.vY = 0;
		position.vX = 0;
		position.y = SCREEN_DIMENSION - position.height;
		flag = GAME_OVER;
	}

	if(collisionCheck())
	{
		soundYouDie();
		position.vY = 0;
		position.vX = 0;
		position.y = SCREEN_DIMENSION - position.height;
		flag = GAME_OVER;
	}

	moveBird(position.vX,position.vY);
}

void initPipes()
{
	int i;
	for (i = 0; i < 20; i++)
	{
		if(i % 2 == 0)
		{
			set_sprite_tile(i + 30, 4);
		}
		else
		{
			set_sprite_tile(i + 30, 5);
		}
	}
}

void moveBird(int x, int y)
{
	position.x += x;
	position.y += y;
}

void animatePlayer()
{
	move_sprite(0, position.x, position.y);
	move_sprite(1, position.x, position.y + position.height / 2);
	move_sprite(2, position.x + position.width / 2, position.y);
	move_sprite(3, position.x + position.width / 2, position.y + position.height / 2);

	if(position.vY < 0)
	{
		set_sprite_tile(0, 16); //Parameter 1 = number of the tile for reference from other functions. Parameter 2: The number of the tile of the GBTD
		set_sprite_tile(1, 17);
		set_sprite_tile(2, 18);
		set_sprite_tile(3, 19);
	}
	else
	{
		set_sprite_tile(0, 12); //Parameter 1 = number of the tile for reference from other functions. Parameter 2: The number of the tile of the GBTD
		set_sprite_tile(1, 13);
		set_sprite_tile(2, 14);
		set_sprite_tile(3, 15);
	}
}

void updatePlumbs()
{
	if(firstJumpDone)
	{

		if(isFirstTime || lowerPlumb.x <= -16 || upperPlumb.x <= -16)
			resetPlumbs();
		
		movePlumbs(lowerPlumb.vX, lowerPlumb.vY);
	}
}

void resetPlumbs()
{
	
	safeZone = random(SPACE_POINTS_BAR + SPACE_TILE, SCREEN_DIMENSION - SPACE_TILE - SAFE_ZONE_SPACE);
	
	upperPlumb.x = SCREEN_DIMENSION;
	upperPlumb.y = safeZone;

	lowerPlumb.x = SCREEN_DIMENSION;
	lowerPlumb.y = safeZone + SAFE_ZONE_SPACE;

	isFirstTime = FALSE;
	hasPassedBy = FALSE;
	//addPoints();
}

void paintPlumbs()
{
	if(firstJumpDone)
	{
		
		unsigned int i;
		unsigned int cont = 30;
		unsigned int upperBarPos = upperPlumb.y - SPACE_TILE;
		unsigned int lowerBarPos = lowerPlumb.y;

		
		move_sprite(20, upperPlumb.x, upperPlumb.y - 8);
		move_sprite(21, upperPlumb.x, upperPlumb.y);
		move_sprite(22, upperPlumb.x + 8, upperPlumb.y - 8);
		move_sprite(23, upperPlumb.x + 8, upperPlumb.y);
		
		move_sprite(10, lowerPlumb.x, lowerPlumb.y - 8);
		move_sprite(11, lowerPlumb.x, lowerPlumb.y);
		move_sprite(12, lowerPlumb.x + 8, lowerPlumb.y - 8);
		move_sprite(13, lowerPlumb.x + 8, lowerPlumb.y);
		
		


		for (i = upperBarPos - 8; i >= SPACE_POINTS_BAR; i -= SPACE_TILE)
		{
				//gotoxy(0, 0);
				//printf("POINTS: %d  %d       ", cont, i);
				move_sprite(cont, upperPlumb.x, i);
				cont++;
				move_sprite(cont, upperPlumb.x + SPACE_TILE, i);
				cont++;
				//delay(1000);
		}

		for(i = lowerBarPos + 8; i <= SCREEN_DIMENSION; i += SPACE_TILE)
		{
			move_sprite(cont, upperPlumb.x, i);
				cont++;
				move_sprite(cont, upperPlumb.x + SPACE_TILE, i);
				cont++;
		}



	}

}

void addPoints()
{
	points += 1;
	gotoxy(0, 0);
	printf("POINTS: %d         ", points);
}

unsigned int random(unsigned int min, unsigned int max)
{
	return rand() % (max + 1 - min) + min;
}

void movePlumbs(int x, int y)
{
	lowerPlumb.x += x;
	lowerPlumb.y += y;
	
	upperPlumb.x += x;
	upperPlumb.y += y;
}

int collisionCheck()
{
	if(position.x + 16 >= upperPlumb.x && position.x <= upperPlumb.x + 16)
	{
		if((position.y + SPACE_TILE) > lowerPlumb.y || (position.y < upperPlumb.y + SPACE_TILE))
		{
			return TRUE;
		}
		else if(!hasPassedBy)
		{
			hasPassedBy = TRUE;
			soundPipe();
			addPoints();
		}
	}

	return FALSE;
}
