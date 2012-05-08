#include "nds.h"
#include "framebuffer.h"
#include "color.h"

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 *             x  x  x                 x  x  x   
 */
int hexGridLineOne(int y)
{
	for(int i = 0; i < 32; i++)
	{
		for(int j=0; j<3; j++)
		{
			fb[SCREEN_WIDTH * y + (4 + j + 8 * i)] = line_color;
		}
	}
	return 0;
}

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 *          x           x           x           x
 */
int hexGridLineTwo(int y)
{
	for(int i = 0; i < 64; i++)
	{
		fb[SCREEN_WIDTH * y + (3 + 4 * i)] = line_color;
	}
	return 0;
}

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * x  x  x                 x  x  x
 */
int hexGridLineThree(int y)
{
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			fb[SCREEN_WIDTH * y + (j + 8 * i)] = line_color;
		}
	}
	return 0;
}

/*
 *   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * 0             x  x  x                 x  x  x     hexGridLineOne
 * 1          x           x           x           x  hexGridLineTwo
 * 2 x  x  x                 x  x  x                 hexGridLineThree
 * 3          x           x           x           x  hexGridLineTwo
 * 4             x  x  x                 x  x  x     hexGridLineOne 
 * 5          x           x           x           x  hexGridLineTwo
 * 6 x  x  x                 x  x  x                 hexGridLineThree
 * 7          x           x           x           x  hexGridLineTwo
 * 8             x  x  x                 x  x  x     hexGridLineOne 
 */
int drawHexGrid()
{
    cleanFB(fb);
    
	for (int i = 0; i < 48; i++)
	{
		hexGridLineOne(4 * i);
		hexGridLineTwo(4 * i + 1);
		hexGridLineThree(4 * i + 2);
		hexGridLineTwo(4 * i + 3);
	}
    return 0;
}

/*
 * Draws the hexagonal cell in the coordinates (intPosX, intPosY) of the specified framebuffer with the specified color
 */
int paintHexCell(int intPosX, int intPosY, unsigned short color, unsigned short* framebuffer)
{

	drawHLine(intPosX, intPosY, 3, color, framebuffer);
	drawHLine(intPosX - 1, intPosY + 1, 5, color, framebuffer);
	drawHLine(intPosX, intPosY + 2, 3, color, framebuffer);
	
	return 0;
}
