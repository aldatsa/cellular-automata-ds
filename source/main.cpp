/*
    Copyright 2012, 2013 Asier Iturralde Sarasola
    
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
#include "cellularautomata.h"
#include "globals.h"
#include "menus.h"

/*************************** START MAIN FUNCTION *****************************/

/*
 *
 */
int main(void)
{           
    /*
     * A variable for the position of the touch
     */
    touchPosition touch;

    /*
     * The position of the arrow
     * Select automata menu: ????
     * Elementary Cellular Automata:
     * in the top screen (0-7) and in the bottom screen (8)
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
                // Delete previous arrow
                printMenuArrow(displayedMenu, automataType, true);

                if (automataType != ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    automataType = automataType - 1;
                }
                else
                {
                    automataType = SELECT_LANGUAGE;
                }
                // Print new arrow
                printMenuArrow(displayedMenu, automataType, false);  
            }
            else if (keys_released & KEY_DOWN)
            {
                // Delete previous arrow
                printMenuArrow(displayedMenu, automataType, true);

                if (automataType != SELECT_LANGUAGE)
                {
                    automataType = automataType + 1;
                }
                else
                {
                    automataType = ELEMENTARY_CELLULAR_AUTOMATA;
                }
                // Print new arrow                
                printMenuArrow(displayedMenu, automataType, false);
            }
            else if (keys_released & KEY_A)
            {
                ca.setType(automataType);
                
                consoleClear();
                
                if (automataType != SELECT_COLORS)
                {
                    printCredits();
                }

                printAutomataType(automataType);

                intArrow = 0;     
                
                displayedMenu = automataType;
                
                if (automataType == LANGTON_ANT)
                {
                    ca.setAntNumPixels(LA_INITIAL_NUM_PIXELS);
                }
                else if (automataType == BOOLEAN_AUTOMATA)
                {
                    // Von Neumann neighborhood (default)
                    ca.setTypeOfNeighborhood(VON_NEUMANN_NEIGHBORHOOD);

                    ca.setDefaultBooleanRuleValues();
                }
                else if (automataType == BOOLEAN_HEXAGONAL_AUTOMATA)
                {   
                    ca.setDefaultBooleanRuleValues();
                }
                else if (automataType == BOOLEAN_TRIANGULAR_AUTOMATA)
                {
                    // Von Neumann neighborhood (default)
                    ca.setTypeOfNeighborhood(VON_NEUMANN_NEIGHBORHOOD);

                    ca.setDefaultBooleanRuleValues();
                }
                else if (automataType == CONWAYS_GAME_OF_LIFE)
                {
                    ca.setInitialState(ACORN);
                }
                else if (automataType == CYCLIC_CELLULAR_AUTOMATA)
                {
                    ca.setNumStates(CCA_INITIAL_NUM_STATES);
                }

                if (automataType != ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    printMenuArrow(displayedMenu, intArrow, false);
                }
                
                printMenu(displayedMenu);
                
                ca.initialize();

                if (automataType == ELEMENTARY_CELLULAR_AUTOMATA)
                {
                    // The rule number must be printed after
                    //  the initialization of the automata
                    printRuleNumber(ca.getRuleNumber());

                    // Draw the arrow in the default position
                    // (intArrow = 0, top left rule (2^7),
                    //  set in ca.initialize())
                    ca.drawArrow(intArrow, line_color);
                }                
                
                // Include the other types of automata when the population
                // count for them is implemented
                if (automataType == ELEMENTARY_CELLULAR_AUTOMATA ||
                    automataType == LANGTON_ANT ||
                    automataType == LANGTON_HEXAGONAL_ANT ||
                    automataType == BOOLEAN_AUTOMATA ||
                    automataType == BOOLEAN_HEXAGONAL_AUTOMATA ||
                    automataType == BOOLEAN_TRIANGULAR_AUTOMATA ||
                    automataType == CONWAYS_GAME_OF_LIFE ||
                    automataType == BML_TRAFFIC_MODEL)
                {
                    printPopulation();
                }

                if (automataType == ELEMENTARY_CELLULAR_AUTOMATA ||
                    automataType == BOOLEAN_AUTOMATA ||
                    automataType == BOOLEAN_HEXAGONAL_AUTOMATA ||
                    automataType == BOOLEAN_TRIANGULAR_AUTOMATA ||
                    automataType == CONWAYS_GAME_OF_LIFE ||
                    automataType == SELECT_LANGUAGE)
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
		        if (intArrow < 8) // Top screen (Automata rules)
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
                // Initial state: One cell
                else if (intArrow == 8)
                {
                    ca.setInitialStateType(ECA_INITIALIZE_ONE_CELL);

                    ca.resetECA();

                    printPopulation();

                    printAsterisk(ECA_MENU_INITIAL_STATE_ONE_CELL_ROW,
                                  ECA_MENU_INITIAL_STATE_ONE_CELL_ASTERISK_COL);
                    deleteAsterisk(ECA_MENU_INITIAL_STATE_RANDOM_ROW,
                                  ECA_MENU_INITIAL_STATE_RANDOM_ASTERISK_COL);
                }
                // Initial state: Random
                else if (intArrow == 9) 
                {
                    ca.setInitialStateType(ECA_INITIALIZE_RANDOM);

                    ca.resetECA();

                    printPopulation();

                    deleteAsterisk(ECA_MENU_INITIAL_STATE_ONE_CELL_ROW,
                                   ECA_MENU_INITIAL_STATE_ONE_CELL_ASTERISK_COL);
                    printAsterisk(ECA_MENU_INITIAL_STATE_RANDOM_ROW,
                                  ECA_MENU_INITIAL_STATE_RANDOM_ASTERISK_COL);
                }
                // Go back to the selection of the type of automata
                else if (intArrow == 10)
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
                else if(intArrow == 9 || intArrow == 10)
                {
                    intArrow = intArrow - 1;
                }
			    else if(intArrow < 4)
			    {
				    intArrow = 10;
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

			    if (intArrow == 10)
			    {
			        intArrow = 0;
			    }
                else if (intArrow == 8 || intArrow == 9)
                {
                    intArrow = intArrow + 1;
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
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);

		        // Calculate the next position of the arrow		        
		        if (intArrow == 0)
		        {
		            intArrow = 1;
		        }
		        else
		        {
		            intArrow = intArrow - 1;
		        }
                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		     
		        // Calculate the next position of the arrow   
		        if (intArrow == 1)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		        // Print the new arrow   
		        printMenuArrow(displayedMenu, intArrow, false);
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
            // If the ant hasn't finished continue with the next step
    	    if (ca.hasFinished() == false)
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
		            else if (intArrow == 1 || intArrow == 2 ||
                             intArrow == 3 || intArrow == 4)
		            {
        		        ca.toggleBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD,
                                                  intArrow - 1);
		            }
		            else if (intArrow == 5)
		            {
		                ca.setTypeOfNeighborhood(MOORE_NEIGHBORHOOD);
		            }
		            else if (intArrow == 6 || intArrow == 7 ||
                             intArrow == 8 || intArrow == 9 ||
                             intArrow == 10 || intArrow == 11 ||
                             intArrow == 12 || intArrow == 13)
		            {
                        ca.toggleBooleanRuleValue(MOORE_NEIGHBORHOOD,
                                                  intArrow - 6);
		            }
                    printMenuAsterisks(BOOLEAN_AUTOMATA);
                    ca.initialize();
                }
            }
            else if(keys_released & KEY_UP)
		    {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        		    
		        if (intArrow == 0) // Von Neumann -> Back to main menu
		        {
		            intArrow = 14;
		        }
                // 1, 2, 3, 4 from Von Neumann -> Von Neumann
		        else if (intArrow == 1 || intArrow == 2 ||
                         intArrow == 3 || intArrow == 4)
		        {
		            intArrow = 0;
		        }
		        else if (intArrow == 5) // Moore -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
                // 1, 2, 3, 4 from Moore -> Moore
		        else if (intArrow == 6 || intArrow == 7 ||
                         intArrow == 8 || intArrow == 9)
		        {
		            intArrow = 5;
		        }
                // 5 -> 1, 6 -> 2, 7 -> 3, 8 -> 4 (Moore)
		        else if (intArrow == 10 || intArrow == 11 ||
                         intArrow == 12 || intArrow == 13)
		        {
		            intArrow = intArrow - 4;
		        }
		        else if (intArrow == 14) // Back to main menu -> 5 from Moore
		        {
		            intArrow = 10;
		        }
                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);		        
		    }
            else if(keys_released & KEY_DOWN)
		    {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        		    
		        if (intArrow == 0) // Von Neumann -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
                // 1, 2, 3, 4 from Von Neumann -> Moore
		        else if (intArrow == 1 || intArrow == 2 ||
                         intArrow == 3 || intArrow == 4)
		        {
		            intArrow = 5;
		        }
		        else if (intArrow == 5) // Moore -> 1 from Moore
		        {
		            intArrow = 6;
		        }
                // 1 -> 5, 2 -> 6, 3 -> 7, 4 -> 8 (Moore)
		        else if (intArrow == 6 || intArrow == 7 ||
                         intArrow == 8 || intArrow == 9)
		        {
		            intArrow = intArrow + 4;
		        }
                // 5, 6, 7, 8 from Moore -> Back to main menu
		        else if (intArrow == 10 || intArrow == 11 ||
                         intArrow == 12 || intArrow == 13)
		        {
		            intArrow = 14;
		        }
                // Back to main menu -> 1 from Von Neumann
		        else if (intArrow == 14)
		        {
		            intArrow = 0;
		        }
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);		        
		    }
		    else if (keys_released & KEY_LEFT)
		    {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
                // 1 -> 4 (Von Neumann), 1 -> 4, 5 -> 8 (Moore)
		        if (intArrow == 1 || intArrow == 6 || intArrow == 10)		                   {
		            intArrow = intArrow + 3;
		        }
                // 2 -> 1, 3 -> 2, 4 -> 3 (Von Neumann),
                //  2 -> 1, 3 -> 2, 4 -> 3, 6 -> 5, 7 -> 6, 8 -> 7 (Moore)
		        else if (intArrow == 2 || intArrow == 3 ||
                         intArrow == 4 || intArrow == 7 ||
                         intArrow == 8 || intArrow == 9 ||
                         intArrow == 11 || intArrow == 12 || intArrow == 13) 
		        {
		            intArrow = intArrow - 1;
		        }
                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);     		   
		    }
		    else if (keys_released & KEY_RIGHT)
		    {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);

                // 4 -> 1 (Von Neumann), 4 -> 1, 8 -> 5 (Moore)
		        if (intArrow == 4 || intArrow == 9 || intArrow == 13)
		        {
		            intArrow = intArrow - 3;
		        }
                // 1 -> 2, 2 -> 3, 3 -> 4 (Von Neumann),
                // 1 -> 2, 2 -> 3, 3 -> 4, 5 -> 6, 6 -> 7, 7 -> 8 (Moore)
		        else if (intArrow == 1 || intArrow == 2 ||
                         intArrow == 3 || intArrow == 6 ||
                         intArrow == 7 || intArrow == 8 ||
                         intArrow == 10 || intArrow == 11 || intArrow == 12) 
		        {
		            intArrow = intArrow + 1;
		        }
                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
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
		        if (intArrow == 0 || intArrow == 1 ||
                    intArrow == 2 || intArrow == 3 ||
                    intArrow == 4 || intArrow == 5)
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
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);

                // 1, 2, 3-> Back to main menu
		        if (intArrow == 0 or intArrow == 1 or intArrow == 2)
		        {
		            intArrow = 6;
		        }
                // 4 -> 1, 5 -> 2, 6 -> 3
		        else if (intArrow == 3 or intArrow == 4 or intArrow == 5)
		        {
		            intArrow = intArrow - 3;
		        }
		        else if (intArrow == 6) // Back to main menu -> 4
		        {
		            intArrow = 3;
                }

                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);		        		        		        
            }		    
            else if(keys_released & KEY_DOWN)
            {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);

                // 1 -> 4, 2 -> 5, 3 -> 6
		        if (intArrow == 0 or intArrow == 1 or intArrow == 2)
		        {
		            intArrow = intArrow + 3;
		        }
                // 4, 5, 6 -> Back to main menu
		        else if (intArrow == 3 or intArrow == 4 or intArrow == 5)
		        {
		            intArrow = 6;
		        }
		        else if (intArrow == 6) // Back to main menu -> 1
		        {
		            intArrow = 0;
                }		            
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);		        		        		        
            }		                
            else if(keys_released & KEY_LEFT)
            {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);

		        if (intArrow == 0 or intArrow == 3) // 1 -> 3, 4 -> 6
		        {
		            intArrow = intArrow + 2;
		        }
                // 2 -> 1, 3 -> 2, 5 -> 4, 6 -> 5
		        else if (intArrow == 1 or intArrow == 2 or
                         intArrow == 4 or intArrow == 5)
		        {
		            intArrow = intArrow - 1;
		        }

                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
            }		        		        		        		                    
            else if(keys_released & KEY_RIGHT)
            {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);

		        if (intArrow == 2 or intArrow == 5) // 3 -> 1, 6 -> 4
		        {
		            intArrow = intArrow - 2;
		        }
                // 1 -> 2, 2 -> 3, 4 -> 5, 5 -> 6
		        else if (intArrow == 0 or intArrow == 1 or
                         intArrow == 3 or intArrow == 4)
		        {
		            intArrow = intArrow + 1;
		        }

                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
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
                    // Von Neumann neighborhood 1, 2, 3
		            else if (intArrow == 1 || intArrow == 2 || intArrow == 3)
		            {
                        ca.toggleBooleanRuleValue(VON_NEUMANN_NEIGHBORHOOD,
                                                  intArrow - 1);
		            }
		            else if (intArrow == 4) // Moore neighborhood
		            {
		                ca.setTypeOfNeighborhood(MOORE_NEIGHBORHOOD);
		            }
                    // Moore neighborhood 1, 2, 3, 4, 5, 6, 7, 8
		            else if (intArrow == 5 || intArrow == 6 ||
                             intArrow == 7 || intArrow == 8 ||
                             intArrow == 9 || intArrow == 10 ||
                             intArrow == 11 || intArrow == 12)
		            {
                        ca.toggleBooleanRuleValue(MOORE_NEIGHBORHOOD,
                                                  intArrow - 5);
		            }
		            printMenuAsterisks(BOOLEAN_TRIANGULAR_AUTOMATA);
		            ca.initialize();
		        }
            }
            else if(keys_released & KEY_UP)
		    {
                // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);		        		    
		        if (intArrow == 0) // Von Neumann -> Back to main menu
		        {
		            intArrow = 13;
		        }
		        // 1, 2, 3 from Von Neumann -> Von Neumann
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3)
		        {
		            intArrow = 0;
		        }
		        else if (intArrow == 4) // Moore -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
		        // 1, 2, 3, 4 from Moore -> Moore
		        else if (intArrow == 5 || intArrow == 6 ||
		                  intArrow == 7 || intArrow == 8)
		        {
		            intArrow = 4;
		        }
		        // 5 -> 1, 6 -> 2, 7 -> 3, 8 -> 4 (Moore)
		        else if (intArrow == 9 || intArrow == 10 ||
		                  intArrow == 11 || intArrow == 12)
		        {
		            intArrow = intArrow - 4;
		        }
		        // Back to main menu -> 5 from Moore
		        else if (intArrow == 13)
		        {
		            intArrow = 9;
		        }
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
            else if(keys_released & KEY_DOWN)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        		    
		        if (intArrow == 0) // Von Neumann -> 1 from Von Neumann
		        {
		            intArrow = 1;
		        }
		        // 1, 2, 3 from Von Neumann -> Moore
		        else if (intArrow == 1 || intArrow == 2 || intArrow == 3)
		        {
		            intArrow = 4;
		        }
		        else if (intArrow == 4) // Moore -> 1 from Moore
		        {
		            intArrow = 5;
		        }
		        // 1 -> 5, 2 -> 6, 3 -> 7, 4 -> 8 (Moore)
		        else if (intArrow == 5 || intArrow == 6 ||
		                  intArrow == 7 || intArrow == 8)
		        {
		            intArrow = intArrow + 4;
		        }
		        // 5, 6, 7, 8 from Moore -> Back to main menu
		        else if (intArrow == 9 || intArrow == 10 ||
		                  intArrow == 11 || intArrow == 12)
		        {
		            intArrow = 13;
		        }
		        // Back to main menu -> 1 from Von Neumann
		        else if (intArrow == 13)
		        {
		            intArrow = 0;
		        }
		        
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
		    else if (keys_released & KEY_LEFT)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        		    
		        if (intArrow == 1)
		        {
		            intArrow = 3;
		        }
		        // 1 -> 4 (Von Neumann), 1 -> 4, 5 -> 8 (Moore)
		        else if (intArrow == 5 || intArrow == 9)
		        {
		            intArrow = intArrow + 3;
		        }
                // 2 -> 1, 3 -> 2, 4 -> 3 (Von Neumann),
                // 2 -> 1, 3 -> 2, 4 -> 3, 6 -> 5, 7 -> 6, 8 -> 7 (Moore)
		        else if (intArrow == 2 || intArrow == 3 || intArrow == 6 ||
		                  intArrow == 7 || intArrow == 8 || intArrow == 10 ||
		                  intArrow == 11 || intArrow == 12) 
		        {
		            intArrow = intArrow - 1;
		        }

                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
		    else if (keys_released & KEY_RIGHT)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        		    
		        if (intArrow == 3)
		        {
		            intArrow = 1;
		        }
		        // 4 -> 1 (Von Neumann), 4 -> 1, 8 -> 5 (Moore)
		        else if (intArrow == 8 || intArrow == 12)
		        {
		            intArrow = intArrow - 3;
		        }
                // 1 -> 2, 2 -> 3, 3 -> 4 (Von Neumann),
                // 1 -> 2, 2 -> 3, 3 -> 4, 5 -> 6, 6 -> 7, 7 -> 8 (Moore)
		        else if (intArrow == 1 || intArrow == 2 ||
		                  intArrow == 5 || intArrow == 6 ||
		                  intArrow == 7 || intArrow == 9 ||
		                  intArrow == 10 || intArrow == 11) 
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
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
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
		        if (intArrow == 0)
		        {
		            intArrow = 4;
		        }
		        else
		        {
		            intArrow = intArrow - 1;
		        }

		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
		        if (intArrow == 4)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }

                // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
        }
        /*
         * Munching squares menu
         */
        else if (displayedMenu == MUNCHING_SQUARES)
        {
            ca.nextStep();

            printNumSteps();
            printPopulation();

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
         * BML traffic model
         */
        else if (displayedMenu == BML_TRAFFIC_MODEL)
        {
            ca.nextStep();

            printNumSteps();
            printPopulation();

            if(keys_released & KEY_A)
            {
                if (intArrow == 0)
                {
                    // Restart the automata
                    ca.initialize();
                    printPopulation();
                }
                else if (intArrow == 2) // Back to main menu
                {
                    showAutomataTypeMenu();
                }
            }
            else if (keys_pressed & KEY_UP)
            {
                // Delete the previous arrow
                printMenuArrow(displayedMenu, intArrow, true);

                if (intArrow == 0)
                {
                    intArrow = 2;
                }
                else
                {
                    intArrow = intArrow - 1;
                }

                // Print the new arrow
                printMenuArrow(displayedMenu, intArrow, false);
            }
            else if (keys_pressed & KEY_DOWN)
            {
                // Delete the previous arrow
                printMenuArrow(displayedMenu, intArrow, true);

                if (intArrow == 2)
                {
                    intArrow = 0;
                }
                else
                {
                    intArrow = intArrow + 1;
                }

                // Print the new arrow
                printMenuArrow(displayedMenu, intArrow, false);
            }
            else if (keys_pressed & KEY_LEFT)
            {
                if (intArrow == 1 && ca.getBMLdensity() > 0)
                {
                    ca.setBMLdensity(ca.getBMLdensity() - 1);
                    printBMLdensity();
                }    
            }
            else if (keys_pressed & KEY_RIGHT)
            {
                if (intArrow == 1 && ca.getBMLdensity() < 100)
                {
                    ca.setBMLdensity(ca.getBMLdensity() + 1);
                    printBMLdensity();
                }                
            }
        }
        /*
         * Cyclic Cellular Automata menu
         */
        else if (displayedMenu == CYCLIC_CELLULAR_AUTOMATA) {
            if (ca.getNumSteps() < 192)
            {
			    ca.nextStep();
            
                printNumSteps();
                printPopulation();
            }
            if (keys_released & KEY_A)
            {
                if (intArrow == 1) // Back to the main menu
                {
                    showAutomataTypeMenu();
                }
            }
            else if(keys_pressed & KEY_UP)
            {
                // Delete the previous arrow
                printMenuArrow(displayedMenu, intArrow, true);

                if (intArrow == 0)
                {
                    intArrow = 1;
                }
                else
                {
                    intArrow = intArrow - 1;
                }

                // Print the new arrow
                printMenuArrow(displayedMenu, intArrow, false);
            }
            else if(keys_pressed & KEY_DOWN)
            {
                // Delete the previous arrow
                printMenuArrow(displayedMenu, intArrow, true);

                if (intArrow == 1)
                {
                    intArrow = 0;
                }
                else
                {
                    intArrow = intArrow + 1;
                }

                // Print the new arrow
                printMenuArrow(displayedMenu, intArrow, false);
            }
            else if (keys_pressed & KEY_LEFT)
            {
                if (intArrow == 0 && ca.getNumStates() > 3)
                {
                    ca.setNumStates(ca.getNumStates() - 1);
                    printCCANumStates();

                    ca.initialize();
                    printPopulation();
                    printNumSteps();
                }
            }
            else if (keys_pressed & KEY_RIGHT)
            {
                if (intArrow == 0 && ca.getNumStates() < 4)
                {
                    ca.setNumStates(ca.getNumStates() + 1);
                    printCCANumStates();

                    ca.initialize();
                    printPopulation();
                    printNumSteps();
                }
            }
        }
	/*
         * Cyclic Cellular Automata menu
         */
        else if (displayedMenu == STEPPING_STONE) {
	    
	    ca.nextStep();
	    
	    if (keys_released & KEY_A)
            {
                if (intArrow == 0) // Back to the main menu
                {
                    showAutomataTypeMenu();
                }
            }
	}
        /*
         * Color selection menu
         */
        else if (displayedMenu == SELECT_COLORS)
        {
  		    if(keys_released & KEY_A)
  		    {
  		        if (intArrow == 12) // Back to main menu
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
		    else if(keys_pressed & KEY_UP)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
		        if (intArrow == 0)
		        {
		            intArrow = 12;
		        }
		        else
		        {
		            intArrow = intArrow - 1;
		        }
		        
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
		        if (intArrow == 12)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
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
		        else if (intArrow == 3)
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
                else if (intArrow == 6)
                {
                    if (FG_R2 != 0)
                    {
                        --FG_R2;
                    }
                }
                else if (intArrow == 7)
                {
                    if (FG_G2 != 0)
                    {
                        --FG_G2;
                    }
                }
                else if (intArrow == 8)
                {
                    if (FG_B2 != 0)
                    {
                        --FG_B2;
                    }
                }	   
		        else if (intArrow == 9)
		        {
		            if (line_R != 0)
		            {
		                --line_R;
		            }
		        }
		        else if (intArrow == 10)
		        {
		            if (line_G != 0)
		            {
		                --line_G;
		            }
		        }		      
		        else if (intArrow == 11)
		        {
		            if (line_B != 0)
		            {
		                --line_B;
		            }
		        }		      		           		            

                printMenu(displayedMenu);		
                		        
		        BG_color = RGB15(BG_R, BG_G, BG_B);
		        FG_color = RGB15(FG_R, FG_G, FG_B);
                FG_color2 = RGB15(FG_R2, FG_G2, FG_B2);
                line_color = RGB15(line_R, line_G, line_B);		        
                
                ca.initialize();
                
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
		        else if (intArrow == 3)
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
                    if (FG_R2 != 31)
                    {
                        ++FG_R2;
                    }
                }
                else if (intArrow == 7)
                {
                    if (FG_G2 != 31)
                    {
                        ++FG_G2;
                    }
                }
                else if (intArrow == 8)
                {
                    if (FG_B2 != 31)
                    {
                        ++FG_B2;
                    }
                }	   
		        else if (intArrow == 9)
		        {
		            if (line_R != 31)
		            {
		                ++line_R;
		            }
		        }
		        else if (intArrow == 10)
		        {
		            if (line_G != 31)
		            {
		                ++line_G;
		            }
		        }		      
		        else if (intArrow == 11)
		        {
		            if (line_B != 31)
		            {
		                ++line_B;
		            }
		        }	
		        
                printMenu(displayedMenu);
                		        
		        BG_color = RGB15(BG_R, BG_G, BG_B);
		        FG_color = RGB15(FG_R, FG_G, FG_B);
                FG_color2 = RGB15(FG_R2, FG_G2, FG_B2);
                line_color = RGB15(line_R, line_G, line_B);
                	      		           		            
                ca.initialize();
                
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
      		    // Change language to english
  		        if (intArrow == 0)
  		        {
  		            setLanguage(EN, intArrow);
  		        }
  		        // Change language to spanish
  		        else if (intArrow == 1)
  		        {
  		            setLanguage(ES, intArrow);
  		        }
  		        // Change language to basque
  		        else if (intArrow == 2)
  		        {
  		            setLanguage(EU, intArrow);
  		        }
  		        // Back to main menu
  		        else if (intArrow == 3)
  		        {
  		            showAutomataTypeMenu();
  		        }
  		    }
		    else if(keys_pressed & KEY_UP)
		    {
    		    // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
		        if (intArrow == 0)
		        {
		            intArrow = 3;
		        }
		        else
		        {
		            intArrow = intArrow - 1;
		        }
		        
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
		    else if(keys_pressed & KEY_DOWN)
		    {
		        // Delete the previous arrow
		        printMenuArrow(displayedMenu, intArrow, true);
		        
		        if (intArrow == 3)
		        {
		            intArrow = 0;
		        }
		        else
		        {
		            intArrow = intArrow + 1;
		        }
		        
		        // Print the new arrow
		        printMenuArrow(displayedMenu, intArrow, false);
		    }
        }
    }
    
	return 0;
}
/***************************** END MAIN FUNCTION ****************************/
