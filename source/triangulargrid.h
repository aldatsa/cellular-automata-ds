#ifndef TRIANGULARGRID_H
#define TRIANGULARGRID_H

int triangularGridLineOne(int y);
int triangularGridLineTwo(int y);
int triangularGridLineThree(int y);

int drawTriangularGrid();
int paintTriangularCell(int intPosX, int intPosY, unsigned short color, unsigned short* framebuffer);

#endif
