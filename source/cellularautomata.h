#ifndef CELLULARAUTOMATA_H
#define CELLULARAUTOMATA_H

class CellularAutomata
{
    private:
        int type;
        
        unsigned int numSteps;
        unsigned int population;

        int paintECAcell(int row, int col, unsigned short color);
        
        // For the Elementary Cellular Automata
        int ECA_initial_state;
        unsigned short ruleLeft[8];
        unsigned short ruleCenter[8];
        unsigned short ruleRight[8];
        unsigned short ruleDown[8];
        int updateECAruleColors();
        int paintInitialState();
        int drawAllRules();

        // For the Langton's ants (normal and hexagonal)
        unsigned short antPosX;
        unsigned short antPosY;
        unsigned short antAngle;
        unsigned short antNumPixels; // Only used by the normal ant not by the hexagonal one
        bool antFinished;    

        int booleanRuleValues[2]; // 0: Von Neumann neighborhood, 1: Moore neighborhood
        int typeOfNeighborhood;

        int rotateAnt(unsigned char rotateTo);
        int paintAnt();
        int forwardAnt();

        int initialState;
        int drawInitialState();

        int drawMunchingSquare(int column, int row, int width, unsigned short color);

    public:
        int setType(int t);
        int getType();
        
        unsigned int getNumSteps();

        unsigned int getPopulation();
        
        int drawElementaryCellularAutomata();
        int getRuleNumber();
        int drawRule(int nth);
        int drawArrow(char nth, unsigned short color);
        unsigned short getRuleDown(int index);
        int setRuleDown(int index, unsigned short color);
        int resetECA();

        int setAntNumPixels(int numPixels);
        int getAntNumPixels();
        
        bool hasFinished();
        int initialize();
        int nextStep();
        
        int initializeBooleanRuleValues();
        bool checkBooleanRuleValue(int neighborhoodType, int ruleIndex);
        int toggleBooleanRuleValue(int neighborhoodType, int ruleIndex);
        int setTypeOfNeighborhood(int value);
        int getTypeOfNeighborhood();

        int getInitialState();
        int setInitialState(int state);
};
#endif
