#ifndef GENERAL_H
#define GENERAL_H

extern unsigned short* fb;
extern unsigned short* fb2;

int cleanFB(unsigned short* framebuffer);
int drawHLine(int column, int row, int lenght, unsigned short color, unsigned short* framebuffer);

#endif
