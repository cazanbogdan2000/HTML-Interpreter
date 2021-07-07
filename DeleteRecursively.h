//DeleteRecursively.h
//->functii care, in functie de o anumita selectie, vor realiza
//operatia de stregere a unui intreg subarbore pe nodurile care
//respecta acea selectie

#include "Override.h"
#ifndef __DELETE_RECURSIVELY__
#define __DELETE_RECURSIVELY__

//functie care face stergerea recursiva a tuturor nodurilor din cadrul unui
//subarbore
void DeleteSubtree(TArb node);

//functie care genereaza noile ID-uri pentru siblings ai nodului sters
void NewIDs(TArb node, TArb next_node);

//functie care face stergera a ultimului copil a unui nod parinte,
//intrucat la stergerea ultimului copil, nu mai e nevoie de reconstruirea
//id-urilor din cadrul arborelui, acestea ramanand neschimbate
void DeleteLastNode(programData program_data, TArb node);

//functie care face stergerea oricarui nod din lista de copii ai unui nod
//parinte, mai putin ultimul nod din cadrul acestei liste, caz in care va
//fi apelata functia DeleteLastNode
void DeleteMiddleNode(programData program_data, TArb node);

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu un anumit id; Cautarea nodului se face prin functia de cautare
//SmartNodeSearch
void DeleteRecursivelyID(programData program_data, char* command);

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu o anumit tag type;
void DeleteRecursivelyAllTags(programData program_data, char* command);

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu o anumita clasa;
void DeleteRecursivelyAllClasses(programData program_data, char* command);

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu o anumita clasa si un anumit tag type
void DeleteRecursivelyGivenClass(programData program_data, char* command);

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod care este fiul unui anume nod parinte
void DeleteRecursivelyChild(programData program_data, char* command);

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod care este descendetul unui anumit stramos
void DeleteRecursivelyAllDescents(programData program_data, char* command);

#endif