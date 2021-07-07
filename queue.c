#include "queue.h"

//functie de initializare a unei cozi
Queue InitQ()
{
    Queue q = malloc(sizeof(TCoada));
    if(!q)
    {
        return NULL;
    }
    q->begin = NULL;
    q->end = NULL;
    return q;
}

//functie de introducere a unui element in coada; alocare+verificare,
//apoi se face legatura cu inceputul listei prin care este definita
//structura de tip coada
void Enqueue(Queue q, void* info)
{
    SimpleList aux = malloc(sizeof(Cell));
    if(!aux)
    {
        return;
    }
    aux->info = info;
    aux->next = NULL;
    if(VIDA_Q(q))
    {
        q->begin = aux;
        q->end = aux;
    }
    else
    {
        q->end->next = aux;
        q->end = aux;
    }
}

//Functie de extragere a primului element din coada; rezultatul va
//fi un element de tip void*, reprezentand informatia elementului
void* Dequeue(Queue q)
{
    if(VIDA_Q(q))
    {
        return NULL;
    }
    void* result = q->begin->info;
    SimpleList aux = q->begin;
    q->begin = aux->next;
    aux->next = NULL;
    free(aux);
    return result;
}

//Permite accesarea elementului din varful cozii, fara a-l extrage insa
//din coada
void* Peek(Queue q)
{
    if(VIDA_Q(q))
    {
        return NULL;
    }
    void* result = q->begin->info;
    return result;
}

//Functie care muta elementele dintr-o coada sursa intr-o alta coada
//destinatie
void MoveQ(Queue destination, Queue source)
{
    while(!VIDA_Q(source))
    {
        Enqueue(destination, Dequeue(source));
    }
}

//Functie care elibereaza memoria dintr-o coada, fara insa a o sterge
//definitiv
void ClearQ(Queue q, TFdelete delEl)
{
    void* info;
    while(!VIDA_Q(q))
    {
        info = Dequeue(q);
        delEl(info);
    }
    q->begin = q->end = NULL;
}

//Functie care sterge elementele din cadrul cozii, dupa care o
//distruge
void DestroyQ(Queue q, TFdelete delEl)
{
    ClearQ(q, delEl);
    free(q);
}

//Functie care face inserarea ordonata intr-o coada, fara insa a itera
//prin coada, ci doar prin functiile definite pentru o coada; comparatia
//se face prin intermediul unei functii de comparatie
void OrderedInsertion(Queue q, void* info,
    long (* cmpEl)(void*, void*), TFdelete delEl)
{
    Queue aux = InitQ();
    if(VIDA_Q(q))
    {
        Enqueue(q, info);
        return;
    }
    while(!VIDA_Q(q))
    {
        void* extracted_info = Dequeue(q);
        //compara elementul de introdus cu elemntul de la inceputul
        //cozii, si il introduce la loc pe cel extras intr-o coada auxiliara
        if(cmpEl(extracted_info, info) < 0)
        {
            Enqueue(aux, extracted_info);
            if(VIDA_Q(q))
            {
                Enqueue(aux, info);
            }
        }
        //sau le introduce pe ambele in coada auxiliara, daca este
        //indeplinita conditia
        else
        {
            Enqueue(aux, info);
            Enqueue(aux, extracted_info);
            break;
        }
    }
    //se reconstruieste coada initiala
    MoveQ(aux, q);
    MoveQ(q, aux);
    //se distruge coada auxiliara
    DestroyQ(aux, delEl);
}

void SimpleFree(void *a)
{
    
}