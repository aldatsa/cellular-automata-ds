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

#include "flash.h"  // Include for the header of the flash image (grit outputs a nice header to reference data)

using namespace std;

/*
 * Constant for the version number of the application
 */
const string strVersionNumber = "0.2.3";

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
 * Variables for the colors of the background
 */
unsigned short BG_R = 31;
unsigned short BG_G = 31;
unsigned short BG_B = 31;
unsigned short BG_color = RGB15(BG_R, BG_G, BG_B);      // Color used on the BackGround (Default: White)

/*
 * Variables for the colors of the elements on the foreground
 */
unsigned short FG_R = 0;
unsigned short FG_G = 0;
unsigned short FG_B = 0; 
unsigned short FG_color = RGB15(FG_R, FG_G, FG_B);         // Color used on the ForeGround (Default: Black)

/*
 * Variables for the color of the lines (different grids, squares for the rules, ...)
 */
unsigned short line_R = 15;
unsigned short line_G = 15;
unsigned short line_B = 15; 
unsigned short line_color = RGB15(line_R, line_G, line_B);    // Color used to draw the lines (Default: Grey)

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
 * 5: Boolean Triangular Automata
 * 6: Conway's game of life
 * 7: Munching Squares
 * 8: Selecet colors
 * 9: Select language
 */
int automataType = ELEMENTARY_CELLULAR_AUTOMATA;

/* This variable sets the language used in the menus
 * "en": english
 * "es": espanol (spanish)
 * "eu": euskara (basque)
 */
string displayedLanguage = "eu";

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

/*
 * The strings of text used in the app in the default language (english).
 *
 */
string stringRuleNumber = "Rule number";
string stringElementaryCellularAutomata = "Elementary cellular automata"; // max 30 characters
string stringLangtonsAnt = "Langton's ant"; // max 30 characters
string stringLangtonsHexagonalAnt = "Langton's hexagonal ant";
string stringBooleanAutomata = "Boolean automata";
string stringBooleanHexagonalAutomata = "Boolean hexagonal automata";
string stringBooleanTriangularAutomata = "Boolean triangular automata";
string stringConwaysGameOfLife = "Conway's game of life";
string stringMunchingSquares = "Munching Squares";
string stringSelectColors = "Select colors";
string stringBackToMainMenu = "Back to main menu";
string stringAntsPixels = "Ant's size (pixels)";
string stringVonNeumannNeighborhood = "Von Neumann neighborhood";
string stringMooreNeighborhood = "Moore neighborhood";
string stringNeighborhood = "Neighborhood";
string stringTypeOfComparation = "Type of comparation";
string stringSmallerThan = "Smaller than";
string stringEqualTo = "Equal to";
string stringTypeOfBooleanOperator = "Type of boolean operator";
string stringThreshold = "Threshold";
string stringBackgroundColor = "Background color";
string stringForegroundColor = "Foreground color";
string stringLineColor = "Line color";
string stringRed = "Red";
string stringGreen = "Green";
string stringBlue = "Blue";
string stringSteps = "Steps";
string stringAutomataType = "Automata type";
string stringVersion = "version " + strVersionNumber;
string stringSelectLanguage = "Select language";
string stringEnglish = "English";
string stringEspanol = "Espanol";
string stringEuskara = "Euskara";

/*********************************** START GENERAL FUNCTIONS *************************************************************************/

/*
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
 * Fills the selected framebuffer with the background color
 */
int cleanFB(unsigned short* framebuffer)
{
	int row, column;

	for(row = 0; row < 192; row++)
	{
	    for(column = 0; column < 256; column++)
	    {
		    framebuffer[row*256 + column] = BG_color;
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
 * Changes the language of the strings of text to the selected language
 * languageCode is the two letters code for each language
 * en : english
 * es : spanish
 * eu : basque
 */
int changeTextLanguage(string languageCode)
{
    if (languageCode == "en") // English
    {
        stringRuleNumber = "Rule number";
        
        stringElementaryCellularAutomata = "Elementary cellular automata"; // max 30 characters
        stringLangtonsAnt = "Langton's ant"; // max 30 characters
        stringLangtonsHexagonalAnt = "Langton's hexagonal ant"; // max 30 characters
        stringBooleanAutomata = "Boolean automata"; // max 30 characters
        stringBooleanHexagonalAutomata = "Boolean hexagonal automata"; // max 30 characters
        stringBooleanTriangularAutomata = "Boolean triangular automata"; // max 30 characters
        stringConwaysGameOfLife = "Conway's game of life";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringSelectColors = "Select colors"; // max 30 characters
        stringBackToMainMenu = "Back to main menu"; // max 30 characters
        
        stringAntsPixels = "Ant's size (pixels)"; // Max 22 characters
        
        stringVonNeumannNeighborhood = "Von Neumann neighborhood";
        stringMooreNeighborhood = "Moore neighborhood";
        stringNeighborhood = "Neighborhood";
        
        stringTypeOfComparation = "Type of comparation";
        stringSmallerThan = "Smaller than";
        stringEqualTo = "Equal to";
        stringTypeOfBooleanOperator = "Type of boolean operator";
        stringThreshold = "Threshold";
        
        stringBackgroundColor = "Background color";
        stringForegroundColor = "Foreground color";
        stringLineColor = "Line color";
        stringRed = "Red";
        stringGreen = "Green";
        stringBlue = "Blue";
        
        stringSteps = "Steps";
        
        stringAutomataType = "Automata type";
        
        stringVersion = "version " + strVersionNumber;
        
        stringSelectLanguage = "Select language";
        stringEnglish = "English";
        stringEspanol = "Espanol";
        stringEuskara = "Euskara";
    }
    else if (languageCode == "es") // Spanish
    {
        stringRuleNumber = "Regla numero"; // It doesn't print 'ú'. Why?
        
        stringElementaryCellularAutomata = "Automata celular elemental"; // max 30 characters
        stringLangtonsAnt = "Hormiga de Langton"; // max 30 characters
        stringLangtonsHexagonalAnt = "Hormiga de Langton hexagonal"; // max 30 characters
        stringBooleanAutomata = "Automata booleano"; // max 30 characters
        stringBooleanHexagonalAutomata = "Automata booleano hexagonal"; // max 30 characters
        stringBooleanTriangularAutomata = "Automata booleano triangular"; // max 30 characters
        stringConwaysGameOfLife = "Juego de la vida de Conway";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringSelectColors = "Seleccionar colores"; // max 30 characters        
        stringBackToMainMenu = "Volver al menu principal"; // max 30 characters

        stringAntsPixels = "Tamano (pixeles)"; // It doesn't print 'ñ'. Why? And "Tamano de la hormiga (pixeles)" is too long. Max 22 characters
        
        stringVonNeumannNeighborhood = "Vecindad de Von Neumann";
        stringMooreNeighborhood = "Vecindad de Moore";
        stringNeighborhood = "Vecindad";
        
        stringTypeOfComparation = "Tipo de comparacion";
        stringSmallerThan = "Menor que";
        stringEqualTo = "Igual que";
        stringTypeOfBooleanOperator = "Tipo de operador booleano";
        stringThreshold = "Umbral";
        
        stringBackgroundColor = "Color de fondo";
        stringForegroundColor = "Color de primer plano";
        stringLineColor = "Color de lineas";
        stringRed = "Rojo";
        stringGreen = "Verde";
        stringBlue = "Azul";
        
        stringSteps = "Pasos";
        
        stringAutomataType = "Tipo de automata";
        
        stringVersion = "version " + strVersionNumber;
        
        stringSelectLanguage = "Seleccionar lenguaje";
        stringEnglish = "English";
        stringEspanol = "Espanol";
        stringEuskara = "Euskara";
    }
    else if (languageCode == "eu") // Basque
    {
        stringRuleNumber = "Arau zenbakia";
        
        stringElementaryCellularAutomata = "Oinarrizko automata zelularra"; // max 30 characters
        stringLangtonsAnt = "Langton-en inurria"; // max 30 characters
        stringLangtonsHexagonalAnt = "Langton-en inurri hexagonala"; // max 30 characters
        stringBooleanAutomata = "Automata boolearra"; // max 30 characters
        stringBooleanHexagonalAutomata = "Automata boolear hexagonala"; // max 30 characters
        stringBooleanTriangularAutomata = "Automata boolear triangularra"; // max 30 characters
        stringConwaysGameOfLife = "Conway-ren biziaren jokoa";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringSelectColors = "Hautatu koloreak"; // max 30 characters
        stringBackToMainMenu = "Itzuli menu nagusira"; // max 30 characters
        
        stringAntsPixels = "Tamaina (pixelak)"; // Max 23 characters
        
        stringVonNeumannNeighborhood = "Von Neumann-en auzoa";
        stringMooreNeighborhood = "Moore-en auzoa";
        stringNeighborhood = "Auzoa";
        
        stringTypeOfComparation = "Konparaketa mota";
        stringSmallerThan = "Txikiago";
        stringEqualTo = "Berdin";
        stringTypeOfBooleanOperator = "Boolear eragile mota";
        stringThreshold = "Ataria";
        
        stringBackgroundColor = "Atzeko planoaren kolorea";
        stringForegroundColor = "Aurreko planoaren kolorea";
        stringLineColor = "Lerroen kolorea";
        stringRed = "Gorria";
        stringGreen = "Berdea";
        stringBlue = "Urdina";
        
        stringSteps = "Pausoak";
        
        stringAutomataType = "Automata mota";
        
        stringVersion = strVersionNumber + " bertsioa";
        
        stringSelectLanguage = "Hautatu hizkuntza";
        stringEnglish = "English";
        stringEspanol = "Espanol";
        stringEuskara = "Euskara";
    }
    
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
			fb[row * 256 + column] = color;
		}
	} 
	
	return 0;
}

/*
 * Draws the current rule of the Elementary Cellular Automata
 */
int drawRule(int nth)
{
	bool fill = false;
	int intRowStart = 0;
	int intColumnStart = 0;

	int intLength = 9;
	int intWidth = 9;
	
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
int drawSquare(int column, int row, int width, unsigned short color)
{
    for (int k = 0; k < width; k++)
    {
        drawHLine(32 + width * column, width * row + k, width, color, fb);
    }
    
    return 0;
}

/*
 * Cleans the main framebuffer 
 * and initiliazes the values of the variables used for the munching squares.
 */
int initializeMunchingSquares()
{
    cleanFB(fb);
    munchingSquaresNumSteps = 0;    // Reset the number of steps to 0.
    munchingSquaresCondition = false;   // Reset the boolean value of the condition to false
    
    return 0;
}

/*
 * Draws the next step of the munching squares
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
int drawNextStepMunchingSquares()
{
    if (munchingSquaresNumSteps < 64)   
    {
        // For each combination of i and j where i = 0, 1, ... 63 and j = 0, 1, ... 63
        // we check if the value of ...
        //  * i xor j (if munchingSquaresOptionOp == 0)
        //  * i and j (if munchingSquaresOptionOp == 1)
        // is smaller than (if munchingSquaresOptionComp == 0) or equal to (if munchingSquaresOptionComp == 1)
        // the sum of munchingSquaresNumSteps + munchingSquaresThreshold
        // If the result of this comparation is true then we draw a square of 3 pixels x 3 pixels using FG color
        // using the function drawSquare starting from the column i and the row j.
        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 64; j++)
            {
				if (munchingSquaresOptionComp == 0) // Comparation type: smaller than
				{
					if (munchingSquaresOptionOp == 0) // Type of boolean operator: XOR
					{
						munchingSquaresCondition = ((i xor j) < (munchingSquaresNumSteps + munchingSquaresThreshold));
					}
					else if (munchingSquaresOptionOp == 1) // Type of boolean operator: AND
					{
						munchingSquaresCondition = ((i and j) < (munchingSquaresNumSteps + munchingSquaresThreshold));
					}
				}
				else if (munchingSquaresOptionComp == 1) // Comparation type: equal to
				{
                     if (munchingSquaresOptionOp == 0) // Type of boolean operator: XOR
                     {
                     	munchingSquaresCondition = ((i xor j) == (munchingSquaresNumSteps + munchingSquaresThreshold));
                     }
                     else if (munchingSquaresOptionOp == 1) // Type of boolean operator: AND
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
        cleanFB(fb); // Paint the framebuffer with the BG color to erase the last step of the munching squares
        initializeMunchingSquares(); // Initialize the variables to start another cycle
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
    cleanFB(fb);
    
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
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 *             x  x  x                 x  x  x   
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
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 *          x           x           x           x
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
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * x  x  x                 x  x  x
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

/************************************* END HEXAGONAL GRID FUNCTIONS ********************************************************/

/************************************* START TRIANGULAR GRID FUNCTIONS *****************************************************/

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
 */
int triangularGridLineOne(int y)
{
	for(int i = 0; i < 255; i++)
	{
    	fb[256 * y + i] = line_color;
	}
	return 0;
}

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 *          x     x           x     x
 */
int triangularGridLineTwo(int y)
{
	for(int i = 3; i < 255; i = i + 6)
	{
		fb[256 * y + i] = line_color;
        fb[256 * y + i + 2] = line_color;
	}
	return 0;
}

/*
 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * x     x           x     x           x     x
 */
int triangularGridLineThree(int y)
{
	for(int i = 0; i < 255; i = i + 6)
	{
		fb[256 * y + i] = line_color;
		fb[256 * y + i + 2] = line_color;
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
    cleanFB(fb);
    
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
        for (int j = 4; j < 187; j = j + 3)
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
    cleanFB(fb);
    cleanFB(fb2);
    
    automataSteps = 0;
        
    fb[intY * 256 + intX] = FG_color;
    
    return 0;
} 

int initializeBooleanHexagonalAutomata(int intX, int intY)
{
    cleanFB(fb);
    cleanFB(fb2);
    
    drawHexGrid();
    
    automataSteps = 0;
        
    paintHexCell(intX, intY, FG_color, fb);
    
    return 0;
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
        for (int j = 1; j < 191; ++j)
        {
            countFG = 0;        

            // top 
            if (fbRef[256 * (j - 1) + i] == FG_color)
            {
                countFG++;
            }

      	    //bottom
            if (fbRef[256 * (j + 1) + i] == FG_color)
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
                        
            // top left
            if (fbRef[256 * (j - 1) + i - 1] == FG_color)
            {
               countFG++;
            }
                
            // top right
            if (fbRef[256 * (j - 1) + i + 1] == FG_color)
            {
               countFG++;
            }
                
            // bottom left    
            if (fbRef[256 * (j + 1) + i - 1] == FG_color)
            {
                countFG++;
            }

            // bottom right                         
            if (fbRef[256 * (j + 1) + i + 1] == FG_color)
            {
                countFG++;
            }
            
            if (countFG < 2 && fbRef[256 * j + i] == FG_color) // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
            {
                fbNew[256 * j + i] = BG_color;
                changeCount++;
            }
            if ((countFG == 2 || countFG == 3) && fbRef[256 * j + i] == FG_color) // Any live cell with two or three live neighbours lives on to the next generation.
            {
                fbNew[256 * j + i] = FG_color;
                changeCount++;
            }
            if (countFG > 3 && fbRef[256 * j + i] == FG_color) // Any live cell with more than three live neighbours dies, as if by overcrowding.
            {
                fbNew[256 * j + i] = BG_color;
                changeCount++;
            }
            if (countFG == 3 && fbRef[256 * j + i] == BG_color) // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            {
                fbNew[256 * j + i] = FG_color;
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
        iprintf("\x1b[11;2H%s: < %d >", stringAntsPixels.c_str(), antNumPixels);
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
        iprintf("\x1b[11;2H%s:", stringTypeOfComparation.c_str());        
        iprintf("\x1b[12;5H%s", stringSmallerThan.c_str());
        iprintf("\x1b[13;5H%s", stringEqualTo.c_str());    
        
        iprintf("\x1b[14;2H%s:", stringTypeOfBooleanOperator.c_str());
        iprintf("\x1b[15;5HXOR");
        iprintf("\x1b[16;5HAND");    
        
        iprintf("\x1b[17;2H%s: < %d >", stringThreshold.c_str(), munchingSquaresThreshold);
        
        iprintf("\x1b[19;2H%s", stringBackToMainMenu.c_str());
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
            row = 13;
        }
    }
    else if (intDisplayedMenu == 8) // Munching squares menu
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
        else if (index == 4) // Threshold
        {
            row = 17;
        }
        else if (index == 5) // Back to main menu
        {    
            row = 19;
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
int printNumSteps(int automataType)
{
    if (automataType == LANGTON_ANT)
    {
        iprintf("\x1b[9;0H%s:     ", stringSteps.c_str());                                                    
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), antNumSteps);
    }
    else if (automataType == LANGTON_HEXAGONAL_ANT)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), antNumSteps);
    }
    else if (automataType == BOOLEAN_AUTOMATA)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), automataSteps);
    }
    else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), automataSteps);    
    }
    else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), automataSteps);
    }
    else if (automataType == CONWAYS_GAME_OF_LIFE)
    {
        iprintf("\x1b[9;0H%s: %d", stringSteps.c_str(), automataSteps);
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
    else if (automataType == CONWAYS_GAME_OF_LIFE)
    {
        showFB();
        dmaCopy(fb, fb2, 128*1024);
        showFB2();
        
        initializeConwaysGameOfLife();
    }
    else if (automataType == MUNCHING_SQUARES)
    {
        showFB();
        initializeMunchingSquares();
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
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(LANGTON_ANT);
                              
                    intArrow = 0;
                    displayedMenu = 2;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                       
                    runAutomata();
                }
                else if (automataType == LANGTON_HEXAGONAL_ANT)
                {
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(LANGTON_HEXAGONAL_ANT);
                    
                    intArrow = 0;
                    displayedMenu = 3;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
                    runAutomata();
                }
                else if (automataType == BOOLEAN_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(BOOLEAN_AUTOMATA);
                    
                    intArrow = 0;
                    displayedMenu = 4;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);                    
                    
                    printBAasterisks();
                    runAutomata();
                }
                else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
                {
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(BOOLEAN_HEXAGONAL_AUTOMATA);
                    
                    intArrow = 0;
                    displayedMenu = 5;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
                    printBHAasterisks();                    
                    runAutomata();
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
                    consoleClear();
                    printCredits();
                    
                    printAutomataType(MUNCHING_SQUARES);
                    
                    intArrow = 0;                    
                    displayedMenu = 8;
                    
                    printMenu(displayedMenu);
                    
                    printMenuArrow(displayedMenu, intArrow, false);
                                        
  		            printMSasterisks();                    
                    runAutomata();
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
		    if (antPosX < 0 or antPosX + antNumPixels > 254 or antPosY < 0 or antPosY + antNumPixels > 191)
    	    {
    	        antStop = true;
    	    }
    	    
    	    if (antStop == false)
    	    {
    	        stepAnt();
    	        antNumSteps++;
                printNumSteps(LANGTON_ANT);
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
                        iprintf("\x1b[11;2H%s: < %d >", stringAntsPixels.c_str(), antNumPixels);                        
                        runAutomata();
                        printNumSteps(LANGTON_ANT);
                    }                        
                }		        
		    }
		    else if (keys_pressed & KEY_RIGHT)
		    {
                if (intArrow == 0)
                {
                    antNumPixels = antNumPixels + 1;
                    iprintf("\x1b[11;2H%s: < %d >", stringAntsPixels.c_str(), antNumPixels);
                    runAutomata();                
                    printNumSteps(LANGTON_ANT);                    
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
                printNumSteps(LANGTON_HEXAGONAL_ANT);
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
            printNumSteps(BOOLEAN_AUTOMATA);

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
            printNumSteps(BOOLEAN_HEXAGONAL_AUTOMATA);

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
            printNumSteps(BOOLEAN_TRIANGULAR_AUTOMATA);
            
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
            printNumSteps(CONWAYS_GAME_OF_LIFE);
            
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
  		        else if (intArrow == 5) // Back to main menu
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        printMenuArrow(displayedMenu, intArrow, true); // Delete the previous arrow
		        
		        if (intArrow == 0)
		        {
		            intArrow = 5;
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
		        
		        if (intArrow == 5)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        printMenuArrow(displayedMenu, intArrow, false); // Print the new arrow
		    }
		    else if (intArrow == 4 && keys_pressed & KEY_LEFT)
		    {
		        if (munchingSquaresThreshold > 0)
		        {
		            --munchingSquaresThreshold;
                    iprintf("\x1b[17;2H%s: < %d >", stringThreshold.c_str(), munchingSquaresThreshold);
                    runAutomata();
		        }
		    }
		    else if (intArrow == 4 && keys_pressed & KEY_RIGHT)
            {
                ++munchingSquaresThreshold;
                iprintf("\x1b[17;2H%s: < %d >", stringThreshold.c_str(), munchingSquaresThreshold);
                runAutomata();
            }
            
            drawNextStepMunchingSquares();
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
