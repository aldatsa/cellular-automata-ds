#ifndef GLOBALS_H
#define GLOBALS_H

/*
 * Constants to define the type of automata:
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
