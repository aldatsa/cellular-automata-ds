#include "cellularautomata.h"
#include <nds.h>
#include "general.h"
#include "color.h"
#include "hexgrid.h"

//*************************************PRIVATE*******************************************

/*
 *
 */
int CellularAutomata::rotateAnt(unsigned char rotateTo)
{
    if (type == "LHA")
    {
	    if (rotateTo == 'R') // right 60
	    {
		    if (antAngle < 300)
		    {
			    antAngle = antAngle + 60;
		    }
		    else
		    {
			    antAngle = 0;
		    }
	    }
	    else if (rotateTo == 'L') // left 60
	    {
		    if (antAngle != 0)
		    {	
			    antAngle = antAngle - 60;	
		    }	
		    else
		    {
			    antAngle = 300;
		    }
	    }
    }
	return 0;
}

/*
 *
 */
int CellularAutomata::paintAnt()
{

	unsigned short tempColor;

    if (type == "LHA")
    {
	    if (fb[antPosY * SCREEN_WIDTH + antPosX] == FG_color)
	    {
		    tempColor = BG_color;
	    }
	    else
	    {
		    tempColor = FG_color;
	    }

	    paintHexCell(antPosX, antPosY, tempColor, fb);
    }
    	
	return 0;
}

/*
 *
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

/*
 *
 */
int CellularAutomata::initializeAnt(unsigned short intAntPosX, unsigned short intAntPosY, unsigned short intAntAngle)
{   
    showFB();
    cleanFB(fb);
    
    if (type == "LHA")
    { 
        drawHexGrid();
    }
    
    antNumPixels = 4; // Only used by the normal ant not by the hexagonal one
    
    antPosX = intAntPosX;
    antPosY = intAntPosY;
    antAngle = intAntAngle;
    numSteps = 0;
    antFinished = false;
    
    return 0;
}

/*
 *
 */
int CellularAutomata::nextStep()
{
    if (type == "LHA")
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
    	
	return 0;
}

/*
 *
 */
bool CellularAutomata::hasFinished()
{
    return antFinished;
}
