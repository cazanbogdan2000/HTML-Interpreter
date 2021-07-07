//Override.h
//Acest header contine prototipuri pentru toate selectiile de care dispune
//operatia de override

#include "AppendStyle.h"
#ifndef __OVERRIDE__
#define __OVERRIDE__

//Functie care face override unui anumit style primit ca parametru, la un
//nod cu un anumit id; Cautarea nodului se face prin functia de cautare
//SmartNodeSearch
void OverrideStyleID(programData program_data, char* command);

//functie care face override cu un anumit style tuturor nodurilor care au
//in cadrul campului node->info->type o anumita valoare;
void OverrideStyleAllTags(programData program_data, char* command);

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
void OverrideStyleAllClasses(programData program_data, char* command);

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
void OverrideStyleGivenClass(programData program_data, char* command);

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
void OverrideStyleChild(programData program_data, char* command);

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru, nodurilor cu un anumit tag type
void OverrideStyleAllDescents(programData program_data, char* command);

#endif