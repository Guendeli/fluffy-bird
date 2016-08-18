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

#include <gb/gb.h> // INCLUDE GBDK FUNCTION LIBRARY
#include <gb/hardware.h> // INCLUDE HANDY HARDWARE REFERENCES
#include <gb/sample.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <rand.h>// INCLUDE RANDOM FUNCTIONS
#include <stdio.h>


#include "sprites.c"

#define PLUMBS 					1
#define GRAVITY 				1
#define SPACE_TILE			 	8
#define MAX_FALL_VELOCITY	 	4
#define SPACE_POINTS_BAR		24
#define SAFE_ZONE_SPACE			76
#define SCREEN_DIMENSION		160

typedef enum 
{
	SPLASH_SCREEN = -1,
	GAME_OVER = 0,
	GAME = 1
} GameStatus;

unsigned int points = 0;
UBYTE time = 0x00;
GameStatus flag = SPLASH_SCREEN;
UBYTE painted = 0;
UBYTE isFirstTime = TRUE;
UBYTE hasPassedThePlumb = 0;
UBYTE iteration = 0;
int pressedA = 0;
int input;
UBYTE musicOn = TRUE;
UBYTE firstJumpDone = FALSE;
UBYTE hasPassedBy = FALSE;

struct LowerPlumb lowerPlumb;
struct UpperPlumb upperPlumb;
struct Position position;

unsigned int safeZone = 0;

struct LowerPlumb
{
	long x;
	long y;
	unsigned int height;
	unsigned int width;

	int vX;
	int vY;
};

struct UpperPlumb
{
	long x;
	long y;
	unsigned int height;
	unsigned int width;

	int vX;
	int vY;
};

struct position
{
	long x;
	long y;
	unsigned int height;
	unsigned int width;

	int vX;
	int vY;
};

const unsigned int lowerPlumbTiles[] = {0, 1, 2, 3, 4, 5};
const unsigned int upperPlumbTiles[] = {6, 7, 8, 9, 10, 11};
const unsigned int PositionTiles[] = {12,13,14,15,16,17,18,19};

void awake();
void start();
void update();
void draw();

void soundInit();
void soundCleanNoise();

void animatePlayer();
void updatePlayer();
void moveBird(int x, int y);
void addPoints();
void initRandomizer();
void initPipes();
unsigned int random(unsigned int min, unsigned int max);
void paintPlumbs();
void movePlumbs(int x, int j);
void updatePlumbs();
void resetPlumbs();

void collision(int safeZone);
int collisionCheck();
