//DoubleList.h
//->functii care opereaza pe stive
//->definitia unei asftel de structuri se poate gasi in fisierul
//header "lib.h"

#include "queue.h"
#ifndef __STACK__
#define __STACK__

//functie de initializare a unei stive
Stack InitS();

//functie de introducere a unui element in stiva
int Push(Stack s, void* info);

//Functie de extragere a primului element din stiva
void* Pop(Stack s);

//Permite accesarea elementului din varful stivei, fara a-l extrage insa
//din stiva
void* Top(Stack s);

//Functie care muta elementele dintr-o stiva sursa intr-o alta stiva
//destinatie
void MoveS(Stack destination, Stack source);

//Functie care elibereaza memoria dintr-o stiva, fara insa a o sterge
//definitiv
void ClearS(Stack s, TFdelete delEl);

//Functie care sterge elementele din cadrul stivei, dupa care o
//distruge
void DestroyS(Stack s, TFdelete delEl);

#endif