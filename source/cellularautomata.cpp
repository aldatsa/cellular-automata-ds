#include "cellularautomata.h"
#include <nds.h>
#include "general.h"
#include "color.h"
#include "hexgrid.h"

/*
 * References:
 *
 * LANGTON'S ANT:
 * http://en.wikipedia.org/wiki/Langton%27s_ant
 * http://mathworld.wolfram.com/LangtonsAnt.html
 *
 * MUNCHING SQUARES:
 * http://en.wikipedia.org/wiki/Munching_square
 * http://mathworld.wolfram.com/MunchingSquares.html
 * http://www.inwap.com/pdp10/hbaker/hakmem/hacks.html#item146
 */

//*************************************PRIVATE*******************************************

/*
 * Calculates the new angle of the ant
 */
int CellularAutomata::rotateAnt(unsigned char rotateTo)
{
    int limitAngle;
    int stepAngle;
    
    /*
     * Langton's hexagonal ant
     * Right ('R'): 0 -> 60 -> 120 -> 180 -> 240 -> 300 (limitAngle) -> 0 -> 60 -> ...
     * Left ('L'): 0 -> 300 (limitAngle) -> 240 -> 180 -> 120 -> 60 -> 0 -> 300 -> ...
     */
    if (type == "LHA") 
    {
        limitAngle = 300;
        stepAngle = 60;
    }
    /*
     * Langton's ant
     * Right ('R'): 0 -> 90 -> 180 -> 270 (limitAngle) -> 0 -> 90 -> ...
     * Left ('L'): 0 -> 270 (limitAngle) -> 180 -> 90 -> 0 -> 270 -> ...
     */
    else if (type == "LA")
    {
        limitAngle = 270;
        stepAngle = 90;
    }
    
    if (rotateTo == 'R')
    {
	    if (antAngle < limitAngle)
	    {
		    antAngle = antAngle + stepAngle;
	    }
	    else
	    {
		    antAngle = 0;
	    }
    }
    else if (rotateTo == 'L')
    {
	    if (antAngle != 0)
	    {
		    antAngle = antAngle - stepAngle;
	    }
	    else
	    {
		    antAngle = limitAngle;
	    }
    }
    
	return 0;
}

/*
 * Paints the cell where the ant is.
 * The cell is painted black if it was white
 * and white if it was black.
 */
int CellularAutomata::paintAnt()
{
	unsigned short tempColor;
	
	if (fb[antPosY * SCREEN_WIDTH + antPosX] == FG_color)
	{
		tempColor = BG_color;
	}
	else
	{
		tempColor = FG_color;
	}

    if (type == "LHA")
    {
	    paintHexCell(antPosX, antPosY, tempColor, fb);
	}
	else if (type == "LA")
	{
	    for(int i = 0; i < antNumPixels; i++)
	    {
		    for(int j = 0; j < antNumPixels; j++)
		    {		
			    fb[(antPosY + j) * SCREEN_WIDTH + (antPosX + i)] = tempColor;
		    }
	    }
    }	
	return 0;
}

/*
 * Moves the ant forward to the next cell
 */
int CellularAutomata::forwardAnt()
{
    if (type == "LHA")
    {
	    switch(antAngle)
	    {
		    case 0:
		     	antPosY = antPosY - 4;
			    break;
		    case 60:
			    antPosX = antPosX + 4;
			    antPosY = antPosY - 2;
			    break;
		    case 120:
			    antPosX = antPosX + 4;
			    antPosY = antPosY + 2;
			    break;

		    case 180:
			    antPosY = antPosY + 4;
			    break;
		    case 240:
			    antPosX = antPosX - 4;
			    antPosY = antPosY + 2;
			    break;
		    case 300:
			    antPosX = antPosX - 4;
			    antPosY = antPosY - 2;
			    break;
	    }
    }
    else if (type == "LA")
    {
    	switch (antAngle)
	    {
		    case 0:
			    antPosX = antPosX + antNumPixels;
			    break;
		    case 90:
			    antPosY = antPosY - antNumPixels;
			    break;
		    case 180:
			    antPosX = antPosX - antNumPixels;
			    break;
		    case 270:
			    antPosY = antPosY + antNumPixels;
			    break;
	    }
    }
	return 0;
}

/*
 * Draw the square in the position (column, row) with the selected color
 * where column is a value in the range 0-63 and row is a value in the range 0-63.
 * To calculate the real x and y of the pixel in the upper left of the square:
 *      x = width * column + 32
 *      y = width * row
 * We add 32 to the x value to center horizontaly the image in the screen
 * (the width of the screen is 256 pixels so 256 - 32 * 2 = 192 pixels)
 * <---32---><---192---><---32--->
 *    0-31      32-191    192-255
 *
 * For example, if column = 0, row = 1 and width = 3:
 * The upper left pixel is:
 *      x = 3 * 0 + 32 = 32
 *      y = 3 * 1 = 3
 * We draw horizontal lines that are 3 pixels long starting from:
 *  k = 0: (32 + 3 * 0, 3 * 1 + 0) that's (32, 3)
 *  k = 1: (32 + 3 * 0, 3 * 1 + 1) that's (32, 4)
 *  k = 2: (32 + 3 * 0, 3 * 1 + 2) that's (32, 5) 
 *      31 32 33 34 35
 *    2  
 *    3     x  x  x
 *    4     x  x  x
 *    5     x  x  x
 *    6
 */
int CellularAutomata::drawMunchingSquare(int column, int row, int width, unsigned short color)
{
    for (int k = 0; k < width; k++)
    {
        drawHLine(32 + width * column, width * row + k, width, color, fb);
    }
    
    return 0;
}

//*************************************PUBLIC*******************************************

int CellularAutomata::setType(string t)
{
    type = t;
    
    return 0;
}

const char* CellularAutomata::getType()
{
    return type.c_str();
}

int CellularAutomata::getNumSteps()
{
    return numSteps;
}

int CellularAutomata::setAntNumPixels(int numPixels)
{
    antNumPixels = numPixels;
    
    return 0;
}

int CellularAutomata::getAntNumPixels()
{
    return antNumPixels;
}

/*
 * Cleans the main framebuffer 
 * and initiliazes the values of the variables used for the cellular automata.
 */
int CellularAutomata::initialize()
{   
    showFB();
    cleanFB(fb);
    
    numSteps = 0; // Reset the number of steps to 0

    if (type == "LHA")
    { 
        drawHexGrid();
        antPosX = 92;
        antPosY = 93;
        antAngle = 0;
        antFinished = false;
    }

    if (type == "LA")
    {
        antPosX = 127;
        antPosY = 95;
        antAngle = 90;
        antFinished = false;
    }

    return 0;
}

/*
 *
 */
int CellularAutomata::nextStep()
{
    /*
     * If the type of the automata is Langton's ant or Langton's hexagonal ant, this function combines 3 functions:
     * First, depending on the color of the cell, rotates the ant to one side or the other using rotateAnt().
     * Next, paints the cell using paintAnt().
     * Finally, moves the ant to the next cell using forwardAnt().
     */
    if (type == "LA" or type == "LHA")
    {
        if (fb[antPosY * SCREEN_WIDTH + antPosX] == BG_color)
        {
	        rotateAnt('R');
	    }
	    else
	    {
	        rotateAnt('L');
	    }
	        
	    paintAnt();
	    forwardAnt();
	    ++numSteps;
	        
	    // Check if the ant has reached any border of the screen.
	    if (antPosX < 0 or antPosX + antNumPixels > 254 or antPosY < 0 or antPosY + antNumPixels > SCREEN_HEIGHT - 1)
	    {
	        antFinished = true;
	    }
    }
    /* Else if the type of the automata is Munching squares , it draws the next step of the munching squares.
     * We use a grid of 64 squares height x 64 squares width where each square is 3 pixels x 3 pixels
     * 64 * 3 = 192 pixels (the height of the screen)
     * See the comments of the function drawSquare above.
     * 
     * XOR is the logical operator "exclusive disjunction", also called "exclusive or".
     * http://en.wikipedia.org/wiki/XOR#Bitwise_operation
     * XOR is a type of logical disjunction on two operands that results in a value of true if exactly one of the operands has a value of true.
     * 1 xor 1 = 0
     * 1 xor 0 = 1
     * 0 xor 1 = 1
     * 0 xor 0 = 0
     * For example, 1110 xor 1001 = 0111
     *
     */
    else if (type == "MS")
    {
        if (numSteps < 64)   
        {
            // For each combination of i and j where i = 0, 1, ... 63 and j = 0, 1, ... 63
            // we check if the value of (i xor j) is smaller than numSteps.
            // If the result of this comparation is true then we draw a square of 3 pixels x 3 pixels using FG color
            // using the function drawMunchingSquare starting from the column i and the row j.
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 64; j++)
                {	
				    if (((i xor j) < numSteps) == true)
				    {
					    drawMunchingSquare(i, j, 3, FG_color);
				    }
                }
            }
		    numSteps++;
        }
        else
        {
            initialize(); // Paint the framebuffer with the BG color to erase the last step of the munching squares and initialize the variables to start another cycle
        }            
    }

	return 0;
}

/*
 *
 */
bool CellularAutomata::hasFinished()
{
    return antFinished;
}
