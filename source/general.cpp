#include <nds.h>
#include "color.h"

/*
 * Pointer to the start of VRAM_A, the memory that we'll use as the main framebuffer
 */ 
unsigned short* fb = VRAM_A;

/*
 * Pointer to the start of VRAM_B, the memory that we'll use as the secondary framebuffer
 */ 
unsigned short* fb2 = VRAM_B;

/*
 * Fills the selected framebuffer with the background color
 */
int cleanFB(unsigned short* framebuffer)
{
	int row, column;

	for(row = 0; row < SCREEN_HEIGHT; row++)
	{
	    for(column = 0; column < SCREEN_WIDTH; column++)
	    {
		    framebuffer[row * SCREEN_WIDTH + column] = BG_color;
		}
    }
	return 0;
}

/*
 * Draws a horizontal line of the specified color
 */
int drawHLine(int column, int row, int lenght, unsigned short color, unsigned short* framebuffer)
{
	for(int i = column; i < column + lenght; i++)
	{
		framebuffer[row * SCREEN_WIDTH + i] = color;
	}
	return 0;
}
