#ifndef CELLULARAUTOMATA_H
#define CELLULARAUTOMATA_H

#include <string>

using namespace std;

class CellularAutomata
{
    private:
        string type;
    
    public:
        int setType(string t);
        const char* getType();
};
#endif
