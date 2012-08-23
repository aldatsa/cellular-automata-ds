#ifndef MENUS_H
#define MENUS_H

int printString(int row, int column, std::string text);
int printRuleNumber(int intRuleNumber);
int printPopulation();
int printAntNumPixels();
int printBMLdensity();
int printCredits();
int printArrow(int row, int column);
int deleteArrow(int row, int column);
int printAsterisk(int row, int column);
int deleteAsterisk(int row, int column);
int printPartialColor(int row, int col, std::string str, int num);
int printMenu(int intDisplayedMenu);
int printMenuAsterisks(int intDisplayedMenu);
int printMenuArrow(int intDisplayedMenu, int index, bool boolDelete);
int showAutomataTypeMenu();
int printAutomataType(int automataType);
int printNumSteps();
int setLanguage(int language);

#endif
