//ConstructTree.h
//->functii pe care le vom folosi pentru construirea arborelui, de la
//cititrea acestuia din fisier pana la formarea lui efectiva
//Aceste functii vor fi folosite preponderent in cadrul functiei main

#include "tree.h"
#ifndef __CONSTRUCT_TREE__
#define __CONSTRUCT_TREE__

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse contents;
void ParseContents(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse opening bracket;
void ParseOpeningBracket(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse tag type;
void ParseTagType(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse closing tag;
void ParseClosingTag(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse rest of tag;
void ParseRestOfTag(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse attribute name;
void ParseAttributeName(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse attribute eq;
void ParseAttributeEq(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse attribute value;
void ParseAttributeValue(programData program_data, char character);

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse self closing;
void ParseSelfClosing(programData program_data, char character);

//Functie care citeste rand pe rand fiecare caracter din fisierul html,
//pana la finalizarea acestuia, si il prelucreaza in functie de
//state-ul pe care acesta intra
void ConstructTree(programData program_data);

#endif