#include <nds.h>

unsigned short BG_R = 31;
unsigned short BG_G = 31;
unsigned short BG_B = 31;
unsigned short BG_color = RGB15(BG_R, BG_G, BG_B); // Color used on the BackGround (Default: White)

unsigned short FG_R = 0;
unsigned short FG_G = 0;
unsigned short FG_B = 0; 
unsigned short FG_color = RGB15(FG_R, FG_G, FG_B); // Color used on the ForeGround (Default: Black)

unsigned short line_R = 15;
unsigned short line_G = 15;
unsigned short line_B = 15; 
unsigned short line_color = RGB15(line_R, line_G, line_B); // Color used to draw the lines (Default: Grey)
