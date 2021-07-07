#include "Stack.h"

//functie de initializare a unei stive
Stack InitS()
{
    Stack s = malloc(sizeof(TStiva));
    if(!s)
    {
        return NULL;
    }
    s->top = NULL;
    return s;
}

//functie de introducere a unui element in stiva; alocare+verificare,
//apoi se face legatura cu inceputul listei prin care este definita
//structura de tip stiva
int Push(Stack s, void* info)
{
    SimpleList aux = malloc(sizeof(Cell));
    if(!aux)
    {
        return 0;
    }
    aux->info = info;
    aux->next = s->top;
    s->top = aux;
    return 1;
}

//Functie de extragere a elementului din varful stivei; rezultatul va
//fi un element de tip void*, reprezentand informatia elementului
void* Pop(Stack s)
{
    if(VIDA_S(s))
    {
        return NULL;
    }
    void* result = s->top->info;
    SimpleList aux = s->top;
    s->top = aux->next;
    aux->next = NULL;
    free(aux);
    return result;
}

//Permite accesarea elementului din varful stivei, fara a-l extrage insa
//din stiva
void* Top(Stack s)
{
    if(VIDA_S(s))
    {
        return NULL;
    }
    void* result = s->top->info;
    return result;
}

//Functie care muta elementele dintr-o stiva sursa intr-o alta stiva
//destinatie
void MoveS(Stack destination, Stack source)
{
    while(!VIDA_S(source))
    {
        Push(destination,Pop(source));
    }
}

//Functie care elibereaza memoria dintr-o stiva, fara insa a o sterge
//definitiv
void ClearS(Stack s, TFdelete delEl)
{
    void* info;
    while(!VIDA_S(s))
    {
        info = Pop(s);
        delEl(info);
    }
    s->top = NULL;
}

//Functie care sterge elementele din cadrul cozii, dupa care o
//distruge
void DestroyS(Stack s, TFdelete delEl)
{
    ClearS(s, delEl);
    free(s);
}
