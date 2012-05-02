#include "cellularautomata.h"
#include <nds.h>
#include "general.h"
#include "color.h"
#include "hexgrid.h"

/*
 * References: 
 * http://en.wikipedia.org/wiki/Langton%27s_ant
 * http://mathworld.wolfram.com/LangtonsAnt.html
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
 * Initializes the Langton's ant
 */
int CellularAutomata::initializeAnt(unsigned short intAntPosX, unsigned short intAntPosY, unsigned short intAntAngle)
{   
    showFB();
    cleanFB(fb);
    
    if (type == "LHA")
    { 
        drawHexGrid();
    }
    
    antPosX = intAntPosX;
    antPosY = intAntPosY;
    antAngle = intAntAngle;
    numSteps = 0;
    antFinished = false;
    
    return 0;
}

/*
 * This function combines 3 functions:
 * First, depending on the color of the cell, rotates the ant to one side or the other using rotateAnt().
 * Next, paints the cell using paintAnt().
 * Finally, moves the ant to the next cell using forwardAnt().
 */
int CellularAutomata::nextStep()
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

	return 0;
}

/*
 *
 */
bool CellularAutomata::hasFinished()
{
    return antFinished;
}
