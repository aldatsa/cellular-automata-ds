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
#define BML_TRAFFIC_MODEL 9
#define SELECT_COLORS 10
#define SELECT_LANGUAGE 11

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

// Automata selection menu
#define AUTOMATA_TYPE_MENU_BML_COL 2
#define AUTOMATA_TYPE_MENU_BML_ARROW_COL 0

// Elementary Cellular Automata menu
#define ECA_MENU_INITIAL_STATE_COLUMN 2
#define ECA_MENU_INITIAL_STATE_ONE_CELL_COLUMN 5
#define ECA_MENU_INITIAL_STATE_ONE_CELL_ASTERISK_COLUMN 4
#define ECA_MENU_INITIAL_STATE_ONE_CELL_ARROW_COLUMN 2
#define ECA_MENU_INITIAL_STATE_RANDOM_COLUMN 5
#define ECA_MENU_INITIAL_STATE_RANDOM_ASTERISK_COLUMN 4
#define ECA_MENU_INITIAL_STATE_RANDOM_ARROW_COLUMN 2
#define ECA_MENU_BACK_TO_MAIN_COLUMN 2

// Munching squares
#define MS_MENU_BACK_TO_MAIN_COLUMN 2

// BML traffic model
#define BML_MENU_RESTART_COL 2
#define BML_MENU_RESTART_ARROW_COL 0
#define BML_MENU_DENSITY_COL 2
#define BML_MENU_DENSITY_ARROW_COL 0
#define BML_MENU_BACK_TO_MAIN_COL 2
#define BML_MENU_BACK_TO_MAIN_ARROW_COL 0

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
#define AUTOMATA_TYPE_MENU_SELECT_COLORS_ROW 17
#define AUTOMATA_TYPE_MENU_SELECT_LANGUAGE_ROW 18

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
#define CGL_MENU_BACK_TO_MAIN_ROW 19

// Munching squares
#define MS_MENU_BACK_TO_MAIN_ROW 13 

// BML traffic model
#define BML_MENU_RESTART_ROW 13
#define BML_MENU_DENSITY_ROW 14
#define BML_MENU_BACK_TO_MAIN_ROW 15

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
