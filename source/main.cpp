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

// The flash image (grit outputs a nice header to reference data)
#include "flash.h"


// Definition of constants
#define VERSION "0.2.3"

#define MENU_AUTOMATA_TYPE_ROW_ECA 8
#define MENU_AUTOMATA_TYPE_ROW_MS 10

#define ECA_RULE_NUMBER_ROW 9

#define ELEMENTARY_CELLULAR_AUTOMATA 0

// Color used on the BackGround (White)
unsigned short BG_color = RGB15(31,31,31);
// Color used on the ForeGround (Black)
unsigned short FG_color = RGB15(0,0,0);
// Color used to draw the lines (Grey)
unsigned short line_color = RGB15(15,15,15);

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
unsigned short ruleDown[8] = {BG_color,FG_color,BG_color,FG_color,FG_color,BG_color,FG_color,BG_color}; //Rule 90

// Pointer to the start of VRAM_A, the memory that we'll use as framebuffer
unsigned short* fb = VRAM_A;

// A variable for the position of the touch
touchPosition touch;

/* This variable sets the type of automata that is going to execute
 * 0: Elementary Cellular Automata
 * 1: ????????
 * 2: Munching Squares
 */
int automataType = ELEMENTARY_CELLULAR_AUTOMATA;

/* This variable sets the menu that is going to be displayed
 * 0: Select automata menu
 * 1: Elementary cellular automata menu
 */
int displayedMenu = 0;

/* The position of the arrow
 * Elementary Cellular Automata: in the top screen (0-7) and in the bottom screen (8)
 * ????
 * Munching Squares: in the bottom screen (0)
 */
int intArrow = 0;

/*
 * This variable is used to count the number of steps of the Munching Squares
 */
int munchingSquaresNumSteps = 0;
int munchingSquaresThreshold = 0;
int munchingSquaresOptionComp = 0; // 0 smaller than; 1 equal to;
int munchingSquaresOptionOp = 0; // 0 XOR; 1 AND;
bool munchingSquaresCondition = false;

/*********************************** START GENERAL FUNCTIONS *************************************************************************/

/*
 * int drawHLine(int column, int row, int lenght, unsigned short color)
 * Draws a horizontal line of the specified color
 */
int drawHLine(int column, int row, int lenght, unsigned short color)
{
	for(int i = column; i < column + lenght; i++)
	{
		fb[row * 256 + i] = color;
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
 * Fills the screen with the background color
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

int showFB()
{
	//Pantaila nagusiaren konfigurazioa
	REG_DISPCNT = MODE_FB0;		//Framebufferra
	
	//VRAM A blokea konfiguratu
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD;

	cleanScreen();
	
	return 0;
}

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
	
	drawHLine(intColumn - 3, intRow - 3, 1, color);
	drawHLine(intColumn - 3, intRow - 2, 2, color);
	drawHLine(intColumn - 6, intRow - 1,  6, color);
	drawHLine(intColumn - 6, intRow, 7, color);
	drawHLine(intColumn - 6, intRow + 1, 6, color);
	drawHLine(intColumn - 3, intRow + 2, 2, color);
	drawHLine(intColumn - 3, intRow + 3, 1, color);

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
	
	drawHLine(intColumnStart, intRowStart, width, line_color);
	drawHLine(intColumnStart, intRowStart + length, width, line_color);
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
						
						if(BG_color == ruleLeft[i]  && fb[(row - 1)*256 + column] == ruleCenter[i] && fb[(row - 1)*256 + (column+1)] == ruleRight[i])
						{
							fb[row*256 + column] = ruleDown[i];								
						}
					}	
					else if(column == 255)
					{
						if(fb[(row - 1)*256 + (column-1)] == ruleLeft[i] && fb[(row - 1)*256 + column] == ruleCenter[i] && BG_color == ruleRight[i])
							fb[row*256 + column] = ruleDown[i];								
					}
				}
			}
			else if(row == 129)
			{
				fb[row*256 + column] = FG_color;
			}
		}
	}
	
	drawRule(0);
	drawRule(1);
	drawRule(2);
	drawRule(3);
	drawRule(4);
	drawRule(5);
	drawRule(6);
	drawRule(7);

	drawArrow(intArrow, line_color);
	
	return 0;
}
/*********************************** END ELEMENTARY CELLULAR AUTOMATA FUNCTIONS ********************************************/

/*********************************** START MUNCHING SQUARES FUNCTIONS ******************************************************/

int drawSquare(int column, int row, int width, unsigned short color)
{
    for (int k = 0; k < width; k++)
    {
        drawHLine(32 + width * column, width * row + k, width, color);
    }
    
    return 0;
}

int initializeMunchingSquares()
{
    munchingSquaresNumSteps = 0;
    munchingSquaresCondition = false;
    
    return 0;
}

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
                	//if ((i xor j) < (n + threshold))
                	//{
                    //	drawSquare(i, j, 3, RGB15(15,15,15));
                	//}
				}
				else if (munchingSquaresOptionComp == 1)
				{
					//if ((i xor j) == (n + threshold))
					//{
					//	drawSquare(i, j, 3, RGB15(15,15,15));
					//}
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
					//drawSquare(i, j, 3, RGB15(15,15,15));
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

int printSelectAutomataTypeMenu()
{
    // Print the menu to select the type of automata
    iprintf("\x1b[%d;2HElementary Cellular Automata", MENU_AUTOMATA_TYPE_ROW_ECA);
    iprintf("\x1b[9;2H?????");
    iprintf("\x1b[%d;2HMunching Squares", MENU_AUTOMATA_TYPE_ROW_MS);
    
    return 0;
}

int printECAmenu()
{
    iprintf("\x1b[11;2HBack to main menu");
    
    return 0;
}

int printMenuArrow(int row)
{
    iprintf("\x1b[%d;0H->", row); // Print the menu arrow
    
    return 0;
}

int printMSmenu()
{
    iprintf("\x1b[11;2HBack to main menu");
    printMenuArrow(11);
    return 0;
}

int printAutomataTypeArrow(int index)
{
    if (index == 0)
    {
        printMenuArrow(MENU_AUTOMATA_TYPE_ROW_ECA); // Print the arrow for option 0 (Elementary Cellular Automata)
    }
    else if (index == 1)
    {
        printMenuArrow(9); // Print the arrow for option 1
    }
    else if (index == 2)
    {    
        printMenuArrow(MENU_AUTOMATA_TYPE_ROW_MS); // Print the arrow for option 2 (Munching squares)
    }
    return 0;
}

int printECAarrow()
{
    printMenuArrow(11);
    return 0;
}

int deleteMenuArrow(int row)
{
    iprintf("\x1b[%d;0H  ", row); // Delete the menu arrow
    
    return 0;
}

int deleteAutomataTypeArrow(int index)
{
    if (index == 0)
    {
        deleteMenuArrow(MENU_AUTOMATA_TYPE_ROW_ECA); // Delete the arrow for option 0 (Elementary Cellular Automata)
    }
    else if (index == 1)
    {
        deleteMenuArrow(9); // Delete the arrow for option 1
    }
    else if (index == 2)
    {
        deleteMenuArrow(MENU_AUTOMATA_TYPE_ROW_MS);    // Delete the arrow for option 2 (Munching squares)
    }
    return 0;    
}

int deleteECAarrow()
{
    deleteMenuArrow(11);
    return 0;
}

int runAutomata()
{
    if (automataType == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        showFB();
  	    drawElementaryCellularAutomata();	        
    }
    else if (automataType == 1)
    {
        cleanScreen();
    }
    else if (automataType == 2)
    {
        showFB();
        initializeMunchingSquares();
    }
    
    return 0;
}

int showAutomataTypeMenu()
{
    displayedMenu = 0;
    automataType = ELEMENTARY_CELLULAR_AUTOMATA;
    showFlash();
    consoleClear();
    printCredits();
    printSelectAutomataTypeMenu();
    printAutomataTypeArrow(automataType);
    
    return 0;
}

/*********************************** START MAIN FUNCTION *************************************************************************/
int main(void)
{
	consoleDemoInit();

    printCredits();
    printSelectAutomataTypeMenu();
    printAutomataTypeArrow(automataType);
    
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
        
        if (displayedMenu == 0) // The main menu to select the type of automata
        {
            if (keys_released & KEY_UP)
            {
                if (automataType != ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    deleteAutomataTypeArrow(automataType);
                    automataType = automataType - 1;
                    printAutomataTypeArrow(automataType);
                }
                else
                {
                    deleteAutomataTypeArrow(automataType);
                    automataType = 2;
                    printAutomataTypeArrow(automataType);
                }   
            }
            else if (keys_released & KEY_DOWN)
            {
                if (automataType != 2)
                {
                    deleteAutomataTypeArrow(automataType);
                    automataType = automataType + 1;
                    printAutomataTypeArrow(automataType);
                }
                else
                {
                    deleteAutomataTypeArrow(automataType);
                    automataType = ELEMENTARY_CELLULAR_AUTOMATA;
                    printAutomataTypeArrow(automataType);
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
                    printECAmenu();
                    
                    intArrow = 0;                    
                    displayedMenu = 1;
                    
                    runAutomata();
                }
                else if (automataType == 1)
                {
                }
                else if (automataType == 2)
                {   
                    consoleClear();
                    printCredits();
                    printf("Current type:\n Munching Squares");
                    printMSmenu();
                    
                    intArrow = 0;                    
                    displayedMenu = 3;
                    
                    //showFB();
                    //drawMunchingSquares();
                    
                    runAutomata();
                }
            }
        }
        else if (displayedMenu == 1) // The Elementary Cellular Automata menu
        {
		    if(keys_released & KEY_A)
		    {
		        if (intArrow != 8)
		        {
        //			printf("A : %d \n", ruleDown[intArrow]);
			
			        if(ruleDown[intArrow] == FG_color)
			        {
				        ruleDown[intArrow] = BG_color;
        //				printf("%d \n", ruleDown[intArrow]);
			        }
			        else
			        {
				        ruleDown[intArrow] = FG_color;
        //				printf("%d \n", ruleDown[intArrow]);
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
			        deleteECAarrow();
			    }
			    
			    if (intArrow == 8)
			    {
			        intArrow = 4;
			    }
			    else if(intArrow < 4)
			    {
				    //intArrow = intArrow + 4;
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
			        printECAarrow();
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
			        deleteECAarrow();
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
				    //intArrow = intArrow - 4;
				    intArrow = 8;
			    }
			    
			    //drawArrow(intArrow, line_color);
			    if (intArrow != 8)
		        {
			        drawArrow(intArrow, line_color);
			    }
			    else
			    {
			        printECAarrow();
			    }
		    }
        }
        else if (displayedMenu == 2) // ??????????????
        {
        }
        else if (displayedMenu == 3) // Munching squares
        {
  		    if(keys_released & KEY_A)
  		    {
  		        showAutomataTypeMenu();
  		    }
            
            //drawMunchingSquares();
            
            drawNextStepMunchingSquares();
        }
    }
    
	return 0;
}
/*********************************** END MAIN FUNCTION *************************************************************************/
