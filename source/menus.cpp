#include <nds.h>
#include <stdio.h>
#include <string>

#include "framebuffer.h"
#include "color.h"
#include "i18n.h"
#include "globals.h"

/*
 * Prints the text in the specified position using iprintf
 */
int printString(int row, int column, std::string text)
{
    iprintf("\x1b[%d;%dH%s", row, column, text.c_str());

    return 0;
}

/*
 * Converts to char array and prints the given intRuleNumber
 * of the Elementary Cellular Automata
 */
int printRuleNumber(int intRuleNumber)
{
    
    // Convert intRuleNumber to char array
    char buffer [3];
    sprintf(buffer, "%d", intRuleNumber);
    
    // Print 3 spaces to erase the previous rule
    iprintf("\x1b[%d;%dH%s:   ", RULE_NUMBER_ROW,
            RULE_NUMBER_COLUMN, stringRuleNumber.c_str());
    
    // Print it in the appropiate position
    iprintf("\x1b[%d;%dH%s:%s", RULE_NUMBER_ROW,
            RULE_NUMBER_COLUMN, stringRuleNumber.c_str(), buffer);
    
    return 0;
}

/*
 * Prints the current population of "alive" cells
 */
int printPopulation()
{
    iprintf("\x1b[%d;%dH%s%d    ", POPULATION_ROW,
            POPULATION_COLUMN, stringPopulation.c_str(), ca.getPopulation());

    return 0;
}

/*
 * Prints the number of pixels of the Langton's ant
 */
int printAntNumPixels()
{
    iprintf("\x1b[%d;%dH%s: < %d > ",
            LA_MENU_ANT_SIZE_ROW,
            LA_MENU_ANT_SIZE_COL,
            stringAntsPixels.c_str(), ca.getAntNumPixels());
    
    return 0;
}

/*
 * Prints the current density of the BML traffic model
 */
int printBMLdensity()
{
    iprintf("\x1b[%d;%dH%s: < %d > ", BML_MENU_DENSITY_ROW,
            BML_MENU_DENSITY_COL, stringDensity.c_str(), ca.getBMLdensity());
    return 0;
}

/*
 * Prints the number of states of the cyclic cellular automata
 */
int printCCANumStates()
{
    iprintf("\x1b[%d;%dH%s: < %d > ",
            CCA_MENU_NUM_STATES_ROW,
            CCA_MENU_NUM_STATES_COL,
            stringNumStates.c_str(), ca.getNumStates());

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
    printf("(c) 2012, 2013\n");
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
 * Prints an asterisk ("*") used to highlight the currently
 * selected menu option 
 */
int printAsterisk(int row, int column)
{
    iprintf("\x1b[%d;%dH*", row, column);
    
    return 0;
}

/*
 * Deletes an asterisk (" ") used to highlight the currently
 * selected menu option 
 */
int deleteAsterisk(int row, int column)
{
    iprintf("\x1b[%d;%dH ", row, column);
    
    return 0;
}

/*
 * Prints a line in the specified row and column to display the
 * number of the corresponding partial color.
 */
int printPartialColor(int row, int col, std::string str, int num)
{
    iprintf("\x1b[%d;%dH%s < %i > ", row, col,
                str.c_str(), num);
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
        printString(AUTOMATA_TYPE_MENU_ECA_ROW,
                    AUTOMATA_TYPE_MENU_ECA_COL,
                    stringElementaryCellularAutomata);
        printString(AUTOMATA_TYPE_MENU_LA_ROW,
                    AUTOMATA_TYPE_MENU_LA_COL,
                    stringLangtonsAnt);
        printString(AUTOMATA_TYPE_MENU_LHA_ROW,
                    AUTOMATA_TYPE_MENU_LHA_COL,
                    stringLangtonsHexagonalAnt);
        printString(AUTOMATA_TYPE_MENU_BA_ROW,
                    AUTOMATA_TYPE_MENU_BA_COL,
                    stringBooleanAutomata);
        printString(AUTOMATA_TYPE_MENU_BHA_ROW,
                    AUTOMATA_TYPE_MENU_BHA_COL,
                    stringBooleanHexagonalAutomata);
        printString(AUTOMATA_TYPE_MENU_BTA_ROW, 
                    AUTOMATA_TYPE_MENU_BTA_COL,
                    stringBooleanTriangularAutomata);
        printString(AUTOMATA_TYPE_MENU_CGL_ROW,
                    AUTOMATA_TYPE_MENU_CGL_COL,
                    stringConwaysGameOfLife);
        printString(AUTOMATA_TYPE_MENU_MS_ROW,
                    AUTOMATA_TYPE_MENU_MS_COL,
                    stringMunchingSquares);
        printString(AUTOMATA_TYPE_MENU_BML_ROW,
                    AUTOMATA_TYPE_MENU_BML_COL,
                    stringBMLtrafficModel);
        printString(AUTOMATA_TYPE_MENU_CCA_ROW,
                    AUTOMATA_TYPE_MENU_CCA_COL,
                    stringCyclicCellularAutomaton);
	printString(AUTOMATA_TYPE_MENU_ST_ROW,
		    AUTOMATA_TYPE_MENU_ST_COL,
		    stringSteppingStone);
        printString(AUTOMATA_TYPE_MENU_SELECT_COLORS_ROW,
                    AUTOMATA_TYPE_MENU_SELECT_COLORS_COL,
                    stringSelectColors);
        printString(AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_ROW,
                    AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_COL,
                    stringSelectLanguage);
    }
    else if (displayedMenu == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        printPopulation();

        printString(ECA_MENU_INITIAL_STATE_ROW,
                    ECA_MENU_INITIAL_STATE_COL,
                    stringInitialState);
        printString(ECA_MENU_INITIAL_STATE_ONE_CELL_ROW,
                    ECA_MENU_INITIAL_STATE_ONE_CELL_COL,
                    stringOneCell);
        printString(ECA_MENU_INITIAL_STATE_RANDOM_ROW,
                    ECA_MENU_INITIAL_STATE_RANDOM_COL,
                    stringRandom);
        printString(ECA_MENU_BACK_TO_MAIN_ROW,
                    ECA_MENU_BACK_TO_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == LANGTON_ANT)
    {
        printPopulation();
        printAntNumPixels();
        printString(LA_MENU_BACK_TO_MAIN_ROW,
                    LA_MENU_BACK_TO_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == LANGTON_HEXAGONAL_ANT)
    {
        printPopulation();
        printString(LHA_MENU_BACK_TO_MAIN_ROW,
                    LHA_MENU_BACK_TO_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == BOOLEAN_AUTOMATA)
    {
        printPopulation();
        printString(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 3,
                    stringVonNeumannNeighborhood);
        printString(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW, 6,
                    string1234);
        printString(BA_MENU_MOORE_NEIGHBORHOOD_ROW, 3,
                    stringMooreNeighborhood);
        printString(BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 6,
                    string1234);
        printString(BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 6,
                    string5678);        
        printString(BA_MENU_BACK_TO_MAIN_ROW, 2,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == BOOLEAN_HEXAGONAL_AUTOMATA)
    {
        printPopulation();
        printString(BHA_MENU_NEIGHBORHOOD_ROW, 2, stringNeighborhood);
        printString(BHA_MENU_NEIGHBORHOOD_123_ROW, 6, string123);
        printString(BHA_MENU_NEIGHBORHOOD_456_ROW, 6, string456);
        printString(BHA_MENU_BACK_TO_MAIN_ROW, 2, stringBackToMainMenu);
    }
    else if (displayedMenu == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        printPopulation();
        printString(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 3,
                    stringVonNeumannNeighborhood);
        printString(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 6,
                    string123);
        printString(BTA_MENU_MOORE_NEIGHBORHOOD_ROW, 3,
                    stringMooreNeighborhood);
        printString(BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW, 6,
                    string1234);
        printString(BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW, 6,
                    string5678);        
        printString(BTA_MENU_BACK_TO_MAIN_ROW, 2,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == CONWAYS_GAME_OF_LIFE)
    {
        printPopulation();
        printString(CGL_MENU_INITIAL_STATE_ROW, 2,
                    stringInitialState);
        printString(CGL_MENU_ACORN_ROW, 3,
                    stringAcorn);
        printString(CGL_MENU_FPENTOMINO_ROW, 3,
                    stringFPentomino);
        printString(CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW, 3,
                    stringFillScreenWithPulsars);
        printString(CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW, 3,
                    stringFillScreenWithPentadecathlons);
        printString(CGL_MENU_BACK_TO_MAIN_ROW, 2,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == MUNCHING_SQUARES)
    {
        printString(MS_MENU_BACK_TO_MAIN_ROW, 
                    MS_MENU_BACK_TO_MAIN_COLUMN,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == BML_TRAFFIC_MODEL)
    {
        printString(BML_MENU_RESTART_ROW,
                    BML_MENU_RESTART_COL,
                    stringRestart);
        printBMLdensity();
        printString(BML_MENU_BACK_TO_MAIN_ROW,
                    BML_MENU_BACK_TO_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == CYCLIC_CELLULAR_AUTOMATA)
    {
        printCCANumStates();
        printString(CCA_MENU_BACK_TO_MAIN_ROW,
                    CCA_MENU_BACK_TO_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == STEPPING_STONE)
    {
        printString(ST_MENU_BACK_TO_MAIN_ROW,
                    ST_MENU_BACK_TO_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == SELECT_COLORS)
    {
        printString(CS_MENU_BG_COLOR_ROW,
                    CS_MENU_MAIN_COL,
                    stringBackgroundColor);
        printPartialColor(CS_MENU_BG_COLOR_R_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringRed.c_str(), BG_R);
        printPartialColor(CS_MENU_BG_COLOR_G_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringGreen.c_str(), BG_G);
        printPartialColor(CS_MENU_BG_COLOR_B_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringBlue.c_str(), BG_B);
        
        printString(CS_MENU_FG_COLOR_ROW,
                    CS_MENU_MAIN_COL,
                    stringForegroundColor);
        printPartialColor(CS_MENU_FG_COLOR_R_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringRed.c_str(), FG_R);
        printPartialColor(CS_MENU_FG_COLOR_G_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringGreen.c_str(), FG_G);
        printPartialColor(CS_MENU_FG_COLOR_B_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringBlue.c_str(), FG_B);

        printString(CS_MENU_FG2_COLOR_ROW,
                    CS_MENU_MAIN_COL,
                    stringSecondForegroundColor);
        printPartialColor(CS_MENU_FG2_COLOR_R_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringRed.c_str(), FG_R2);
        printPartialColor(CS_MENU_FG2_COLOR_G_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringGreen.c_str(), FG_G2);
        printPartialColor(CS_MENU_FG2_COLOR_B_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringBlue.c_str(), FG_B2);

        printString(CS_MENU_LINE_COLOR_ROW,
                    CS_MENU_MAIN_COL,
                    stringLineColor);
        printPartialColor(CS_MENU_LINE_COLOR_R_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringRed.c_str(), line_R);
        printPartialColor(CS_MENU_LINE_COLOR_G_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringGreen.c_str(), line_G);
        printPartialColor(CS_MENU_LINE_COLOR_B_ROW,
                          CS_MENU_PARTIAL_COLOR_COL,
                          stringBlue.c_str(), line_B);
                
        printString(CS_MENU_BACK_TO_MAIN_ROW,
                    CS_MENU_MAIN_COL,
                    stringBackToMainMenu);
    }
    else if (displayedMenu == SELECT_LANGUAGE)
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
    if (intDisplayedMenu == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        if (ca.getInitialStateType() == ECA_INITIALIZE_ONE_CELL)
        {
            printAsterisk(ECA_MENU_INITIAL_STATE_ONE_CELL_ROW,
                          ECA_MENU_INITIAL_STATE_ONE_CELL_ASTERISK_COL);
        }
        else if (ca.getInitialStateType() == ECA_INITIALIZE_RANDOM)
        {
            printAsterisk(ECA_MENU_INITIAL_STATE_RANDOM_ROW,
                          ECA_MENU_INITIAL_STATE_RANDOM_ASTERISK_COL);
        }
    }
    else if (intDisplayedMenu == BOOLEAN_AUTOMATA)
    {
        if (ca.getTypeOfNeighborhood() == VON_NEUMANN_NEIGHBORHOOD)
        {
            printAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 2);
            deleteAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_ROW, 2);
        }
        else
        {
            deleteAsterisk(BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW, 2);
            printAsterisk(BA_MENU_MOORE_NEIGHBORHOOD_ROW, 2);
        }
        
        // {1, 2, 3, 4}
        // For the boolean square automata with Von Neumann neighborhood
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
                
        // {1, 2, 3, 4, 5, 6, 7, 8}
        // For the boolean square automata with Moore neighborhood
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
        // {1, 2, 3, 4, 5, 6}
        // For the boolean hexagonal automata with Moore neighborhood
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
        
        // {1, 2, 3}
        // For the boolean triangular automata with Von Neumann neighborhood
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 0))
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 5);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 5);
        }
        
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 1))
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 10);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 10);
        }
                
        if (ca.checkBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD, 2))
        {
            printAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 15);
        }
        else
        {
            deleteAsterisk(BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW, 15);
        }

        // {1, 2, 3, 4, 5, 6, 7, 8}
        // For the boolean triangular automata with Moore neighborhood
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
        if (index == ELEMENTARY_CELLULAR_AUTOMATA)
        {
            row = AUTOMATA_TYPE_MENU_ECA_ROW;
        }
        else if (index == LANGTON_ANT)
        {
            row = AUTOMATA_TYPE_MENU_LA_ROW;
        }
        else if (index == LANGTON_HEXAGONAL_ANT)
        {
            row = AUTOMATA_TYPE_MENU_LHA_ROW;
        }        
        else if (index == BOOLEAN_AUTOMATA)
        {    
            row = AUTOMATA_TYPE_MENU_BA_ROW;
        }
        else if (index == BOOLEAN_HEXAGONAL_AUTOMATA)
        {
            row = AUTOMATA_TYPE_MENU_BHA_ROW;
        }
        else if (index == BOOLEAN_TRIANGULAR_AUTOMATA)
        {
            row = AUTOMATA_TYPE_MENU_BTA_ROW;
        }
        else if (index == CONWAYS_GAME_OF_LIFE)
        {
            row = AUTOMATA_TYPE_MENU_CGL_ROW;
        }        
        else if (index == MUNCHING_SQUARES)
        {    
            row = AUTOMATA_TYPE_MENU_MS_ROW;
        }
        else if (index == BML_TRAFFIC_MODEL)
        {
            row = AUTOMATA_TYPE_MENU_BML_ROW;
            column = AUTOMATA_TYPE_MENU_BML_ARROW_COL;
        }
        else if (index == CYCLIC_CELLULAR_AUTOMATA)
        {
            row = AUTOMATA_TYPE_MENU_CCA_ROW;
            column = AUTOMATA_TYPE_MENU_CCA_ARROW_COL;
        }
	else if (index == STEPPING_STONE)
	{
	    row = AUTOMATA_TYPE_MENU_ST_ROW;
	    column = AUTOMATA_TYPE_MENU_ST_ARROW_COL;
	}
        else if (index == SELECT_COLORS)
        {
            row = AUTOMATA_TYPE_MENU_SELECT_COLORS_ROW;
        }
        else if (index == SELECT_LANGUAGE)
        {
            row = AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_ROW;
        }
    }
    
    else if (intDisplayedMenu == ELEMENTARY_CELLULAR_AUTOMATA)
    {
        if (index == 8)
        {
            row = ECA_MENU_INITIAL_STATE_ONE_CELL_ROW;
            column = ECA_MENU_INITIAL_STATE_ONE_CELL_ARROW_COL;
        }
        if (index == 9)
        {
            row = ECA_MENU_INITIAL_STATE_RANDOM_ROW;
            column = ECA_MENU_INITIAL_STATE_RANDOM_ARROW_COL;
        }
        else if (index == 10)
        {
            row = ECA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == LANGTON_ANT)
    {
        // Print the arrow in the option to choose the size of the ant
        if (index == 0)
        {
            row = LA_MENU_ANT_SIZE_ROW;
        }
        // Print the arrow in the option to go back to main menu
        else if (index == 1)
        {
            row = LA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == LANGTON_HEXAGONAL_ANT)
    {
        if (index == 0)
        {
            row = LHA_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == BOOLEAN_AUTOMATA)
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
    else if (intDisplayedMenu == BOOLEAN_HEXAGONAL_AUTOMATA)
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
    else if (intDisplayedMenu == BOOLEAN_TRIANGULAR_AUTOMATA)
    {
        if (index == 0) // Von Neumann neighborhood
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW;
        }
        else if (index == 1) // 1
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW;
            column = 3;
        }
        else if (index == 2) // 2
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW;
            column = 8;
        }
        else if (index == 3) // 3
        {
            row = BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW;
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
    else if (intDisplayedMenu == CONWAYS_GAME_OF_LIFE)
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
            // Fill screen with pulsars
            row = CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW;
        }
        else if (index == 3)
        {
            // Fill screen with pentadecathlons
            row = CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW;
        }
        else if (index == 4)
        {
            row = CGL_MENU_BACK_TO_MAIN_ROW; // Back to main menu
        }
    }
    else if (intDisplayedMenu == MUNCHING_SQUARES)
    {
        if (index == 0)
        {
            row = MS_MENU_BACK_TO_MAIN_ROW; // Back to main menu
        }
    }
    else if (intDisplayedMenu == BML_TRAFFIC_MODEL)
    {
        if (index == 0)
        {
            row = BML_MENU_RESTART_ROW;
            column = BML_MENU_RESTART_ARROW_COL;
        }
        else if (index == 1)
        {
            row = BML_MENU_DENSITY_ROW; 
            column = BML_MENU_DENSITY_ARROW_COL;
        }
        else if (index == 2)
        {
            row = BML_MENU_BACK_TO_MAIN_ROW; // Back to main menu
            column = BML_MENU_BACK_TO_MAIN_ARROW_COL;
        }
    }
    else if (intDisplayedMenu == CYCLIC_CELLULAR_AUTOMATA)
    {
        if (index == 0) {
            row = CCA_MENU_NUM_STATES_ROW;
            column = CCA_MENU_NUM_STATES_ARROW_COL;
        }
        else if (index == 1) // Back to main menu
        {
            row = CCA_MENU_BACK_TO_MAIN_ROW;
            column = CCA_MENU_BACK_TO_MAIN_ARROW_COL;
        }
    }
    else if (intDisplayedMenu == STEPPING_STONE)
    {
        if (index == 0) // Back to main menu
        {
            row = ST_MENU_BACK_TO_MAIN_ROW;
            column = ST_MENU_BACK_TO_MAIN_ARROW_COL;
        }
    }
    else if (intDisplayedMenu == SELECT_COLORS)
    {
        if (index == 0) // BG red
        {
            row = CS_MENU_BG_COLOR_R_ROW;
        }
        else if (index == 1) // BG green
        {
            row = CS_MENU_BG_COLOR_G_ROW;
        }
        else if (index == 2) // BG blue
        {
            row = CS_MENU_BG_COLOR_B_ROW;
        }
        else if (index == 3) // FG red
        {
            row = CS_MENU_FG_COLOR_R_ROW;
        }
        else if (index == 4) // FG green
        {
            row = CS_MENU_FG_COLOR_G_ROW;
        }
        else if (index == 5) // FG blue
        {
            row = CS_MENU_FG_COLOR_B_ROW;
        }
        else if (index == 6) // FG2 red
        {
            row = CS_MENU_FG2_COLOR_R_ROW;
        }
        else if (index == 7) // FG2 green
        {
            row = CS_MENU_FG2_COLOR_G_ROW;
        }
        else if (index == 8) // FG2 blue
        {
            row = CS_MENU_FG2_COLOR_B_ROW;
        }
        else if (index == 9) // Line color red
        {
            row = CS_MENU_LINE_COLOR_R_ROW;
        }
        else if (index == 10) // Line color green
        {
            row = CS_MENU_LINE_COLOR_G_ROW;
        }
        else if (index == 11) // Line color blue
        {
            row = CS_MENU_LINE_COLOR_B_ROW;
        }
        else if (index == 12) // Back to main menu
        {
            row = CS_MENU_BACK_TO_MAIN_ROW;
        }
    }
    else if (intDisplayedMenu == SELECT_LANGUAGE)    
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
    else if (automataType == BML_TRAFFIC_MODEL)
    {
        printf("%s", stringBMLtrafficModel.c_str());
    }
    else if (automataType == CYCLIC_CELLULAR_AUTOMATA)
    {
        printf("%s", stringCyclicCellularAutomaton.c_str());
    }
    else if (automataType == STEPPING_STONE)
    {
	printf("%s", stringSteppingStone.c_str());
    }

    return 0;
}

/*
 * Prints the number of steps
 */
int printNumSteps()
{
    iprintf("\x1b[%d;%dH%s:     ", NUM_STEPS_ROW,
            NUM_STEPS_COLUMN, stringSteps.c_str());                                                    
    iprintf("\x1b[%d;%dH%s: %d", NUM_STEPS_ROW,
            NUM_STEPS_COLUMN, stringSteps.c_str(), ca.getNumSteps());

    return 0;
}

/*
 *
 */
int setLanguage(int language, int intArrow)
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


