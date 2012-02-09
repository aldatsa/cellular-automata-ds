/*
    Copyright 2012 Asier Iturralde Sarasola
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <nds.h>
#include <stdio.h>
#include <cmath>    // To calculate powers of 2 in the calculateRuleNumber function. Is it possible to do it in a more simple way? Using binary operators?
#include <stdlib.h>

#include "flash.h"  // Include for the header of the flash image (grit outputs a nice header to reference data)


/*
 * Constant for the version number of the application
 */
#define VERSION "0.2.3"

/*
 * Constants for the rows where the menu items of the automata type selection menu are printed
 */
#define MENU_AUTOMATA_TYPE_ROW_ECA 8
#define MENU_AUTOMATA_TYPE_ROW_LA 9
#define MENU_AUTOMATA_TYPE_ROW_LHA 10
#define MENU_AUTOMATA_TYPE_ROW_BA 11
#define MENU_AUTOMATA_TYPE_ROW_BHA 12
#define MENU_AUTOMATA_TYPE_ROW_BTA 13
#define MENU_AUTOMATA_TYPE_ROW_MS 14

/*
 * Constant for the row where the rule of the Elementary Cellular Automaton is printed
 */
#define ECA_RULE_NUMBER_ROW 9

/*
 * Constants for the variable automataType:
 * 0: Elementary Cellular Automata
 * 1: Langton's ant
 * 2: Langton's hexagonal ant
 * 3: Boolean Square Automata
 * 4: Boolean Hexagonal Automata
 * 5: Boolean Triangular Automata
 * 6: Munching Squares
 */
#define ELEMENTARY_CELLULAR_AUTOMATA 0
#define LANGTON_ANT 1
#define LANGTON_HEXAGONAL_ANT 2
#define BOOLEAN_AUTOMATA 3
#define BOOLEAN_HEXAGONAL_AUTOMATA 4
#define BOOLEAN_TRIANGULAR_AUTOMATA 5
#define MUNCHING_SQUARES 6

/*
 * Variables for the colors of the background, the elements on the foreground and the lines
 */
unsigned short BG_color = RGB15(31,31,31);      // Color used on the BackGround (Default: White)
unsigned short FG_color = RGB15(0,0,0);         // Color used on the ForeGround (Default: Black)
unsigned short line_color = RGB15(15,15,15);    // Color used to draw the lines (Default: Grey)

/*
 * The rules for the Elementary Cellular Automata
 *  xxx xxo xox xoo oxx oxo oox ooo
 *   o   x   o   x   x   o   x   o  -> Rule 90: 0*128 + 1*64 + 0*32 + 1*16 + 1*8 + 0*4 + 1*2 + 0*1
 *
 * where x == Foreground color with value 1
 *       o == Background color with value 0
 *
 *       ruleLeft  ruleCenter  ruleRight
 *                  ruleDown
 */
unsigned short ruleLeft[8] = {FG_color,FG_color,FG_color,FG_color,BG_color,BG_color,BG_color,BG_color};
unsigned short ruleCenter[8] = {FG_color,FG_color,BG_color,BG_color,FG_color,FG_color,BG_color,BG_color};
unsigned short ruleRight[8] = {FG_color,BG_color,FG_color,BG_color,FG_color,BG_color,FG_color,BG_color};

// The rule that will be displayed on start on the Elementary Cellular Automata
unsigned short ruleDown[8] = {BG_color,FG_color,BG_color,FG_color,FG_color,BG_color,FG_color,BG_color}; //Rule 90 (Default)

/*
 * Pointer to the start of VRAM_A, the memory that we'll use as the main framebuffer
 */ 
unsigned short* fb = VRAM_A;

/*
 * Pointer to the start of VRAM_B, the memory that we'll use as the secondary framebuffer
 */ 
unsigned short* fb2 = VRAM_B;

/*
 * A variable for the position of the touch
 */
touchPosition touch;

/* This variable sets the type of automata that is going to execute
 * 0: Elementary Cellular Automata
 * 1: Langton's ant
 * 2: Langton's hexagonal ant
 * 3: Boolean Square automata
 * 4: Boolean Hexagonal automata
 * 5: Munching Squares
 */
int automataType = ELEMENTARY_CELLULAR_AUTOMATA;

/* This variable sets the menu that is going to be displayed
 * 0: Select automata menu
 * 1: Elementary cellular automata menu
 * 2: Langton's ant menu
 * 3: Langton's hexagonal ant menu
 * 4: Boolean automata menu
 * 5: Boolean Hexagonal Automata menu
 * 6: Munching squares menu
 */
int displayedMenu = 0;

/* The position of the arrow
 * Elementary Cellular Automata: in the top screen (0-7) and in the bottom screen (8)
 * Langton's ant: ????
 * Langton's hexagonal ant: ????
 * Boolean automata menu: ?????
 * Boolean Hexagonal Automata menu: ????
 * Boolean Triangular Automata menu: ????
 * Munching Squares: in the bottom screen (0-2)
 */
int intArrow = 0;

/*
 * Variables used for the munching squares
 */
int munchingSquaresNumSteps = 0; // This variable is used to count the number of steps of the Munching Squares
int munchingSquaresThreshold = 0;
int munchingSquaresOptionComp = 0; // 0 smaller than; 1 equal to;
int munchingSquaresOptionOp = 0; // 0 XOR; 1 AND;
bool munchingSquaresCondition = false; // if true draw square else don't

/*
 * Variables used for the Langton's ant (normal and hexagonal)
 */
unsigned short antPosX;
unsigned short antPosY;
unsigned short antAngle;
unsigned int antNumSteps;
unsigned short antNumPixels = 4; // Only used by the normal ant not by the hexagonal one
bool antStop = false;

/*
 * Variables used for the boolean automaton
 */
int automataSteps = 0; // It's equivalent to antNumSteps, I should use only one of them (Used in Boolean Square Automata, Boolean Hexagonal Automata and Boolean Triangular Automata)
int intTypeOfNeighborhood = 0; // 0: Von Neumann - 1: Moore (Used in Boolean Square Automata and Boolean Triangular Automata)
int intBooleanRulesValuesSqVN [4] = {1, 0, 0, 0};    // {1, 2, 3, 4} For the Boolean Square Automata with Von Neumann neighborhood
int intBooleanRulesValuesSqM [8] = {1, 0, 0, 0, 0, 0, 0, 0}; // {1, 2, 3, 4, 5, 6, 7, 8} For the Boolean Square Automata with Moore neighborhood
int intBooleanRulesValuesHex [6] = {1, 0, 0, 0, 0, 0}; // {1, 2, 3, 4, 5, 6}; For the Boolean Hexagonal Automata
int intBooleanRulesValuesTriVN [3] = {1, 0, 0}; // {1, 2, 3}; For the Boolean Triangular Automata with Modified Von Neumann neighborhood
int intBooleanRulesValuesTriM [8] = {1, 0, 0, 0, 0, 0, 0, 0}; // {1, 2, 3, 4, 5, 6, 7, 8}; For the Boolean Triangular Automata with Modified Moore neighborhood

/*********************************** START GENERAL FUNCTIONS *************************************************************************/

/*
 * int drawHLine(int column, int row, int lenght, unsigned short color)
 * Draws a horizontal line of the specified color
 */
int drawHLine(int column, int row, int lenght, unsigned short color, unsigned short* framebuffer)
{
	for(int i = column; i < column + lenght; i++)
	{
		framebuffer[row * 256 + i] = color;
	}
	return 0;
}

/*
 * int drawVLine(int column, int row, int lenght, unsigned short color)
 * Draws a vertical line of the specified color
 */ 
int drawVLine(int column, int row, int lenght, unsigned short color)
{
    for(int i = row; i < row + lenght; i++)
    {
        fb[i * 256 + column] = color;
    }
    return 0;
}

/*
 * int cleanScreen()
 * Fills the main framebuffer with the background color
 */
int cleanScreen()
{
	int row, column;

	for(row = 0; row < 192; row++)
	{
	    for(column = 0; column < 256; column++)
	    {
		    fb[row*256 + column] = BG_color;
		}
    }
	return 0;
}

/*
 * int cleanScreen2()
 * Fills the secondary framebuffer (fb2) with the background color
 * I should use only one function, not two (cleanScreen() and cleanScreen2())
 */
int cleanScreen2()
{
	int row, column;

	for(row = 0; row < 192; row++)
	{
		for(column = 0; column < 256; column++)
		{
			fb2[row*256 + column] = BG_color;
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

/*********************************** END GENERAL FUNCTIONS *************************************************************************/

/*********************************** START ELEMENTARY CELLULAR AUTOMATA FUNCTIONS **************************************************/

/*
 * References:
 * http://mathworld.wolfram.com/ElementaryCellularAutomaton.html
 * http://atlas.wolfram.com/01/01/
 * http://en.wikipedia.org/wiki/Cellular_automaton#Elementary_cellular_automata
 * http://en.wikipedia.org/wiki/Elementary_cellular_automaton
 * 
 */
 
/*
 * int paintInitialCell()
 * Paints the initial cell in the center of the first row
 */
int paintInitialCell()
{
    fb[128] = FG_color;
    
    return 0;
}

/*
 * int calculateRuleNumber()
 * Calculates the rule number for the current rule of the Elementary Cellular Automata
 */ 
int calculateRuleNumber()
{
    int ruleNumber = 0;
    
    for (int i = 0; i < 8; i++)
    {
        if (ruleDown[i] == FG_color)
        {
            ruleNumber = ruleNumber + pow(2, 7 - i);
        }
    }
    
    return ruleNumber;
}

/*
 *  int printRuleNumber(int intRuleNumber)
 *  Converts to char array and prints the given intRuleNumber of the Elementary Cellular Automata
 */
int printRuleNumber(int intRuleNumber)
{
    
    // Convert intRuleNumber to char array
    char buffer [3];
    sprintf(buffer, "%d", intRuleNumber);
    
    // Print 3 spaces to erase the previous rule
    iprintf("\x1b[%d;2HRule Number:   ", ECA_RULE_NUMBER_ROW);
    
    // Print it in the appropiate position
    iprintf("\x1b[%d;2HRule Number:%s", ECA_RULE_NUMBER_ROW, buffer);
    
    return 0;
}

/*
 * int drawArrow(char nth, unsigned short color)
 * Draws an arrow in the top screen (0-7) of the Elementary Cellular Automata
 */
int drawArrow(char nth, unsigned short color)
{	
	
	int intRow = 0;
	int intColumn = 0;

	switch (nth)
	{
		case 0 :
			intRow = 149;
			intColumn = 38;
		 	break;
		case 1 :
			intRow = 149;
			intColumn = 94;
		 	break;		
		case 2 :
			intRow = 149;
			intColumn = 150;
		 	break;	
		case 3 :
			intRow = 149;
			intColumn = 206;
		 	break;	
		case 4 :
			intRow = 181;
			intColumn = 38;
		 	break;
		case 5 :
			intRow = 181;
			intColumn = 94;
		 	break;		
		case 6 :
			intRow = 181;
			intColumn = 150;
		 	break;	
		case 7 :
			intRow = 181;
			intColumn = 206;
		 	break;	
	}
	
	drawHLine(intColumn - 3, intRow - 3, 1, color, fb);
	drawHLine(intColumn - 3, intRow - 2, 2, color, fb);
	drawHLine(intColumn - 6, intRow - 1,  6, color, fb);
	drawHLine(intColumn - 6, intRow, 7, color, fb);
	drawHLine(intColumn - 6, intRow + 1, 6, color, fb);
	drawHLine(intColumn - 3, intRow + 2, 2, color, fb);
	drawHLine(intColumn - 3, intRow + 3, 1, color, fb);

	return 0;
}

/*
 * int drawRectangle(char chrYesNo, int intRowStart, int intColumnStart, int length, int width)
 * Draws a rectangle, filled with FG_color or BG_color
 * This function is used by drawRule() to draw the rectangles to visualize the current rule
 */
int drawRectangle(char chrYesNo, int intRowStart, int intColumnStart, int length, int width)
{
	// chrYesNo	1 -> Yes, fill with FG_color		2 -> No, fill with BG_color

	unsigned char column = 0;
	unsigned char row = 0;
	unsigned short color;
	
	drawHLine(intColumnStart, intRowStart, width, line_color, fb);
	drawHLine(intColumnStart, intRowStart + length, width, line_color, fb);
	drawVLine(intColumnStart, intRowStart, length, line_color);
	drawVLine(intColumnStart + width, intRowStart, length, line_color);
	
	if(chrYesNo == 1)
	{
		color = FG_color;		
	}
	else
	{
		color = BG_color;
	}
	
	for(column = intColumnStart + 1; column < intColumnStart + width; column++)
	{
		for(row = intRowStart + 1; row < intRowStart + length; row++)
		{
			fb[row * 256 + column] = color;
		}
	} 
	
	return 0;
}

/*
 * int drawRule(int nth)
 * Draws the current rule of the Elementary Cellular Automata
 */
int drawRule(int nth)
{
	char chrYesNo = 0;
	
	int intRowStart = 0;
	int intColumnStart = 0;

	int intLength = 8;
	int intWidth = 8;
	
	switch (nth)
	{
		case 0 :
			intRowStart = 136;
			intColumnStart = 32;
		 	break;
		case 1 :
			intRowStart = 136;
			intColumnStart = 88;
		 	break;		
		case 2 :
			intRowStart = 136;
			intColumnStart = 144;
		 	break;	
		case 3 :
			intRowStart = 136;
			intColumnStart = 200;
		 	break;	

		case 4 :
			intRowStart = 168;
			intColumnStart = 32;
		 	break;
		case 5 :
			intRowStart = 168;
			intColumnStart = 88;
		 	break;		
		case 6 :
			intRowStart = 168;
			intColumnStart = 144;
		 	break;	
		case 7 :
			intRowStart = 168;
			intColumnStart = 200;
		 	break;	
	}

	if(ruleLeft[nth] == FG_color)
	{
		chrYesNo = 1;	
	}
	
	drawRectangle(chrYesNo, intRowStart, intColumnStart, intLength, intWidth);

	chrYesNo = 0;

	if(ruleCenter[nth] == FG_color)
	{
		chrYesNo = 1;	
	}
		
	drawRectangle(chrYesNo, intRowStart, intColumnStart + 8, intLength, intWidth);

	chrYesNo = 0;

	if(ruleRight[nth] == FG_color)
	{
		chrYesNo = 1;	
	}

	drawRectangle(chrYesNo, intRowStart, intColumnStart + 16, intLength, intWidth);

	chrYesNo = 0;

	if(ruleDown[nth] == FG_color)
	{
		chrYesNo = 1;	
	}

	drawRectangle(chrYesNo, intRowStart + 8, intColumnStart + 8, intLength, intWidth);
	
	return 0;
}

/*
 * int drawElementaryCellularAutomata()
 * Draws the Elementary Cellular Automata that corresponds to the current rules
 */
int drawElementaryCellularAutomata()
{
	int row, column;
	unsigned char i;
	
	cleanScreen();

    paintInitialCell(); // Paints the initial cell in the center of the first row (fb[128] = FG_color)

	for(row = 0; row < 130; row++)
	{
		for(column = 0; column < 256; column++)
		{
			if(row != 0 && row < 128)
			{
				for(i=0;i<=7;i++)
				{
					if(column != 0 && column != 255)
					{ 
						if (fb[(row - 1)*256 + (column-1)] == ruleLeft[i] && fb[(row - 1)*256 + column] == ruleCenter[i] && fb[(row - 1)*256 + (column+1)] == ruleRight[i])
							fb[row*256 + column] = ruleDown[i];
					}
					else if(column == 0)
					{
						// The left cell is out of the screen, instead we'll use the center cell (column 0) to compare to ruleLeft
						if(fb[(row - 1)*256 + column] == ruleLeft[i]  && fb[(row - 1)*256 + column] == ruleCenter[i] && fb[(row - 1)*256 + (column+1)] == ruleRight[i])
						{
							fb[row*256 + column] = ruleDown[i];								
						}
					}	
					else if(column == 255)
					{
                        // The right cell is out of the screen, instead we'll use the center cell (column 255) to compare to ruleRight
						if(fb[(row - 1)*256 + (column-1)] == ruleLeft[i] && fb[(row - 1)*256 + column] == ruleCenter[i] && fb[(row - 1)*256 + column] == ruleRight[i])
                        {
							fb[row*256 + column] = ruleDown[i];								
                        }
					}
				}
			}
			else if(row == 129)
			{
				fb[row*256 + column] = FG_color;
			}
		}
	}
	
	// Draw the rules (0-7)
	for (int i = 0; i < 8; i++)
	{
	    drawRule(i);
	}

    // Draw the arrow 
	drawArrow(intArrow, line_color);
	
	return 0;
}
/*********************************** END ELEMENTARY CELLULAR AUTOMATA FUNCTIONS ********************************************/

/*********************************** START MUNCHING SQUARES FUNCTIONS ******************************************************/

/*
 * References: 
 * http://en.wikipedia.org/wiki/Munching_square
 * http://mathworld.wolfram.com/MunchingSquares.html
 * http://www.inwap.com/pdp10/hbaker/hakmem/hacks.html#item146
 */

/*
 *
 */
int drawSquare(int column, int row, int width, unsigned short color)
{
    for (int k = 0; k < width; k++)
    {
        drawHLine(32 + width * column, width * row + k, width, color, fb);
    }
    
    return 0;
}

/*
 *
 */
int initializeMunchingSquares()
{
    cleanScreen();
    munchingSquaresNumSteps = 0;
    munchingSquaresCondition = false;
    
    return 0;
}

/*
 *
 */
int drawNextStepMunchingSquares()
{
    if (munchingSquaresNumSteps < 64)   
    {
        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 64; j++)
            {
				if (munchingSquaresOptionComp == 0)
				{
					if (munchingSquaresOptionOp == 0)
					{
						munchingSquaresCondition = ((i xor j) < (munchingSquaresNumSteps + munchingSquaresThreshold));
					}
					else if (munchingSquaresOptionOp == 1)
					{
						munchingSquaresCondition = ((i and j) < (munchingSquaresNumSteps + munchingSquaresThreshold));
					}
				}
				else if (munchingSquaresOptionComp == 1)
				{
                     if (munchingSquaresOptionOp == 0)
                     {
                     	munchingSquaresCondition = ((i xor j) == (munchingSquaresNumSteps + munchingSquaresThreshold));
                     }
                     else if (munchingSquaresOptionOp == 1)
                     {
                         munchingSquaresCondition = ((i and j) == (munchingSquaresNumSteps + munchingSquaresThreshold));
                     }
				}
				if (munchingSquaresCondition == true)
				{
					drawSquare(i, j, 3, FG_color);
				}
            }
        }
		munchingSquaresNumSteps++;
    }
    else
    {
        cleanScreen();
        initializeMunchingSquares();
    }    
    
    return 0;
}

/*********************************** END MUNCHING SQUARES FUNCTIONS ********************************************************/

/*********************************** START LANGTON'S ANT FUNCTIONS *********************************************************/

/*
 * References: 
 * http://en.wikipedia.org/wiki/Langton%27s_ant
 * http://mathworld.wolfram.com/LangtonsAnt.html
 */
 
/*
 * Calculates the new angle of the ant
 */
int rotateAnt(unsigned char rotateTo)
{
	if (rotateTo == 'R') //right 90
	{
		if (antAngle < 270)
		{
			antAngle = antAngle + 90;
		}
		else
		{
			antAngle = 0;
		}
	}
	else if (rotateTo == 'L') //left 90
	{
		if (antAngle != 0)
		{
			antAngle = antAngle - 90;
		}
		else
		{
			antAngle = 270;
		}
	}
	return 0;
}

/*
 * Paints the cell where the ant is.
 * The cell is painted black if it was white
 * and white if it was black.
 */
int paintAnt()
{
	unsigned short tempColor;
	
	if (fb[antPosY * 256 + antPosX] == FG_color)
	{
		tempColor = BG_color;
	}
	else
	{
		tempColor = FG_color;
	}

	for(int i = 0; i < antNumPixels; i++)
	{
		for(int j = 0; j < antNumPixels; j++)
		{		
			fb[(antPosY + j) * 256 + (antPosX + i)] = tempColor;
		}
	}
	return 0;
}

/*
 * Moves the ant forward to the next cell
 */
int forwardAnt()
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
	return 0;
}

/*
 * This function combines the previous 3 functions:
 * First, depending on the color of the cell, rotates the ant to one side or the other.
 * Next, paints the cell.
 * Finally, moves the ant to the next cell.
 */
void stepAnt()
{
	if (fb[antPosY * 256 + antPosX] == BG_color)
	{
		rotateAnt('R');
	}
	else
	{
		rotateAnt('L');
	}

	paintAnt();
	forwardAnt();
	
}

/*
 * Initializes the Langton's ant
 */
int initializeAnt(unsigned short intAntPosX, unsigned short intAntPosY, unsigned short intAntAngle, unsigned short intAntNumPixels)
{
    cleanScreen();
    
    antPosX = intAntPosX;
    antPosY = intAntPosY;
    antAngle = intAntAngle;
    antNumSteps = 0;
    antNumPixels = intAntNumPixels;
    antStop = false;
    
    return 0;
}

/*********************************** END LANGTON'S ANT FUNCTIONS ***********************************************************/

/*********************************** START HEXAGONAL GRID FUNCTIONS ********************************************************/

/*
 *
 */
int hexGridLineOne(int y)
{
	for(int i = 0; i < 32; i++)
	{
		for(int j=0; j<3; j++)
		{
			fb[256 * y + (4 + j + 8 * i)] = line_color;
		}
	}
	return 0;
}

/*
 *
 */
int hexGridLineTwo(int y)
{
	for(int i = 0; i < 64; i++)
	{
		fb[256 * y + (3 + 4 * i)] = line_color;
	}
	return 0;
}

/*
 *
 */
int hexGridLineThree(int y)
{
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			fb[256 * y + (j + 8 * i)] = line_color;
		}
	}
	return 0;
}

/*
 *
 */
int drawHexGrid()
{
    cleanScreen();
    
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

/************************************* END HEXAGONAL GRID FUNCTIONS ********************************************************/

/************************************* START TRIANGULAR GRID FUNCTIONS *****************************************************/

int triangularGridLineOne(int y)
{
	for(int i = 0; i < 255; i++)
	{
    	fb[256 * y + i] = line_color;
	}
	return 0;
}

int triangularGridLineTwo(int y)
{
	for(int i = 3; i < 255; i = i + 6)
	{
		fb[256 * y + i] = line_color;
        fb[256 * y + i + 2] = line_color;
	}
	return 0;
}

int triangularGridLineThree(int y)
{
	for(int i = 0; i < 255; i = i + 6)
	{
		fb[256 * y + i] = line_color;
		fb[256 * y + i + 2] = line_color;
	}
	return 0;
}

int drawTriangularGrid()
{
    cleanScreen();
    
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

int paintTriangularCell(int intPosX, int intPosY, unsigned short color, unsigned short* framebuffer)
{
	if ((intPosX % 2 == 0 and intPosY % 2 == 0) or (intPosX % 2 != 0 and intPosY % 2 != 0))
    {
	    drawHLine(intPosX -1, intPosY, 3, color, framebuffer);  
	    drawHLine(intPosX, intPosY + 1, 1, color, framebuffer); 
    }
    else if ((intPosX % 2 != 0 and intPosY % 2 == 0) or (intPosX % 2 == 0 and intPosY % 2 != 0))
	{
	    drawHLine(intPosX, intPosY, 1, color, framebuffer);     
	    drawHLine(intPosX - 1, intPosY + 1, 3, color, framebuffer);	
	}
	return 0;
}

/************************************* END TRIANGULAR GRID FUNCTIONS *******************************************************/

/****************************** START HEXAGONAL LANGTON'S ANT FUNCTIONS ****************************************************/

/*
 *
 */
int rotateHexAnt(unsigned char rotateTo)
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
	return 0;
}

/*
 *
 */
int paintHexAnt()
{

	unsigned short tempColor;
	
	if (fb[antPosY * 256 + antPosX] == FG_color)
	{
		tempColor = BG_color;
	}
	else
	{
		tempColor = FG_color;
	}

	paintHexCell(antPosX, antPosY, tempColor, fb);
	
	return 0;
}

/*
 *
 */
int forwardHexAnt()
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
	return 0;
}

/*
 *
 */
int stepHexAnt()
{
	if (fb[antPosY * 256 + antPosX] == BG_color)
	{
		rotateHexAnt('R');
	}
	else
	{
		rotateHexAnt('L');
	}

	paintHexAnt();
	forwardHexAnt();
	
	return 0;
}

/*
 *
 */
int initializeHexAnt(unsigned short intAntPosX, unsigned short intAntPosY, unsigned short intAntAngle)
{   
    cleanScreen();
    
    drawHexGrid();
            
    antPosX = intAntPosX;
    antPosY = intAntPosY;
    antAngle = intAntAngle;
    antNumSteps = 0;
    antStop = false;
    
    return 0;
}

/******************************* END HEXAGONAL LANGTON'S ANT FUNCTIONS *****************************************************/

/************************************* START BOOLEAN AUTOMATA FUNCTIONS ****************************************************/

/*
 *
 */
bool booleanRule(int count)
{
    int upperLimit;
    
    if (intTypeOfNeighborhood == 0) // Von Neumann neighborhood
    {
        upperLimit = 4;
    }
    else // Moore neighborhood
    {
        upperLimit = 8;
    }
    
    for (int i = 0; i < upperLimit; i++)
    {
        if (intTypeOfNeighborhood == 0) // Von Neumann neighborhood    
        {
            if (count == intBooleanRulesValuesSqVN[i])
            {
                return true;
            }
        }
        else // Moore neighborhood
        {
            if (count == intBooleanRulesValuesSqM[i])
            {
                return true;
            }            
        }
    }
    return false;
}

/*
 *
 */
bool booleanHexagonalRule(int count)
{
    for (int i = 0; i < 6; i++)
    {
        //if (count == values[i])
        if (count == intBooleanRulesValuesHex[i])
        {
            return true;
        }            
    }
    
    return false;
}

/*
 *
 */
bool booleanTriangularRule(int count)
{
    int upperLimit;
    
    if (intTypeOfNeighborhood == 0) // Von Neumann neighborhood
    {
        upperLimit = 3;
    }
    else // Moore neighborhood
    {
        upperLimit = 8;
    }
    
    for (int i = 0; i < upperLimit; i++)
    {
        if (intTypeOfNeighborhood == 0) // Von Neumann neighborhood    
        {
            if (count == intBooleanRulesValuesTriVN[i])
            {
                return true;
            }
        }
        else // Moore neighborhood
        {
            if (count == intBooleanRulesValuesTriM[i])
            {
                return true;
            }            
        }
    }
    return false;
}

/*
 * Calculates and draws the next step of the boolean square automata.
 * The return value indicates if the automata has finished (return 0)
 * or not (return != 0)
 */
int calculateNextStep(int typeOfNeighborhood)
{
    /* typeOfNeighborhood = 0 Von Neumann neighborhood
     * 4 neighbors.
     *      x
     *    x o x
     *      x
     * http://en.wikipedia.org/wiki/Von_Neumann_neighborhood
     *
     * typeOfNeighborhood = 1 -> Moore neighborhood
     * 8 neighbors.
     *    x x x
     *    x o x
     *    x x x
     * http://en.wikipedia.org/wiki/Moore_neighborhood
     */
          
    unsigned short* fbRef;
    unsigned short* fbNew;
    
    /*
     * changeCount is used to know if the next step is different from the current step.
     * If changeCount == 0 then there're no changes and the automata has finished,
     * so we can start again from step 0.
     * If changeCount != 0 then the automata has not finished yet.
     */ 
    int changeCount = 0; 

    /* This two lines were used to debug the implementation of changeCount (changeCount wasn't 0 when the automata finishes)
    iprintf("\x1b[20;3HChange count:        ");    
    iprintf("\x1b[20;3HChange count: %d", changeCount);
    */
    
    int countFG = 0;
    
    if (automataSteps % 2 == 0 and automataSteps != 1)
    {
        fbRef = fb2;
        fbNew = fb;
    }
    else 
    {
        fbRef = fb;
        fbNew = fb2;
    }
    
    dmaCopy(fbRef, fbNew, 128*1024);
    
    for (int i = 1; i < 254; i++)
    {   
        for (int j = 1; j < 191; j++)
        {
            countFG = 0;        
            

            // top 
            if (fbRef[256 * (j - 1) + i] == FG_color)
            {
                countFG++;
            }
            
            // left
            if (fbRef[256 * j + i - 1] == FG_color)
            {
                countFG++;
            }
            
            // right    
            if (fbRef[256 * j + i + 1] == FG_color)
            {
                countFG++;
            }
            
            // bottom                            
            if (fbRef[256 * (j + 1) + i] == FG_color)
            {
                countFG++;
            }
            
            if (typeOfNeighborhood == 1)
            {
                // Top left
                if (fbRef[256 * (j - 1) + i - 1] == FG_color)
                {
                    countFG++;
                }
                
                // top right                
                if (fbRef[256 * (j - 1) + i + 1] == FG_color)
                {
                    countFG++;
                }
                                
                // Bottom left
                if (fbRef[256 * (j + 1) + i - 1] == FG_color)
                {
                    countFG++;
                }
                
                // bottom right
                if (fbRef[256 * (j + 1) + i + 1] == FG_color)
                {
                    countFG++;
                }            
            }
                        
            if (countFG != 0 and booleanRule(countFG))
            {
                // If the current cell's color is not already changed, change it to FG_color.
                // Without this condition each cell is painted more than one time and changeCount is never equal to 0.
                if (fbNew[256 * j + i] != FG_color) 
                {
                    fbNew[256 * j + i] = FG_color;
                    ++changeCount;
                }
            }
        }
    }
    
    /* This two lines were used to debug the implementation of changeCount (changeCount wasn't 0 when the automata finishes)
    iprintf("\x1b[21;3HChange count end:        ");    
    iprintf("\x1b[21;3HChange count end: %d", changeCount);
    */
    
    return changeCount;
}

/*
 * Calculates and draws the next step of the boolean hexagonal automata.
 * The return value indicates if the automata has finished (return 0)
 * or not (return != 0)
 */
int calculateNextStepHex()
{          
    unsigned short* fbRef;
    unsigned short* fbNew;

    /*
     * changeCount is used to know if the next step is different from the current step.
     * If changeCount == 0 then there're no changes and the automata has finished,
     * so we can start again from step 0.
     * If changeCount != 0 then the automata has not finished yet.
     */ 
    int changeCount = 0; 
        
    int countFG = 0;
    
    if (automataSteps % 2 == 0 and automataSteps != 1)
    {
        fbRef = fb2;
        fbNew = fb;
    }
    else 
    {
        fbRef = fb;
        fbNew = fb2;
    }
    
    dmaCopy(fbRef, fbNew, 128*1024);
    
    for (int i = 8; i < 254; i = i + 8)
    {   
        for (int j = 3; j < 191; j = j + 4)
        {
            countFG = 0;        

            // top left
            if (fbRef[256 * j + i - 2] == FG_color)
            {
                countFG++;
            }

            // top 
            if (fbRef[256 * (j - 2) + i] == FG_color)
            {
                countFG++;
            }
            
            
            // top right
            if (fbRef[256 * j + i + 4] == FG_color)
            {
                countFG++;
            }
            
            // bottom left    
            if (fbRef[256 * (j + 2) + i - 2] == FG_color)
            {
                countFG++;
            }
            
            // bottom                            
            if (fbRef[256 * (j + 4) + i] == FG_color)
            {
                countFG++;
            }

            // bottom right                         
            if (fbRef[256 * (j + 2) + i + 4] == FG_color)
            {
                countFG++;
            }
                        
            if (countFG != 0 and booleanHexagonalRule(countFG))
            {
                // If the current cell's color is not already changed, change it to FG_color.
                // Without this condition each cell is painted more than one time and changeCount is never equal to 0.
                if (fbNew[256 * j + i] != FG_color) 
                {
                    paintHexCell(i, j, FG_color, fbNew);
                    ++changeCount;
                }
            }
        }
    }
    for (int i = 4; i < 252; i = i + 8)
    {   
        for (int j = 5; j < 189; j = j + 4)
        {
            countFG = 0;        

            // top left
            if (fbRef[256 * j + i - 2] == FG_color)
            {
                countFG++;
            }

            // top 
            if (fbRef[256 * (j - 2) + i] == FG_color)
            {
                countFG++;
            }
            
            
            // top right
            if (fbRef[256 * j + i + 4] == FG_color)
            {
                countFG++;
            }
            
            // bottom left    
            if (fbRef[256 * (j + 2) + i - 2] == FG_color)
            {
                countFG++;
            }
            
            // bottom                            
            if (fbRef[256 * (j + 4) + i] == FG_color)
            {
                countFG++;
            }

            // bottom right                         
            if (fbRef[256 * (j + 2) + i + 4] == FG_color)
            {
                countFG++;
            }
                        
            if (countFG != 0 and booleanHexagonalRule(countFG))
            {
                // If the current cell's color is not already changed, change it to FG_color.
                // Without this condition each cell is painted more than one time and changeCount is never equal to 0.
                if (fbNew[256 * j + i] != FG_color) 
                {
                    paintHexCell(i, j, FG_color, fbNew);
                    ++changeCount;
                }
            }
        }
    }
    
    return changeCount;
}

/*
 * Calculates and draws the next step of the boolean triangular automata.
 * The return value indicates if the automata has finished (return 0)
 * or not (return != 0)
 */
int calculateNextStepTriangular(int typeOfNeighborhood)
{          
    /* typeOfNeighborhood = 0 Modified Von Neumann neighborhood
     * 3 neighbors.
     *
     *      x
     *    x o x
     *  
     *  or
     *
     *    x o x
     *      x 
     *
     * http://en.wikipedia.org/wiki/Von_Neumann_neighborhood
     *
     * typeOfNeighborhood = 1 -> Modified Moore neighborhood
     * 8 neighbors.
     *
     *    x x x
     *    x o x
     *    x x x
     *
     * http://en.wikipedia.org/wiki/Moore_neighborhood
     */
    unsigned short* fbRef;
    unsigned short* fbNew;

    /*
     * changeCount is used to know if the next step is different from the current step.
     * If changeCount == 0 then there're no changes and the automata has finished,
     * so we can start again from step 0.
     * If changeCount != 0 then the automata has not finished yet.
     */ 
    int changeCount = 0; 
        
    int countFG = 0;
    
    if (automataSteps % 2 == 0 and automataSteps != 1)
    {
        fbRef = fb2;
        fbNew = fb;
    }
    else 
    {
        fbRef = fb;
        fbNew = fb2;
    }
    
    dmaCopy(fbRef, fbNew, 128*1024);
    
    for (int i = 4; i < 253; i = i + 3)
    {   
        for (int j = 4; j < 190; j = j + 3)
        {
            countFG = 0;        

            // xxx
            //  x
            if (typeOfNeighborhood == 1 or ((i % 2 == 0 and j % 2 == 0) or (i % 2 != 0 and j % 2 != 0)))
            {    
                // top 
                if (fbRef[256 * (j - 2) + i] == FG_color)
                {
                    countFG++;
                }
            }
            //  x
            // xxx
            if (typeOfNeighborhood ==  1 or ((i % 2 != 0 and j % 2 == 0) or (i % 2 == 0 and j % 2 != 0)))
        	{
        	    //bottom
                if (fbRef[256 * (j + 3) + i] == FG_color)
                {
                    countFG++;
                }        	    
        	}
            
            // left
            if (fbRef[256 * j + i - 3] == FG_color)
            {
                countFG++;
            }

            // right
            if (fbRef[256 * j + i + 3] == FG_color)
            {
                countFG++;
            }
                        
            if (typeOfNeighborhood == 1)            
            {                        
                // top left
                if (fbRef[256 * (j - 2) + i - 3] == FG_color)
                {
                    countFG++;
                }
                
                // top right
                if (fbRef[256 * (j - 2) + i + 3] == FG_color)
                {
                    countFG++;
                }
                
                // bottom left    
                if (fbRef[256 * (j + 3) + i - 3] == FG_color)
                {
                    countFG++;
                }

                // bottom right                         
                if (fbRef[256 * (j + 3) + i + 3] == FG_color)
                {
                    countFG++;
                }
            }
            
            if (countFG != 0 and booleanTriangularRule(countFG))
            {
                // If the current cell's color is not already changed, change it to FG_color.
                // Without this condition each cell is painted more than one time and changeCount is never equal to 0.
                if (fbNew[256 * j + i] != FG_color) 
                {
                    paintTriangularCell(i, j, FG_color, fbNew);
                    changeCount++;
                }
            }
        }
    }
    
    return changeCount;
}

/*
 *
 */
int initializeBooleanAutomata(int intX, int intY)
{    
    cleanScreen();
    cleanScreen2();
    
    automataSteps = 0;
        
    fb[intY * 256 + intX] = FG_color;
    
    return 0;
} 

int initializeBooleanHexagonalAutomata(int intX, int intY)
{
    cleanScreen();
    cleanScreen2();
    
    drawHexGrid();
    
    automataSteps = 0;
        
    paintHexCell(intX, intY, FG_color, fb);
    
    return 0;
}

int initializeBooleanTriangularAutomata(int intX, int intY)
{
    cleanScreen();
    cleanScreen2();
    
    drawTriangularGrid();
    
    automataSteps = 0;
        
    paintTriangularCell(intX, intY, FG_color, fb);
    
    return 0;
}

/*************************************** END BOOLEAN AUTOMATA FUNCTIONS ****************************************************/

/******************************************* START MENU FUNCTIONS **********************************************************/

/*
 * Prints app's credits
 */
int printCredits()
{
	printf("Cellular Automata DS\n");
	printf("version %s\n", VERSION);
	printf("\n");
    printf("(c) 2012\n");
    printf("Asier Iturralde Sarasola\n");
    printf("\n");
    
    return 0;
}    

/*
 * Prints the arrow ("->") used to highlight the currently selected menu 
 */
int printArrow(int row, int column)
{
    iprintf("\x1b[%d;%dH->", row, column); // Print the menu arrow
    
    return 0;
}

/*
 * Deletes the arrow ("  ") used to highlight the currently selected menu
 */
int deleteArrow(int row, int column)
{
    iprintf("\x1b[%d;%dH  ", row, column); // Delete the menu arrow
    
    return 0;
}

/*
 * Prints the menu items but without the arrow.
 * Use printMenuArrow() to print the arrow
 */
int printMenu(int intDisplayedMenu)
{
    if (displayedMenu == 0) //The menu to select the type of automata
    {
        iprintf("\x1b[%d;2HElementary Cellular Automata", MENU_AUTOMATA_TYPE_ROW_ECA);
        iprintf("\x1b[%d;2HLangton's ant", MENU_AUTOMATA_TYPE_ROW_LA);
        iprintf("\x1b[%d;2HLangton's hexagonal ant", MENU_AUTOMATA_TYPE_ROW_LHA);
        iprintf("\x1b[%d;2HBoolean automata", MENU_AUTOMATA_TYPE_ROW_BA);
        iprintf("\x1b[%d;2HBoolean Hexagonal automata", MENU_AUTOMATA_TYPE_ROW_BHA);
        iprintf("\x1b[%d;2HBoolean Triangular automata", MENU_AUTOMATA_TYPE_ROW_BTA);
        iprintf("\x1b[%d;2HMunching Squares", MENU_AUTOMATA_TYPE_ROW_MS);
    }
    else if (displayedMenu == 1) // The menu of the Elementary Cellular Automaton
    {
        iprintf("\x1b[11;2HBack to main menu");
    }
    else if (displayedMenu == 2) // The menu of the Langton's ant
    {
        iprintf("\x1b[11;2HAnt's pixels: < %d >", antNumPixels);
        iprintf("\x1b[13;2HBack to main menu");
        printArrow(13, 0);        
    }
    else if (displayedMenu == 3) // The menu of the Langton's hexagonal ant
    {
        iprintf("\x1b[13;2HBack to main menu");
        printArrow(13, 0);
    }
    else if (displayedMenu == 4) // The menu of the boolean automaton
    {
        iprintf("\x1b[13;2H Von Neumann neighborhood");
        iprintf("\x1b[14;5H 1    2    3    4");
        iprintf("\x1b[15;2H Moore neighborhood");
        iprintf("\x1b[16;5H 1    2    3    4");
        iprintf("\x1b[17;5H 5    6    7    8");        
        iprintf("\x1b[19;2HBack to main menu");
        printArrow(13, 0);
    }
    else if (displayedMenu == 5) // The menu of the Boolean Hexagonal Automaton
    {
        iprintf("\x1b[13;2HNeighborhood:");
        iprintf("\x1b[14;5H 1    2    3");
        iprintf("\x1b[15;5H 4    5    6");
        iprintf("\x1b[17;2HBack to main menu");
        printArrow(14, 3);    
    }
    else if (displayedMenu == 6) // The menu of the Boolean Triangular Automaton
    {
        iprintf("\x1b[17;2HBack to main menu");
        printArrow(17, 0);
    }
    else if (displayedMenu == 7) // The menu of the munching squares
    {
        iprintf("\x1b[11;2HType of comparation:");        
        iprintf("\x1b[12;5HSmaller than");
        iprintf("\x1b[13;5HEqual to");    
        
        iprintf("\x1b[14;2HType of boolean operator:");
        iprintf("\x1b[15;5HXOR");
        iprintf("\x1b[16;5HAND");    
            
        iprintf("\x1b[18;2HBack to main menu");
        printArrow(12, 0);        
    }
    return 0;
}

/*
 * Prints the asterisk used to mark the options in the menu of the Munching Squares
 */
int printMSasterisks()
{
    if (munchingSquaresOptionComp == 0)
    {
        iprintf("\x1b[12;3H*");
        iprintf("\x1b[13;3H ");        
    }
    else
    {
        iprintf("\x1b[12;3H ");
        iprintf("\x1b[13;3H*");        
    }
    
    if (munchingSquaresOptionOp == 0)
    {
        iprintf("\x1b[15;3H*");
        iprintf("\x1b[16;3H ");        
    }
    else
    {
        iprintf("\x1b[15;3H ");
        iprintf("\x1b[16;3H*");        
    }
    
    return 0;
}

/*
 * Prints the asterisk used to mark the options in the menu of the Boolean automata
 */
int printBAasterisks()
{
    if (intTypeOfNeighborhood == 0)
    {
        iprintf("\x1b[13;2H*");
        iprintf("\x1b[15;2H ");        
    }
    else
    {
        iprintf("\x1b[13;2H ");
        iprintf("\x1b[15;2H*");        
    }
    
    // {1, 2, 3, 4} For the boolean square automata with Von Neumann neighborhood
    if (intBooleanRulesValuesSqVN[0] == 1)
    {
        iprintf("\x1b[14;5H*");    
    }
    else
    {
        iprintf("\x1b[14;5H ");    
    }
    
    if (intBooleanRulesValuesSqVN[1] == 2) 
    {
        iprintf("\x1b[14;10H*");    
    }
    else
    {
        iprintf("\x1b[14;10H ");    
    }
            
    if (intBooleanRulesValuesSqVN[2] == 3) 
    {
        iprintf("\x1b[14;15H*");    
    }
    else
    {
        iprintf("\x1b[14;15H ");    
    }
            
    if (intBooleanRulesValuesSqVN[3] == 4) 
    {
        iprintf("\x1b[14;20H*");    
    }
    else
    {
        iprintf("\x1b[14;20H ");    
    }
            
    // {1, 2, 3, 4, 5, 6, 7, 8} For the boolean square automata with Moore neighborhood
    if (intBooleanRulesValuesSqM[0] == 1) 
    {
        iprintf("\x1b[16;5H*");    
    }
    else
    {
        iprintf("\x1b[16;5H ");    
    }
    
    if (intBooleanRulesValuesSqM[1] == 2) 
    {
        iprintf("\x1b[16;10H*");    
    }
    else
    {
        iprintf("\x1b[16;10H ");    
    }        
    
    if (intBooleanRulesValuesSqM[2] == 3) 
    {
        iprintf("\x1b[16;15H*");    
    }
    else
    {
        iprintf("\x1b[16;15H ");    
    }
        
    if (intBooleanRulesValuesSqM[3] == 4) 
    {
        iprintf("\x1b[16;20H*");    
    }
    else
    {
        iprintf("\x1b[16;20H ");    
    }
        
    if (intBooleanRulesValuesSqM[4] == 5) 
    {
        iprintf("\x1b[17;5H*");    
    }
    else
    {
        iprintf("\x1b[17;5H ");    
    }
        
    if (intBooleanRulesValuesSqM[5] == 6) 
    {
        iprintf("\x1b[17;10H*");    
    }
    else
    {
        iprintf("\x1b[17;10H ");    
    }
            
    if (intBooleanRulesValuesSqM[6] == 7) 
    {
        iprintf("\x1b[17;15H*");    
    }
    else
    {
        iprintf("\x1b[17;15H ");    
    }
        
    if (intBooleanRulesValuesSqM[7] == 8)     
    {
        iprintf("\x1b[17;20H*");    
    }
    else
    {
        iprintf("\x1b[17;20H ");    
    }
    
    return 0;
}

/*
 * Prints the asterisk used to mark the options in the menu of the Boolean hexagonal automata
 */
int printBHAasterisks()
{
    // {1, 2, 3, 4, 5, 6} For the boolean square automata with Moore neighborhood
    if (intBooleanRulesValuesHex[0] == 1) 
    {
        iprintf("\x1b[14;5H*");    
    }
    else
    {
        iprintf("\x1b[14;5H ");    
    }
    
    if (intBooleanRulesValuesHex[1] == 2) 
    {
        iprintf("\x1b[14;10H*");    
    }
    else
    {
        iprintf("\x1b[14;10H ");    
    }        
    
    if (intBooleanRulesValuesHex[2] == 3) 
    {
        iprintf("\x1b[14;15H*");    
    }
    else
    {
        iprintf("\x1b[14;15H ");    
    }
        
    if (intBooleanRulesValuesHex[3] == 4) 
    {
        iprintf("\x1b[15;5H*");    
    }
    else
    {
        iprintf("\x1b[15;5H ");    
    }
        
    if (intBooleanRulesValuesHex[4] == 5) 
    {
        iprintf("\x1b[15;10H*");    
    }
    else
    {
        iprintf("\x1b[15;10H ");    
    }
        
    if (intBooleanRulesValuesHex[5] == 6) 
    {
        iprintf("\x1b[15;15H*");    
    }
    else
    {
        iprintf("\x1b[15;15H ");    
    }
    
    return 0;
}

/*
 * Prints the arrow used to highlight the currently selected menu item
 */
int printMenuArrow(int intDisplayedMenu, int index, bool boolDelete)
{
    int row;
    int column = 0;
    
    if (intDisplayedMenu == 0) // Main menu
    {
        if (index == 0) // Elementary Cellular Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_ECA;
        }
        else if (index == 1) // Langton's ant
        {
            row = MENU_AUTOMATA_TYPE_ROW_LA;
        }
        else if (index == 2) // Langton's hexagonal ant
        {
            row = MENU_AUTOMATA_TYPE_ROW_LHA;
        }        
        else if (index == 3) // Boolean automata
        {    
            row = MENU_AUTOMATA_TYPE_ROW_BA;
        }
        else if (index == 4) // Boolean Hexagonal Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_BHA;
        }
        else if (index == 5) // Boolean Triangular Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_BTA;
        }
        else if (index == 6) // Munching squares
        {    
            row = MENU_AUTOMATA_TYPE_ROW_MS;
        }        
    }
    
    else if (intDisplayedMenu == 1) // Elementary Cellular Automaton menu
    {
        if (index == 8) // The first element of the bottom screen
        {
            row = 11;
        }
    }
    else if (intDisplayedMenu == 2) // Langton's ant menu
    {
        if (index == 0) // Print the arrow in the option to choose the size of the ant
        {
            row = 11;
        }
        else if (index == 1) // Print the arrow in the option to go back to main menu
        {
            row = 13;
        }
    }
    else if (intDisplayedMenu == 3) // Langton's hexagonal ant menu
    {
        if (index == 0)
        {
            row = 13;
        }
    }
    else if (intDisplayedMenu == 4) // Boolean automata
    {
        if (index == 0) // Von Neumann neighborhood
        {
            row = 13;
        }
        else if (index == 1) // 1
        {
            row = 14;
            column = 3;
        }
        else if (index == 2) // 2
        {
            row = 14;
            column = 8;
        }
        else if (index == 3) // 3
        {
            row = 14;
            column = 13;
        }
        else if (index == 4) // 4
        {
            row = 14;
            column = 18;
        }
        else if (index == 5) // Moore neighborhood
        {
            row = 15;
        }
        else if (index == 6) // 1
        {
            row = 16;
            column = 3;
        }
        else if (index == 7) // 2
        {
            row = 16;
            column = 8;
        }
        else if (index == 8) // 3
        {
            row = 16;
            column = 13;
        }
        else if (index == 9) // 4
        {
            row = 16;
            column = 18;
        }
        else if (index == 10) // 5
        {
            row = 17;
            column = 3;
        }
        else if (index == 11) // 6
        {
            row = 17;
            column = 8;
        }
        else if (index == 12) // 7
        {
            row = 17;
            column = 13;
        }
        else if (index == 13) // 8
        {
            row = 17;
            column = 18;
        }
        else if (index == 14) // Back to main menu
        {
            row = 19;
        }
    }
    else if (intDisplayedMenu == 5) // Boolean Hexagonal Automata
    {
        if (index == 0)
        {
            row = 14;
            column = 3;
        }
        else if (index == 1)
        {
            row = 14;
            column = 8;
        }
        else if (index == 2)
        {
            row = 14;
            column = 13;
        }
        else if (index == 3)
        {
            row = 15;
            column = 3;
        }
        else if (index == 4)
        {
            row = 15;
            column = 8;
        }                  
        else if (index == 5)
        {
            row = 15;
            column = 13;
        }              
        else if (index == 6)
        {
            row = 17;
        }
    }
    else if (intDisplayedMenu == 6) // Boolean Triagonal Automata
    {
        if (index == 0)
        {
            row = 17;
        }
    }            
    else if (intDisplayedMenu == 7) // Munching squares menu
    {
        if (index == 0) // Comparation type: Smaller than
        {
            row = 12;
        }
        else if (index == 1) // Comparation type: Equal to
        {
            row = 13;
        }
        else if (index == 2) // Boolean operator: XOR
        {    
            row = 15;
        }
        else if (index == 3) // Boolean operator: AND
        {    
            row = 16;
        }
        else if (index == 4) // Back to main menu
        {    
            row = 18;
        }            
    }
    
    if (boolDelete == false)
    {
        printArrow(row, column);
    }
    else
    {
        deleteArrow(row, column);
    }
    
    return 0;
}

/*
 * Shows the menu used to select the automata type (a.k.a. main menu)
 */
int showAutomataTypeMenu()
{
    displayedMenu = 0;
    automataType = ELEMENTARY_CELLULAR_AUTOMATA;
    showFlash();
    consoleClear();
    printCredits();
    printMenu(displayedMenu);
    printMenuArrow(displayedMenu, automataType, false);
    
    return 0;
}

/************************************************** END MENU FUNCTIONS ***********************************************************/

/*
 *
 */
int runAutomata()
{
    if (automataType == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        showFB();
  	    drawElementaryCellularAutomata();	        
    }
    else if (automataType == LANGTON_ANT)
    {
        showFB();
        initializeAnt(127, 95, 90, antNumPixels);        
    }
    else if (automataType == LANGTON_HEXAGONAL_ANT)
    {
        showFB();
        initializeHexAnt(92, 93, 0);
    }
    else if (automataType == BOOLEAN_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128*1024);
        showFB2();
        
        initializeBooleanAutomata(127, 91);
    }
    else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128* 1024);
        showFB2();

        initializeBooleanHexagonalAutomata(124, 93);
    }
    else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128*1024);
        showFB2();
        
        initializeBooleanTriangularAutomata(127, 91);
    }
    else if (automataType == MUNCHING_SQUARES)
    {
        showFB();
        initializeMunchingSquares();
    }
    
    return 0;
}

/*********************************** START MAIN FUNCTION *************************************************************************/

/*
 *
 */
int main(void)
{
	consoleDemoInit();

    showAutomataTypeMenu();
    
/*    printf("This program is free software:\n");
    printf("you can redistribute it and/or\n");
    printf("modify it under the terms of the\n");
    printf("GNU General Public License as\n");
    printf("publised by the Free Software\n");
    printf("Foundation, either version 3,\n");
    printf("or any later version.\n");
    printf("This program is distributed in the\n");
    printf("hope that it will be useful, but\n");
    printf("WITHOUT ANY WARRANTY; without even\n");    
    printf("the implied warranty of\n");    
    printf("MERCHANTABILITY or FITNESS FOR A\n");    
    printf("PARTICULAR PURPOSE. See the GNU\n"); 
    printf("General Public License for more\n"); 
    printf("details.\n"); 
    printf("You should have received a copy of\n");
    printf("the GNU General Public License\n");                             
    printf("along with this program. If not, see\n");     
    printf("<http://www.gnu.org/licenses/>\n"); 
*/
        
    showFlash();
		
	while (true)
	{
        int keys_pressed, keys_held, keys_released;

		// get key state
		scanKeys();
		// get touchscreen touch coord.
		touchRead(&touch);

		keys_pressed = keysDown();
		keys_held = keysHeld();
		keys_released = keysUp();

        /*
         * The main menu to select the type of automata
         */        
        if (displayedMenu == 0)
        {
            if (keys_released & KEY_UP)
            {
                if (automataType != ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    printMenuArrow(displayedMenu, automataType, true); // Delete previous arrow
                    automataType = automataType - 1;
                    printMenuArrow(displayedMenu, automataType, false); // Print new arrow
                }
                else
                {
                    printMenuArrow(displayedMenu, automataType, true); // Delete previous arrow
                    automataType = 6;
                    printMenuArrow(displayedMenu, automataType, false); // Print new arrow
                }   
            }
            else if (keys_released & KEY_DOWN)
            {
                if (automataType != 6)
                {
                    printMenuArrow(displayedMenu, automataType, true); // Delete previous arrow
                    automataType = automataType + 1;
                    printMenuArrow(displayedMenu, automataType, false); // Print new arrow                   
                }
                else
                {
                    printMenuArrow(displayedMenu, automataType, true); // Delete previous arrow
                    automataType = ELEMENTARY_CELLULAR_AUTOMATA;
                    printMenuArrow(displayedMenu, automataType, false); // Print new arrow                    
                }   
            }
            else if (keys_released & KEY_A)
            {
                if (automataType == ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
            	    printf("Current type:\n Elementary Cellular Automata"); 
                    printRuleNumber(calculateRuleNumber());
                    
                    intArrow = 0;                    
                    displayedMenu = 1;
                    
                    printMenu(displayedMenu);
                    
                    runAutomata();
                }
                else if (automataType == LANGTON_ANT)
                {
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Langton's ant");
                              
                    intArrow = 0;
                    displayedMenu = 2;
                    
                    printMenu(displayedMenu);
                    
                    runAutomata();
                }
                else if (automataType == LANGTON_HEXAGONAL_ANT)
                {
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Langton's hexagonal ant");
                    
                    intArrow = 0;
                    displayedMenu = 3;
                    
                    printMenu(displayedMenu);
                    
                    runAutomata();
                }
                else if (automataType == BOOLEAN_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Boolean automata");
                    
                    intArrow = 0;
                    displayedMenu = 4;
                    
                    printMenu(displayedMenu);
                    
                    printBAasterisks();
                    runAutomata();
                }
                else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Boolean Hexagonal Automata");
                    
                    intArrow = 0;
                    displayedMenu = 5;
                    
                    printMenu(displayedMenu);
                    
                    printBHAasterisks();                    
                    runAutomata();
                }
                else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Boolean Triangular Automata");
                    
                    intArrow = 0;
                    displayedMenu = 6;
                    
                    printMenu(displayedMenu);
                    
                    runAutomata();
                }
                else if (automataType == MUNCHING_SQUARES)
                {   
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Munching Squares");
                    
                    intArrow = 0;                    
                    displayedMenu = 7;
                    
                    printMenu(displayedMenu);
                    
  		            printMSasterisks();                    
                    runAutomata();
                }
            }
        }
        /*
         * The Elementary Cellular Automata menu
         */                
        else if (displayedMenu == 1)
        {
		    if(keys_released & KEY_A)
		    {
		        if (intArrow != 8)
		        {
			        if(ruleDown[intArrow] == FG_color)
			        {
				        ruleDown[intArrow] = BG_color;
			        }
			        else
			        {
				        ruleDown[intArrow] = FG_color;
			        }
			        drawRule(0);
			
                    printRuleNumber(calculateRuleNumber());
                    
			        drawElementaryCellularAutomata();
                }
                else // Go back to the selection of the type of automata
                {
                    showAutomataTypeMenu();
                }
		    }
		
		    if(keys_pressed & KEY_RIGHT)
		    {
		        if (intArrow != 8)
			    {
			        drawArrow(intArrow, BG_color);
			
			        if(intArrow < 7)
			        {
				        intArrow = intArrow + 1;
			        }
			        else
			        {
				        intArrow = 0;
			        }
	
			        drawArrow(intArrow, line_color);
                }					     			
		    }
		    else if(keys_pressed & KEY_LEFT)
		    {
                if (intArrow != 8)
			    {		    
			        drawArrow(intArrow, BG_color);
			
			        if(intArrow > 0)
			        {
				        intArrow = intArrow - 1;
			        }		
			        else
			        {
				        intArrow = 7;
			        }
			        drawArrow(intArrow, line_color);
                }			        
		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        if (intArrow != 8)
		        {
			        drawArrow(intArrow, BG_color);
			    }
			    else
			    {
			        printMenuArrow(displayedMenu, intArrow, true);
			    }
			    
			    if (intArrow == 8)
			    {
			        intArrow = 4;
			    }
			    else if(intArrow < 4)
			    {
				    intArrow = 8;
			    }
			    else
			    {
				    intArrow = intArrow - 4;
			    }

			    if (intArrow != 8)
			    {
			        drawArrow(intArrow, line_color);
			    }
			    else 
			    {
			        printMenuArrow(displayedMenu, intArrow, false);
			    }
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
		        if (intArrow != 8)
		        {
			        drawArrow(intArrow, BG_color);
			    }
			    else
			    {
			        printMenuArrow(displayedMenu, intArrow, true);			        
			    }

			    if (intArrow == 8)
			    {
			        intArrow = 0;
			    }			
			    else if(intArrow < 4)
			    {
				    intArrow = intArrow + 4;
			    }
			    else
			    {
				    intArrow = 8;
			    }
			    
			    if (intArrow != 8)
		        {
			        drawArrow(intArrow, line_color);
			    }
			    else
			    {
			        printMenuArrow(displayedMenu, intArrow, false);			        
			    }
		    }
        }
        /*
         * Langton's ant menu
         */                
        else if (displayedMenu == 2)
        {
		    if (antPosX < 0 or antPosX + antNumPixels > 254 or antPosY < 0 or antPosY + antNumPixels > 191)
    	    {
    	        antStop = true;
    	    }
    	    
    	    if (antStop == false)
    	    {
    	        stepAnt();
    	        antNumSteps++;
    	        iprintf("\x1b[9;0HStep #: %d", antNumSteps);
    	        swiWaitForVBlank();
    	    }
    	    
    	    if(keys_released & KEY_A)
		    {
		        if (intArrow == 1)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }                    
		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow

		        // Calculate the next position of the arrow		        
		        if (intArrow == 0)
		        {
		            intArrow = 1;
		        }
		        else
		        {
		            intArrow = intArrow - 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		     
		        // Calculate the next position of the arrow   
		        if (intArrow == 1)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		           
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
		    }
		    else if (keys_pressed & KEY_LEFT)
		    {
                if (intArrow == 0)
                {
                    if (antNumPixels > 0)
                    {
                        antNumPixels = antNumPixels - 1;
                        iprintf("\x1b[11;2HAnt's pixels: < %d >", antNumPixels);                        
                        runAutomata();
            	        iprintf("\x1b[9;0HStep #:    ");                                                
            	        iprintf("\x1b[9;0HStep #: %d", antNumSteps);                        
                    }                        
                }		        
		    }
		    else if (keys_pressed & KEY_RIGHT)
		    {
                if (intArrow == 0)
                {
                    antNumPixels = antNumPixels + 1;
                    iprintf("\x1b[11;2HAnt's pixels: < %d >", antNumPixels);
                    runAutomata();                
          	        iprintf("\x1b[9;0HStep #:    ");                                                                    
           	        iprintf("\x1b[9;0HStep #: %d", antNumSteps);                    
                }		    
		    }
        }
        /*
         * Langton's hexagonal ant menu
         */                
        else if (displayedMenu == 3)
        {
		    if (antPosX < 0 or antPosX + antNumPixels > 254 or antPosY < 0 or antPosY + antNumPixels > 191)
    	    {
    	        antStop = true;
    	    }
    	    
    	    if (antStop == false)
    	    {
    	        stepHexAnt();
    	        antNumSteps++;
    	        iprintf("\x1b[9;0HStep #: %d", antNumSteps);
    	        swiWaitForVBlank();
    	    }
    	    
    	    if(keys_released & KEY_A)
		    {
		        if (intArrow == 0)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }                    
		    }
        }
        /*
         * Boolean square automata
         */
        else if (displayedMenu == 4)
        {
            automataSteps++;
            iprintf("\x1b[9;0HStep #: %d", automataSteps);

            if (calculateNextStep(intTypeOfNeighborhood) == 0) // The automata has finished so we are going to reinitiate the cycle
            {
                runAutomata(); 
            }
            else // the automata has not finished yet
            {
                if (automataSteps % 2 == 0 and automataSteps != 1)
                {
                    showFB();
                }
                else
                {
                    showFB2();
                }
                swiWaitForVBlank();
            }
            
       	    if(keys_released & KEY_A)
		    {
		        if (intArrow == 0)
		        {
		            intTypeOfNeighborhood = 0;
		            printBAasterisks();
		            runAutomata();
		        }
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4)
		        {
		            if (intBooleanRulesValuesSqVN[intArrow - 1] == intArrow)
		            {
		                intBooleanRulesValuesSqVN[intArrow - 1] = 0;
		            }
		            else
		            {
		                intBooleanRulesValuesSqVN[intArrow - 1] = intArrow;
		            }
		            
		            printBAasterisks();
		            runAutomata();
		        }
		        else if (intArrow == 5)
		        {
		            intTypeOfNeighborhood = 1;
		            printBAasterisks();
		            runAutomata();
		        }
		        else if (intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12 || intArrow == 13)
		        {
		            if (intBooleanRulesValuesSqM[intArrow - 6] == intArrow - 5)
		            {
		                intBooleanRulesValuesSqM[intArrow - 6] = 0;
		            }
		            else
		            {
		                intBooleanRulesValuesSqM[intArrow - 6] = intArrow - 5;
		            }		      
		            
		            printBAasterisks();
		            runAutomata();  
		        }
		        else if (intArrow == 14)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }
            }
            else if(keys_released & KEY_UP)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        		    
		        if (intArrow == 0) // Von Neumann -> Back to main menu
		        {
		            intArrow = 14;
		        }
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4) // 1, 2, 3, 4 from Von Neumann -> Von Neumann
		        {
		            intArrow = 0;
		        }
		        else if (intArrow == 5) // Moore -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
		        else if (intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9) // 1, 2, 3, 4 from Moore -> Moore
		        {
		            intArrow = 5;
		        }
		        else if (intArrow == 10 || intArrow == 11 || intArrow == 12 || intArrow == 13) // 5 -> 1, 6 -> 2, 7 -> 3, 8 -> 4 (Moore)
		        {
		            intArrow = intArrow - 4;
		        }
		        else if (intArrow == 14) // Back to main menu -> 5 from Moore
		        {
		            intArrow = 10;
		        }
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        
		    }
            else if(keys_released & KEY_DOWN)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        		    
		        if (intArrow == 0) // Von Neumann -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4) // 1, 2, 3, 4 from Von Neumann -> Moore
		        {
		            intArrow = 5;
		        }
		        else if (intArrow == 5) // Moore -> 1 from Moore
		        {
		            intArrow = 6;
		        }
		        else if (intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9) // 1 -> 5, 2 -> 6, 3 -> 7, 4 -> 8 (Moore)
		        {
		            intArrow = intArrow + 4;
		        }
		        else if (intArrow == 10 || intArrow == 11 || intArrow == 12 || intArrow == 13) // 5, 6, 7, 8 from Moore -> Back to main menu
		        {
		            intArrow = 14;
		        }
		        else if (intArrow == 14) // Back to main menu -> 1 from Von Neumann
		        {
		            intArrow = 0;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        
		    }
		    else if (keys_released & KEY_LEFT)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        		    
		        if (intArrow == 1 || intArrow == 6 || intArrow == 10) // 1 -> 4 (Von Neumann), 1 -> 4, 5 -> 8 (Moore)
		        {
		            intArrow = intArrow + 3;
		        }
                // 2 -> 1, 3 -> 2, 4 -> 3 (Von Neumann), 2 -> 1, 3 -> 2, 4 -> 3, 6 -> 5, 7 -> 6, 8 -> 7 (Moore)
		        else if (intArrow == 2 || intArrow == 3 || intArrow == 4 || intArrow == 7 || intArrow == 8 || intArrow == 9 || intArrow == 11 || intArrow == 12 || intArrow == 13) 
		        {
		            intArrow = intArrow - 1;
		        }

		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        		        
		    }
		    else if (keys_released & KEY_RIGHT)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        		    
		        if (intArrow == 4 || intArrow == 9 || intArrow == 13) // 4 -> 1 (Von Neumann), 4 -> 1, 8 -> 5 (Moore)
		        {
		            intArrow = intArrow - 3;
		        }
                // 1 -> 2, 2 -> 3, 3 -> 4 (Von Neumann), 1 -> 2, 2 -> 3, 3 -> 4, 5 -> 6, 6 -> 7, 7 -> 8 (Moore)
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 10 || intArrow == 11 || intArrow == 12) 
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        		        
		    }		    
        }
        /*
         * Boolean hexagonal automata menu
         */                
        else if (displayedMenu == 5)
        {
            automataSteps++;
            iprintf("\x1b[9;0HStep #: %d", automataSteps);

            if (calculateNextStepHex() == 0) // The automata has finished so we are going to reinitiate the cycle
            {
                runAutomata(); 
            }
            else // the automata has not finished yet
            {
                if (automataSteps % 2 == 0 and automataSteps != 1)
                {
                    showFB();
                }
                else
                {
                    showFB2();
                }
                swiWaitForVBlank();
            }
            
            if (keys_released & KEY_A)
		    {
		        if (intArrow == 0 || intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4 || intArrow == 5)
		        {
		            if (intBooleanRulesValuesHex[intArrow] == intArrow + 1)
		            {
		                intBooleanRulesValuesHex[intArrow] = 0;
		            }
		            else
		            {
		                intBooleanRulesValuesHex[intArrow] = intArrow + 1;
		            }
		            
		            printBHAasterisks();
		            runAutomata();
		        }                
                else if (intArrow == 6)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }                    		        
		    }
            else if(keys_released & KEY_UP)
            {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow

		        if (intArrow == 0 or intArrow == 1 or intArrow == 2) // 1, 2, 3-> Back to main menu
		        {
		            intArrow = 6;
		        }
		        else if (intArrow == 3 or intArrow == 4 or intArrow == 5) // 4 -> 1, 5 -> 2, 6 -> 3
		        {
		            intArrow = intArrow - 3;
		        }
		        else if (intArrow == 6) // Back to main menu -> 4
		        {
		            intArrow = 3;
                }		            
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        		        		        
            }		    
            else if(keys_released & KEY_DOWN)
            {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow

		        if (intArrow == 0 or intArrow == 1 or intArrow == 2) // 1 -> 4, 2 -> 5, 3 -> 6
		        {
		            intArrow = intArrow + 3;
		        }
		        else if (intArrow == 3 or intArrow == 4 or intArrow == 5) // 4, 5, 6 -> Back to main menu
		        {
		            intArrow = 6;
		        }
		        else if (intArrow == 6) // Back to main menu -> 1
		        {
		            intArrow = 0;
                }		            
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        		        		        
            }		                
            else if(keys_released & KEY_LEFT)
            {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow

		        if (intArrow == 0 or intArrow == 3) // 1 -> 3, 4 -> 6
		        {
		            intArrow = intArrow + 2;
		        }
		        else if (intArrow == 1 or intArrow == 2 or intArrow == 4 or intArrow == 5) // 2 -> 1, 3 -> 2, 5 -> 4, 6 -> 5
		        {
		            intArrow = intArrow - 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
            }		        		        		        		                    
            else if(keys_released & KEY_RIGHT)
            {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow

		        if (intArrow == 2 or intArrow == 5) // 3 -> 1, 6 -> 4
		        {
		            intArrow = intArrow - 2;
		        }
		        else if (intArrow == 0 or intArrow == 1 or intArrow == 3 or intArrow == 4) // 1 -> 2, 2 -> 3, 4 -> 5, 5 -> 6
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
            }            
        }
        /*
         * Boolean triangular automata menu
         */                
        else if (displayedMenu == 6)
        {
            automataSteps++;
            iprintf("\x1b[9;0HStep #: %d", automataSteps);
            
            if (calculateNextStepTriangular(intTypeOfNeighborhood) == 0) // The automata has finished so we are going to reinitiate the cycle
            {
                runAutomata(); 
            }
            else // the automata has not finished yet
            {
                if (automataSteps % 2 == 0 and automataSteps != 1)
                {
                    showFB();
                }
                else
                {
                    showFB2();
                }
                swiWaitForVBlank();
            }
            
            if (keys_released & KEY_A)
		    {
		        if (intArrow == 0)
		        {
  		            showAutomataTypeMenu();		        
  		        }
		    }
        }
        /*
         * Munching squares menu
         */
        else if (displayedMenu == 7)
        {
  		    if(keys_released & KEY_A)
  		    {
  		        if (intArrow == 0) // Comparation type: Less than
  		        {
  		            munchingSquaresOptionComp = 0;
  		            printMSasterisks();  		            
  		            runAutomata();
  		        }
  		        else if (intArrow == 1) // Comparation type: Equal to
  		        {
  		            munchingSquaresOptionComp = 1;
  		            printMSasterisks();
  		            runAutomata();  		            
  		        }
  		        else if (intArrow == 2) // Boolean operator type: XOR
  		        {
  		            munchingSquaresOptionOp = 0;
  		            printMSasterisks();  		            
  		            runAutomata();
  		        }
  		        else if (intArrow == 3) // Boolean operator type: AND
  		        {
  		            munchingSquaresOptionOp = 1;
  		            printMSasterisks();
  		            runAutomata();  		            
  		        }
  		        else if (intArrow == 4) // Back to main menu
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        
		        if (intArrow == 0)
		        {
		            intArrow = 4;
		        }
		        else
		        {
		            intArrow = intArrow - 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow        
		        
		        if (intArrow == 4)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
		    }            
            
            drawNextStepMunchingSquares();
        }
    }
    
	return 0;
}
/*********************************** END MAIN FUNCTION *************************************************************************/
