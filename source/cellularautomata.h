#ifndef CELLULARAUTOMATA_H
#define CELLULARAUTOMATA_H

#include <string>

using namespace std;

class CellularAutomata
{
    private:
        string type;
        
        unsigned int numSteps;
        
        // For the Langton's ants (normal and hexagonal)
        unsigned short antPosX;
        unsigned short antPosY;
        unsigned short antAngle;
        unsigned short antNumPixels; // Only used by the normal ant not by the hexagonal one
        bool antFinished;    

        int rotateAnt(unsigned char rotateTo);
        int paintAnt();
        int forwardAnt();

        int drawMunchingSquare(int column, int row, int width, unsigned short color);

    public:
        int setType(string t);
        const char* getType();
        
        int getNumSteps();
        
        int setAntNumPixels(int numPixels);
        int getAntNumPixels();
        
        bool hasFinished();
        int initialize();
        int nextStep();
};
#endif
