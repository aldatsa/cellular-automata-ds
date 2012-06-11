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
#include <stdlib.h>
#include <string>

#include "framebuffer.h"
#include "color.h"
#include "i18n.h"
#include "cellularautomata.h"
#include "globals.h"

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
 * A variable for the position of the touch
 */
touchPosition touch;

CellularAutomata ca;

/******************************************* START MENU FUNCTIONS **********************************************************/

/*
 * Prints the text in the specified position using iprintf
 */
int printString(int row, int column, std::string text)
{
    iprintf("\x1b[%d;%dH%s", row, column, text.c_str());

    return 0;
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
 * Prints the current population of "alive" cells
 */
int printPopulation()
{
    iprintf("\x1b[%d;%dH%s%d    ", POPULATION_ROW, POPULATION_COLUMN, stringPopulation.c_str(), ca.getPopulation());

    return 0;
}

/*
 * Prints the number of pixels of the Langton's ant
 */
int printAntNumPixels()
{
    iprintf("\x1b[%d;2H%s: < %d > ", LA_MENU_ANT_SIZE_ROW, stringAntsPixels.c_str(), ca.getAntNumPixels());
    
    return 0;
}

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
 * Prints an asterisk ("*") used to highlight the currently selected menu option 
 */
int printAsterisk(int row, int column)
{
    iprintf("\x1b[%d;%dH*", row, column);
    
    return 0;
}

/*
 * Deletes an asterisk (" ") used to highlight the currently selected menu option 
 */
int deleteAsterisk(int row, int column)
{
    iprintf("\x1b[%d;%dH ", row, column);
    
    return 0;
}

/*
 * Prints the menu items but without the arrow.
 * Use printMenuArrow() to print the arrow
 */
int printMenu(int intDisplayedMenu)
{
    if (displayedMenu == MAIN_MENU) //The menu to select the type of automata
    {
        printString(MENU_AUTOMATA_TYPE_ROW_ECA, 2, stringElementaryCellularAutomata);
        printString(MENU_AUTOMATA_TYPE_ROW_LA, 2, stringLangtonsAnt);
        printString(MENU_AUTOMATA_TYPE_ROW_LHA, 2, stringLangtonsHexagonalAnt);
        printString(MENU_AUTOMATA_TYPE_ROW_BA, 2, stringBooleanAutomata);
        printString(MENU_AUTOMATA_TYPE_ROW_BHA, 2, stringBooleanHexagonalAutomata);
        printString(MENU_AUTOMATA_TYPE_ROW_BTA, 2, stringBooleanTriangularAutomata);
        printString(MENU_AUTOMATA_TYPE_ROW_CGL, 2, stringConwaysGameOfLife);
        printString(MENU_AUTOMATA_TYPE_ROW_MS, 2, stringMunchingSquares);
        printString(MENU_SELECT_COLORS, 2, stringSelectColors);
        printString(MENU_SELECT_LANGUAGE, 2, stringSelectLanguage);
    }
    else if (displayedMenu == ELEMENTARY_CELLULAR_AUTOMATA) // The menu of the Elementary Cellular Automaton
    {
        printPopulation();
        printString(ECA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == LANGTON_ANT) // The menu of the Langton's ant
    {
        printPopulation();
        printAntNumPixels();
        printString(LA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == LANGTON_HEXAGONAL_ANT) // The menu of the Langton's hexagonal ant
    {
        printPopulation();
        printString(LHA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == BOOLEAN_AUTOMATA) // The menu of the boolean automaton
    {
        printPopulation();
        printString(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 3, stringVonNeumannNeighborhood);
        printString(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 6, "1    2    3    4");
        printString(BA_MENU_MOORE_NEIGHBORHOOD_ROW, 3, stringMooreNeighborhood);
        printString(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 6, "1    2    3    4");
        printString(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 6, "5    6    7    8");        
        printString(BA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == BOOLEAN_HEXAGONAL_AUTOMATA) // The menu of the Boolean Hexagonal Automaton
    {
        printPopulation();
        printString(BHA_MENU_NEIGHBORHOOD_ROW, 2, stringNeighborhood);
        printString(BHA_MENU_NEIGHBORHOOD_123_ROW, 6, "1    2    3");
        printString(BHA_MENU_NEIGHBORHOOD_456_ROW, 6, "4    5    6");
        printString(BHA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == BOOLEAN_TRIANGULAR_AUTOMATA) // The menu of the Boolean Triangular Automaton
    {
        printPopulation();
        printString(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 3, stringVonNeumannNeighborhood);
        printString(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 6, "1    2    3");
        printString(BTA_MENU_MOORE_NEIGHBORHOOD_ROW, 3, stringMooreNeighborhood);
        printString(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 6, "1    2    3    4");
        printString(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 6, "5    6    7    8");        
        printString(BTA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == CONWAYS_GAME_OF_LIFE) // The menu of the Conway's game of life
    {
        printPopulation();
        printString(CGL_MENU_INITIAL_STATE_ROW, 2, stringInitialState);
        printString(CGL_MENU_ACORN_ROW, 3, stringAcorn);
        printString(CGL_MENU_FPENTOMINO_ROW, 3, stringFPentomino);
        printString(CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW, 3, stringFillScreenWithPulsars);
        printString(CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW, 3, stringFillScreenWithPentadecathlons);
        printString(CGL_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == MUNCHING_SQUARES) // The menu of the munching squares
    {
        printString(13, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == SELECT_COLORS) // The menu of color selection
    {
        printString(8, 2, stringBackgroundColor);
        iprintf("\x1b[9;5H%s < %i > ", stringRed.c_str(), BG_R);
        iprintf("\x1b[10;5H%s < %i > ", stringGreen.c_str(), BG_G);
        iprintf("\x1b[11;5H%s < %i > ", stringBlue.c_str(), BG_B);
        
        printString(12, 2, stringForegroundColor);
        iprintf("\x1b[13;5H%s < %i > ", stringRed.c_str(), FG_R);
        iprintf("\x1b[14;5H%s < %i > ", stringGreen.c_str(), FG_G);
        iprintf("\x1b[15;5H%s < %i > ", stringBlue.c_str(), FG_B);
                
        printString(16, 2, stringLineColor);
        iprintf("\x1b[17;5H%s < %i > ", stringRed.c_str(), line_R);
        iprintf("\x1b[18;5H%s < %i > ", stringGreen.c_str(), line_G);
        iprintf("\x1b[19;5H%s < %i > ", stringBlue.c_str(), line_B);
                
        printString(21, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == SELECT_LANGUAGE) // The menu of language selection
    {
        printString(10, 3, stringEnglish);
        printString(11, 3, stringEspanol);
        printString(12, 3, stringEuskara);
        
        printString(14, 2, stringBackToMainMenu);
    }
    
    return 0;
}

/*
 * Prints the asterisks used to mark the option in the menus
 */
int printMenuAsterisks(int intDisplayedMenu)
{
    
    if (intDisplayedMenu == BOOLEAN_AUTOMATA)
    {
        if (ca.getTypeOfNeighborhood() == 0)
        {
            printAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 2);
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_ROW, 2);
        }
        else
        {
            deleteAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 2);
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_ROW, 2);
        }
        
        // {1, 2, 3, 4} For the boolean square automata with Von Neumann neighborhood
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 0))
        {
            printAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 5);
        }
        else
        {
            deleteAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 5);
        }

        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 1))
        {
            printAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 10);
        }
        else
        {
            deleteAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 10);
        }

        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 2))
        {
            printAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 15);
        }
        else
        {
            deleteAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 15);
        }
                
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 3)) 
        {
            printAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 20);
        }
        else
        {
            deleteAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 20);
        }
                
        // {1, 2, 3, 4, 5, 6, 7, 8} For the boolean square automata with Moore neighborhood
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 0))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 5);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 5);
        }
        
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 1))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 10);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 10);
        }        
        
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 2))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 15);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 15);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 3))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 20);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 20);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 4))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 5);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 5);
        }

        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 5))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 10);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 10);
        }

        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 6))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 15);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 15);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 7))
        {
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 20);
        }
        else
        {
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 20);
        }
    }
    else if (displayedMenu == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        // {1, 2, 3, 4, 5, 6} For the boolean hexagonal automata with Moore neighborhood
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 0))
        {
            printAsterisk(BHA_MENU_NEIGHBORHOOD_123_ROW, 5);
        }
        else
        {
            deleteAsterisk(BHA_MENU_NEIGHBORHOOD_123_ROW, 5);
        }
        
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 1))
        {
            printAsterisk(BHA_MENU_NEIGHBORHOOD_123_ROW, 10);
        }
        else
        {
            deleteAsterisk(BHA_MENU_NEIGHBORHOOD_123_ROW, 10);
        }        
        
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 2))
        {
            printAsterisk(BHA_MENU_NEIGHBORHOOD_123_ROW, 15);
        }
        else
        {
            deleteAsterisk(BHA_MENU_NEIGHBORHOOD_123_ROW, 15);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 3))
        {
            printAsterisk(BHA_MENU_NEIGHBORHOOD_456_ROW, 5);
        }
        else
        {
            deleteAsterisk(BHA_MENU_NEIGHBORHOOD_456_ROW, 5);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 4))
        {
            printAsterisk(BHA_MENU_NEIGHBORHOOD_456_ROW, 10);
        }
        else
        {
            deleteAsterisk(BHA_MENU_NEIGHBORHOOD_456_ROW, 10);
        }

        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 5))
        {
            printAsterisk(BHA_MENU_NEIGHBORHOOD_456_ROW, 15);
        }
        else
        {
            deleteAsterisk(BHA_MENU_NEIGHBORHOOD_456_ROW, 15);
        }
    }
    else if (displayedMenu == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        if (ca.getTypeOfNeighborhood() == 0)
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 2);
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_ROW, 2);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 2);
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_ROW, 2);        
        }
        
        // {1, 2, 3} For the boolean triangular automata with Von Neumann neighborhood
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 0))
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 5);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 5);
        }
        
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 1))
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 10);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 10);
        }
                
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 2))
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 15);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 15);
        }

        // {1, 2, 3, 4, 5, 6, 7, 8} For the boolean triangular automata with Moore neighborhood
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 0))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 5);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 5);
        }
        
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 1))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 10);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 10);
        }        
        
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 2))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 15);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 15);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 3))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 20);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 20);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 4))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 5);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 5);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 5))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 10);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 10);
        }
                
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 6))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 15);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 15);
        }
            
        if (ca.checkBooleanRuleValue(MOORE_NEIGHBORHOOD, 7))
        {
            printAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 20);
        }
        else
        {
            deleteAsterisk(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 20);
        }
    }
    else if (displayedMenu == CONWAYS_GAME_OF_LIFE)
    {
        if (ca.getInitialState() == ACORN)
        {
            printAsterisk(CGL_MENU_ACORN_ROW, 2);
            deleteAsterisk(CGL_MENU_FPENTOMINO_ROW, 2);
            deleteAsterisk(CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW, 2);
            deleteAsterisk(CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW, 2);
        }
        else if (ca.getInitialState() == FPENTOMINO)
        {
            deleteAsterisk(CGL_MENU_ACORN_ROW, 2);
            printAsterisk(CGL_MENU_FPENTOMINO_ROW, 2);
            deleteAsterisk(CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW, 2);
            deleteAsterisk(CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW, 2);
        }
        else if (ca.getInitialState() == FILL_SCREEN_WITH_PULSARS)
        {
            deleteAsterisk(CGL_MENU_ACORN_ROW, 2);
            deleteAsterisk(CGL_MENU_FPENTOMINO_ROW, 2);
            printAsterisk(CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW, 2);
            deleteAsterisk(CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW, 2);
        }
        else if (ca.getInitialState() == FILL_SCREEN_WITH_PENTADECATHLONS)
        {
            deleteAsterisk(CGL_MENU_ACORN_ROW, 2);
            deleteAsterisk(CGL_MENU_FPENTOMINO_ROW, 2);
            deleteAsterisk(CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW, 2);
            printAsterisk(CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW, 2);
        }
    }
    else if (displayedMenu == SELECT_LANGUAGE)
    {
        if (displayedLanguage == EN)
        {
            printAsterisk(10, 2);
            deleteAsterisk(11, 2);
            deleteAsterisk(12, 2);
        }
        else if (displayedLanguage == ES)
        {
            deleteAsterisk(10, 2);
            printAsterisk(11, 2);
            deleteAsterisk(12, 2);
        }
        else if (displayedLanguage == EU)
        {
            deleteAsterisk(10, 2);
            deleteAsterisk(11, 2);
            printAsterisk(12, 2);
        }
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
    
    if (intDisplayedMenu == MAIN_MENU) // Main menu
    {
        if (index == ELEMENTARY_CELLULAR_AUTOMATA) // Elementary Cellular Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_ECA;
        }
        else if (index == LANGTON_ANT) // Langton's ant
        {
            row = MENU_AUTOMATA_TYPE_ROW_LA;
        }
        else if (index == LANGTON_HEXAGONAL_ANT) // Langton's hexagonal ant
        {
            row = MENU_AUTOMATA_TYPE_ROW_LHA;
        }        
        else if (index == BOOLEAN_AUTOMATA) // Boolean automata
        {    
            row = MENU_AUTOMATA_TYPE_ROW_BA;
        }
        else if (index == BOOLEAN_HEXAGONAL_AUTOMATA) // Boolean Hexagonal Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_BHA;
        }
        else if (index == BOOLEAN_TRIANGULAR_AUTOMATA) // Boolean Triangular Automata
        {
            row = MENU_AUTOMATA_TYPE_ROW_BTA;
        }
        else if (index == CONWAYS_GAME_OF_LIFE) // Conway's game of life
        {
            row = MENU_AUTOMATA_TYPE_ROW_CGL;
        }        
        else if (index == MUNCHING_SQUARES) // Munching squares
        {    
            row = MENU_AUTOMATA_TYPE_ROW_MS;
        }        
        else if (index == SELECT_COLORS) // Select colors
        {
            row = MENU_SELECT_COLORS;
        }
        else if (index == SELECT_LANGUAGE) // Select language
        {
            row = MENU_SELECT_LANGUAGE;
        }
    }
    
    else if (intDisplayedMenu == ELEMENTARY_CELLULAR_AUTOMATA) // Elementary Cellular Automaton menu
    {
        if (index == 8)
        {
            row = ECA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == LANGTON_ANT) // Langton's ant menu
    {
        if (index == 0) // Print the arrow in the option to choose the size of the ant
        {
            row = LA_MENU_ANT_SIZE_ROW;
        }
        else if (index == 1) // Print the arrow in the option to go back to main menu
        {
            row = LA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == LANGTON_HEXAGONAL_ANT) // Langton's hexagonal ant menu
    {
        if (index == 0)
        {
            row = LHA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == BOOLEAN_AUTOMATA) // Boolean automata
    {
        if (index == 0) // Von Neumann neighborhood
        {
            row = BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW;
        }
        else if (index == 1) // 1
        {
            row = BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 3;
        }
        else if (index == 2) // 2
        {
            row = BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 8;
        }
        else if (index == 3) // 3
        {
            row = BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 13;
        }
        else if (index == 4) // 4
        {
            row = BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 18;
        }
        else if (index == 5) // Moore neighborhood
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_ROW;
        }
        else if (index == 6) // 1
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 3;
        }
        else if (index == 7) // 2
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 8;
        }
        else if (index == 8) // 3
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 13;
        }
        else if (index == 9) // 4
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 18;
        }
        else if (index == 10) // 5
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 3;
        }
        else if (index == 11) // 6
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 8;
        }
        else if (index == 12) // 7
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 13;
        }
        else if (index == 13) // 8
        {
            row = BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 18;
        }
        else if (index == 14) // Back to main menu
        {
            row = BA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == BOOLEAN_HEXAGONAL_AUTOMATA) // Boolean Hexagonal Automata
    {
        if (index == 0)
        {
            row = BHA_MENU_NEIGHBORHOOD_123_ROW;
            column = 3;
        }
        else if (index == 1)
        {
            row = BHA_MENU_NEIGHBORHOOD_123_ROW;
            column = 8;
        }
        else if (index == 2)
        {
            row = BHA_MENU_NEIGHBORHOOD_123_ROW;
            column = 13;
        }
        else if (index == 3)
        {
            row = BHA_MENU_NEIGHBORHOOD_456_ROW;
            column = 3;
        }
        else if (index == 4)
        {
            row = BHA_MENU_NEIGHBORHOOD_456_ROW;
            column = 8;
        }                  
        else if (index == 5)
        {
            row = BHA_MENU_NEIGHBORHOOD_456_ROW;
            column = 13;
        }              
        else if (index == 6)
        {
            row = BHA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == BOOLEAN_TRIANGULAR_AUTOMATA) // Boolean Triangular Automata
    {
        if (index == 0) // Von Neumann neighborhood
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW;
        }
        else if (index == 1) // 1
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 3;
        }
        else if (index == 2) // 2
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 8;
        }
        else if (index == 3) // 3
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW;
            column = 13;
        }
        else if (index == 4) // Moore neighborhood
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_ROW;
        }
        else if (index == 5) // 1
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 3;
        }
        else if (index == 6) // 2
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 8;
        }
        else if (index == 7) // 3
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 13;
        }
        else if (index == 8) // 4
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW;
            column = 18;
        }
        else if (index == 9) // 5
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 3;
        }
        else if (index == 10) // 6
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 8;
        }
        else if (index == 11) // 7
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 13;
        }
        else if (index == 12) // 8
        {
            row = BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW;
            column = 18;
        }
        else if (index == 13) // Back to main menu
        {
            row = BTA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == CONWAYS_GAME_OF_LIFE) // Conway's game of life menu
    {
        if (index == 0)
        {
            row = CGL_MENU_ACORN_ROW; // Accorn
        }
        else if (index == 1)
        {
            row = CGL_MENU_FPENTOMINO_ROW; // F-Pentomino
        }
        else if (index == 2)
        {
            row = CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW; // Fill screen with pulsars
        }
        else if (index == 3)
        {
            row = CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW; // Fill screen with pentadecathlons
        }
        else if (index == 4)
        {
            row = CGL_MENU_BACK_TO_MAIN_ROW; // Back to main menu
        }
    }
    else if (intDisplayedMenu == MUNCHING_SQUARES) // Munching squares menu
    {
        if (index == 0)
        {
            row = 13; // Back to main menu
        }
    }
    else if (intDisplayedMenu == SELECT_COLORS) // Select colors
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
    else if (intDisplayedMenu == SELECT_LANGUAGE) // Select language    
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
    displayedMenu = MAIN_MENU;
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
    if (automataType == SELECT_COLORS)
    {
        printf("%s:", stringSelectColors.c_str());
    }
    else if (automataType == SELECT_LANGUAGE)
    {
        printf("%s:", stringSelectLanguage.c_str());
    }
    else
    {
        printf("%s:\n", stringAutomataType.c_str());
    } 
    
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
int printNumSteps()
{
    iprintf("\x1b[%d;0H%s:     ", NUM_STEPS_ROW, stringSteps.c_str());                                                    
    iprintf("\x1b[%d;0H%s: %d", NUM_STEPS_ROW, stringSteps.c_str(), ca.getNumSteps());

    return 0;
}

/*
 *
 */
int setLanguage(int language)
{
    displayedLanguage = language;
    
    changeTextLanguage(language);

    consoleClear();
    printCredits();
                    
    printf("%s:", stringSelectLanguage.c_str());
  		            
    printMenu(displayedMenu);
                    
    printMenuArrow(displayedMenu, intArrow, false);
    printMenuAsterisks(SELECT_LANGUAGE);
    
    return 0;
}

/************************************************** END MENU FUNCTIONS ***********************************************************/

/*
 *
 */
int runAutomata()
{
    if(automataType == SELECT_COLORS)
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
        if (displayedMenu == MAIN_MENU)
        {
            if (keys_released & KEY_UP)
            {
                printMenuArrow(displayedMenu, automataType, true); // Delete previous arrow

                if (automataType != ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    automataType = automataType - 1;
                }
                else
                {
                    automataType = SELECT_LANGUAGE;
                }

                printMenuArrow(displayedMenu, automataType, false); // Print new arrow   
            }
            else if (keys_released & KEY_DOWN)
            {
                printMenuArrow(displayedMenu, automataType, true); // Delete previous arrow
                
                if (automataType != SELECT_LANGUAGE)
                {
                    automataType = automataType + 1;
                }
                else
                {
                    automataType = ELEMENTARY_CELLULAR_AUTOMATA;
                }
                
                printMenuArrow(displayedMenu, automataType, false); // Print new arrow
            }
            else if (keys_released & KEY_A)
            {
                ca.setType(automataType);
                
                consoleClear();

                printCredits();
                
                printAutomataType(automataType);

                intArrow = 0;     
                
                displayedMenu = automataType;
                
                if (automataType == LANGTON_ANT)
                {
                    ca.setAntNumPixels(4);
                }
                else if (automataType == BOOLEAN_AUTOMATA)
                {
                    ca.setTypeOfNeighborhood(VON_NEUMANN_NEIGHBORHOOD); // Von Neumann neighborhood (default)

                    ca.initializeBooleanRuleValues();
                }
                else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
                {   
                    ca.initializeBooleanRuleValues();
                }
                else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
                {
                    ca.setTypeOfNeighborhood(VON_NEUMANN_NEIGHBORHOOD); // Von Neumann neighborhood (default)

                    ca.initializeBooleanRuleValues();
                }
                else if (automataType == CONWAYS_GAME_OF_LIFE)
                {
                    ca.setInitialState(ACORN);
                }
                
                if (automataType != ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    printMenuArrow(displayedMenu, intArrow, false);
                }
                
                printMenu(displayedMenu);
                
                if ((automataType != SELECT_COLORS) && (automataType != SELECT_LANGUAGE))
                {
                    ca.initialize();
                }
                else
                {
                    runAutomata();
                }

                if (automataType == ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    printRuleNumber(ca.getRuleNumber()); // The rule number must be printed after the initialization of the automata
                    ca.drawArrow(intArrow, line_color);  // Draw the arrow in the default position (intArrow = 0, top left rule (2^7), set in ca.initialize())
                }                
                
                // Include the other types of automata when the population count for them is implemented
                if (automataType == ELEMENTARY_CELLULAR_AUTOMATA || automataType == LANGTON_ANT || automataType == LANGTON_HEXAGONAL_ANT || automataType == BOOLEAN_AUTOMATA || automataType == BOOLEAN_HEXAGONAL_AUTOMATA ||  automataType == BOOLEAN_TRIANGULAR_AUTOMATA || automataType == CONWAYS_GAME_OF_LIFE)
                {
                    printPopulation();
                }

                if ((automataType == BOOLEAN_AUTOMATA) || (automataType == BOOLEAN_HEXAGONAL_AUTOMATA) || (automataType == BOOLEAN_TRIANGULAR_AUTOMATA) || (automataType == CONWAYS_GAME_OF_LIFE) ||  (automataType == SELECT_LANGUAGE))
                {
                    printMenuAsterisks(automataType);
                }
            }
        }
        /*
         * The Elementary Cellular Automata menu
         */                
        else if (displayedMenu == ELEMENTARY_CELLULAR_AUTOMATA)
        {
            ca.nextStep();
            printPopulation();

		    if(keys_released & KEY_A)
		    {
		        if (intArrow < 8)
		        {
			        if(ca.getRuleDown(intArrow) == FG_color)
			        {
				        ca.setRuleDown(intArrow, BG_color);
			        }
			        else
			        {
				        ca.setRuleDown(intArrow, FG_color);
			        }
			
                    printRuleNumber(ca.getRuleNumber());
                    
                    ca.drawRule(intArrow);  // Draw the rule that has changed

			        ca.resetECA();

                    printPopulation();
                }
                else if (intArrow == 8) // Go back to the selection of the type of automata
                {
                    showAutomataTypeMenu();
                }
		    }
		
		    if(keys_pressed & KEY_RIGHT)
		    {
		        if (intArrow < 8)
			    {
			        ca.drawArrow(intArrow, BG_color);
			
			        if(intArrow < 7)
			        {
				        intArrow = intArrow + 1;
			        }
			        else
			        {
				        intArrow = 0;
			        }
	
			        ca.drawArrow(intArrow, line_color);
                }					     			
		    }
		    else if(keys_pressed & KEY_LEFT)
		    {
                if (intArrow < 8)
			    {		    
			        ca.drawArrow(intArrow, BG_color);
			
			        if(intArrow > 0)
			        {
				        intArrow = intArrow - 1;
			        }		
			        else
			        {
				        intArrow = 7;
			        }
			        ca.drawArrow(intArrow, line_color);
                }			        
		    }
		    else if(keys_pressed & KEY_UP)
		    {
		        if (intArrow < 8)
		        {
			        ca.drawArrow(intArrow, BG_color);
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

			    if (intArrow < 8)
			    {
			        ca.drawArrow(intArrow, line_color);
			    }
			    else 
			    {
			        printMenuArrow(displayedMenu, intArrow, false);
			    }
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
		        if (intArrow < 8)
		        {
			        ca.drawArrow(intArrow, BG_color);
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
			    
			    if (intArrow < 8)
		        {
			        ca.drawArrow(intArrow, line_color);
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
        else if (displayedMenu == LANGTON_ANT)
        {
    	    if (ca.hasFinished() == false)
    	    {
    	        ca.nextStep();
                printNumSteps();
                printPopulation();
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
                        printAntNumPixels();
                        ca.initialize();
                        printPopulation();
                        printNumSteps();
                    }                        
                }		        
		    }
		    else if (keys_pressed & KEY_RIGHT)
		    {
                if (intArrow == 0)
                {
                    ca.setAntNumPixels(ca.getAntNumPixels() + 1);
                    printAntNumPixels();
                    ca.initialize();
                    printPopulation();
                    printNumSteps();
                }		    
		    }
        }
        /*
         * Langton's hexagonal ant menu
         */                
        else if (displayedMenu == LANGTON_HEXAGONAL_ANT)
        {
    	    if (ca.hasFinished() == false) // If the ant hasn't finished continue with the next step
    	    {
    	        ca.nextStep();
                printNumSteps();
                printPopulation();
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
        else if (displayedMenu == BOOLEAN_AUTOMATA)
        {
            ca.nextStep();
            printNumSteps();
            printPopulation();
            
       	    if(keys_released & KEY_A)
		    {   
		        if (intArrow == 14)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }
                else
                {
		            if (intArrow == 0)
		            {
		                ca.setTypeOfNeighborhood(VON_NEUMANN_NEIGHBORHOOD);
		            }
		            else if (intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4)
		            {
        		        ca.toggleBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, intArrow - 1);
		            }
		            else if (intArrow == 5)
		            {
		                ca.setTypeOfNeighborhood(MOORE_NEIGHBORHOOD);
		            }
		            else if (intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12 || intArrow == 13)
		            {
                        ca.toggleBooleanRuleValue(MOORE_NEIGHBORHOOD, intArrow - 6);
		            }
                    printMenuAsterisks(BOOLEAN_AUTOMATA);
                    ca.initialize();
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
        else if (displayedMenu == BOOLEAN_HEXAGONAL_AUTOMATA)
        {
            ca.nextStep();
            
            printNumSteps();
            printPopulation();

            if (keys_released & KEY_A)
		    {
		        if (intArrow == 0 || intArrow == 1 || intArrow == 2 || intArrow == 3 || intArrow == 4 || intArrow == 5)
		        {
                    ca.toggleBooleanRuleValue(MOORE_NEIGHBORHOOD, intArrow);

		            printMenuAsterisks(BOOLEAN_HEXAGONAL_AUTOMATA);

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
        else if (displayedMenu == BOOLEAN_TRIANGULAR_AUTOMATA)
        {
            ca.nextStep();

            printNumSteps();
            printPopulation();

       	    if(keys_released & KEY_A)
		    {
    		    if (intArrow == 13)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }
                else
                {
		            if (intArrow == 0) // Von Neumann neighborhood
		            {
		                ca.setTypeOfNeighborhood(VON_NEUMANN_NEIGHBORHOOD);
		            }
		            else if (intArrow == 1 || intArrow == 2 || intArrow == 3) // Von Neumann neighborhood 1, 2, 3
		            {
                        ca.toggleBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, intArrow - 1);
		            }
		            else if (intArrow == 4) // Moore neighborhood
		            {
		                ca.setTypeOfNeighborhood(MOORE_NEIGHBORHOOD);
		            }
		            else if (intArrow == 5 || intArrow == 6 || intArrow == 7 || intArrow == 8 || intArrow == 9 || intArrow == 10 || intArrow == 11 || intArrow == 12) // Moore neighborhood 1, 2, 3, 4, 5, 6, 7, 8
		            {
                        ca.toggleBooleanRuleValue(MOORE_NEIGHBORHOOD, intArrow - 5);
		            }
		            printMenuAsterisks(BOOLEAN_TRIANGULAR_AUTOMATA);
		            ca.initialize();
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
        else if (displayedMenu == CONWAYS_GAME_OF_LIFE)
        {    	    
            ca.nextStep();
            
            printNumSteps();
            printPopulation();

    	    if(keys_released & KEY_A)
		    {
    		    if (intArrow == 4)
		        {
		            // Go back to the selection of the type of automata
                    showAutomataTypeMenu();
                }
                else
                {
		            if (intArrow == 0)
		            {
		                ca.setInitialState(ACORN);
		                ca.initialize();
		            }
		            else if (intArrow == 1)
		            {
		                ca.setInitialState(FPENTOMINO);
		                ca.initialize();
		            }
		            else if (intArrow == 2)
		            {
		                ca.setInitialState(FILL_SCREEN_WITH_PULSARS);
		                ca.initialize();
		            }
		            else if (intArrow == 3)
		            {
		                ca.setInitialState(FILL_SCREEN_WITH_PENTADECATHLONS);
		                ca.initialize();
		            }
		            printMenuAsterisks(CONWAYS_GAME_OF_LIFE);
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
        }
        /*
         * Munching squares menu
         */
        else if (displayedMenu == MUNCHING_SQUARES)
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
        else if (displayedMenu == SELECT_COLORS)
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
        else if (displayedMenu == SELECT_LANGUAGE)
        {
  		    if(keys_released & KEY_A)
  		    {
  		        if (intArrow == 0) // Change language to english
  		        {
  		            setLanguage(EN);
  		        }
  		        else if (intArrow == 1) // Change language to spanish
  		        {
  		            setLanguage(ES);
  		        }
  		        else if (intArrow == 2) // Change language to basque
  		        {
  		            setLanguage(EU);
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
