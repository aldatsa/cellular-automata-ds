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
 * Constants to define the different initial states of the Conway's game of life
 */
#define ACORN 0
#define FPENTOMINO 1
#define FILL_SCREEN_WITH_PULSARS 2
#define FILL_SCREEN_WITH_PENTADECATHLONS 3

/*
 * Constants to define the row where each thing is printed
 */
#define POPULATION_ROW 10 // Used in the following menus: Elementary Cellular Automata, Langton's ant, Langton's hexagonal ant
#define ECA_MENU_BACK_TO_MAIN_ROW 12
#define LA_MENU_ANT_SIZE_ROW 12
#define LA_MENU_BACK_TO_MAIN_ROW 14
#define LHA_MENU_BACK_TO_MAIN_ROW 14

/************************
 *   GLOBAL VARIABLES   *
 ************************/
 
/*
 * This variable sets the type of automata that is going to be executed (Uses the constants defined above)
 */
extern int automataType;

/*
 * This variable sets the menu that is going to be displayed (Uses the constants defined above)
 */
extern int displayedMenu;

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
extern int intArrow;

#endif
