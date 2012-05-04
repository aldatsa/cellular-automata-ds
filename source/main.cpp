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
#include <string>

#include "general.h"
#include "color.h"
#include "hexgrid.h"
#include "triangulargrid.h"
#include "i18n.h"

#include "cellularautomata.h"

using namespace std;

/*
 * Constants for the rows where the menu items of the automata type selection menu are printed
 */
#define MENU_AUTOMATA_TYPE_ROW_ECA 8
#define MENU_AUTOMATA_TYPE_ROW_LA 9
#define MENU_AUTOMATA_TYPE_ROW_LHA 10
#define MENU_AUTOMATA_TYPE_ROW_BA 11
#define MENU_AUTOMATA_TYPE_ROW_BHA 12
#define MENU_AUTOMATA_TYPE_ROW_BTA 13
#define MENU_AUTOMATA_TYPE_ROW_CGL 14
#define MENU_AUTOMATA_TYPE_ROW_MS 15
#define MENU_SELECT_COLORS 16
#define MENU_SELECT_LANGUAGE 17

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
 * 6: Conway's game of life
 * 6: Munching Squares
 * 7: Select colors
 * 8: Select language
 */
#define ELEMENTARY_CELLULAR_AUTOMATA 0
#define LANGTON_ANT 1
#define LANGTON_HEXAGONAL_ANT 2
#define BOOLEAN_AUTOMATA 3
#define BOOLEAN_HEXAGONAL_AUTOMATA 4
#define BOOLEAN_TRIANGULAR_AUTOMATA 5
#define CONWAYS_GAME_OF_LIFE 6
#define MUNCHING_SQUARES 7
#define SELECT_COLORS 8
#define SELECT_LANGUAGE 9

/*
 * This variable sets the type of automata that is going to execute (See above)
 */
int automataType = ELEMENTARY_CELLULAR_AUTOMATA;

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
 * A variable for the position of the touch
 */
touchPosition touch;

/* This variable sets the menu that is going to be displayed
 * 0: Select automata menu
 * 1: Elementary cellular automata menu
 * 2: Langton's ant menu
 * 3: Langton's hexagonal ant menu
 * 4: Boolean automata menu
 * 5: Boolean Hexagonal Automata menu
 * 6: Boolean Triangular Automata menu
 * 7: Conway's game of life
 * 8: Munching squares menu
 * 9: Select colors
 * 10: Select language
 */
int displayedMenu = 0;

/* The position of the arrow
 * Select automata menu: ????
 * Elementary Cellular Automata: in the top screen (0-7) and in the bottom screen (8)
 * Langton's ant: ????
 * Langton's hexagonal ant: ????
 * Boolean automata menu: ?????
 * Boolean Hexagonal Automata menu: ????
 * Boolean Triangular Automata menu: ????
 * Conway's game of life: ????
 * Munching Squares: in the bottom screen (0-2)
 * Select colors: ????
 * Select language: ????
 */
int intArrow = 0;

/*
 * Variables used for the boolean automaton
 */
int automataSteps = 0; // It's equivalent to antNumSteps, I should use only one of them (Used in Boolean Square Automata, Boolean Hexagonal Automata and Boolean Triangular Automata)
int intTypeOfNeighborhood = 0; // 0: Von Neumann - 1: Moore (Used in Boolean Square Automata and Boolean Triangular Automata)
int intBooleanRulesValuesTriVN [3] = {1, 0, 0}; // {1, 2, 3}; For the Boolean Triangular Automata with Modified Von Neumann neighborhood
int intBooleanRulesValuesTriM [8] = {1, 0, 0, 0, 0, 0, 0, 0}; // {1, 2, 3, 4, 5, 6, 7, 8}; For the Boolean Triangular Automata with Modified Moore neighborhood

CellularAutomata ca;

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
 * Updates the colors of the background, foreground and lines
 * It's used to update the colors when new colors are selected
 * in the color selection menu
 */
int updateColors()
{
    ruleLeft[0] = FG_color;
    ruleLeft[1] = FG_color;
    ruleLeft[2] = FG_color;
    ruleLeft[3] = FG_color;
    ruleLeft[4] = BG_color;
    ruleLeft[5] = BG_color;
    ruleLeft[6] = BG_color;
    ruleLeft[7] = BG_color;

    ruleCenter[0] = FG_color;
    ruleCenter[1] = FG_color;
    ruleCenter[2] = BG_color;
    ruleCenter[3] = BG_color;
    ruleCenter[4] = FG_color;
    ruleCenter[5] = FG_color;
    ruleCenter[6] = BG_color;
    ruleCenter[7] = BG_color;

    ruleRight[0] = FG_color;
    ruleRight[1] = BG_color;
    ruleRight[2] = FG_color;
    ruleRight[3] = BG_color;
    ruleRight[4] = FG_color;
    ruleRight[5] = BG_color;
    ruleRight[6] = FG_color;
    ruleRight[7] = BG_color;

    // The rule that will be displayed on start on the Elementary Cellular Automata
    ruleDown[0] = BG_color;
    ruleDown[1] = FG_color;
    ruleDown[2] = BG_color;
    ruleDown[3] = FG_color;
    ruleDown[4] = FG_color;
    ruleDown[5] = BG_color;
    ruleDown[6] = FG_color;
    ruleDown[7] = BG_color;

    return 0;
}

/*
 * Paints the initial cell in the center of the first row
 */
int paintInitialCell()
{
    fb[128] = FG_color;
    
    return 0;
}

/*
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
 *  Converts to char array and prints the given intRuleNumber of the Elementary Cellular Automata
 */
int printRuleNumber(int intRuleNumber)
{
    
    // Convert intRuleNumber to char array
    char buffer [3];
    sprintf(buffer, "%d", intRuleNumber);
    
    // Print 3 spaces to erase the previous rule
    iprintf("\x1b[%d;2H%s:   ", ECA_RULE_NUMBER_ROW, stringRuleNumber.c_str());
    
    // Print it in the appropiate position
    iprintf("\x1b[%d;2H%s:%s", ECA_RULE_NUMBER_ROW, stringRuleNumber.c_str(), buffer);
    
    return 0;
}

/*
 * Draws an arrow in the top screen (0-7) of the Elementary Cellular Automata
 */
int drawArrow(char nth, unsigned short color)
{	

    const int intTopRow = 149;
    const int intBottomRow = 181;

    const int intColumn1 = 38;
    const int intColumn2 = 94;
    const int intColumn3 = 150;
    const int intColumn4 = 206;
    
	int intRow = 0;
	int intColumn = 0;
     
	switch (nth)
	{
		case 0 :
			intRow = intTopRow;
			intColumn = intColumn1;
		 	break;
		case 1 :
			intRow = intTopRow;
			intColumn = intColumn2;
		 	break;		
		case 2 :
			intRow = intTopRow;
			intColumn = intColumn3;
		 	break;	
		case 3 :
			intRow = intTopRow;
			intColumn = intColumn4;
		 	break;	
		case 4 :
			intRow = intBottomRow;
			intColumn = intColumn1;
		 	break;
		case 5 :
			intRow = intBottomRow;
			intColumn = intColumn2;
		 	break;		
		case 6 :
			intRow = intBottomRow;
			intColumn = intColumn3;
		 	break;	
		case 7 :
			intRow = intBottomRow;
			intColumn = intColumn4;
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
 * Draws a rectangle, filled with FG_color or BG_color
 * This function is used by drawRule() to draw the rectangles to visualize the current rule
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

/*
 * Draws the current rule of the Elementary Cellular Automata
 */
int drawRule(int nth)
{

    const int intTopRow = 136;
    const int intBottomRow = 168;

    const int intColumn1 = 32;
    const int intColumn2 = 88;
    const int intColumn3 = 144;
    const int intColumn4 = 200;

	const int intLength = 9;
	const int intWidth = 9;
    
	bool fill = false;
	int intRowStart = 0;
	int intColumnStart = 0;
	
	switch (nth)
	{
		case 0 :
			intRowStart = intTopRow;
			intColumnStart = intColumn1;
		 	break;
		case 1 :
			intRowStart = intTopRow;
			intColumnStart = intColumn2;
		 	break;		
		case 2 :
			intRowStart = intTopRow;
			intColumnStart = intColumn3;
		 	break;	
		case 3 :
			intRowStart = intTopRow;
			intColumnStart = intColumn4;
		 	break;	

		case 4 :
			intRowStart = intBottomRow;
			intColumnStart = intColumn1;
		 	break;
		case 5 :
			intRowStart = intBottomRow;
			intColumnStart = intColumn2;
		 	break;		
		case 6 :
			intRowStart = intBottomRow;
			intColumnStart = intColumn3;
		 	break;	
		case 7 :
			intRowStart = intBottomRow;
			intColumnStart = intColumn4;
		 	break;	
	}

	if(ruleLeft[nth] == FG_color)
	{
		fill = true;	
	}
	
	drawRectangle(fill, intRowStart, intColumnStart, intLength, intWidth);

	fill = false;

	if(ruleCenter[nth] == FG_color)
	{
		fill = true;
	}
		
	drawRectangle(fill, intRowStart, intColumnStart + intWidth - 1, intLength, intWidth);

	fill = false;

	if(ruleRight[nth] == FG_color)
	{
        fill = true;
	}

	drawRectangle(fill, intRowStart, intColumnStart + (intWidth - 1) * 2, intLength, intWidth);

    fill = false;

	if(ruleDown[nth] == FG_color)
	{
		fill = true;
	}

	drawRectangle(fill, intRowStart + intLength - 1, intColumnStart + intWidth -1, intLength, intWidth);
	
	return 0;
}

/*
 * Draws the Elementary Cellular Automata that corresponds to the current rules
 */
int drawElementaryCellularAutomata()
{
	int row, column;
	unsigned char i;
	
	cleanFB(fb);

    paintInitialCell(); // Paints the initial cell in the center of the first row (fb[128] = FG_color)

	for(row = 0; row < 130; row++)
	{
		for(column = 0; column < SCREEN_WIDTH; column++)
		{
			if(row != 0 && row < 128)
			{
				for(i=0; i <= 7; i++)
				{
					if(column != 0 && column != SCREEN_WIDTH - 1)
					{ 
						if (fb[(row - 1) * SCREEN_WIDTH + (column-1)] == ruleLeft[i] && fb[(row - 1) * SCREEN_WIDTH + column] == ruleCenter[i] && fb[(row - 1) * SCREEN_WIDTH + (column+1)] == ruleRight[i])
							fb[row * SCREEN_WIDTH + column] = ruleDown[i];
					}
					else if(column == 0)
					{
						// The left cell is out of the screen, instead we'll use the center cell (column 0) to compare to ruleLeft
						if(fb[(row - 1) * SCREEN_WIDTH + column] == ruleLeft[i]  && fb[(row - 1) * SCREEN_WIDTH + column] == ruleCenter[i] && fb[(row - 1) * SCREEN_WIDTH + (column+1)] == ruleRight[i])
						{
							fb[row * SCREEN_WIDTH + column] = ruleDown[i];								
						}
					}	
					else if(column == SCREEN_WIDTH - 1)
					{
                        // The right cell is out of the screen, instead we'll use the center cell (column 255) to compare to ruleRight
						if(fb[(row - 1) * SCREEN_WIDTH + (column-1)] == ruleLeft[i] && fb[(row - 1) * SCREEN_WIDTH + column] == ruleCenter[i] && fb[(row - 1) * SCREEN_WIDTH + column] == ruleRight[i])
                        {
							fb[row * SCREEN_WIDTH + column] = ruleDown[i];								
                        }
					}
				}
			}
			else if(row == 129)
			{
				fb[row * SCREEN_WIDTH + column] = FG_color;
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

/************************************* START BOOLEAN AUTOMATA FUNCTIONS ****************************************************/

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
        for (int j = 4; j < 187; j = j + 3)
        {
            countFG = 0;        

            // xxx
            //  x
            if (typeOfNeighborhood == 1 or ((i % 2 == 0 and j % 2 == 0) or (i % 2 != 0 and j % 2 != 0)))
            {    
                // top 
                if (fbRef[SCREEN_WIDTH * (j - 2) + i] == FG_color)
                {
                    countFG++;
                }
            }
            //  x
            // xxx
            if (typeOfNeighborhood ==  1 or ((i % 2 != 0 and j % 2 == 0) or (i % 2 == 0 and j % 2 != 0)))
        	{
        	    //bottom
                if (fbRef[SCREEN_WIDTH * (j + 3) + i] == FG_color)
                {
                    countFG++;
                }        	    
        	}
            
            // left
            if (fbRef[SCREEN_WIDTH * j + i - 3] == FG_color)
            {
                countFG++;
            }

            // right
            if (fbRef[SCREEN_WIDTH * j + i + 3] == FG_color)
            {
                countFG++;
            }
                        
            if (typeOfNeighborhood == 1)            
            {                        
                // top left
                if (fbRef[SCREEN_WIDTH * (j - 2) + i - 3] == FG_color)
                {
                    countFG++;
                }
                
                // top right
                if (fbRef[SCREEN_WIDTH * (j - 2) + i + 3] == FG_color)
                {
                    countFG++;
                }
                
                // bottom left    
                if (fbRef[SCREEN_WIDTH * (j + 3) + i - 3] == FG_color)
                {
                    countFG++;
                }

                // bottom right                         
                if (fbRef[SCREEN_WIDTH * (j + 3) + i + 3] == FG_color)
                {
                    countFG++;
                }
            }
            
            if (countFG != 0 and booleanTriangularRule(countFG))
            {
                // If the current cell's color is not already changed, change it to FG_color.
                // Without this condition each cell is painted more than one time and changeCount is never equal to 0.
                if (fbNew[SCREEN_WIDTH * j + i] != FG_color) 
                {
                    paintTriangularCell(i, j, FG_color, fbNew);
                    changeCount++;
                }
            }
        }
    }
    
    return changeCount;
}

int initializeBooleanTriangularAutomata(int intX, int intY)
{
    cleanFB(fb);
    cleanFB(fb2);
    
    drawTriangularGrid();
    
    automataSteps = 0;
        
    paintTriangularCell(intX, intY, FG_color, fb);
    
    return 0;
}

/*************************************** END BOOLEAN AUTOMATA FUNCTIONS ****************************************************/

/**************************************** START CONWAY'S GAME OF LIFE FUNCTIONS ********************************************/

/*
 * References:
 *  http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 */
 
/*
 * Draws the initial state that develops into the period 3 oscillator called "pulsar".
 * The "pulsar" is the most common period 3 oscillator.
 *
 * The initial state is like that:
 *
 *          *****
 *                  (empty line)
 *           ***
 *                  (empty line)
 *          *****
 */
int initializePulsar(int intX, int intY)
{
    drawHLine(intX, intY, 5, FG_color, fb);
    drawHLine(intX + 1, intY + 2, 3, FG_color, fb);
    drawHLine(intX, intY + 4, 5, FG_color, fb);
    
    return 0;
}

/*
 * Fills the screen with multiple rows and columns of "pulsars".
 */
int fillScreenWithPulsars()
{
    for (int i = 12; i < 249; i = i + 16)
    {
        for (int j = 12; j < 185; j = j + 16)
        { 
            initializePulsar(i, j);
        }
    }
    
    return 0;
}

/*
 * Draws the F-pentomino.
 * During this early research, Conway discovered that the F-pentomino (which he called the "R-pentomino") failed to stabilize in a small number of generations.
 * In fact, it takes 1103 generations to stabilize, by which time it has a population of 116 and has fired six escaping gliders (these were the first gliders ever discovered). 
 * 
 * The initial state is like that:
 *
 *          **
 *         **
 *          * 
 */
int initializeFpentomino(int intX, int intY)
{
    drawHLine(intX, intY, 2, FG_color, fb);
    drawHLine(intX - 1, intY + 1, 2, FG_color, fb);
    drawHLine(intX, intY + 2, 1, FG_color, fb);
    
    return 0;
}

/*
 * Draws the Acorn
 * It takes 5206 generations to generate 633 cells including 13 escaped gliders.
 * 
 * The initial state is like that:
 *
 *           *
 *             *
 *          **  ***
 */
int initializeAcorn(int intX, int intY)
{
    drawHLine(intX, intY, 1, FG_color, fb);
    drawHLine(intX + 2, intY + 1, 1, FG_color, fb);
    drawHLine(intX - 1, intY + 2, 2, FG_color, fb);
    drawHLine(intX + 3, intY + 2, 3, FG_color, fb);    
    
    return 0;
}

/*
 * 
 */
int initializeConwaysGameOfLife()
{
    cleanFB(fb);
    cleanFB(fb2);
    
    automataSteps = 0;
    
    //fillScreenWithPulsars();
    //initializeFpentomino(120, 90);
    initializeAcorn(120, 90);
    
    return 0;
}

/*
 * 
 */
int calculateNextStepConwaysGameOfLife()
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
    
    for (int i = 1; i < 254; ++i)
    {   
        for (int j = 1; j < SCREEN_HEIGHT - 1; ++j)
        {
            countFG = 0;        

            // top 
            if (fbRef[SCREEN_WIDTH * (j - 1) + i] == FG_color)
            {
                countFG++;
            }

      	    //bottom
            if (fbRef[SCREEN_WIDTH * (j + 1) + i] == FG_color)
            {
                countFG++;
            }        	    
            
            // left
            if (fbRef[SCREEN_WIDTH * j + i - 1] == FG_color)
            {
                countFG++;
            }

            // right
            if (fbRef[SCREEN_WIDTH * j + i + 1] == FG_color)
            {
                countFG++;
            }
                        
            // top left
            if (fbRef[SCREEN_WIDTH * (j - 1) + i - 1] == FG_color)
            {
               countFG++;
            }
                
            // top right
            if (fbRef[SCREEN_WIDTH * (j - 1) + i + 1] == FG_color)
            {
               countFG++;
            }
                
            // bottom left    
            if (fbRef[SCREEN_WIDTH * (j + 1) + i - 1] == FG_color)
            {
                countFG++;
            }

            // bottom right                         
            if (fbRef[SCREEN_WIDTH * (j + 1) + i + 1] == FG_color)
            {
                countFG++;
            }
            
            if (countFG < 2 && fbRef[SCREEN_WIDTH * j + i] == FG_color) // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
            {
                fbNew[SCREEN_WIDTH * j + i] = BG_color;
                changeCount++;
            }
            if ((countFG == 2 || countFG == 3) && fbRef[SCREEN_WIDTH * j + i] == FG_color) // Any live cell with two or three live neighbours lives on to the next generation.
            {
                fbNew[SCREEN_WIDTH * j + i] = FG_color;
                changeCount++;
            }
            if (countFG > 3 && fbRef[SCREEN_WIDTH * j + i] == FG_color) // Any live cell with more than three live neighbours dies, as if by overcrowding.
            {
                fbNew[SCREEN_WIDTH * j + i] = BG_color;
                changeCount++;
            }
            if (countFG == 3 && fbRef[SCREEN_WIDTH * j + i] == BG_color) // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            {
                fbNew[SCREEN_WIDTH * j + i] = FG_color;
                changeCount++;
            }            
        }
    }
    
    return changeCount;
}

/**************************************** END CONWAY'S GAME OF LIFE FUNCTIONS ********************************************/

/******************************************* START MENU FUNCTIONS **********************************************************/

/*
 * Prints app's credits
 */
int printCredits()
{
	printf("Cellular Automata DS\n");
	printf("%s\n", stringVersion.c_str());
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
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_ECA, stringElementaryCellularAutomata.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_LA, stringLangtonsAnt.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_LHA, stringLangtonsHexagonalAnt.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_BA, stringBooleanAutomata.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_BHA, stringBooleanHexagonalAutomata.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_BTA, stringBooleanTriangularAutomata.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_CGL, stringConwaysGameOfLife.c_str());
        iprintf("\x1b[%d;2H%s", MENU_AUTOMATA_TYPE_ROW_MS, stringMunchingSquares.c_str());
        iprintf("\x1b[%d;2H%s", MENU_SELECT_COLORS, stringSelectColors.c_str());
        iprintf("\x1b[%d;2H%s", MENU_SELECT_LANGUAGE, stringSelectLanguage.c_str());
    }
    else if (displayedMenu == 1) // The menu of the Elementary Cellular Automaton
    {
        iprintf("\x1b[11;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 2) // The menu of the Langton's ant
    {
        iprintf("\x1b[11;2H%s: < %d >", stringAntsPixels.c_str(), ca.getAntNumPixels());
        iprintf("\x1b[13;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 3) // The menu of the Langton's hexagonal ant
    {
        iprintf("\x1b[13;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 4) // The menu of the boolean automaton
    {
        iprintf("\x1b[13;2H %s", stringVonNeumannNeighborhood.c_str());
        iprintf("\x1b[14;5H 1    2    3    4");
        iprintf("\x1b[15;2H %s", stringMooreNeighborhood.c_str());
        iprintf("\x1b[16;5H 1    2    3    4");
        iprintf("\x1b[17;5H 5    6    7    8");        
        iprintf("\x1b[19;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 5) // The menu of the Boolean Hexagonal Automaton
    {
        iprintf("\x1b[13;2H%s:", stringNeighborhood.c_str());
        iprintf("\x1b[14;5H 1    2    3");
        iprintf("\x1b[15;5H 4    5    6");
        iprintf("\x1b[17;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 6) // The menu of the Boolean Triangular Automaton
    {
        iprintf("\x1b[13;2H %s", stringVonNeumannNeighborhood.c_str());
        iprintf("\x1b[14;5H 1    2    3");
        iprintf("\x1b[15;2H %s", stringMooreNeighborhood.c_str());
        iprintf("\x1b[16;5H 1    2    3    4");
        iprintf("\x1b[17;5H 5    6    7    8");        
        iprintf("\x1b[19;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 7) // The menu of the Langton's hexagonal ant
    {
        iprintf("\x1b[13;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 8) // The menu of the munching squares
    {
        iprintf("\x1b[13;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 9) // The menu of color selection
    {
        iprintf("\x1b[8;2H%s:", stringBackgroundColor.c_str());
        iprintf("\x1b[9;5H%s < %i > ", stringRed.c_str(), BG_R);
        iprintf("\x1b[10;5H%s < %i > ", stringGreen.c_str(), BG_G);
        iprintf("\x1b[11;5H%s < %i > ", stringBlue.c_str(), BG_B);
        
        iprintf("\x1b[12;2H%s:", stringForegroundColor.c_str());
        iprintf("\x1b[13;5H%s < %i > ", stringRed.c_str(), FG_R);
        iprintf("\x1b[14;5H%s < %i > ", stringGreen.c_str(), FG_G);
        iprintf("\x1b[15;5H%s < %i > ", stringBlue.c_str(), FG_B);
                
        iprintf("\x1b[16;2H%s:", stringLineColor.c_str());
        iprintf("\x1b[17;5H%s < %i > ", stringRed.c_str(), line_R);
        iprintf("\x1b[18;5H%s < %i > ", stringGreen.c_str(), line_G);
        iprintf("\x1b[19;5H%s < %i > ", stringBlue.c_str(), line_B);
                
        iprintf("\x1b[21;2H%s", stringBackToMainMenu.c_str());
    }
    else if (displayedMenu == 10) // The menu of language selection
    {
        iprintf("\x1b[10;3H%s", stringEnglish.c_str());
        iprintf("\x1b[11;3H%s", stringEspanol.c_str());
        iprintf("\x1b[12;3H%s", stringEuskara.c_str());
        
        iprintf("\x1b[14;2H%s", stringBackToMainMenu.c_str());
    }
    
    return 0;
}

/*
 * Prints the asterisk used to mark the current language in the language selection menu
 */
int printLanguageAsterisks()
{
    if (displayedLanguage == "en")
    {
        iprintf("\x1b[10;2H*");
        iprintf("\x1b[11;2H ");
        iprintf("\x1b[12;2H ");
    }
    else if (displayedLanguage == "es")
    {
        iprintf("\x1b[10;2H ");
        iprintf("\x1b[11;2H*");
        iprintf("\x1b[12;2H ");
    }
    else if (displayedLanguage == "eu")
    {
        iprintf("\x1b[10;2H ");
        iprintf("\x1b[11;2H ");
        iprintf("\x1b[12;2H*");
    }
    
    return 0;
}

/*
 * Prints the asterisk used to mark the options in the menu of the Boolean automata
 */
int printBAasterisks()
{
    if (ca.getTypeOfNeighborhood() == 0)
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
    if (ca.getBooleanRuleValue(0, 0) == 1)
    {
        iprintf("\x1b[14;5H*");    
    }
    else
    {
        iprintf("\x1b[14;5H ");    
    }

    if (ca.getBooleanRuleValue(0, 1) == 2)
    {
        iprintf("\x1b[14;10H*"); 
    }
    else
    {
        iprintf("\x1b[14;10H ");
    }

    if (ca.getBooleanRuleValue(0, 2) == 3)
    {
        iprintf("\x1b[14;15H*");    
    }
    else
    {
        iprintf("\x1b[14;15H ");    
    }
            
    if (ca.getBooleanRuleValue(0, 3) == 4) 
    {
        iprintf("\x1b[14;20H*");    
    }
    else
    {
        iprintf("\x1b[14;20H ");    
    }
            
    // {1, 2, 3, 4, 5, 6, 7, 8} For the boolean square automata with Moore neighborhood
    if (ca.getBooleanRuleValue(1, 0) == 1)
    {
        iprintf("\x1b[16;5H*");    
    }
    else
    {
        iprintf("\x1b[16;5H ");    
    }
    
    if (ca.getBooleanRuleValue(1, 1) == 2)
    {
        iprintf("\x1b[16;10H*");    
    }
    else
    {
        iprintf("\x1b[16;10H ");    
    }        
    
    if (ca.getBooleanRuleValue(1, 2) == 3)
    {
        iprintf("\x1b[16;15H*");    
    }
    else
    {
        iprintf("\x1b[16;15H ");    
    }
        
    if (ca.getBooleanRuleValue(1, 3) == 4)
    {
        iprintf("\x1b[16;20H*");    
    }
    else
    {
        iprintf("\x1b[16;20H ");    
    }
        
    if (ca.getBooleanRuleValue(1, 4) == 5)
    {
        iprintf("\x1b[17;5H*");    
    }
    else
    {
        iprintf("\x1b[17;5H ");    
    }

    if (ca.getBooleanRuleValue(1, 5) == 6)
    {
        iprintf("\x1b[17;10H*");
    }
    else
    {
        iprintf("\x1b[17;10H ");
    }

    if (ca.getBooleanRuleValue(1, 6) == 7)
    {
        iprintf("\x1b[17;15H*");
    }
    else
    {
        iprintf("\x1b[17;15H "); 
    }
        
    if (ca.getBooleanRuleValue(1, 7) == 8)
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
    // {1, 2, 3, 4, 5, 6} For the boolean hexagonal automata with Moore neighborhood
    if (ca.getBooleanRuleValue(1, 0) == 1) 
    {
        iprintf("\x1b[14;5H*");    
    }
    else
    {
        iprintf("\x1b[14;5H ");    
    }
    
    if (ca.getBooleanRuleValue(1, 1) == 2)
    {
        iprintf("\x1b[14;10H*");    
    }
    else
    {
        iprintf("\x1b[14;10H ");    
    }        
    
    if (ca.getBooleanRuleValue(1, 2) == 3) 
    {
        iprintf("\x1b[14;15H*");    
    }
    else
    {
        iprintf("\x1b[14;15H ");    
    }
        
    if (ca.getBooleanRuleValue(1, 3) == 4) 
    {
        iprintf("\x1b[15;5H*");    
    }
    else
    {
        iprintf("\x1b[15;5H ");    
    }
        
    if (ca.getBooleanRuleValue(1, 4) == 5)
    {
        iprintf("\x1b[15;10H*"); 
    }
    else
    {
        iprintf("\x1b[15;10H ");
    }

    if (ca.getBooleanRuleValue(1, 5) == 6) 
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
 * Prints the asterisks used to mark the options in the menu of the Boolean Triangular automata
 */
int printBTAasterisks()
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
    
    // {1, 2, 3} For the boolean triangular automata with Von Neumann neighborhood
    if (intBooleanRulesValuesTriVN[0] == 1)
    {
        iprintf("\x1b[14;5H*");    
    }
    else
    {
        iprintf("\x1b[14;5H ");    
    }
    
    if (intBooleanRulesValuesTriVN[1] == 2) 
    {
        iprintf("\x1b[14;10H*");    
    }
    else
    {
        iprintf("\x1b[14;10H ");    
    }
            
    if (intBooleanRulesValuesTriVN[2] == 3) 
    {
        iprintf("\x1b[14;15H*");    
    }
    else
    {
        iprintf("\x1b[14;15H ");    
    }
            
    // {1, 2, 3, 4, 5, 6, 7, 8} For the boolean square automata with Moore neighborhood
    if (intBooleanRulesValuesTriM[0] == 1) 
    {
        iprintf("\x1b[16;5H*");    
    }
    else
    {
        iprintf("\x1b[16;5H ");    
    }
    
    if (intBooleanRulesValuesTriM[1] == 2) 
    {
        iprintf("\x1b[16;10H*");    
    }
    else
    {
        iprintf("\x1b[16;10H ");    
    }        
    
    if (intBooleanRulesValuesTriM[2] == 3) 
    {
        iprintf("\x1b[16;15H*");    
    }
    else
    {
        iprintf("\x1b[16;15H ");    
    }
        
    if (intBooleanRulesValuesTriM[3] == 4) 
    {
        iprintf("\x1b[16;20H*");    
    }
    else
    {
        iprintf("\x1b[16;20H ");    
    }
        
    if (intBooleanRulesValuesTriM[4] == 5) 
    {
        iprintf("\x1b[17;5H*");    
    }
    else
    {
        iprintf("\x1b[17;5H ");    
    }
        
    if (intBooleanRulesValuesTriM[5] == 6) 
    {
        iprintf("\x1b[17;10H*");    
    }
    else
    {
        iprintf("\x1b[17;10H ");    
    }
            
    if (intBooleanRulesValuesTriM[6] == 7) 
    {
        iprintf("\x1b[17;15H*");    
    }
    else
    {
        iprintf("\x1b[17;15H ");    
    }
        
    if (intBooleanRulesValuesTriM[7] == 8)     
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
        else if (index == 6) // Boolean Triangular Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_CGL;
        }        
        else if (index == 7) // Munching squares
        {    
            row = MENU_AUTOMATA_TYPE_ROW_MS;
        }        
        else if (index == 8) // Select colors
        {
            row = MENU_SELECT_COLORS;
        }
        else if (index == 9) // Select language
        {
            row = MENU_SELECT_LANGUAGE;
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
        else if (index == 4) // Moore neighborhood
        {
            row = 15;
        }
        else if (index == 5) // 1
        {
            row = 16;
            column = 3;
        }
        else if (index == 6) // 2
        {
            row = 16;
            column = 8;
        }
        else if (index == 7) // 3
        {
            row = 16;
            column = 13;
        }
        else if (index == 8) // 4
        {
            row = 16;
            column = 18;
        }
        else if (index == 9) // 5
        {
            row = 17;
            column = 3;
        }
        else if (index == 10) // 6
        {
            row = 17;
            column = 8;
        }
        else if (index == 11) // 7
        {
            row = 17;
            column = 13;
        }
        else if (index == 12) // 8
        {
            row = 17;
            column = 18;
        }
        else if (index == 13) // Back to main menu
        {
            row = 19;
        }
    }
    else if (intDisplayedMenu == 7) // Conway's game of life menu
    {
        if (index == 0)
        {
            row = 13; // Back to main menu
        }
    }
    else if (intDisplayedMenu == 8) // Munching squares menu
    {
        if (index == 0)
        {
            row = 13; // Back to main menu
        }
    }
    else if (intDisplayedMenu == 9) // Select colors
    {
        if (index == 0) // BG red
        {
            row = 9;
        }
        else if (index == 1) // BG green
        {
            row = 10;
        }
        else if (index == 2) // BG blue
        {
            row = 11;
        }
        else if (index == 3) // FG red
        {
            row = 13;
        }
        else if (index == 4) // FG green
        {
            row = 14;
        }
        else if (index == 5) // FG blue
        {
            row = 15;
        }
        else if (index == 6) // Line color red
        {
            row = 17;
        }
        else if (index == 7) // Line color green
        {
            row = 18;
        }
        else if (index == 8) // Line color blue
        {
            row = 19;
        }
        else if (index == 9) // Back to main menu
        {
            row = 21;
        }
    }
    else if (intDisplayedMenu == 10) // Select language    
    {
        if (index == 0) // English
        {
            row = 10;
        }
        else if (index == 1) // Espanol
        {
            row = 11;
        }
        else if (index == 2) // Euskara
        {
            row = 12;
        }
        else if (index == 3) // Back to main menu
        {
            row = 14;
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

int printAutomataType(int automataType)
{
    printf("%s:\n", stringAutomataType.c_str()); 
    
    if (automataType == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        printf("%s", stringElementaryCellularAutomata.c_str());
    }
    else if (automataType == LANGTON_ANT)
    {
        printf("%s", stringLangtonsAnt.c_str());
    }
    else if (automataType == LANGTON_HEXAGONAL_ANT)
    {
        printf("%s", stringLangtonsHexagonalAnt.c_str());
    }
    else if (automataType == BOOLEAN_AUTOMATA)
    {
        printf("%s", stringBooleanAutomata.c_str());
    }
    else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        printf("%s", stringBooleanHexagonalAutomata.c_str());
    }
    else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        printf("%s", stringBooleanTriangularAutomata.c_str());
    }
    else if (automataType == CONWAYS_GAME_OF_LIFE)
    {
        printf("%s", stringConwaysGameOfLife.c_str());
    }
    else if (automataType == MUNCHING_SQUARES)
    {
        printf("%s", stringMunchingSquares.c_str());
    }
    
    return 0;
}

/*
 * Prints the number of steps
 */
int printNumSteps(int automataType, int steps)
{
    if (automataType == LANGTON_ANT)
    {
        iprintf("\x1b[9;0H%s:     ", stringSteps.c_str());                                                    
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), ca.getNumSteps());
    }
    else if (automataType == LANGTON_HEXAGONAL_ANT)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), ca.getNumSteps());
    }
    else if (automataType == BOOLEAN_AUTOMATA)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), steps);
    }
    else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), ca.getNumSteps());
    }
    else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), steps);
    }
    else if (automataType == CONWAYS_GAME_OF_LIFE)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), steps);
    }    
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
        updateColors();
        printRuleNumber(calculateRuleNumber());
  	    drawElementaryCellularAutomata();	        
    }
    else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128*1024);
        showFB2();
        
        initializeBooleanTriangularAutomata(127, 91);
    }
    else if (automataType == CONWAYS_GAME_OF_LIFE)
    {
        showFB();
        dmaCopy(fb, fb2, 128*1024);
        showFB2();
        
        initializeConwaysGameOfLife();
    }
    else if(automataType == SELECT_COLORS)
    {
        showFB();
        cleanFB(fb);
                
        drawRectangle(true, 75, 100, 50, 50);

        // top
        for (int i = 65; i < 75; ++i)
        {
            drawHLine(90, i, 70, line_color, fb);
        }
        
        // bottom
        for (int i = 125; i < 135; ++i)
        {
            drawHLine(90, i, 70, line_color, fb);
        }

        // left
        for (int i = 90; i < 100; ++i)
        {
            drawVLine(i, 65, 70, line_color);
        }
                
        // right
        for (int i = 150; i < 160; ++i)
        {
            drawVLine(i, 65, 70, line_color);
        }
    }
    else if (automataType == SELECT_LANGUAGE)
    {
        showFlash();
        cleanFB(fb);
    }
    
    return 0;
}

/*********************************** START MAIN FUNCTION *************************************************************************/

/*
 *
 */
int main(void)
{   
    changeTextLanguage(displayedLanguage);
            
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
                    automataType = 9;
                    printMenuArrow(displayedMenu, automataType, false); // Print new arrow
                }   
            }
            else if (keys_released & KEY_DOWN)
            {
                if (automataType != 9)
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
                    
                    printAutomataType(ELEMENTARY_CELLULAR_AUTOMATA);
                    
                    intArrow = 0;                    
                    displayedMenu = 1;
                    
                    printMenu(displayedMenu);
                    
                    runAutomata();
                }
                else if (automataType == LANGTON_ANT)
                {
                    ca.setType("LA");
                    
                    ca.setAntNumPixels(4);
                                        
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(LANGTON_ANT);
                              
                    intArrow = 0;
                    displayedMenu = 2;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                    
                    ca.initialize();
                }
                else if (automataType == LANGTON_HEXAGONAL_ANT)
                {
                    ca.setType("LHA");

                    consoleClear();
                    printCredits();
                    
                    printAutomataType(LANGTON_HEXAGONAL_ANT);
                    
                    intArrow = 0;
                    displayedMenu = 3;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
                    ca.initialize();
                }
                else if (automataType == BOOLEAN_AUTOMATA)
                {
                    ca.setType("BA");
                    ca.setTypeOfNeighborhood(0); // Von Neumann neighborhood (default)

                    consoleClear();
                    printCredits();

                    printAutomataType(BOOLEAN_AUTOMATA);

                    intArrow = 0;
                    displayedMenu = 4;

                    printMenu(displayedMenu);

                    printMenuArrow(displayedMenu, intArrow, false);                    

                    ca.initializeBooleanRuleValues();

                    ca.initialize();

                    printBAasterisks();
                }
                else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
                {
                    ca.setType("BHA");

                    consoleClear();
                    printCredits();
                    
                    printAutomataType(BOOLEAN_HEXAGONAL_AUTOMATA);
                    
                    intArrow = 0;
                    displayedMenu = 5;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);

                    ca.initializeBooleanRuleValues();               
                    ca.initialize();

                    printBHAasterisks();
                }
                else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(BOOLEAN_TRIANGULAR_AUTOMATA);
                    
                    intArrow = 0;
                    displayedMenu = 6;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
                    printBTAasterisks();
                    runAutomata();
                }
                else if (automataType == CONWAYS_GAME_OF_LIFE)
                {
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(CONWAYS_GAME_OF_LIFE);
                    
                    intArrow = 0;
                    displayedMenu = 7;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
                    //printCGLasterisks();
                    runAutomata();
                }
                else if (automataType == MUNCHING_SQUARES)
                {   
                    ca.setType("MS");

                    consoleClear();
                    printCredits();
                    
                    printAutomataType(MUNCHING_SQUARES);
                    
                    intArrow = 0;                    
                    displayedMenu = 8;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);

                    ca.initialize();                                        
                }
                else if (automataType == SELECT_COLORS)
                {
                    consoleClear();
                    printCredits();
                    
                    printf("%s:", stringSelectColors.c_str());
                    
                    intArrow = 0;
                    displayedMenu = 9;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
                    runAutomata();
                }
                else if (automataType == SELECT_LANGUAGE)
                {
                    consoleClear();
                    printCredits();
                    
                    printf("%s:", stringSelectLanguage.c_str());
                    
                    intArrow = 0;
                    displayedMenu = 10;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                    printLanguageAsterisks();
                    
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
    	    if (ca.hasFinished() == false)
    	    {
    	        ca.nextStep();
                printNumSteps(LANGTON_ANT, ca.getNumSteps());
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
                    if (ca.getAntNumPixels() > 1)
                    {
                        ca.setAntNumPixels(ca.getAntNumPixels() - 1);
                        iprintf("\x1b[11;2H%s: < %d >", stringAntsPixels.c_str(), ca.getAntNumPixels());
                        ca.initialize();
                        printNumSteps(LANGTON_ANT, ca.getNumSteps());
                    }                        
                }		        
		    }
		    else if (keys_pressed & KEY_RIGHT)
		    {
                if (intArrow == 0)
                {
                    ca.setAntNumPixels(ca.getAntNumPixels() + 1);
                    iprintf("\x1b[11;2H%s: < %d >", stringAntsPixels.c_str(), ca.getAntNumPixels());
                    ca.initialize();
                    printNumSteps(LANGTON_ANT, ca.getNumSteps());                    
                }		    
		    }
        }
        /*
         * Langton's hexagonal ant menu
         */                
        else if (displayedMenu == 3)
        {
    	    if (ca.hasFinished() == false) // If the ant hasn't finished continue with the next step
    	    {
    	        ca.nextStep();
                printNumSteps(LANGTON_HEXAGONAL_ANT, ca.getNumSteps());
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
            ca.nextStep();
            printNumSteps(BOOLEAN_AUTOMATA, ca.getNumSteps());
            
       	    if(keys_released & KEY_A)
		    {
		        if (intArrow == 0)
		        {
		            ca.setTypeOfNeighborhood(0);
		            printBAasterisks();
		            ca.initialize();
		        }
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4)
		        {
		            if (ca.checkBooleanRuleValue(intArrow - 1, intArrow))
		            {
		                ca.setBooleanRuleValue(intArrow - 1, 0);
		            }
		            else
		            {
		                ca.setBooleanRuleValue(intArrow - 1, intArrow);
		            }
		            
		            printBAasterisks();

		            ca.initialize();
		        }
		        else if (intArrow == 5)
		        {
		            ca.setTypeOfNeighborhood(1);
		            printBAasterisks();
		            ca.initialize();
		        }
		        else if (intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12 || intArrow == 13)
		        {
		            if (ca.checkBooleanRuleValue(intArrow - 6, intArrow - 5))
		            {
		                ca.setBooleanRuleValue(intArrow - 6, 0);
		            }
		            else
		            {
		                ca.setBooleanRuleValue(intArrow - 6, intArrow - 5);
		            }

		            printBAasterisks();

		            ca.initialize();
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
            ca.nextStep();
            
            printNumSteps(BOOLEAN_HEXAGONAL_AUTOMATA, ca.getNumSteps());

            if (keys_released & KEY_A)
		    {
		        if (intArrow == 0 || intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4 || intArrow == 5)
		        {
		            if (ca.checkBooleanRuleValue(intArrow, intArrow + 1))
		            {
		                ca.setBooleanRuleValue(intArrow, 0);
		            }
		            else
		            {
		                ca.setBooleanRuleValue(intArrow, intArrow + 1);
		            }
		            
		            printBHAasterisks();

		            ca.initialize();
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
            printNumSteps(BOOLEAN_TRIANGULAR_AUTOMATA, automataSteps);
            
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

       	    if(keys_released & KEY_A)
		    {
		        if (intArrow == 0) // Von Neumann neighborhood
		        {
		            intTypeOfNeighborhood = 0;
		            printBTAasterisks();
		            runAutomata();
		        }
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3) // Von Neumann neighborhood 1, 2, 3
		        {
		            if (intBooleanRulesValuesTriVN[intArrow - 1] == intArrow)
		            {
		                intBooleanRulesValuesTriVN[intArrow - 1] = 0;
		            }
		            else
		            {
		                intBooleanRulesValuesTriVN[intArrow - 1] = intArrow;
		            }
		            
		            printBTAasterisks();
		            runAutomata();
		        }
		        else if (intArrow == 4) // Moore neighborhood
		        {
		            intTypeOfNeighborhood = 1;
		            printBTAasterisks();
		            runAutomata();
		        }
		        else if (intArrow == 5 || intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12) // Moore neighborhood 1, 2, 3, 4, 5, 6, 7, 8
		        {
		            if (intBooleanRulesValuesTriM[intArrow - 5] == intArrow - 4)
		            {
		                intBooleanRulesValuesTriM[intArrow - 5] = 0;
		            }
		            else
		            {
		                intBooleanRulesValuesTriM[intArrow - 5] = intArrow - 4;
		            }		      
		            
		            printBTAasterisks();
		            runAutomata();  
		        }
		        else if (intArrow == 13)
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
		            intArrow = 13;
		        }
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3) // 1, 2, 3 from Von Neumann -> Von Neumann
		        {
		            intArrow = 0;
		        }
		        else if (intArrow == 4) // Moore -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
		        else if (intArrow == 5 || intArrow == 6 || intArrow == 7 || intArrow == 8) // 1, 2, 3, 4 from Moore -> Moore
		        {
		            intArrow = 4;
		        }
		        else if (intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12) // 5 -> 1, 6 -> 2, 7 -> 3, 8 -> 4 (Moore)
		        {
		            intArrow = intArrow - 4;
		        }
		        else if (intArrow == 13) // Back to main menu -> 5 from Moore
		        {
		            intArrow = 9;
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
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3) // 1, 2, 3 from Von Neumann -> Moore
		        {
		            intArrow = 4;
		        }
		        else if (intArrow == 4) // Moore -> 1 from Moore
		        {
		            intArrow = 5;
		        }
		        else if (intArrow == 5 || intArrow == 6 || intArrow == 7 || intArrow == 8) // 1 -> 5, 2 -> 6, 3 -> 7, 4 -> 8 (Moore)
		        {
		            intArrow = intArrow + 4;
		        }
		        else if (intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12) // 5, 6, 7, 8 from Moore -> Back to main menu
		        {
		            intArrow = 13;
		        }
		        else if (intArrow == 13) // Back to main menu -> 1 from Von Neumann
		        {
		            intArrow = 0;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        
		    }
		    else if (keys_released & KEY_LEFT)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        		    
		        if (intArrow == 1)
		        {
		            intArrow = 3;
		        }
		        else if (intArrow == 5 || intArrow == 9) // 1 -> 4 (Von Neumann), 1 -> 4, 5 -> 8 (Moore)
		        {
		            intArrow = intArrow + 3;
		        }
                // 2 -> 1, 3 -> 2, 4 -> 3 (Von Neumann), 2 -> 1, 3 -> 2, 4 -> 3, 6 -> 5, 7 -> 6, 8 -> 7 (Moore)
		        else if (intArrow == 2 || intArrow == 3 || intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 10 || intArrow == 11 || intArrow == 12) 
		        {
		            intArrow = intArrow - 1;
		        }

		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        		        
		    }
		    else if (keys_released & KEY_RIGHT)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        		    
		        if (intArrow == 3)
		        {
		            intArrow = 1;
		        }
		        else if (intArrow == 8 || intArrow == 12) // 4 -> 1 (Von Neumann), 4 -> 1, 8 -> 5 (Moore)
		        {
		            intArrow = intArrow - 3;
		        }
                // 1 -> 2, 2 -> 3, 3 -> 4 (Von Neumann), 1 -> 2, 2 -> 3, 3 -> 4, 5 -> 6, 6 -> 7, 7 -> 8 (Moore)
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 5 || intArrow == 6 || intArrow == 7 || intArrow == 9 || intArrow == 10 || intArrow == 11) 
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow		        		        
		    }		                
        }
        /*
         * Conway's game of life
         */                
        else if (displayedMenu == 7)
        {    	    
            automataSteps++;
            printNumSteps(CONWAYS_GAME_OF_LIFE, automataSteps);
            
            calculateNextStepConwaysGameOfLife();
            
            if (automataSteps % 2 == 0 and automataSteps != 1)
            {
                showFB();
            }
            else
            {
                showFB2();
            }
            swiWaitForVBlank(); 

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
         * Munching squares menu
         */
        else if (displayedMenu == 8)
        {
  		    if(keys_released & KEY_A)
  		    {
                if (intArrow == 0) // Back to main menu
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
            
            ca.nextStep();
        }
        /*
         * Color selection menu
         */
        else if (displayedMenu == 9)
        {
  		    if(keys_released & KEY_A)
  		    {
  		        if (intArrow == 9) // Back to main menu
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        
		        if (intArrow == 0)
		        {
		            intArrow = 9;
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
		        
		        if (intArrow == 9)
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
		            if (BG_R != 0)
		            {
		                --BG_R;
		            }
		        }
		        else if (intArrow == 1)
		        {
		            if (BG_G != 0)
		            {
		                --BG_G;
		            }
		        }		      
		        else if (intArrow == 2)
		        {
		            if (BG_B != 0)
		            {
		                --BG_B;
		            }
		        }		      
		        if (intArrow == 3)
		        {
		            if (FG_R != 0)
		            {
		                --FG_R;
		            }
		        }
		        else if (intArrow == 4)
		        {
		            if (FG_G != 0)
		            {
		                --FG_G;
		            }
		        }		      
		        else if (intArrow == 5)
		        {
		            if (FG_B != 0)
		            {
		                --FG_B;
		            }
		        }		   
		        if (intArrow == 6)
		        {
		            if (line_R != 0)
		            {
		                --line_R;
		            }
		        }
		        else if (intArrow == 7)
		        {
		            if (line_G != 0)
		            {
		                --line_G;
		            }
		        }		      
		        else if (intArrow == 8)
		        {
		            if (line_B != 0)
		            {
		                --line_B;
		            }
		        }		      		           		            

                printMenu(displayedMenu);		
                		        
		        BG_color = RGB15(BG_R, BG_G, BG_B);
		        FG_color = RGB15(FG_R, FG_G, FG_B);
                line_color = RGB15(line_R, line_G, line_B);		        
                
                runAutomata();
                
                swiWaitForVBlank();        
		    }    
		    else if (keys_pressed & KEY_RIGHT)
		    {
		        if (intArrow == 0)
		        {
		            if (BG_R != 31)
		            {
		                ++BG_R;
		            }
		        }
		        else if (intArrow == 1)
		        {
		            if (BG_G != 31)
		            {
		                ++BG_G;
		            }
		        }		      
		        else if (intArrow == 2)
		        {
		            if (BG_B != 31)
		            {
		                ++BG_B;
		            }
		        }		      
		        if (intArrow == 3)
		        {
		            if (FG_R != 31)
		            {
		                ++FG_R;
		            }
		        }
		        else if (intArrow == 4)
		        {
		            if (FG_G != 31)
		            {
		                ++FG_G;
		            }
		        }		      
		        else if (intArrow == 5)
		        {
		            if (FG_B != 31)
		            {
		                ++FG_B;
		            }
		        }		   
		        if (intArrow == 6)
		        {
		            if (line_R != 31)
		            {
		                ++line_R;
		            }
		        }
		        else if (intArrow == 7)
		        {
		            if (line_G != 31)
		            {
		                ++line_G;
		            }
		        }		      
		        else if (intArrow == 8)
		        {
		            if (line_B != 31)
		            {
		                ++line_B;
		            }
		        }	
		        
                printMenu(displayedMenu);
                		        
		        BG_color = RGB15(BG_R, BG_G, BG_B);
		        FG_color = RGB15(FG_R, FG_G, FG_B);
                line_color = RGB15(line_R, line_G, line_B);
                	      		           		            
                runAutomata();
                
                swiWaitForVBlank();                                	      		           		            
		    }    		    
        }
        /*
         * Language selection menu
         */
        else if (displayedMenu == 10)
        {
  		    if(keys_released & KEY_A)
  		    {
  		        if (intArrow == 0) // Change language to english
  		        {
  		            displayedLanguage = "en";
  		            changeTextLanguage(displayedLanguage);

                    consoleClear();
                    printCredits();
                    
                    printf("%s:", stringSelectLanguage.c_str());
  		            
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                    printLanguageAsterisks();
  		        }
  		        else if (intArrow == 1) // Change language to spanish
  		        {
  		            displayedLanguage = "es";
  		            changeTextLanguage(displayedLanguage);

                    consoleClear();
                    printCredits();
                    
                    printf("%s:", stringSelectLanguage.c_str());

                    printMenu(displayedMenu);

                    printMenuArrow(displayedMenu, intArrow, false);
                    printLanguageAsterisks();
  		        }
  		        else if (intArrow == 2) // Change language to basque
  		        {
  		            displayedLanguage = "eu";
  		            changeTextLanguage(displayedLanguage);

                    consoleClear();
                    printCredits();
                    
                    printf("%s:", stringSelectLanguage.c_str());

                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                    printLanguageAsterisks();
  		        }
  		        else if (intArrow == 3) // Back to main menu
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        
		        if (intArrow == 0)
		        {
		            intArrow = 3;
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
		        
		        if (intArrow == 3)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
		    }
        }
    }
    
	return 0;
}
/*********************************** END MAIN FUNCTION *************************************************************************/
