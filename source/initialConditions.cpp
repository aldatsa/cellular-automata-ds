#include "framebuffer.h"
#include "color.h"

/*
 * Functions used to draw initial conditions for the Conway's game of life
 */


/*
 * Draws the initial state that develops into the period 3 oscillator
 * called "pulsar".
 * The "pulsar" is the most common period 3 oscillator.
 *
 * The initial state is like that:
 *
 *          *****
 *                  (empty line)
 *           ***
 *                  (empty line)
 *          *****
 *
 * http://www.conwaylife.com/wiki/Pulsar
 */
int drawPulsar(int intX, int intY)
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
            drawPulsar(i, j);
        }
    }

    return 0;
}

/*
 * Draws the F-pentomino (also known as R-pentomino).
 * During this early research, Conway discovered that the F-pentomino
 * (which he called the "R-pentomino") failed to stabilize in a small
 * number of generations.
 * In fact, it takes 1103 generations to stabilize, by which time it has
 * a population of 116 and has fired six escaping gliders
 * (these were the first gliders ever discovered).
 *
 * The initial state is like that:
 *
 *          **
 *         **
 *          *
 *
 * http://www.conwaylife.com/wiki/F-pentomino
 */
int drawFpentomino(int intX, int intY)
{
    drawHLine(intX, intY, 2, FG_color, fb);
    drawHLine(intX - 1, intY + 1, 2, FG_color, fb);
    drawHLine(intX, intY + 2, 1, FG_color, fb);

    return 0;
}

/*
 * Draws the Acorn
 * It takes 5206 generations to generate 633 cells including 13 escaped gliders
 *
 * The initial state is like that:
 *
 *           *
 *             *
 *          **  ***
 *
 * http://www.conwaylife.com/wiki/Acorn
 */
int drawAcorn(int intX, int intY)
{
    drawHLine(intX, intY, 1, FG_color, fb);
    drawHLine(intX + 2, intY + 1, 1, FG_color, fb);
    drawHLine(intX - 1, intY + 2, 2, FG_color, fb);
    drawHLine(intX + 3, intY + 2, 3, FG_color, fb);

    return 0;
}

/*
 * Draws the initial state that develops into the period 15 oscillator
 * called "pentadecathlon".
 * The "pentadecathlon" is the most natural oscillator of period greater
 * than 3.
 * It is the only known oscillator that is a polyomino in more than one phase
 * (besides the blinker).
 *
 * The initial state is like that:
 *
 *          *    *
 *        ** **** **
 *          *    *
 *
 * But I'm going to use 10 cells placed in a row because it's more simple to
 * draw and evolves into the pentadecathlon.
 *
 *          **********
 *
 * http://www.conwaylife.com/wiki/Pulsar
 */
int drawPentadecathlon(int intX, int intY)
{
    drawHLine(intX, intY, 10, FG_color, fb);

    return 0;
}

/*
 * Fills the screen with multiple rows and columns of "pentadecathlons".
 */
int fillScreenWithPentadecathlons()
{
    for (int i = 11; i < 245; i = i + 17)
    {
        for (int j = 5; j < 191; j = j + 10)
        {
            drawPentadecathlon(i, j);
        }
    }
    return 0;
}

/*
 * Draws the initial conditions of the XKCD memorial comic 2293: RIP John Conway
 * John Conway passed away of COVID-19 on April 11, 2020.
 *
 * Two days later, Randall created this memorial comic:
 * https://xkcd.com/2293/
 *
 * This comic begins with the shape of a stick figure as the starting cell
 * configuration of the Game of Life. The black cells are "alive" and the white
 * cells are "dead". This configuration then evolves via Conway's rules,
 * disintegrating into nothingness except for a five-cell pattern known as a
 * "glider", which ascends up and to the right. This visually suggests a "soul"
 * breaking away from the disintegrating corporeal body. The glider is perhaps
 * the most iconic pattern of the Game of Life, and is often used symbolically
 * to represent the phenomenon of emergence.
 *
 * https://www.explainxkcd.com/wiki/index.php/2293:_RIP_John_Conway
 *
 * The initial state is like this:
 *
 *           ***
 *           * *
 *           * *
 *            *
 *         * ***
 *          * * *
 *            *  *
 *           * *
 *           * *
 *
 */
int drawRIPJohnConway(int intX, int intY)
{
   drawHLine(intX, intY, 3, FG_color, fb);
   drawHLine(intX, intY + 1, 1, FG_color, fb);
   drawHLine(intX + 2, intY + 1, 1, FG_color, fb);
   drawHLine(intX, intY + 2, 1, FG_color, fb);
   drawHLine(intX + 2, intY + 2, 1, FG_color, fb);
   drawHLine(intX + 1, intY + 3, 1, FG_color, fb);
   drawHLine(intX - 2, intY + 4, 1, FG_color, fb);
   drawHLine(intX, intY + 4, 3, FG_color, fb);
   drawHLine(intX - 1, intY + 5, 1, FG_color, fb);
   drawHLine(intX + 1, intY + 5, 1, FG_color, fb);
   drawHLine(intX + 3, intY + 5, 1, FG_color, fb);
   drawHLine(intX + 1, intY + 6, 1, FG_color, fb);
   drawHLine(intX + 4, intY + 6, 1, FG_color, fb);
   drawHLine(intX, intY + 7, 1, FG_color, fb);
   drawHLine(intX + 2, intY + 7, 1, FG_color, fb);
   drawHLine(intX, intY + 8, 1, FG_color, fb);
   drawHLine(intX + 2, intY + 8, 1, FG_color, fb);

    return 0;
}
