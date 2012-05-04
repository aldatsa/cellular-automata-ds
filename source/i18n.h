#ifndef I18N_H
#define I18N_H

#include <string>

using namespace std;

/*
 * Constant for the version number of the application
 */
const string strVersionNumber = "0.3.1.4";

/* This variable sets the language used in the menus
 * "en": english
 * "es": espanol (spanish)
 * "eu": euskara (basque)
 */
string displayedLanguage = "en";

/*
 * The strings of text used in the app in the default language (english).
 *
 */
string stringRuleNumber = "Rule number";
string stringElementaryCellularAutomata = "Elementary cellular automata"; // max 30 characters
string stringLangtonsAnt = "Langton's ant"; // max 30 characters
string stringLangtonsHexagonalAnt = "Langton's hexagonal ant";
string stringBooleanAutomata = "Boolean automata";
string stringBooleanHexagonalAutomata = "Boolean hexagonal automata";
string stringBooleanTriangularAutomata = "Boolean triangular automata";
string stringConwaysGameOfLife = "Conway's game of life";
string stringMunchingSquares = "Munching Squares";
string stringSelectColors = "Select colors";
string stringBackToMainMenu = "Back to main menu";
string stringAntsPixels = "Ant's size (pixels)";
string stringVonNeumannNeighborhood = "Von Neumann neighborhood";
string stringMooreNeighborhood = "Moore neighborhood";
string stringNeighborhood = "Neighborhood";
string stringTypeOfComparation = "Type of comparation";
string stringSmallerThan = "Smaller than";
string stringEqualTo = "Equal to";
string stringTypeOfBooleanOperator = "Type of boolean operator";
string stringThreshold = "Threshold";
string stringBackgroundColor = "Background color";
string stringForegroundColor = "Foreground color";
string stringLineColor = "Line color";
string stringRed = "Red";
string stringGreen = "Green";
string stringBlue = "Blue";
string stringSteps = "Steps";
string stringAutomataType = "Automata type";
string stringVersion = "version " + strVersionNumber;
string stringSelectLanguage = "Select language";
string stringEnglish = "English";
string stringEspanol = "Espanol";
string stringEuskara = "Euskara";

/*
 * Changes the language of the strings of text to the selected language
 * languageCode is the two letters code for each language
 * en : english
 * es : spanish
 * eu : basque
 */
int changeTextLanguage(string languageCode)
{
    if (languageCode == "en") // English
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
        stringEnglish = "English";
        stringEspanol = "Espanol";
        stringEuskara = "Euskara";
    }
    else if (languageCode == "es") // Spanish
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
        stringEnglish = "English";
        stringEspanol = "Espanol";
        stringEuskara = "Euskara";
    }
    else if (languageCode == "eu") // Basque
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
        stringEnglish = "English";
        stringEspanol = "Espanol";
        stringEuskara = "Euskara";
    }
    
    return 0;
}

#endif
