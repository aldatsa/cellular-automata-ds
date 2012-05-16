#ifndef I18N_H
#define I18N_H

#include <string>
#include "globals.h"

/*
 * Constant for the version number of the application
 */
const std::string strVersionNumber = "0.3.3.4";

/* This variable sets the language used in the menus
 * using constants defined in globals.h
 * EN: English
 * ES: Spanish
 * EU: Basque
 */
int displayedLanguage = EN;

/*
 * The strings of text used in the app in the default language (english).
 *
 */
std::string stringRuleNumber = "Rule number";
std::string stringElementaryCellularAutomata = "Elementary cellular automata"; // max 30 characters
std::string stringLangtonsAnt = "Langton's ant"; // max 30 characters
std::string stringLangtonsHexagonalAnt = "Langton's hexagonal ant";
std::string stringBooleanAutomata = "Boolean automata";
std::string stringBooleanHexagonalAutomata = "Boolean hexagonal automata";
std::string stringBooleanTriangularAutomata = "Boolean triangular automata";
std::string stringConwaysGameOfLife = "Conway's game of life";
std::string stringMunchingSquares = "Munching Squares";
std::string stringSelectColors = "Select colors";
std::string stringBackToMainMenu = "Back to main menu";
std::string stringAntsPixels = "Ant's size (pixels)";
std::string stringVonNeumannNeighborhood = "Von Neumann neighborhood";
std::string stringMooreNeighborhood = "Moore neighborhood";
std::string stringNeighborhood = "Neighborhood";
std::string stringTypeOfComparation = "Type of comparation";
std::string stringSmallerThan = "Smaller than";
std::string stringEqualTo = "Equal to";
std::string stringTypeOfBooleanOperator = "Type of boolean operator";
std::string stringThreshold = "Threshold";
std::string stringBackgroundColor = "Background color";
std::string stringForegroundColor = "Foreground color";
std::string stringLineColor = "Line color";
std::string stringRed = "Red";
std::string stringGreen = "Green";
std::string stringBlue = "Blue";
std::string stringSteps = "Steps";
std::string stringAutomataType = "Automata type";
std::string stringVersion = "version " + strVersionNumber;
std::string stringSelectLanguage = "Select language";
std::string stringEnglish = "English";
std::string stringEspanol = "Espanol";
std::string stringEuskara = "Euskara";

/*
 * Changes the language of the strings of text to the selected language
 * languageCode is the two letters code for each language
 * en : english
 * es : spanish
 * eu : basque
 */
int changeTextLanguage(int language)
{
    if (language == EN) // English
    {
        stringRuleNumber = "Rule number";
        
        stringElementaryCellularAutomata = "Elementary cellular automata"; // max 30 characters
        stringLangtonsAnt = "Langton's ant"; // max 30 characters
        stringLangtonsHexagonalAnt = "Langton's hexagonal ant"; // max 30 characters
        stringBooleanAutomata = "Boolean automata"; // max 30 characters
        stringBooleanHexagonalAutomata = "Boolean hexagonal automata"; // max 30 characters
        stringBooleanTriangularAutomata = "Boolean triangular automata"; // max 30 characters
        stringConwaysGameOfLife = "Conway's game of life";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringSelectColors = "Select colors"; // max 30 characters
        stringBackToMainMenu = "Back to main menu"; // max 30 characters
        
        stringAntsPixels = "Ant's size (pixels)"; // Max 22 characters
        
        stringVonNeumannNeighborhood = "Von Neumann neighborhood";
        stringMooreNeighborhood = "Moore neighborhood";
        stringNeighborhood = "Neighborhood";
        
        stringBackgroundColor = "Background color";
        stringForegroundColor = "Foreground color";
        stringLineColor = "Line color";
        stringRed = "Red";
        stringGreen = "Green";
        stringBlue = "Blue";
        
        stringSteps = "Steps";
        
        stringAutomataType = "Automata type";
        
        stringVersion = "version " + strVersionNumber;
        
        stringSelectLanguage = "Select language";
    }
    else if (language == ES) // Spanish
    {
        stringRuleNumber = "Regla numero"; // It doesn't print 'ú'. Why?
        
        stringElementaryCellularAutomata = "Automata celular elemental"; // max 30 characters
        stringLangtonsAnt = "Hormiga de Langton"; // max 30 characters
        stringLangtonsHexagonalAnt = "Hormiga de Langton hexagonal"; // max 30 characters
        stringBooleanAutomata = "Automata booleano"; // max 30 characters
        stringBooleanHexagonalAutomata = "Automata booleano hexagonal"; // max 30 characters
        stringBooleanTriangularAutomata = "Automata booleano triangular"; // max 30 characters
        stringConwaysGameOfLife = "Juego de la vida de Conway";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringSelectColors = "Seleccionar colores"; // max 30 characters        
        stringBackToMainMenu = "Volver al menu principal"; // max 30 characters

        stringAntsPixels = "Tamano (pixeles)"; // It doesn't print 'ñ'. Why? And "Tamano de la hormiga (pixeles)" is too long. Max 22 characters
        
        stringVonNeumannNeighborhood = "Vecindad de Von Neumann";
        stringMooreNeighborhood = "Vecindad de Moore";
        stringNeighborhood = "Vecindad";
                
        stringBackgroundColor = "Color de fondo";
        stringForegroundColor = "Color de primer plano";
        stringLineColor = "Color de lineas";
        stringRed = "Rojo";
        stringGreen = "Verde";
        stringBlue = "Azul";
        
        stringSteps = "Pasos";
        
        stringAutomataType = "Tipo de automata";
        
        stringVersion = "version " + strVersionNumber;
        
        stringSelectLanguage = "Seleccionar lenguaje";
    }
    else if (language == EU) // Basque
    {
        stringRuleNumber = "Arau zenbakia";
        
        stringElementaryCellularAutomata = "Oinarrizko automata zelularra"; // max 30 characters
        stringLangtonsAnt = "Langton-en inurria"; // max 30 characters
        stringLangtonsHexagonalAnt = "Langton-en inurri hexagonala"; // max 30 characters
        stringBooleanAutomata = "Automata boolearra"; // max 30 characters
        stringBooleanHexagonalAutomata = "Automata boolear hexagonala"; // max 30 characters
        stringBooleanTriangularAutomata = "Automata boolear triangularra"; // max 30 characters
        stringConwaysGameOfLife = "Conway-ren biziaren jokoa";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringSelectColors = "Hautatu koloreak"; // max 30 characters
        stringBackToMainMenu = "Itzuli menu nagusira"; // max 30 characters
        
        stringAntsPixels = "Tamaina (pixelak)"; // Max 23 characters
        
        stringVonNeumannNeighborhood = "Von Neumann-en auzoa";
        stringMooreNeighborhood = "Moore-en auzoa";
        stringNeighborhood = "Auzoa";
                
        stringBackgroundColor = "Atzeko planoaren kolorea";
        stringForegroundColor = "Aurreko planoaren kolorea";
        stringLineColor = "Lerroen kolorea";
        stringRed = "Gorria";
        stringGreen = "Berdea";
        stringBlue = "Urdina";
        
        stringSteps = "Pausoak";
        
        stringAutomataType = "Automata mota";
        
        stringVersion = strVersionNumber + " bertsioa";
        
        stringSelectLanguage = "Hautatu hizkuntza";
    }
    
    return 0;
}

#endif
