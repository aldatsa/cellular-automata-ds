// I use cmath to calculate powers of 2 in the getRuleNumber function.
// Is it possible to do it in a more simple way? Using binary operators?
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <stdio.h>

#include <nds.h>

#include "cellularautomata.h"
#include "framebuffer.h"
#include "color.h"
#include "hexgrid.h"
#include "triangulargrid.h"
#include "globals.h"
#include "initialConditions.h"
#include "random_numbers.h"

/*
 * References:
 *
 * ELEMENTARY CELLULAR AUTOMATA:
 * http://mathworld.wolfram.com/ElementaryCellularAutomaton.html
 * http://atlas.wolfram.com/01/01/
 * http://en.wikipedia.org/wiki/Cellular_automaton#Elementary_cellular_automata
 * http://en.wikipedia.org/wiki/Elementary_cellular_automaton
 *
 * LANGTON'S ANT:
 * http://en.wikipedia.org/wiki/Langton%27s_ant
 * http://mathworld.wolfram.com/LangtonsAnt.html
 *
 * MUNCHING SQUARES:
 * http://en.wikipedia.org/wiki/Munching_square
 * http://mathworld.wolfram.com/MunchingSquares.html
 * http://www.inwap.com/pdp10/hbaker/hakmem/hacks.html#item146
 *
 * CONWAY'S GAME OF LIFE:
 * http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 * http://www.conwaylife.com/wiki/Main_Page
 * Stephen Silver's Life Lexicon:
 *  http://www.argentum.freeserve.co.uk/lex_home.htm
 * Eric Weisstein's Treasure Trove of the Life Cellular Automaton:
 * http://www.ericweisstein.com/encyclopedias/life/
 *
 * BIHAM-MIDDLETON-LEVINE TRAFFIC MODEL:
 * http://en.wikipedia.org/wiki/Biham-Middleton-Levine_traffic_model
 * http://www.jasondavies.com/bml/
 * http://mae.ucdavis.edu/dsouza/bml.html
 * http://openprocessing.org/sketch/27491 
 *
 * CYCLIC CELLULAR AUTOMATA:
 * http://en.wikipedia.org/wiki/Cyclic_cellular_automaton
 */

/*********************************PRIVATE*************************************/

/*
 * Calculates the new angle of the ant
 */
int CellularAutomata::rotateAnt(unsigned char rotateTo)
{
    int limitAngle;
    int stepAngle;
    
    /*
     * Langton's hexagonal ant
     * Right ('R'): 0 -> 60 -> 120 -> 180 -> 240 ->
     *            300 (limitAngle) -> 0 -> 60 -> ...
     * Left ('L'): 0 -> 300 (limitAngle) -> 240 ->
     *           180 -> 120 -> 60 -> 0 -> 300 -> ...
     */
    if (type == LANGTON_HEXAGONAL_ANT) 
    {
        limitAngle = 300;
        stepAngle = 60;
    }
    /*
     * Langton's ant
     * Right ('R'): 0 -> 90 -> 180 -> 270 (limitAngle) -> 0 -> 90 -> ...
     * Left ('L'): 0 -> 270 (limitAngle) -> 180 -> 90 -> 0 -> 270 -> ...
     */
    else if (type == LANGTON_ANT)
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
	
	// If the cell has FG_color paint it with BG_color
	if (fb[antPosY * SCREEN_WIDTH + antPosX] == FG_color)
	{
		tempColor = BG_color;
		--population; // The population of alive cells decreases
	}
	// else if the cell has BG_color paint it with FG_color
	else
	{
		tempColor = FG_color;
		++population; // The population of alive cells increases
	}

    if (type == LANGTON_HEXAGONAL_ANT)
    {
	    paintHexCell(antPosX, antPosY, tempColor, fb);
	}
	else if (type == LANGTON_ANT)
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
    if (type == LANGTON_HEXAGONAL_ANT)
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
    else if (type == LANGTON_ANT)
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
 * where column is a value in the range 0-63 and row is a value in the
 * range 0-63.
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
int CellularAutomata::drawMunchingSquare(int column,
                                         int row,
                                         int width,
                                         unsigned short color)
{
    for (int k = 0; k < width; k++)
    {
        drawHLine(32 + width * column, width * row + k, width, color, fb);
    }
    
    return 0;
}

/*
 * Paints the Elementary Cellular Automata's cell with the corresponding color
 * and if the color matches FG_color, increments the population of alive cells
 */
int CellularAutomata::paintECAcell(int row, int col, unsigned short color)
{
    fb[row * SCREEN_WIDTH + col] = color;

    if (color == FG_color)
    {
        ++population;
    }

    return 0;
}
  
/*
 * Updates the variables used for the colors of the background,
 * foreground and lines.
 * It's used to update the colors of the rules of the elementary
 * cellular automata when new colors are selected in the color selection menu  
 * and to initiliaze the values when the automata is executed for
 * the first time.
 *
 *  xxx xxo xox xoo oxx oxo oox ooo
 *   o   x   o   x   x   o   x   o  -> Rule 90
 * 
 * 0*128 + 1*64 + 0*32 + 1*16 + 1*8 + 0*4 + 1*2 + 0*1 = 90
 *
 * where x == Foreground color with value 1
 *       o == Background color with value 0
 *
 *       ruleLeft  ruleCenter  ruleRight
 *                  ruleDown
 *
 */
int CellularAutomata::updateECAruleColors()
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

    /* 
     * The rule that will be displayed on start on the Elementary Cellular
     * Automata: Rule 90 (Default)
     */
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
 * Calculates the rule number for the current rule of the
 * Elementary Cellular Automata
 */ 
int CellularAutomata::getRuleNumber()
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
 *
 */
unsigned short CellularAutomata::getRuleDown(int index)
{
    return ruleDown[index];
}

/*
 *
 */
int CellularAutomata::setRuleDown(int index, unsigned short color)
{
    ruleDown[index] = color;

    return 0;
}

/*
 * Draws an arrow in the top screen (0-7) of the Elementary Cellular Automata
 */
int CellularAutomata::drawArrow(char nth, unsigned short color)
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
 * Draws the initial state of the cellular automata
 */
int CellularAutomata::drawInitialState()
{   
    if (type == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        // Paint the initial cell configuration of the first row for the
        // Elementary Cellular Automata
        if (ECA_initial_state == ECA_INITIALIZE_ONE_CELL)
        {
            fb[128] = FG_color;
        
            ++population; // The population of alive cells increases
        }
        else if (ECA_initial_state == ECA_INITIALIZE_RANDOM)
        {
            int rand_value = 0;

            // Initialize random seed
            srand(time(0));

            // Move along the cells of the first row (0-255)
            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                // Create random value (0 or 1)
                rand_value = rand() % 2;

                // If the value is 1, paint the corresponding cell with
                // FG_color and add 1 to the population count
                if (rand_value == 1)
                {
                    fb[i] = FG_color;
                    ++population;
                }
            }
        }
    }
    else if (type == CONWAYS_GAME_OF_LIFE) //Conway's game of life
    {
        if (initialState == ACORN)
        {
            drawAcorn(120, 90);
            population = population + 7; // 7 initial cells
        }
        else if (initialState == FPENTOMINO)
        {
            drawFpentomino(120, 90);
            population = population + 5; // 5 initial cells
        }
        else if (initialState == FILL_SCREEN_WITH_PULSARS)
        {
            fillScreenWithPulsars();
            // 13 cells per pulsar multiplied by 165 pulsars
            // (11 rows of 15 columns)
            population = population + 13 * 165;
        }
        else if (initialState == FILL_SCREEN_WITH_PENTADECATHLONS)
        {
            fillScreenWithPentadecathlons();
            // 12 cells per pentadecathlon multiplied by 266 pentadecathlons
            // (19 rows of 14 columns)
            population = population + 12 * 266;
        }
    }
    else if (type == CYCLIC_CELLULAR_AUTOMATA)
    {
        int rand_value = 0;

        // Initialize random seed
        srand(time(0));

        // Move along the cells of the first row (0-255)
        for (int i = 0; i < SCREEN_WIDTH; ++i)
        {
            // Create random value between 0 and 3 (n = 4)
            rand_value = rand() % 4;
            
            // Paint the cell with the corresponding color
            fb[i] = cyclicAutomataColors[rand_value];
            ++population;
        }
    }
    else if (type == STEPPING_STONE)
    {
	int rand_value = 0;
	
	int color_r = 0;
	int color_g = 0;
	int color_b = 0;
	
	// Initializa random seed.
	srand(time(0));
	
	for (int i = 0; i < SCREEN_WIDTH; ++i)
	{
	    for (int j = 0; j < SCREEN_HEIGHT; ++j)
	    {
		// Create a random value between 0 and 31.
		color_r = rand() % 32;
		color_g = rand() % 32;
		color_b = rand() % 32;
		
		fb[j * SCREEN_WIDTH + i] = RGB15(color_r, color_g, color_b);
	    }
	}
    }
    
    return 0;
}

/********************************PUBLIC**************************************/

// Class constructor
CellularAutomata::CellularAutomata()
{
    setDefaultBooleanRuleValues();

    BML_density = BML_INITIAL_DENSITY;
}

int CellularAutomata::setType(int t)
{
    type = t;
    
    return 0;
}

int CellularAutomata::getType()
{
    return type;
}

unsigned int CellularAutomata::getNumSteps()
{
    return numSteps;
}

unsigned int CellularAutomata::getPopulation()
{
    return population;
}

int CellularAutomata::getNumStates()
{
    return numStates;
}

int CellularAutomata::setNumStates(int n)
{
    numStates = n;

    return 0;
}

int CellularAutomata::getInitialStateType()
{
    return ECA_initial_state;
}

int CellularAutomata::setInitialStateType(int initialStateType)
{
    ECA_initial_state = initialStateType;

    return 0;
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
 * Sets the default boolean rule values
 */
int CellularAutomata::setDefaultBooleanRuleValues()
{
    booleanRuleValues[VON_NEUMANN_NEIGHBORHOOD] = 1;
    booleanRuleValues[MOORE_NEIGHBORHOOD] = 1;

    return 0;
}

/*
 *
 */
bool CellularAutomata::checkBooleanRuleValue(int neighborhoodType,
                                             int ruleIndex)
{
    return ((booleanRuleValues[neighborhoodType] & (1 << ruleIndex)) ==
             (1 << ruleIndex));
}

/*
 *
 */
int CellularAutomata::toggleBooleanRuleValue(int neighborhoodType,
                                             int ruleIndex)
{
    /*
     * As we start with only one square, if the first value is not checked
     * the automata enters a loop of start, nothing changes, restart and    
     * the screen flashes.
     */
    if (ruleIndex != 0)
    {
        // Toggle the bit
        booleanRuleValues[neighborhoodType] ^= 1 << ruleIndex;
    }

    return 0;
}

int CellularAutomata::setTypeOfNeighborhood(int value)
{
    typeOfNeighborhood = value;
    
    return 0;
}

int CellularAutomata::getTypeOfNeighborhood()
{
    return typeOfNeighborhood;
}

/*
 * Returns the type of initial state of the Conway's game of life
 */
int CellularAutomata::getInitialState()
{
    return initialState;
}

/*
 * Sets the type of initial state of the Conway's game of life
 */
int CellularAutomata::setInitialState(int state)
{
    initialState = state;
    return 0;
}

/*
 * Returns the current density of the BML traffic model
 */
int CellularAutomata::getBMLdensity()
{
    return BML_density;
}

/*
 * Sets the density of the BML traffic model
 */
int CellularAutomata::setBMLdensity(int BMLdensity)
{
    BML_density = BMLdensity;

    return 0;
}

/*
 * Cleans the current drawing of the Elementary Cellular Automata,
 * sets numSteps (in this case equivalent to row) to zero,
 * and paints the initial cell distribution (row zero).
 * It's used to reset the ECA when the user changes the rules.
 */
int CellularAutomata::resetECA()
{
    for (int row = 0; row < 130; ++row)
    {
        for (int column = 0; column < SCREEN_WIDTH; ++column)
        {
            fb[row * SCREEN_WIDTH + column] = BG_color;
        }
    }
    
    numSteps = 0;   // Reset the number of steps to 0
    population = 0; // Reset the population of "alive" cells to 0

    drawInitialState();
    
    return 0;
}

/*
 * Cleans the main framebuffer 
 * and initiliazes the values of the variables used for the cellular automata.
 */
int CellularAutomata::initialize()
{
    if (type == SELECT_LANGUAGE)
    {
        showFlash();
    }
    else
    {
        showFB();
        cleanFB(fb);
    }

    numSteps = 0;   // Reset the number of steps to 0
    population = 0; // Reset the population of "alive" cells to 0

    if (type == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        updateECAruleColors();

        // This configuration is for painting the initial cell in
        // the center of the first row (fb[128] = FG_color)
        ECA_initial_state = ECA_INITIALIZE_ONE_CELL;        
        //ECA_initial_state = ECA_INITIALIZE_RANDOM;

        // Paint the first row of the automata that corresponds to the
        // state set using ECA_initial_state
        drawInitialState();

        // Draw the rules that correspond to the default rule
        // (Rule 90, set in updateECAruleColors())
        drawAllRules();
    }
    else if (type == LANGTON_HEXAGONAL_ANT)
    { 
        drawHexGrid();
        antPosX = 92;
        antPosY = 93;
        antAngle = 0;
        antFinished = false;
    }
    else if (type == LANGTON_ANT)
    {
        antPosX = LA_INITIAL_X;
        antPosY = LA_INITIAL_Y;
        antAngle = LA_INITIAL_ANGLE;
        antFinished = false;
    }
    else if (type == BOOLEAN_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128* 1024);
        showFB2();
        
        cleanFB(fb);
        cleanFB(fb2);
    
        fb[91 * SCREEN_WIDTH + 127] = FG_color; // Paint the initial point
        ++population;
    }
    else if (type == BOOLEAN_HEXAGONAL_AUTOMATA)
    {        
        showFB();
        dmaCopy(fb, fb2, 128* 1024);
        showFB2();
        
        cleanFB(fb);
        cleanFB(fb2);

        drawHexGrid();

        /*
         * We use the Moore neighborhood
         * In this case it's a hexagonal neighborhood but as the Moore 
         * neighborhood is a array of 8 ints there is enough space for 6 ints.
         */
        typeOfNeighborhood = MOORE_NEIGHBORHOOD;
        
        paintHexCell(124, 93, FG_color, fb);
        ++population;
    }
    else if (type == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128* 1024);
        showFB2();
        
        cleanFB(fb);
        cleanFB(fb2);

        drawTriangularGrid();
        
        paintTriangularCell(127, 91, FG_color, fb);
        ++population;
    }
    else if (type == CONWAYS_GAME_OF_LIFE)
    {
        showFB();
        dmaCopy(fb, fb2, 128* 1024);
        showFB2();
        
        cleanFB(fb);
        cleanFB(fb2);

        drawInitialState();
    }
    else if (type == BML_TRAFFIC_MODEL)
    {
        showFB();
        dmaCopy(fb, fb2, 128* 1024);
        showFB2();

        cleanFB(fb);
        cleanFB(fb2);

        int total_pixels = SCREEN_WIDTH * SCREEN_HEIGHT;
        double BML_density_percent = (double) BML_density / 100;
        int vector_length = BML_density_percent * total_pixels;

        std::vector<int> random_pixels = get_random_int_vector(vector_length,
                                                               0,
                                                               total_pixels);
        for (std::vector<int>::size_type i = 0;
             i != random_pixels.size(); i++)
        {
            if (i % 2 == 0)
            {
                fb[random_pixels[i]] = FG_color;
            }
            else
            {
                fb[random_pixels[i]] = FG_color2;
            }
            ++population;
        }
    }
    else if (type == CYCLIC_CELLULAR_AUTOMATA)
    {
        showFB();
        dmaCopy(fb, fb2, 128 * 1024);
        showFB2();

        cleanFB(fb);
        cleanFB(fb2);
       
        cyclicAutomataColors[0] = FG_color;
        cyclicAutomataColors[1] = FG_color2;
        cyclicAutomataColors[2] = FG_color3;
        cyclicAutomataColors[3] = FG_color4;

        drawInitialState();
    }
    else if (type == STEPPING_STONE)
    {
        showFB();
        dmaCopy(fb, fb2, 128 * 1024);
        showFB2();

        cleanFB(fb);
        cleanFB(fb2);

        drawInitialState();
	
	showFB();
    }
    else if (type == SELECT_COLORS)
    {
        drawRectangle(75, 70, 50, 50, FG_color);
        drawRectangle(75, 130, 50, 50, FG_color2);

        // top (horizontal)
        for (int i = 65; i < 75; ++i)
        {
            drawHLine(60, i, 130, line_color, fb);
        }

        // bottom (horizontal)
        for (int i = 125; i < 135; ++i)
        {
            drawHLine(60, i, 130, line_color, fb);
        }

        // left (vertical)
        for (int i = 60; i < 70; ++i)
        {
            drawVLine(i, 65, 70, line_color);
        }

        // center (vertical)
        for (int i = 120; i < 130; ++i)
        {
            drawVLine(i, 65, 70, line_color);
        }

        // right (vertical)
        for (int i = 180; i < 190; ++i)
        {
            drawVLine(i, 65, 70, line_color);
        }
    }

    return 0;
}

/*
 *
 */
int CellularAutomata::nextStep()
{
    /*
     * If the type of the automata is Langton's ant or Langton's hexagonal ant,
     * this function combines 3 functions:
     * First, depending on the color of the cell, rotates the ant to one side 
     * or the other using rotateAnt().
     * Next, paints the cell using paintAnt().
     * Finally, moves the ant to the next cell using forwardAnt().
     */
    if (type == LANGTON_ANT or type == LANGTON_HEXAGONAL_ANT)
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
	    if (antPosX < 0 or antPosX + antNumPixels > 254 or antPosY < 0 or
	        antPosY + antNumPixels > SCREEN_HEIGHT - 1)
	    {
	        antFinished = true;
	    }
    }
    else if (type == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        int column;
	    unsigned char i;

        ++numSteps; // In this case numSteps is equivalent to the actual row
        
	    if (numSteps < 130)
	    {
		    for(column = 0; column < SCREEN_WIDTH; column++)
		    {
			    if(numSteps < 128)
			    {
				    for(i = 0; i <= 7; i++)
				    {
					    if(column != 0 && column != SCREEN_WIDTH - 1)
					    { 
						    if (fb[(numSteps - 1) * SCREEN_WIDTH + (column-1)]
						            == ruleLeft[i] &&
						        fb[(numSteps - 1) * SCREEN_WIDTH + column] ==
						            ruleCenter[i] &&
						        fb[(numSteps - 1) * SCREEN_WIDTH + (column+1)] 
						            == ruleRight[i])
						    {
						        // Paint the Elementary Cellular Automata's
						        // cell with ruleDown[i] color
						        // and if the color matches FG_color increment 
						        // the population of alive cells
						        paintECAcell(numSteps, column, ruleDown[i]);
							}
					    }
					    else if(column == 0)
					    {
						    // The left cell is out of the screen, instead
						    // we'll use the center cell (column 0) to compare
						    // to ruleLeft
						    if(fb[(numSteps - 1) * SCREEN_WIDTH + column] ==
						            ruleLeft[i]
						       && fb[(numSteps - 1) * SCREEN_WIDTH + column]
						            == ruleCenter[i]
						       && fb[(numSteps - 1) * SCREEN_WIDTH +
						            (column+1)] == ruleRight[i])
						    {
    						    // Paint the Elementary Cellular Automata's
    						    // cell with ruleDown[i] color
    						    // and if the color matches FG_color increment
    						    // the population of alive cells
						        paintECAcell(numSteps, column, ruleDown[i]); 
            			    }
					    }	
					    else if(column == SCREEN_WIDTH - 1)
					    {
                            // The right cell is out of the screen, instead
                            // we'll use the center cell (column 255) to
                            // compare to ruleRight
						    if(fb[(numSteps - 1) * SCREEN_WIDTH + (column-1)]
						            == ruleLeft[i]
						       && fb[(numSteps - 1) * SCREEN_WIDTH + column]
						            == ruleCenter[i]
						       && fb[(numSteps - 1) * SCREEN_WIDTH + column]
						            == ruleRight[i])
                            {
                                // Paint the Elementary Cellular Automata's
                                // cell with ruleDown[i] color
                                // and if the color matches FG_color increment
                                // the population of alive cells
						        paintECAcell(numSteps, column, ruleDown[i]);
						    }
					    }
				    }
			    }
			    else if(numSteps == 129)
			    {
				    fb[numSteps * SCREEN_WIDTH + column] = FG_color;
			    }
		    }
	    }
    }
    /*
     * Else if the type of the automata is Munching squares , it draws the
     * next step of the munching squares.
     * We use a grid of 64 squares height x 64 squares width where each square
     * is 3 pixels x 3 pixels
     * 64 * 3 = 192 pixels (the height of the screen)
     * See the comments of the function drawSquare above.
     * 
     * XOR is the logical operator "exclusive disjunction", also called
     * "exclusive or".
     * http://en.wikipedia.org/wiki/XOR#Bitwise_operation
     * XOR is a type of logical disjunction on two operands that results in a
     * value of true if exactly one of the operands has a value of true.
     * 1 xor 1 = 0
     * 1 xor 0 = 1
     * 0 xor 1 = 1
     * 0 xor 0 = 0
     * For example, 1110 xor 1001 = 0111
     */
    else if (type == MUNCHING_SQUARES)
    {
        if (numSteps < 64)   
        {
            /*
             * For each combination of i and j where i = 0, 1, ... 63 and
             * j = 0, 1, ... 63
             * we check if the value of (i xor j) is smaller than numSteps.
             * If the result of this comparation is true then we draw a square
             * of 3 pixels x 3 pixels using FG color
             * using the function drawMunchingSquare starting from the column
             * i and the row j.
             */
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
            /*
             * Paint the framebuffer with the BG color to erase the last step
             * of the munching squares and initialize the variables to start
             * another cycle
             */
            initialize();
        }            
    }
    /*
     * Calculates and draws the next step of the boolean square automata.
     * The return value indicates if the automata has finished (return 0)
     * or not (return != 0)
     */
    else if (type == BOOLEAN_AUTOMATA)
    {
        /*
         * typeOfNeighborhood = VON_NEUMANN_NEIGHBORHOOD (0)
         * 4 neighbors.
         *      x
         *    x o x
         *      x
         * http://en.wikipedia.org/wiki/Von_Neumann_neighborhood
         *
         * typeOfNeighborhood = MOORE_NEIGHBORHOOD (1)
         * 8 neighbors.
         *    x x x
         *    x o x
         *    x x x
         * http://en.wikipedia.org/wiki/Moore_neighborhood
         */
              
        unsigned short* fbRef;
        unsigned short* fbNew;
        
        /*
         * changeCount is used to know if the next step is different from the
         * current step.
         * If changeCount == 0 then there're no changes and the automata has
         * finished, so we can start again from step 0.
         * If changeCount != 0 then the automata has not finished yet.
         */ 
        int changeCount = 0; 

        /*
         * This two lines were used to debug the implementation of changeCount
         * (changeCount wasn't 0 when the automata finishes)
         * iprintf("\x1b[20;3HChange count:        ");    
         * iprintf("\x1b[20;3HChange count: %d", changeCount);
         */
        
        int countFG = 0;

        ++numSteps;
        
        if (numSteps % 2 == 0 and numSteps != 1)
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
            for (int j = 1; j < SCREEN_HEIGHT - 1; j++)
            {
                countFG = 0;        
                

                // top 
                if (fbRef[SCREEN_WIDTH * (j - 1) + i] == FG_color)
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
                
                // bottom                            
                if (fbRef[SCREEN_WIDTH * (j + 1) + i] == FG_color)
                {
                    countFG++;
                }
                
                if (typeOfNeighborhood == MOORE_NEIGHBORHOOD)
                {
                    // Top left
                    if (fbRef[SCREEN_WIDTH * (j - 1) + i - 1] == FG_color)
                    {
                        countFG++;
                    }
                    
                    // top right                
                    if (fbRef[SCREEN_WIDTH * (j - 1) + i + 1] == FG_color)
                    {
                        countFG++;
                    }
                                    
                    // Bottom left
                    if (fbRef[SCREEN_WIDTH * (j + 1) + i - 1] == FG_color)
                    {
                        countFG++;
                    }
                    
                    // bottom right
                    if (fbRef[SCREEN_WIDTH * (j + 1) + i + 1] == FG_color)
                    {
                        countFG++;
                    }            
                }
                /*
                 * if the count of surrounding cells with FG_color is n we'll
                 * check if the index n-1 of the boolean rule is checked
                 * (value=1)
                 */
                if (countFG != 0
                    and checkBooleanRuleValue(typeOfNeighborhood,
                                              countFG - 1)) 
                {
                    /*
                     * If the current cell's color is not already changed,
                     * change it to FG_color.
                     * Without this condition each cell is painted more than
                     * one time and changeCount is never equal to 0.
                     */
                    if (fbNew[SCREEN_WIDTH * j + i] != FG_color) 
                    {
                        fbNew[SCREEN_WIDTH * j + i] = FG_color;
                        ++changeCount;
                        // The population of alive cells increases
                        ++population;
                    }
                }
            }
        }
        
        /*
         * This two lines were used to debug the implementation of changeCount
         * (changeCount wasn't 0 when the automata finishes)
         * iprintf("\x1b[21;3HChange count end:        ");    
         * iprintf("\x1b[21;3HChange count end: %d", changeCount);
         */
        
        if (changeCount == 0)
        {
            initialize();
        }
        else // the automata has not finished yet
        {
            if (numSteps % 2 == 0 and numSteps != 1)
            {
                showFB();
            }
            else
            {
                showFB2();
            }
            swiWaitForVBlank();
        }
    }
    /*
     * Else if the the type of automata is Boolean Hexagonal Automata
     * calculates and draws the next step.
     * The return value indicates if the automata has finished (return 0)
     * or not (return != 0)
     */
    else if (type == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        unsigned short* fbRef;
        unsigned short* fbNew;

        /*
         * changeCount is used to know if the next step is different from the
         * current step.
         * If changeCount == 0 then there're no changes and the automata has
         * finished,
         * so we can start again from step 0.
         * If changeCount != 0 then the automata has not finished yet.
         */ 
        int changeCount = 0; 
            
        int countFG = 0;

        ++numSteps;
        
        if (numSteps % 2 == 0 and numSteps != 1)
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
            for (int j = 3; j < SCREEN_HEIGHT - 1; j = j + 4)
            {
                countFG = 0;        

                // top left
                if (fbRef[SCREEN_WIDTH * j + i - 2] == FG_color)
                {
                    countFG++;
                }

                // top 
                if (fbRef[SCREEN_WIDTH * (j - 2) + i] == FG_color)
                {
                    countFG++;
                }
                
                
                // top right
                if (fbRef[SCREEN_WIDTH * j + i + 4] == FG_color)
                {
                    countFG++;
                }
                
                // bottom left    
                if (fbRef[SCREEN_WIDTH * (j + 2) + i - 2] == FG_color)
                {
                    countFG++;
                }
                
                // bottom                            
                if (fbRef[SCREEN_WIDTH * (j + 4) + i] == FG_color)
                {
                    countFG++;
                }

                // bottom right                         
                if (fbRef[SCREEN_WIDTH * (j + 2) + i + 4] == FG_color)
                {
                    countFG++;
                }

                /*
                 * if the count of surrounding cells with FG_color is n we'll
                 * check if the index n-1 of the boolean rule is checked
                 * (value=1)
                 */
                if (countFG != 0
                    and checkBooleanRuleValue(typeOfNeighborhood,
                                              countFG - 1))
                {
                    /*
                     * If the current cell's color is not already changed,
                     * change it to FG_color.
                     * Without this condition each cell is painted more than
                     * one time and changeCount is never equal to 0.
                     */
                    if (fbNew[SCREEN_WIDTH * j + i] != FG_color) 
                    {
                        paintHexCell(i, j, FG_color, fbNew);
                        ++changeCount;
                        // The population of alive cells increases
                        ++population;
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
                if (fbRef[SCREEN_WIDTH * j + i - 2] == FG_color)
                {
                    countFG++;
                }

                // top 
                if (fbRef[SCREEN_WIDTH * (j - 2) + i] == FG_color)
                {
                    countFG++;
                }
                
                
                // top right
                if (fbRef[SCREEN_WIDTH * j + i + 4] == FG_color)
                {
                    countFG++;
                }
                
                // bottom left    
                if (fbRef[SCREEN_WIDTH * (j + 2) + i - 2] == FG_color)
                {
                    countFG++;
                }
                
                // bottom                            
                if (fbRef[SCREEN_WIDTH * (j + 4) + i] == FG_color)
                {
                    countFG++;
                }

                // bottom right                         
                if (fbRef[SCREEN_WIDTH * (j + 2) + i + 4] == FG_color)
                {
                    countFG++;
                }

                /*
                 * if the count of surrounding cells with FG_color is n we'll
                 * check if the index n-1 of the boolean rule is checked
                 * (value=1)
                 */
                if (countFG != 0
                    and checkBooleanRuleValue(typeOfNeighborhood,
                                              countFG - 1))
                {
                    /*
                     * If the current cell's color is not already changed,
                     * change it to FG_color.
                     * Without this condition each cell is painted more than
                     * one time and changeCount is never equal to 0.
                     */
                    if (fbNew[SCREEN_WIDTH * j + i] != FG_color) 
                    {
                        paintHexCell(i, j, FG_color, fbNew);
                        ++changeCount;
                    }
                }
            }
        }

        // The automata has finished so we are going to reinitiate the cycle
        if (changeCount == 0)
        {
            initialize();
        }
        else // the automata has not finished yet
        {
            if (numSteps % 2 == 0 and numSteps != 1)
            {
                showFB();
            }
            else
            {
                showFB2();
            }
            swiWaitForVBlank();
        }
    }
    /*
     * Calculates and draws the next step of the boolean triangular automata.
     * The return value indicates if the automata has finished (return 0)
     * or not (return != 0)
     */
    else if (type == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        /*
         * typeOfNeighborhood = VON_NEUMANN_NEIGHBORHOOD (0) ->
         * Modified Von Neumann neighborhood
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
         * typeOfNeighborhood = MOORE_NEIGHBORHOOD (1) ->
         * Modified Moore neighborhood
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
         * changeCount is used to know if the next step is different from
         * the current step.
         * If changeCount == 0 then there're no changes and the automata
         * has finished,
         * so we can start again from step 0.
         * If changeCount != 0 then the automata has not finished yet.
         */ 
        int changeCount = 0; 
            
        int countFG = 0;

        ++numSteps;

        if (numSteps % 2 == 0 and numSteps != 1)
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
                if (typeOfNeighborhood == MOORE_NEIGHBORHOOD or
                    ((i % 2 == 0 and j % 2 == 0) or
                    (i % 2 != 0 and j % 2 != 0)))
                {    
                    // top 
                    if (fbRef[SCREEN_WIDTH * (j - 2) + i] == FG_color)
                    {
                        countFG++;
                    }
                }
                //  x
                // xxx
                if (typeOfNeighborhood ==  MOORE_NEIGHBORHOOD or
                    ((i % 2 != 0 and j % 2 == 0) or
                    (i % 2 == 0 and j % 2 != 0)))
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
                            
                if (typeOfNeighborhood == MOORE_NEIGHBORHOOD)            
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
                
                /*
                 * if the count of surrounding cells with FG_color is n we'll
                 * check if the index n-1 of the boolean rule is checked
                 * (value=1)
                 */
                if (countFG != 0
                    and checkBooleanRuleValue(typeOfNeighborhood,
                                              countFG - 1))
                {
                    /*
                     * If the current cell's color is not already changed,
                     * change it to FG_color.
                     * Without this condition each cell is painted more than
                     * one time and changeCount is never equal to 0.
                     */
                    if (fbNew[SCREEN_WIDTH * j + i] != FG_color) 
                    {
                        paintTriangularCell(i, j, FG_color, fbNew);
                        changeCount++;
                        // The population of alive cells increases
                        ++population;
                    }
                }
            }
        }

        /*
         * If no cell has changed the automata has finished so we are going
         * to reinitiate the cycle
         */
        if (changeCount == 0)
        {
            initialize();
        }
        else // the automata has not finished yet
        {
            if (numSteps % 2 == 0 and numSteps != 1)
            {
                showFB();
            }
            else
            {
                showFB2();
            }
            swiWaitForVBlank();
        }
    }
    else if(type == CONWAYS_GAME_OF_LIFE)
    {
    
        unsigned short* fbRef;
        unsigned short* fbNew;

        /*
         * changeCount is used to know if the next step is different from the
         * current step.
         * If changeCount == 0 then there're no changes and the automata has
         * finished,
         * so we can start again from step 0.
         * If changeCount != 0 then the automata has not finished yet.
         */ 
        int changeCount = 0; 

        int countFG = 0;
 
        ++numSteps;

        if (numSteps % 2 == 0 and numSteps != 1)
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
                
                /*
                 * Any live cell with fewer than two live neighbours dies,
                 * as if caused by under-population.
                 */
                if (countFG < 2 && fbRef[SCREEN_WIDTH * j + i] == FG_color) 
                {
                    fbNew[SCREEN_WIDTH * j + i] = BG_color;
                    changeCount++;
                    --population;
                }
                
                /*
                 * Any live cell with two or three live neighbours lives on to
                 * the next generation.
                 */
                if ((countFG == 2 || countFG == 3)
                    && fbRef[SCREEN_WIDTH * j + i] == FG_color) 
                {
                    fbNew[SCREEN_WIDTH * j + i] = FG_color;
                    changeCount++;
                    // the population doesn't change in this case
                }
                
                /*
                 * Any live cell with more than three live neighbours dies,
                 * as if by overcrowding.
                 */
                if (countFG > 3 && fbRef[SCREEN_WIDTH * j + i] == FG_color)
                {
                    fbNew[SCREEN_WIDTH * j + i] = BG_color;
                    changeCount++;
                    --population;
                }
                
                /*
                 * Any dead cell with exactly three live neighbours becomes
                 * a live cell, as if by reproduction.
                 */
                if (countFG == 3 && fbRef[SCREEN_WIDTH * j + i] == BG_color)
                {
                    fbNew[SCREEN_WIDTH * j + i] = FG_color;
                    changeCount++;
                    ++population;
                }            
            }
        }
        
        /*
         * If no cell has changed the automata has finished so we are going to
         * reinitiate the cycle.
         */
        if (changeCount == 0) 
        {
            initialize();
        }
        else // the automata has not finished yet
        {
            if (numSteps % 2 == 0 and numSteps != 1)
            {
                showFB();
            }
            else
            {
                showFB2();
            }
            swiWaitForVBlank();
        }
    }
    else if(type == BML_TRAFFIC_MODEL)
    {
    
        unsigned short* fbRef;
        unsigned short* fbNew;

        int cell_step = 0; // 1 or SCREEN_WIDTH (256)
        unsigned short currentColor = 0;
        
        ++numSteps;

        if (numSteps % 2 == 0 and numSteps != 1)
        // move vertically (top to bottom)
        {
            fbRef = fb2;
            fbNew = fb;
            cell_step = SCREEN_WIDTH;
            currentColor = FG_color;
        }
        else // move horizontally (left to right)
        {
            fbRef = fb;
            fbNew = fb2;
            cell_step = 1;
            currentColor = FG_color2;
        }

        dmaCopy(fbRef, fbNew, 128*1024);

        for (int col = 0; col < SCREEN_WIDTH; ++col)
        {
            for (int row = 0; row < SCREEN_HEIGHT; ++row)
            {
                // Vertical movement
                if (cell_step == SCREEN_WIDTH)
                {
                    // If the row is not the last one
                    if (row != SCREEN_HEIGHT - 1)
                    {
                        // if the current cell is FG_color and
                        // the next cell is BG_color,
                        // move down to the next cell
                        if (fbRef[SCREEN_WIDTH * row + col] == currentColor &&
                            fbRef[SCREEN_WIDTH * row + col + cell_step] ==
                                BG_color)
                        {
                            // Paint the current cell with BG_color
                            fbNew[SCREEN_WIDTH * row + col] = BG_color;

                            // Paint the next cell with FG_color
                            fbNew[SCREEN_WIDTH * row + col + cell_step] =
                                currentColor;
                        }
                    }
                    // else if the row is the last one
                    else if (row == SCREEN_HEIGHT - 1)
                    {
                        // If the first cell of the col is empty (BG_color)
                        // and the last cell of the column is FG_color2,
                        // move the current cell to the start of the column
                        if (fbRef[col] == BG_color &&
                            fbRef[SCREEN_WIDTH * row + col] == currentColor)
                        {
                            // Paint the current cell with BG_color
                            fbNew[SCREEN_WIDTH * row + col] = BG_color;

                            // goes back to the top
                            fbNew[col] = currentColor;
                        }

                    }
                }
                // Horizontal movement
                else if (cell_step == 1)
                {   
                    // If the col is not the last one
                    if (col != SCREEN_WIDTH - 1)
                    {
                        // if the current cell is FG_color2 and
                        // the next cell is BG_color,
                        // move the current cell to the left
                        if (fbRef[SCREEN_WIDTH * row + col] == currentColor &&
                            fbRef[SCREEN_WIDTH * row + col + cell_step] ==
                                 BG_color)
                        {
                            // Paint the current cell with BG_color
                            fbNew[SCREEN_WIDTH * row + col] = BG_color;

                            // Paint the next cell with FG_color
                            fbNew[SCREEN_WIDTH * row + col + cell_step] =
                                currentColor;
                        }
                    }
                    // else if the col is the last one
                    else if (col == SCREEN_WIDTH - 1)
                    {
                        // If the first cell of the row is empty (BG_color)
                        // and the last cell of the row is FG_color2,
                        // move the current cell to the start of the row
                        if (fbRef[SCREEN_WIDTH * row] == BG_color &&
                            fbRef[SCREEN_WIDTH * row + col] == currentColor)
                        {
                            // Paint the current cell with BG_color
                            fbNew[SCREEN_WIDTH * row + col] = BG_color;

                            // goes back to the left side
                            fbNew[SCREEN_WIDTH * row] = currentColor;
                        }

                    }
                }
            }
        }
 
        if (numSteps % 2 == 0 and numSteps != 1)
        {
            showFB();
        }
        else
        {
            showFB2();
        }
        swiWaitForVBlank();
    }
    else if (type == CYCLIC_CELLULAR_AUTOMATA)
	{
		unsigned short* fbRef;
		unsigned short* fbNew;

        unsigned short currentColorIndex;
        unsigned short currentLeftColorIndex;
        unsigned short currentRightColorIndex;

        ++numSteps;

  		if (numSteps % 2 == 0 and numSteps != 1)
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

        for (int column = 0; column < SCREEN_WIDTH; column++)
	    {
            // Determine the index of the color of the current cell
            for (int i = 0; i < numStates; i++) {
                if (cyclicAutomataColors[i] == 
                        fbRef[SCREEN_WIDTH * (numSteps - 1) + column]) 
                {
                    currentColorIndex = i;
                }                
            }
            
            // Determine the index of the color of the cell on the left side
            // of the current cell
            for (int i = 0; i < numStates; i++) {
                if (cyclicAutomataColors[i] == 
                        fbRef[SCREEN_WIDTH * (numSteps - 1) + column - 1])
                {
                    currentLeftColorIndex = i;
                } 
            }

            // Determine the index of the color of the cell on the right side
            // of the current cell
            for (int i = 0; i < numStates; i++) {
                if (cyclicAutomataColors[i] ==
                        fbRef[SCREEN_WIDTH * (numSteps - 1) + column + 1])
                {
                    currentRightColorIndex = i;
                } 
            }
            
            // If the index of the left neighbour is the
            // successor of the index of the current cell
            // the cell on the next row gets the successor color.
            if (currentColorIndex == (currentLeftColorIndex + 1) % numStates)
            {
                fbNew[SCREEN_WIDTH * numSteps + column] =
                    cyclicAutomataColors[currentLeftColorIndex];
            }
            // If the index of the right neighbour is the
            // successor of the index of the current cell
            // the cell on the next row gets the successor color.
            else if (currentColorIndex == (currentRightColorIndex + 1) % numStates)
            {
                fbNew[SCREEN_WIDTH * numSteps + column] = 
                    cyclicAutomataColors[currentRightColorIndex];
            }
            // Else the cell on the next row gets the same color
            // as the current cell.
            else
            {
                fbNew[SCREEN_WIDTH * numSteps + column] =
                    cyclicAutomataColors[currentColorIndex];
            }
	    }
		
       	if (numSteps % 2 == 0 and numSteps != 1)
        {
            showFB();
        }
        else
        {
            showFB2();
        }
        
        swiWaitForVBlank();
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

/*
 * Draws the current rule of the Elementary Cellular Automata
 */
int CellularAutomata::drawRule(int nth)
{

    const int intTopRow = 136;
    const int intBottomRow = 168;

    const int intColumn1 = 32;
    const int intColumn2 = 88;
    const int intColumn3 = 144;
    const int intColumn4 = 200;

	const int intLength = 9;
	const int intWidth = 9;
    
    unsigned int color = BG_color;
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
        color = FG_color;
	}
	
	drawRectangle(intRowStart, intColumnStart, intLength, intWidth, color);

	color = BG_color;

	if(ruleCenter[nth] == FG_color)
	{
		color = FG_color;
	}
		
	drawRectangle(intRowStart, intColumnStart + intWidth - 1,
                  intLength, intWidth, color);

    color = BG_color;

	if(ruleRight[nth] == FG_color)
	{
        color = FG_color;
	}

	drawRectangle(intRowStart, intColumnStart + (intWidth - 1) * 2,
                  intLength, intWidth, color);

    color = BG_color;

	if(ruleDown[nth] == FG_color)
	{
        color = FG_color;
	}

	drawRectangle(intRowStart + intLength - 1, intColumnStart + intWidth -1,
                  intLength, intWidth, color);
	
	return 0;
}

/*
 * Draws all the rules (0-7) of the Elementary Cellular Automata
 */
int CellularAutomata::drawAllRules()
{
	for (int i = 0; i < 8; i++)
	{
	    drawRule(i);
	}
	
	return 0;
}
