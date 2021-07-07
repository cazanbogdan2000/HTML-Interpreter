//AppendStyle.h
//->functii care, in functie de o anumita selectie, vor realiza
//operatia de append a style-ului pe nodurile care respecta acea selectie

#include "ConstructTree.h"
#ifndef __APPEND_STYLE__
#define __APPEND_STYLE__

//funcite care face citirea din fisierul commands in a style-ului, atunci cand
//vine vorba despre functiile de append sau de override
char* readStyle(programData program_data);

//Functie care face append unui anumit style primit ca parametru, la un
//nod cu un anumit id; Cautarea nodului se face prin functia de cautare
//SmartNodeSearch
void AppendStyleID(programData program_data, char* command);

//functie care face append cu un anumit style tuturor nodurilor care au
//in cadrul campului node->info->type o anumita valoare;
void AppendStyleAllTags(programData program_data, char* command);

//functie care face append cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
void AppendStyleAllClasses(programData program_data, char* command);

//functie care face append cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru, nodurilor cu un anumit tag type
void AppendStyleGivenClass(programData program_data, char* command);

//functie care face append tuturor fiilor cu un anumit tag type, si care de
//asemenea au un tata cu un anumit tag type
void AppendStyleChild(programData program_data, char* command);

//Functie care face append cu un anumit style tuturor nodurilor care au un
//anumit tag type si care au stramos, de asemenea, cu un anumit tag type
void AppendStyleAllDescents(programData program_data, char* command);

#endif