#ifndef HEXGRID_H
#define HEXGRID_H

int hexGridLineOne(int y);
int hexGridLineTwo(int y);
int hexGridLineThree(int y);

int drawHexGrid();

int paintHexCell(int intPosX, int intPosY, unsigned short color, unsigned short* framebuffer);

#endif
