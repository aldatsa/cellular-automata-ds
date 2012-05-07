#include <nds.h>
#include "color.h"
#include "flash.h"  // Include for the header of the flash image (grit outputs a nice header to reference data)

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
 *
 */
int showFB()
{
	// Configuration of the main screen
	REG_DISPCNT = MODE_FB0;		//Framebuffer
	
	// Configure the VRAM A block
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD;
	
	return 0;
}

/*
 *
 */
int showFB2()
{
    // Configuration of the main screen
	REG_DISPCNT = MODE_FB1;		// Use MODE_FB1 for VRAM_B
	
	// Configure the VRAM B block
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_LCD;
	
	return 0;
}

/*
 * Show the flash screen on the main screen
 */
int showFlash()
{   
    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);

	// set the sub background up for text display (we could just print to one
	// of the main display text backgrounds just as easily
	//videoSetModeSub(MODE_0_2D); //sub bg 0 will be used to print text

	vramSetBankA(VRAM_A_MAIN_BG);

	// set up our bitmap background
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	
	decompress(flashBitmap, BG_GFX,  LZ77Vram);

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

/*
 * Draws a vertical line of the specified color
 */ 
int drawVLine(int column, int row, int lenght, unsigned short color)
{
    for(int i = row; i < row + lenght; i++)
    {
        fb[i * SCREEN_WIDTH + column] = color;
    }
    return 0;
}

/*
 * Draws a rectangle, filled with FG_color or BG_color
 * This function is used by ca.drawRule() to draw the rectangles to visualize the current rule
 * and in the color selection screen
 */
int drawRectangle(bool fill, int intRowStart, int intColumnStart, int length, int width)
{
	// fill-> true, fill with FG_color		fill -> false, fill with BG_color

	unsigned char column = 0;
	unsigned char row = 0;
	unsigned short color;
	
	/*
	 * It must be intRowStart + length - 1 and intColumnStart + width - 1
	 * else we get this: (using intRowStart + length and intColumnStart + width)
	 * example: intRowStart = 0, intColumnStart = 0, length = 4, width = 4
	 *    0 1 2 3 4 
	 *  0 x x x x x    horizontal line from (0, 0) to (0, 3) and vertical line from (0, 0) to (3, 0)
	 *  1 x o o o x    
	 *  2 x o o o x 
	 *  3 x o o o x  
	 *  4 x x x x      horizontal line from (4, 0) to (4, 3) and vertical line from (4, 0) to (4, 3)
	 * 
	 * The filling was like this:
	 *	for(column = intColumnStart + 1; column < intColumnStart + width; column++)
	 *  {
	 *	    for(row = intRowStart + 1; row < intRowStart + length; row++)
     *
	 * But now it must be corrected to:
	 *	for(column = intColumnStart + 1; column < intColumnStart + width - 1; column++)
	 *  {
	 *  	for(row = intRowStart + 1; row < intRowStart + length - 1; row++)
     *
     * This way we get the correct drawing:
	 *    0 1 2 3 
	 *  0 x x x x    horizontal line from (0, 0) to (0, 3) and vertical line from (0, 0) to (3, 0)
	 *  1 x o o x    
	 *  2 x o o x 
	 *  3 x x x x    horizontal line from (3, 0) to (3, 3) and vertical line from (3, 0) to (3, 3)     
	 */ 
	drawHLine(intColumnStart, intRowStart, width, line_color, fb);
	drawHLine(intColumnStart, intRowStart + length - 1, width, line_color, fb);
	drawVLine(intColumnStart, intRowStart, length, line_color);
	drawVLine(intColumnStart + width - 1, intRowStart, length, line_color);
	
	if(fill == true)
	{
		color = FG_color;		
	}
	else
	{
		color = BG_color;
	}
	
	for(column = intColumnStart + 1; column < intColumnStart + width - 1; column++)
	{
		for(row = intRowStart + 1; row < intRowStart + length - 1; row++)
		{
			fb[row * SCREEN_WIDTH + column] = color;
		}
	} 
	
	return 0;
}
