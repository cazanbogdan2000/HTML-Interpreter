//queue.h
//->functii care opereaza pe cozi
//->definitia unei asftel de structuri se poate gasi in fisierul
//header "lib.h"

#include "lib.h"
#ifndef __QUEUE__
#define __QUEUE__

//functie de initializare a unei cozi
Queue InitQ();

//functie de introducere a unui element in coada
void Enqueue(Queue q, void* info);

//Functie de extragere a primului element din coada
void* Dequeue(Queue q);

//Permite accesarea elementului din varful cozii, fara a-l extrage insa
//din coada
void* Peek(Queue q);

//Functie care muta elementele dintr-o coada sursa intr-o alta coada
//destinatie
void MoveQ(Queue destination, Queue source);

//Functie care elibereaza memoria dintr-o coada, fara insa a o sterge
//definitiv
void ClearQ(Queue q, TFdelete delEl);

//Functie care sterge elementele din cadrul cozii, dupa care o
//distruge
void DestroyQ(Queue q, TFdelete delEl);

//Functie care face inserarea ordonata intr-o coada, fara insa a itera
//prin coada
void OrderedInsertion(Queue q, void* info,
    long (* cmpEl)(void*, void*), TFdelete delEl);

void SimpleFree(void *a);

#endif