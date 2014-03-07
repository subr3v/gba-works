#include <cmath>
#include <cstdlib>
#include "FixedPoint.h"
#include "gba.h"
#include "font.h"
#include <cstdio>

#define TM_FREQ_SYS			 0	//!< System clock timer (16.7 Mhz)
#define TM_FREQ_1			 0	//!< 1 cycle/tick (16.7 Mhz)
#define TM_FREQ_64		0x0001	//!< 64 cycles/tick (262 kHz)
#define TM_FREQ_256		0x0002	//!< 256 cycles/tick (66 kHz)
#define TM_FREQ_1024	0x0003	//!< 1024 cycles/tick (16 kHz)
#define TM_CASCADE		0x0004	//!< Increment when preceding timer overflows
#define TM_IRQ			0x0040	//!< Enable timer irq
#define TM_ENABLE		0x0080	//!< Enable timer

//! Start a profiling run
/*!	\note Routine uses timers 3 and 3; if you're already using these
*	  somewhere, chaos is going to ensue.
*/
void profile_start(void)
{
	REG_TM2D= 0;	REG_TM3D= 0;
	REG_TM2CNT= 0;	REG_TM3CNT= 0;
	REG_TM3CNT= TM_ENABLE | TM_CASCADE;
	REG_TM2CNT= TM_ENABLE;
}

//! Stop a profiling run and return the time since its start.
/*!	\return 32bit cycle count
*/
uint32_t profile_stop(void)
{
	REG_TM2CNT= 0;
	return (REG_TM3D<<16)|REG_TM2D;
}

struct Vector2f
{
public:
	Vector2f() { } 
	Vector2f(FixedPoint x, FixedPoint y)
	{
		this->x = x;
		this->y = y;
	}

	FixedPoint x;
	FixedPoint y;
};

uint16_t currKeyReg;
uint16_t lastKeyReg;

const uint16_t KEY_MASK = 0x03FF; // Key mask used to extract input data from gba's input registers
const float PI = 3.141592653598f;

// Palette indices.
const uint8_t floorColor = 50;
const uint8_t ceilingColor = 51;

// Packed version of the ceiling color - precomputed for ceiling rendering.
const uint32_t ceilingColorPacked = ceilingColor | (ceilingColor << 8) | (ceilingColor << 16) | (ceilingColor << 24);

// Constants used for rendering, field of view, height of the wall and size of the wall.
const int PLAYER_FOV = 60;
const int HALF_PLAYER_FOV = PLAYER_FOV / 2;
const int WALL_HEIGHT = 16; 
const int WALL_SIZE = 32; // A wall is 32 by 32 units
const int WALL_SIZE_SHIFT_DIVIDE = 5; // A wall is 32 by 32 units

const FixedPoint MAX_DISTANCE = FixedPoint(5000); // Max distance value used for raycasting.
const FixedPoint WALL_SIZE_FIXED_POINT = FixedPoint(WALL_SIZE); // Precomputed value of wall size in fixed point format.
const FixedPoint INV_WALL_SIZE_FIXED_POINT = FixedPoint(1.0f / WALL_SIZE); // Precomputed value of wall size in fixed point format.

const int MAP_WIDTH = 32, MAP_HEIGHT = 20;

uint32_t profileTime = 0;

// Color encoded map --- 
// 0 is an empty cell.
// 1 - 8 is colored.
// The map is divided in 6 areas (color starts from 1)
// The end of the maze is the wall with number 8.

const int END_OF_MAZE_WALL = 8;

const char map[MAP_WIDTH * MAP_HEIGHT] = 
{ 
	1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 3 , 3 , 3 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5,
	1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 4 , 0 , 0 , 0 , 0 , 4 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 5,
	1 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 4 , 0 , 0 , 0 , 0 , 5 , 5 , 5 , 5 , 5 , 5 , 0 , 0 , 5,
	1 , 1 , 1 , 0 , 0 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 4 , 0 , 0 , 4 , 0 , 5 , 0 , 0 , 0 , 0 , 5 , 5 , 5 , 5,
	1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 3 , 3 , 4 , 0 , 4 , 4 , 0 , 4 , 4 , 0 , 0 , 4 , 0 , 5 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 5,
	1 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 3 , 0 , 0 , 0 , 4 , 4 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 5 , 0 , 0 , 5 , 0 , 0 , 0 , 0 , 5,
	1 , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 0 , 0 , 0 , 0 , 5 , 5 , 5 , 0 , 0 , 5,
	2 , 2 , 0 , 2 , 2 , 2 , 2 , 0 , 0 , 0 , 3 , 3 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 0 , 0 , 0 , 0 , 5 , 0 , 0 , 0 , 0 , 5,
	2 , 0 , 0 , 0 , 0 , 0 , 2 , 0 , 3 , 3 , 3 , 3 , 0 , 0 , 0 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 5 , 5 , 5 , 5 , 5 , 0 , 0 , 5 , 5 , 5,
	2 , 0 , 0 , 0 , 0 , 0 , 2 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 6,
	2 , 0 , 0 , 0 , 0 , 0 , 2 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 0 , 0 , 6 , 0 , 6 , 6 , 6 , 0 , 0 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 6,
	2 , 0 , 0 , 0 , 0 , 0 , 2 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 6 , 6 , 6 , 0 , 6 , 6 , 6 , 0 , 0 , 6,
	2 , 0 , 0 , 0 , 0 , 0 , 2 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 6 , 6 , 0 , 6 , 6 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 6,
	2 , 2 , 2 , 2 , 2 , 2 , 2 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 0 , 0 , 0 , 6 , 6 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 6,
	7 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 0 , 6 , 6 , 6 , 6 , 6 , 0 , 6 , 6 , 6 , 6 , 6 , 6,
	7 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 0 , 6 , 0 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 6,
	7 , 0 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 0 , 0 , 0 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 6,
	7 , 0 , 7 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 0 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 0 , 6,
	7 , 0 , 7 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 0 , 7 , 7 , 7 , 7 , 6 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 6,
	7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 7 , 8 , 7 , 7 , 7 , 7 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 , 6 }; 

// Constant values used by UpdateGame function: 
const int playerAngleRotationSpeed = 2; // Indicates how many angles per frame the player is rotated when pressing right or left
const FixedPoint playerSpeed = FixedPoint(1.0f); // Indicates how many units per frame the player moves by when pressing up - down

// Player position in units size
Vector2f playerPosition(48,48);
// Player angle of view
int playerAngle = 70;

/*
*	Samples a point in the map and returns the value
*	Does not perform any bounds checking.
*/
uint8_t SampleMap(int32_t x,int32_t y)
{
	return map[ y * MAP_WIDTH + x]; 
}

// Precomputed values for trig functions
FixedPoint InvTanTable[360];
FixedPoint TanTable[360];
FixedPoint InvCosTable[360];
FixedPoint InvSinTable[360];
FixedPoint XStepTable[360];
FixedPoint YStepTable[360];
FixedPoint CosTable[360];
FixedPoint SinTable[360];
int32_t CorrectedDistanceTable[512];

/*
*	Initializes the trig functions table with angles ranging from 0° to 359°
*/
void InitTables()
{
	for(int i = 0; i < 360; i++)
	{
		float angle = ((float)i * PI / 180.0f); // degree to radians conversion
		float tanVal = tan(angle);
		float cosVal = cos(angle);
		float sinVal = sin(angle);
		TanTable[i] = tanVal;
		CosTable[i] = cosVal;
		SinTable[i] = sinVal;
		InvTanTable[i] = (tanVal != 0.0f ? (1.0f / tanVal) : FixedPoint(0));
		InvCosTable[i] = (cosVal != 0.0f ? (1.0f / cosVal) : FixedPoint(0));
		InvSinTable[i] = (sinVal != 0.0f ? (1.0f / sinVal) : FixedPoint(0));
		XStepTable[i] = WALL_SIZE_FIXED_POINT * InvTanTable[i];
		YStepTable[i] = WALL_SIZE_FIXED_POINT * TanTable[i];
	}
	for(int i = 1; i < 512; i++)
	{
		CorrectedDistanceTable[i] =  (WALL_HEIGHT * 277) / i;
	}
	CorrectedDistanceTable[0] = CorrectedDistanceTable[1];
}

/*
*	Returns true if button is currently held down, false otherwise.
*/
bool IsButtonDown(int button) { return (currKeyReg & button); }

/*
*	Returns true if button is being pressed in the current frame, false otherwise.
*/
bool IsButtonPressed(int button) { return (currKeyReg & button) && (~lastKeyReg & button); }

/*
*	 Updates variables used for input checking functions.
*/
void UpdateInput()
{
	lastKeyReg = currKeyReg;
	currKeyReg = ~REG_KEYINPUT & KEY_MASK;
}

/*
*	Checks gba input and moves player's angle of view and position accordingly.
*/
void UpdateMaze()
{
	if( IsButtonDown(KEY_RIGHT) )
		playerAngle += playerAngleRotationSpeed;
	if( IsButtonDown(KEY_LEFT) )
		playerAngle -= playerAngleRotationSpeed;

	if( playerAngle < 0 )
		playerAngle += 360;
	if ( playerAngle >= 360 )
		playerAngle -= 360;

	Vector2f lastPosition = playerPosition;

	// Player movement is relative to the current angle of view, so we use cos and sin function to get the proper movement vector.
	if( IsButtonDown(KEY_UP) )
	{
		playerPosition.x += playerSpeed * CosTable[playerAngle];
		playerPosition.y += playerSpeed * SinTable[playerAngle];
	}

	if( IsButtonDown(KEY_DOWN) )
	{
		playerPosition.x -= playerSpeed * CosTable[playerAngle];
		playerPosition.y -= playerSpeed * SinTable[playerAngle];
	}

	// We perform a basic collision test between the player and the map: 
	// If the player is about to move in a cell inside the map which is different from 0 we disallow movement for that frame.
	if( lastPosition.x != playerPosition.x || lastPosition.y != playerPosition.y )
	{
		int32_t xGrid = FixedPoint::ToInt(playerPosition.x)  >> WALL_SIZE_SHIFT_DIVIDE;
		int32_t yGrid = FixedPoint::ToInt(playerPosition.y) >> WALL_SIZE_SHIFT_DIVIDE;
		int touchedIndex = SampleMap(xGrid,yGrid);
		if( touchedIndex == END_OF_MAZE_WALL ) // The player has reached the end of the maze
			playerPosition = Vector2f(48,48);
		else if( touchedIndex != 0)
			playerPosition = lastPosition;
	}
}


/*
*	This function uses DMA to fill the screen with a single color.
*/
void ClearScreen8Fast(uint8_t colour)
{
	volatile uint32_t value = colour | (colour << 8) | (colour << 16) | (colour << 24); // This is volatile to ensure that the compiler's optimizer doesn't skip the initialization of the variable.
	REG_DM3SAD = (uint32_t)&value;
	REG_DM3DAD = (uint32_t)BackBuffer;
	REG_DM3CNT = (SCREEN_WIDTH * SCREEN_HEIGHT / 4) | DMACNT_ENABLE | DMACNT_32BIT | DMACNT_FIXSRC | DMACNT_INCDEST;
}


/*
*	Draws the maze using a simple raycasting algorithm.
*	An implementation of Digital differential analyzer (DDA) is used to calculate collision between rays and the map.
*	The algorithm casts 60 rays, each ray represents 4 pixels.
*	The ray is then checked against the map by using DDA's algorithm and the shortest distance is used to calculate the correct height of the wall.
*/
void DrawMaze()
{
	ClearScreen8Fast(floorColor);

	int32_t currentAngle = playerAngle - HALF_PLAYER_FOV;
	if( currentAngle < 0 )
		currentAngle += 360;
	if( currentAngle == 360)
		currentAngle = 0;

	int32_t angleDistance = playerAngle - currentAngle;
	if( angleDistance > 180 )
		angleDistance -= 360;
	if( angleDistance < -180 )
		angleDistance += 360;

	if( angleDistance < 0 )
		angleDistance += 360;


	for(int32_t currentRay = 0; currentRay < SCREEN_WIDTH; currentRay += 4)
	{
		int32_t horizontalCollisionID, verticalCollisionID;

		Vector2f intersectionPoint;

		// Calculates horizontal distance
		FixedPoint horizontalDistance = MAX_DISTANCE;
		if( currentAngle != 0 && currentAngle != 180 )
		{
			FixedPoint stepX;
			int stepY;
			if( currentAngle > 0 && currentAngle < 180 ) // Up direction
			{
				intersectionPoint.y = FixedPoint::Floor(playerPosition.y * INV_WALL_SIZE_FIXED_POINT) * WALL_SIZE + WALL_SIZE;
				stepY = WALL_SIZE;
				stepX = XStepTable[currentAngle];
			}
			else // Down direction
			{
				intersectionPoint.y = FixedPoint::Floor(playerPosition.y * INV_WALL_SIZE_FIXED_POINT) * WALL_SIZE;
				stepY = -WALL_SIZE;
				stepX = -XStepTable[currentAngle];
			}
			intersectionPoint.x = playerPosition.x + (intersectionPoint.y - playerPosition.y) * InvTanTable[currentAngle];

			int halfStepY = stepY >> 1;

			while(true)
			{
				int gridX = FixedPoint::ToInt( intersectionPoint.x * INV_WALL_SIZE_FIXED_POINT );
				int gridY = FixedPoint::ToInt( (intersectionPoint.y + halfStepY ) * INV_WALL_SIZE_FIXED_POINT);
				if( gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT )
					break;
				else if( (horizontalCollisionID = SampleMap(gridX,gridY)) != 0 )
				{
					if( currentAngle != 90 && currentAngle != 270 )
						horizontalDistance = (intersectionPoint.x - playerPosition.x) * InvCosTable[currentAngle];
					else
						horizontalDistance = (intersectionPoint.y - playerPosition.y) * InvSinTable[currentAngle];
					break;
				}
				intersectionPoint.x += stepX;
				intersectionPoint.y += stepY;
			}
		}

		// Calculates vertical distance.
		FixedPoint verticalDistance = MAX_DISTANCE;
		if( currentAngle != 90 && currentAngle != 270  )
		{
			FixedPoint stepY;
			int stepX;
			if( currentAngle < 90 || currentAngle > 270) // Right direction
			{
				intersectionPoint.x = FixedPoint::Floor(playerPosition.x * INV_WALL_SIZE_FIXED_POINT) * WALL_SIZE + WALL_SIZE;
				stepX = WALL_SIZE;
				stepY = YStepTable[currentAngle];
			}
			else // Left direction
			{
				intersectionPoint.x = FixedPoint::Floor(playerPosition.x * INV_WALL_SIZE_FIXED_POINT) * WALL_SIZE;
				stepX = -WALL_SIZE;
				stepY = -YStepTable[currentAngle];
			}

			intersectionPoint.y = playerPosition.y + (intersectionPoint.x - playerPosition.x) * TanTable[currentAngle];
			
			int halfStepX = stepX >> 1;

			while(true)
			{
				int gridX = FixedPoint::ToInt( (intersectionPoint.x + halfStepX) * INV_WALL_SIZE_FIXED_POINT );
				int gridY = FixedPoint::ToInt( (intersectionPoint.y * INV_WALL_SIZE_FIXED_POINT) );
				if( gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT )
					break;
				else if( (verticalCollisionID = SampleMap(gridX,gridY)) != 0 )
				{
					if( currentAngle != 180 && currentAngle != 0 )
						verticalDistance = (intersectionPoint.y - playerPosition.y) * InvSinTable[currentAngle];
					else
						verticalDistance = (intersectionPoint.x - playerPosition.x) * InvCosTable[currentAngle];
					break;
				}
				intersectionPoint.x += stepX;
				intersectionPoint.y += stepY;
			}
		}


		FixedPoint wallDistance = horizontalDistance < verticalDistance ? horizontalDistance : verticalDistance;
		int32_t collisionID = horizontalDistance < verticalDistance ? horizontalCollisionID : verticalCollisionID;

		if ( wallDistance < MAX_DISTANCE )
		{
			int32_t correctedDistance = FixedPoint::ToInt(wallDistance * CosTable[angleDistance]); 
			if( correctedDistance < 0 ) correctedDistance *= -1;
			int32_t lineLength = CorrectedDistanceTable[correctedDistance]; // + 1 guarantees that this value is at least 1 -- avoiding divide by zero problems

			int32_t startY = (SCREEN_HEIGHT / 2) - lineLength / 2;
			int32_t endY = startY + lineLength;

			if( startY < 0 )
			{
				startY = 0;
				endY = SCREEN_HEIGHT;
			}
			
			uint32_t paletteColorOffset = horizontalDistance < verticalDistance ? 0 : 1; // We use a darker/lighter version of the color depending on the side of wall we are rendering.
			uint32_t col = 1 + ( (collisionID - 1) * 2) + paletteColorOffset; // This formula is used to fetch the correct value from the palette index.
			uint32_t colPacked = col | (col << 8) | (col << 16) | (col << 24); // We are packing the color in a 32 bit value so that we copy it 4 times per assignment.

			// Rounding to the next even number using bitwise tricks; this is used to exploit an optimization (loop unrolling for the copying part)
			startY = (startY + 1) & ~1; 
			endY = (endY + 1) & ~1; 

			// Draw ceiling first (if there is any) and then the wall.
			volatile uint32_t* ceilingStartPtr = (volatile uint32_t*)( BackBuffer + ((currentRay + 0 * SCREEN_WIDTH) >> 1 ));
			uint32_t* ceilingEndPtr = (uint32_t*)( BackBuffer + ((currentRay + startY * SCREEN_WIDTH) >> 1));

			volatile uint32_t* wallStartPtr = (volatile uint32_t*)( BackBuffer + ((currentRay + startY * SCREEN_WIDTH) >> 1));
			uint32_t* wallEndPtr = (uint32_t*)( BackBuffer + ((currentRay + endY * SCREEN_WIDTH) >> 1));

			const uint32_t rowSize = SCREEN_WIDTH / 4;
			const uint32_t doubleRowSize = SCREEN_WIDTH / 2;

			while( ceilingStartPtr < ceilingEndPtr )
			{
				*ceilingStartPtr = ceilingColorPacked;
				*(ceilingStartPtr + rowSize) = ceilingColorPacked;
				ceilingStartPtr += doubleRowSize;
			}

			while( wallStartPtr < wallEndPtr )
			{
				*wallStartPtr = colPacked;
				*(wallStartPtr + rowSize) = colPacked;
				wallStartPtr += doubleRowSize;
			}

		}

		//We batch rays in 4 increments, since FOV is 60° we'd have an increment of 1 degree every 4 rays.
		++currentAngle;
		--angleDistance;

		if( currentAngle >= 360 )
			currentAngle -= 360;

		// Since angle distance is: playerAngle - currentAngle if we increment the angle by 1 the distance has to be decrement by 1 as well.
		if( angleDistance < 0 )
			angleDistance += 360;
	}
}

/*
*	Sets the correct palette index
*	hexColor is an RGB color with each component ranging from 0 to 255
*	wallIndex is the index inside the grid indicating the palette index inside the gba
*	The function performs a conversion from [0,255] to [0,31] for each component
*/
void SetWallColor(int wallIndex, int32_t hexColor)
{
	int16_t r = ( (hexColor & 0xFF0000) >> 16 ) * 31 / 255;
	int16_t g = ( (hexColor & 0x00FF00) >> 8 ) * 31 / 255;
	int16_t b = ( hexColor & 0x0000FF) * 31 / 255;

	int16_t r_dark = r - 4; if(r_dark < 0 ) r_dark = 0;
	int16_t g_dark = g - 4; if(g_dark < 0 ) g_dark = 0;
	int16_t b_dark = b - 4; if(b_dark < 0 ) b_dark = 0;

	uint32_t paletteIndex = 1 + ( (wallIndex - 1) * 2);
	SetPaletteBG(paletteIndex, RGB(r,g,b)); // light version
	SetPaletteBG(paletteIndex + 1, RGB(r_dark,g_dark,b_dark)); // dark version
}

/*
*	Setups the GBA registers for Mode 4 and fills GBA palette with the values needed for correct rendering.
*/
void SetupGraphics()
{
	REG_DISPCNT = MODE4 | BG2_ENABLE;

	SetWallColor(1,0xa33827);
	SetWallColor(2,0x176975);
	SetWallColor(3,0x679111);
	SetWallColor(4,0xf7ac37);
	SetWallColor(5,0x8d0458);
	SetWallColor(6,0xc5c5c5);
	SetWallColor(7,0x55476c);
	SetWallColor(8,0xFFFFFF);

	SetPaletteBG(floorColor,RGB(16,16,16));
	SetPaletteBG(ceilingColor,RGB(4,4,4));
}

// Draw ASCII character c at position (x, y) in colour colour.
void DrawChar8(int x, int y, int colour, char c) {
	for (int yo = 0; yo < 8; yo++) {
		for (int xo = 0; xo < 8; xo++) {
			if (font_bold[int(c)][(yo * 8) + xo]) {
				PlotPixel8(x + xo, y + yo, colour);
			}
		}
	}
}

// Draw \0-terminated string s at position (x, y) in colour colour.
void DrawString8(int x, int y, int colour, const char *s) {
	while (*s != '\0') {
		DrawChar8(x, y, colour, *s);
		x += 8;
		s++;
	}
}

// The entry point for the game
int main()
{
	InitTables();
	SetupGraphics();

	while (true)
	{
		profile_start();
		WaitVSync();
		FlipBuffers();
		profileTime = profile_stop();
		
		UpdateInput();
		UpdateMaze();
		DrawMaze();

		char txt[32];
		sprintf(txt,"Cycles left: %d", profileTime);
		DrawString8(10, 10, 8,txt);
	}

	return 0;
}