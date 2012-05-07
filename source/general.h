#ifndef GENERAL_H
#define GENERAL_H

extern unsigned short* fb;
extern unsigned short* fb2;

int cleanFB(unsigned short* framebuffer);
int showFB();
int showFB2();
int showFlash();

int drawHLine(int column, int row, int lenght, unsigned short color, unsigned short* framebuffer);
int drawVLine(int column, int row, int lenght, unsigned short color);
int drawRectangle(bool fill, int intRowStart, int intColumnStart, int length, int width);
#endif
