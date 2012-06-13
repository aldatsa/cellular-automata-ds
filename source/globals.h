#ifndef GLOBALS_H
#define GLOBALS_H


/*****************
 *   CONSTANTS   *
 *****************/

/*
 * Constants to define the type of automata and the corresponding menu:
 * Used in the variables: automataType and displayedMenu
 * 0: Main menu
 * 1: Elementary Cellular Automata
 * 2: Langton's ant
 * 3: Langton's hexagonal ant
 * 4: Boolean Square Automata
 * 5: Boolean Hexagonal Automata
 * 6: Boolean Triangular Automata
 * 7: Conway's game of life
 * 8: Munching Squares
 * 9: Select colors
 * 10: Select language
 */
#define MAIN_MENU 0
#define ELEMENTARY_CELLULAR_AUTOMATA 1
#define LANGTON_ANT 2
#define LANGTON_HEXAGONAL_ANT 3
#define BOOLEAN_AUTOMATA 4
#define BOOLEAN_HEXAGONAL_AUTOMATA 5
#define BOOLEAN_TRIANGULAR_AUTOMATA 6
#define CONWAYS_GAME_OF_LIFE 7
#define MUNCHING_SQUARES 8
#define SELECT_COLORS 9
#define SELECT_LANGUAGE 10

/*
 * Constants to define the type of neighborhood
 */
#define VON_NEUMANN_NEIGHBORHOOD 0
#define MOORE_NEIGHBORHOOD 1

/*
 * Constants to define the language of the text of the user interfaze
 */
#define EN 0
#define ES 1
#define EU 2

/*
 * Constants to define the different initial states of
 * the Conway's game of life
 */
#define ACORN 0
#define FPENTOMINO 1
#define FILL_SCREEN_WITH_PULSARS 2
#define FILL_SCREEN_WITH_PENTADECATHLONS 3

/*
 * Constants to define the different initial states of
 * the Elementary Cellular Automata
 */
#define ECA_INITIALIZE_ONE_CELL 0
#define ECA_INITIALIZE_RANDOM 1

/*
 * Constants to define the column where each thing is printed
 */

// General
#define POPULATION_COLUMN 0
#define NUM_STEPS_COLUMN 0
#define RULE_NUMBER_COLUMN 0

// Munching squares
#define MS_MENU_BACK_TO_MAIN_COLUMN 2

/*
 * Constants to define the row where each thing is printed
 */

// General
#define POPULATION_ROW 10
#define NUM_STEPS_ROW 9
#define RULE_NUMBER_ROW 9

// Automata selection menu
#define AUTOMATA_TYPE_MENU_ECA_ROW 8
#define AUTOMATA_TYPE_MENU_LA_ROW 9
#define AUTOMATA_TYPE_MENU_LHA_ROW 10
#define AUTOMATA_TYPE_MENU_BA_ROW 11
#define AUTOMATA_TYPE_MENU_BHA_ROW 12
#define AUTOMATA_TYPE_MENU_BTA_ROW 13
#define AUTOMATA_TYPE_MENU_CGL_ROW 14
#define AUTOMATA_TYPE_MENU_MS_ROW 15
#define AUTOMATA_TYPE_MENU_SELECT_COLORS_ROW 16
#define AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_ROW 17

// Elementary Cellular Automata menu
#define ECA_MENU_BACK_TO_MAIN_ROW 12

// Langton's ant menu
#define LA_MENU_ANT_SIZE_ROW 12
#define LA_MENU_BACK_TO_MAIN_ROW 14

// Langton's hexagonal ant menu
#define LHA_MENU_BACK_TO_MAIN_ROW 14

// Boolean automata menu
#define BA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW 13
#define BA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW 14
#define BA_MENU_MOORE_NEIGHBORHOOD_ROW 15
#define BA_MENU_MOORE_NEIGHBORHOOD_1234_ROW 16
#define BA_MENU_MOORE_NEIGHBORHOOD_5678_ROW 17
#define BA_MENU_BACK_TO_MAIN_ROW 19

// Boolean hexagonal automata menu
#define BHA_MENU_NEIGHBORHOOD_ROW 13
#define BHA_MENU_NEIGHBORHOOD_123_ROW 14
#define BHA_MENU_NEIGHBORHOOD_456_ROW 15
#define BHA_MENU_BACK_TO_MAIN_ROW 17

// Boolean triangular automata menu
#define BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_ROW 13
#define BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_1234_ROW 14
#define BTA_MENU_MOORE_NEIGHBORHOOD_ROW 15
#define BTA_MENU_MOORE_NEIGHBORHOOD_1234_ROW 16
#define BTA_MENU_MOORE_NEIGHBORHOOD_5678_ROW 17
#define BTA_MENU_BACK_TO_MAIN_ROW 19

// Conway's game of life
#define CGL_MENU_INITIAL_STATE_ROW 13
#define CGL_MENU_ACORN_ROW 14
#define CGL_MENU_FPENTOMINO_ROW 15
#define CGL_MENU_FILL_SCREEN_WITH_PULSARS_ROW 16
#define CGL_MENU_FILL_SCREEN_WITH_PENTADECATHLONS_ROW 17
#define CGL_MENU_BACK_TO_MAIN_ROW 19

// Munching squares
#define MS_MENU_BACK_TO_MAIN_ROW 13 

/************************
 *   GLOBAL VARIABLES   *
 ************************/
 
/*
 * This variable sets the type of automata that is going to be executed
 * (Uses the constants defined above)
 */
extern int automataType;

/*
 * This variable sets the menu that is going to be displayed
 * (Uses the constants defined above)
 */
extern int displayedMenu;

/* The position of the arrow
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
extern int intArrow;

#endif
