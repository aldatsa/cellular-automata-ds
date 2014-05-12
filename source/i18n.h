#ifndef I18N_H
#define I18N_H

#include <string>
#include "globals.h"

/*
 * Constant for the version number of the application
 */
const std::string strVersionNumber = "0.10.4.0";

/*
 * The strings of text used in the app in the default language (english).
 *
 */
std::string stringRuleNumber = "Rule number";
std::string stringElementaryCellularAutomata = "Elementary cellular automata";
// max 30 characters
std::string stringLangtonsAnt = "Langton's ant"; // max 30 characters
std::string stringLangtonsHexagonalAnt = "Langton's hexagonal ant";
std::string stringBooleanAutomata = "Boolean automata";
std::string stringBooleanHexagonalAutomata = "Boolean hexagonal automata";
std::string stringBooleanTriangularAutomata = "Boolean triangular automata";
std::string stringConwaysGameOfLife = "Conway's game of life";
std::string stringMunchingSquares = "Munching Squares";
std::string stringBMLtrafficModel = "BML traffic model";
std::string stringCyclicCellularAutomaton = "Cyclic cellular automaton";
std::string stringSteppingStone = "Stepping stone";
std::string stringSelectColors = "Select colors";
std::string stringBackToMainMenu = "Back to main menu";
std::string stringPopulation = "Population:";
std::string stringOneCell = "One cell";
std::string stringRandom = "Random";
std::string stringAntsPixels = "Ant's size (pixels)";
std::string stringNumStates = "Number of states";
std::string stringVonNeumannNeighborhood = "Von Neumann neighborhood";
std::string stringMooreNeighborhood = "Moore neighborhood";
std::string stringNeighborhood = "Neighborhood:";
std::string stringInitialState ="Initial state:";
std::string stringAcorn = "Acorn";
std::string stringFPentomino = "F-Pentomino";
std::string stringFillScreenWithPulsars = "Fill screen with pulsars";
std::string stringFillScreenWithPentadecathlons = "Fill with pentadecathlons";
std::string stringBackgroundColor = "Background color:";
std::string stringForegroundColor = "First foreground color:";
std::string stringSecondForegroundColor = "Second foreground color:";
std::string stringLineColor = "Line color:";
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
std::string string1234 = "1    2    3    4";
std::string string5678 = "5    6    7    8";
std::string string123 = "1    2    3";
std::string string456 = "4    5    6"; 
std::string stringRestart = "Restart";
std::string stringDensity = "Density";

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
        
        stringElementaryCellularAutomata = "Elementary cellular automata";
        // max 30 characters
        stringLangtonsAnt = "Langton's ant"; // max 30 characters
        stringLangtonsHexagonalAnt = "Langton's hexagonal ant";
        // max 30 characters
        stringBooleanAutomata = "Boolean automata"; // max 30 characters
        stringBooleanHexagonalAutomata = "Boolean hexagonal automata";
        // max 30 characters
        stringBooleanTriangularAutomata = "Boolean triangular automata";
        // max 30 characters
        stringConwaysGameOfLife = "Conway's game of life";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringBMLtrafficModel = "BML traffic model";
        stringCyclicCellularAutomaton = "Cyclic cellular automaton";
        stringSteppingStone = "Stepping stone";
        stringSelectColors = "Select colors"; // max 30 characters
        stringBackToMainMenu = "Back to main menu"; // max 30 characters
        stringPopulation = "Population:";

        stringOneCell = "One cell";
        stringRandom = "Random";

        stringAntsPixels = "Ant's size (pixels)"; // Max 22 characters
        stringNumStates = "Number of states";        
        stringVonNeumannNeighborhood = "Von Neumann neighborhood";
        stringMooreNeighborhood = "Moore neighborhood";
        stringNeighborhood = "Neighborhood:";

        stringInitialState ="Initial state:";
        stringAcorn = "Acorn";
        stringFPentomino = "F-Pentomino";
        stringFillScreenWithPulsars = "Fill screen with pulsars";
        stringFillScreenWithPentadecathlons = "Fill with pentadecathlons";

        stringBackgroundColor = "Background color:";
        stringForegroundColor = "First foreground color:";
        stringSecondForegroundColor = "Second foreground color:";
        stringLineColor = "Line color:";
        stringRed = "Red";
        stringGreen = "Green";
        stringBlue = "Blue";
        
        stringSteps = "Steps";
        
        stringAutomataType = "Automata type";
        
        stringVersion = "version " + strVersionNumber;
        
        stringSelectLanguage = "Select language";

        stringRestart = "Restart";

        stringDensity = "Density";
    }
    else if (language == ES) // Spanish
    {
        stringRuleNumber = "Regla numero"; // It doesn't print 'ú'. Why?
        
        stringElementaryCellularAutomata = "Automata celular elemental";
        // max 30 characters
        stringLangtonsAnt = "Hormiga de Langton"; // max 30 characters
        stringLangtonsHexagonalAnt = "Hormiga de Langton hexagonal";
        // max 30 characters
        stringBooleanAutomata = "Automata booleano"; // max 30 characters
        stringBooleanHexagonalAutomata = "Automata booleano hexagonal";
        // max 30 characters
        stringBooleanTriangularAutomata = "Automata booleano triangular";
        // max 30 characters
        stringConwaysGameOfLife = "Juego de la vida de Conway";
        stringMunchingSquares = "Munching Squares";
        // max 30 characters
        stringBMLtrafficModel = "Modelo de trafico BML";
        stringSelectColors = "Seleccionar colores";
        // max 30 characters        
        stringCyclicCellularAutomaton = "Automata celular ciclico";
        stringSteppingStone = "Camino de piedras";
        stringBackToMainMenu = "Volver al menu principal"; // max 30 characters
        stringPopulation = "Populacion:";

        stringOneCell = "Una celda";
        stringRandom = "Aleatorio";

        stringAntsPixels = "Tamano (pixeles)";
        // It doesn't print 'ñ'. Why?
        // And "Tamano de la hormiga (pixeles)" is too long. Max 22 characters
        stringNumStates = "Numero de estados";
         
        stringVonNeumannNeighborhood = "Vecindad de Von Neumann";
        stringMooreNeighborhood = "Vecindad de Moore";
        stringNeighborhood = "Vecindad:";

        stringInitialState ="Estado inicial:";
        stringAcorn = "Acorn";
        stringFPentomino = "F-Pentomino";
        stringFillScreenWithPulsars = "Llenar pantalla de pulsares";
        stringFillScreenWithPentadecathlons = "Llenar de pentadecatlones";

        stringBackgroundColor = "Color de fondo:";
        stringForegroundColor = "Primer color de primer plano:";
        stringSecondForegroundColor = "Segundo color de primer plano:";
        stringLineColor = "Color de linea:";
        stringRed = "Rojo";
        stringGreen = "Verde";
        stringBlue = "Azul";
        
        stringSteps = "Pasos";
        
        stringAutomataType = "Tipo de automata";
        
        stringVersion = "version " + strVersionNumber;
        
        stringSelectLanguage = "Seleccionar lenguaje";

        stringRestart = "Reiniciar";

        stringDensity = "Densidad";
    }
    else if (language == EU) // Basque
    {
        stringRuleNumber = "Arau zenbakia";
        
        stringElementaryCellularAutomata = "Oinarrizko automata zelularra";
        // max 30 characters
        stringLangtonsAnt = "Langton-en inurria"; // max 30 characters
        stringLangtonsHexagonalAnt = "Langton-en inurri hexagonala";
        // max 30 characters
        stringBooleanAutomata = "Automata boolearra"; // max 30 characters
        stringBooleanHexagonalAutomata = "Automata boolear hexagonala";
        // max 30 characters
        stringBooleanTriangularAutomata = "Automata boolear triangularra";
        // max 30 characters
        stringConwaysGameOfLife = "Conway-ren biziaren jokoa";
        stringMunchingSquares = "Munching Squares"; // max 30 characters
        stringBMLtrafficModel = "BML trafiko eredua";
        stringCyclicCellularAutomaton = "Automata zelular ziklikoa";
        stringSteppingStone = "Harri bidea";
        stringSelectColors = "Hautatu koloreak"; // max 30 characters
        stringBackToMainMenu = "Itzuli menu nagusira"; // max 30 characters
        stringPopulation = "Populazioa:";

        stringOneCell = "Gelaxka bat";
        stringRandom = "Ausazkoa";

        stringAntsPixels = "Tamaina (pixelak)"; // Max 23 characters
        stringNumStates = "Egoera kopurua"; 
        stringVonNeumannNeighborhood = "Von Neumann-en auzoa";
        stringMooreNeighborhood = "Moore-en auzoa";
        stringNeighborhood = "Auzoa:";

        stringInitialState ="Hasierako egoera:";
        stringAcorn = "Acorn";
        stringFPentomino = "F-Pentomino";
        stringFillScreenWithPulsars = "Pantaila pulsarekin bete";
        stringFillScreenWithPentadecathlons = "Pentadekatlonekin bete";

        stringBackgroundColor = "Atzeko planoaren kolorea:";
        stringForegroundColor = "Aurreko planoko lehen kolorea:";
        stringSecondForegroundColor = "Aurreko planoko 2. kolorea:";
        stringLineColor = "Lerroen kolorea:";
        stringRed = "Gorria";
        stringGreen = "Berdea";
        stringBlue = "Urdina";
        
        stringSteps = "Pausoak";
        
        stringAutomataType = "Automata mota";
        
        stringVersion = strVersionNumber + " bertsioa";
        
        stringSelectLanguage = "Hautatu hizkuntza";

        stringRestart = "Berrabiarazi";

        stringDensity = "Dentsitatea";
    }
    
    return 0;
}

#endif
