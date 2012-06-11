#include <nds.h>
#include "framebuffer.h"
#include "color.h"

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
 */
int triangularGridLineOne(int y)
{
	for(int i = 0; i < SCREEN_WIDTH - 1; i++)
	{
    	fb[SCREEN_WIDTH * y + i] = line_color;
	}
	return 0;
}

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 *          x     x           x     x
 */
int triangularGridLineTwo(int y)
{
	for(int i = 3; i < SCREEN_WIDTH - 1; i = i + 6)
	{
		fb[SCREEN_WIDTH * y + i] = line_color;
        fb[SCREEN_WIDTH * y + i + 2] = line_color;
	}
	return 0;
}

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * x     x           x     x           x     x
 */
int triangularGridLineThree(int y)
{
	for(int i = 0; i < SCREEN_WIDTH - 1; i = i + 6)
	{
		fb[SCREEN_WIDTH * y + i] = line_color;
		fb[SCREEN_WIDTH * y + i + 2] = line_color;
	}
	return 0;
}

/*
 *   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * 0 x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  triangularGridLineOne
 * 1          x     x           x     x              triangularGridLineTwo
 * 2 x     x           x     x           x     x     triangularGridLineThree
 * 3 x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  triangularGridLineOne
 * 4 x     x           x     x           x     x     triangularGridLineThree
 * 5          x     x           x     x              triangularGridLineTwo
 * 6 x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  triangularGridLineOne
 * 7          x     x           x     x              triangularGridLineTwo
 * 8 x     x           x     x           x     x     triangularGridLineThree
 * 9 x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  triangularGridLineOne
 */
int drawTriangularGrid()
{
    cleanFB(fb);
    
	for (int i = 0; i < 31; i++)
	{
		triangularGridLineOne(6 * i);
        triangularGridLineTwo(6 * i + 1);
		triangularGridLineThree(6 * i + 2);
		triangularGridLineOne(6 * i + 3);
		triangularGridLineThree(6 * i + 4);				
		triangularGridLineTwo(6 * i + 5);
	}
	
    return 0;
}

int paintTriangularCell(int intPosX, int intPosY, unsigned short color,
                        unsigned short* framebuffer)
{
	if ((intPosX % 2 == 0 and intPosY % 2 == 0) or 
        (intPosX % 2 != 0 and intPosY % 2 != 0))
    {
	    drawHLine(intPosX -1, intPosY, 3, color, framebuffer);
	    drawHLine(intPosX, intPosY + 1, 1, color, framebuffer);
    }
    else if ((intPosX % 2 != 0 and intPosY % 2 == 0) or
             (intPosX % 2 == 0 and intPosY % 2 != 0))
	{
	    drawHLine(intPosX, intPosY, 1, color, framebuffer);
	    drawHLine(intPosX - 1, intPosY + 1, 3, color, framebuffer);
	}
	return 0;
}
