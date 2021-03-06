#ifndef GLOBALS_H
#define GLOBALS_H

#include "cellularautomata.h"

/*****************
 *   CONSTANTS   *
 *****************/

/*
 * Constants to define the type of automata and the corresponding menu:
 * Used in the variables: automataType and displayedMenu
 *  0: Initial language selection menu
 *  1: Main menu
 *  2: Elementary Cellular Automata
 *  3: Langton's ant
 *  4: Langton's hexagonal ant
 *  5: Boolean Square Automata
 *  6: Boolean Hexagonal Automata
 *  7: Boolean Triangular Automata
 *  8: Conway's game of life
 *  9: Munching Squares
 * 10: Biham-Middleton-Levine traffic model
 * 11: Cyclic Cellular Automata
 * 12: Stepping stone
 * 13: Select colors
 * 14: Select language
 */
#define INITIAL_LANGUAGE_MENU 0
#define MAIN_MENU 1
#define ELEMENTARY_CELLULAR_AUTOMATA 2
#define LANGTON_ANT 3
#define LANGTON_HEXAGONAL_ANT 4
#define BOOLEAN_AUTOMATA 5
#define BOOLEAN_HEXAGONAL_AUTOMATA 6
#define BOOLEAN_TRIANGULAR_AUTOMATA 7
#define CONWAYS_GAME_OF_LIFE 8
#define MUNCHING_SQUARES 9
#define BML_TRAFFIC_MODEL 10
#define CYCLIC_CELLULAR_AUTOMATA 11
#define STEPPING_STONE 12
#define SELECT_COLORS 13
#define SELECT_LANGUAGE 14

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
#define RIP_JOHN_CONWAY 4
/*
 * Constants to define the different initial states of
 * the Elementary Cellular Automata
 */
#define ECA_INITIALIZE_ONE_CELL 0
#define ECA_INITIALIZE_RANDOM 1

/*
 * Constants to define the initial values of the Langton's ant
 */
#define LA_INITIAL_X 127
#define LA_INITIAL_Y 95
#define LA_INITIAL_ANGLE 90
#define LA_INITIAL_NUM_PIXELS 4

/*
 * Constant to define the initial value of the density
 * of the Biham-Middleton-Levine traffic model
 */
#define BML_INITIAL_DENSITY 40

/*
 * Constant to define the initial number of states
 * of the cyclic cellular automata.
 */
#define CCA_INITIAL_NUM_STATES 3

/*
 * Constants to define the column where each thing is printed
 */

// General
#define POPULATION_COLUMN 0
#define NUM_STEPS_COLUMN 0
#define RULE_NUMBER_COLUMN 0

// Initial language selection menu
#define INITIAL_LANGUAGE_MENU_EN_COL 10
#define INITIAL_LANGUAGE_MENU_ES_COL 10
#define INITIAL_LANGUAGE_MENU_EU_COL 10
#define INITIAL_LANGUAGE_MENU_ARROW_COL 8

// Automata selection menu
#define AUTOMATA_TYPE_MENU_ECA_COL 2
#define AUTOMATA_TYPE_MENU_LA_COL 2
#define AUTOMATA_TYPE_MENU_LHA_COL 2
#define AUTOMATA_TYPE_MENU_BA_COL 2
#define AUTOMATA_TYPE_MENU_BHA_COL 2
#define AUTOMATA_TYPE_MENU_BTA_COL 2
#define AUTOMATA_TYPE_MENU_CGL_COL 2
#define AUTOMATA_TYPE_MENU_MS_COL 2
#define AUTOMATA_TYPE_MENU_BML_COL 2
#define AUTOMATA_TYPE_MENU_BML_ARROW_COL 0
#define AUTOMATA_TYPE_MENU_CCA_COL 2
#define AUTOMATA_TYPE_MENU_CCA_ARROW_COL 0
#define AUTOMATA_TYPE_MENU_ST_COL 2
#define AUTOMATA_TYPE_MENU_ST_ARROW_COL 0
#define AUTOMATA_TYPE_MENU_SELECT_COLORS_COL 2
#define AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_COL 2

// Elementary Cellular Automata menu
#define ECA_MENU_INITIAL_STATE_COL 2
#define ECA_MENU_INITIAL_STATE_ONE_CELL_COL 5
#define ECA_MENU_INITIAL_STATE_ONE_CELL_ASTERISK_COL 4
#define ECA_MENU_INITIAL_STATE_ONE_CELL_ARROW_COL 2
#define ECA_MENU_INITIAL_STATE_RANDOM_COL 5
#define ECA_MENU_INITIAL_STATE_RANDOM_ASTERISK_COL 4
#define ECA_MENU_INITIAL_STATE_RANDOM_ARROW_COL 2
#define ECA_MENU_BACK_TO_MAIN_COL 2

// Langton's ant menu
#define LA_MENU_ANT_SIZE_COL 2
#define LA_MENU_BACK_TO_MAIN_COL 2

// Langton's hexagonal ant menu
#define LHA_MENU_BACK_TO_MAIN_COL 2

// Munching squares
#define MS_MENU_BACK_TO_MAIN_COLUMN 2

// BML traffic model
#define BML_MENU_RESTART_COL 2
#define BML_MENU_RESTART_ARROW_COL 0
#define BML_MENU_DENSITY_COL 2
#define BML_MENU_DENSITY_ARROW_COL 0
#define BML_MENU_BACK_TO_MAIN_COL 2
#define BML_MENU_BACK_TO_MAIN_ARROW_COL 0

// Cyclic Cellular Automata
#define CCA_MENU_NUM_STATES_COL 2
#define CCA_MENU_NUM_STATES_ARROW_COL 0
#define CCA_MENU_BACK_TO_MAIN_COL 2
#define CCA_MENU_BACK_TO_MAIN_ARROW_COL 0

// Stepping stone
#define ST_MENU_BACK_TO_MAIN_COL 2
#define ST_MENU_BACK_TO_MAIN_ARROW_COL 0

// Color selection
#define CS_MENU_MAIN_COL 2
#define CS_MENU_PARTIAL_COLOR_COL 5

/*
 * Constants to define the row where each thing is printed
 */

// General
#define POPULATION_ROW 10
#define NUM_STEPS_ROW 9
#define RULE_NUMBER_ROW 9

// Initial language selection menu
#define INITIAL_LANGUAGE_MENU_EN_ROW 13
#define INITIAL_LANGUAGE_MENU_EN_ARROW_ROW 13
#define INITIAL_LANGUAGE_MENU_ES_ROW 14
#define INITIAL_LANGUAGE_MENU_ES_ARROW_ROW 14
#define INITIAL_LANGUAGE_MENU_EU_ROW 15
#define INITIAL_LANGUAGE_MENU_EU_ARROW_ROW 15

// Automata selection menu
#define AUTOMATA_TYPE_MENU_ECA_ROW 8
#define AUTOMATA_TYPE_MENU_LA_ROW 9
#define AUTOMATA_TYPE_MENU_LHA_ROW 10
#define AUTOMATA_TYPE_MENU_BA_ROW 11
#define AUTOMATA_TYPE_MENU_BHA_ROW 12
#define AUTOMATA_TYPE_MENU_BTA_ROW 13
#define AUTOMATA_TYPE_MENU_CGL_ROW 14
#define AUTOMATA_TYPE_MENU_MS_ROW 15
#define AUTOMATA_TYPE_MENU_BML_ROW 16
#define AUTOMATA_TYPE_MENU_CCA_ROW 17
#define AUTOMATA_TYPE_MENU_ST_ROW 18
#define AUTOMATA_TYPE_MENU_SELECT_COLORS_ROW 19
#define AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_ROW 20

// Elementary Cellular Automata menu
#define ECA_MENU_INITIAL_STATE_ROW 12
#define ECA_MENU_INITIAL_STATE_ONE_CELL_ROW 13
#define ECA_MENU_INITIAL_STATE_RANDOM_ROW 14
#define ECA_MENU_BACK_TO_MAIN_ROW 16

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
#define BTA_MENU_VON_NEUMANN_NEIGHBORHOOD_123_ROW 14
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
#define CGL_MENU_RIP_JOHN_CONWAY_ROW 18
#define CGL_MENU_BACK_TO_MAIN_ROW 20

// Munching squares
#define MS_MENU_BACK_TO_MAIN_ROW 13

// BML traffic model
#define BML_MENU_RESTART_ROW 13
#define BML_MENU_DENSITY_ROW 14
#define BML_MENU_BACK_TO_MAIN_ROW 15

// Cyclic Cellular Automata
#define CCA_MENU_NUM_STATES_ROW  13
#define CCA_MENU_BACK_TO_MAIN_ROW 14

// Stepping stone
#define ST_MENU_BACK_TO_MAIN_ROW 13

// Color selection
#define CS_MENU_BG_COLOR_ROW 2
#define CS_MENU_BG_COLOR_R_ROW 3
#define CS_MENU_BG_COLOR_G_ROW 4
#define CS_MENU_BG_COLOR_B_ROW 5
#define CS_MENU_FG_COLOR_ROW 7
#define CS_MENU_FG_COLOR_R_ROW 8
#define CS_MENU_FG_COLOR_G_ROW 9
#define CS_MENU_FG_COLOR_B_ROW 10
#define CS_MENU_FG2_COLOR_ROW 12
#define CS_MENU_FG2_COLOR_R_ROW 13
#define CS_MENU_FG2_COLOR_G_ROW 14
#define CS_MENU_FG2_COLOR_B_ROW 15
#define CS_MENU_LINE_COLOR_ROW 17
#define CS_MENU_LINE_COLOR_R_ROW 18
#define CS_MENU_LINE_COLOR_G_ROW 19
#define CS_MENU_LINE_COLOR_B_ROW 20
#define CS_MENU_BACK_TO_MAIN_ROW 22

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

extern CellularAutomata ca;

extern int displayedLanguage;

#endif
