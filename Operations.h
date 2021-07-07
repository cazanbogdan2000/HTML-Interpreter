//Operations.h
//fisier ce contine functii care realizeaza comenzile primite, cum ar fi
//format, append, deleteRecursively, add si override

#include "DeleteRecursively.h"
#ifndef __OPERATIONS__
#define __OPERATIONS__

//Functia care face adaugarea unui nou nod in cadrul arborelui, in cazul
//in care se gaseste nodul care sa poata fi parintele lui
void ADD(programData program_data);

//Functie care face afisarea unui tag intreg, cu numarul de taburi necesare;
//printarea consta in afisarea numelui tagului (a.k.a type), a listei de 
//atribute specifice style-ului, apoi afisarea restului de atribute, iar,
//la final, contents
void PrintTag(programData program_data, TArb node, int nr_tabs);

//functie care realizeaza printarea unui tag de tipul self closing,
//intrucat acestea nu prezinta un element de inchidere dupa printarea
//tuturor atributelor, si de asemenea nici campul contents (deoarece se
//inchid inainte de acesta)
void PrintSelfClosingTag(programData program_data, TArb node, int nr_tabs);

//Se afiseaza tagurile "normale", care nu sunt self Closing
//Pentru a evita un posibil Stack Overflow, am evitat printarea recursiva 
//a codului html, folosind o stiva pentru elementul de inchidere
void NormalTags(programData program_data, int* nr_tabs, Stack ending_formula);

//Functia care formateaza tot textul html, afisand cu tot cu taburi toate
//tagurile arborelui, fie ele self closing sau nu
void FORMAT(programData program_data);

//functie care, in functie de selectorii primiti ca parametru din fisierul
//de comenzi, urmand ca mai apoi sa realizeze operatia de append
//pe style-ul unui nod (sau mai multor noduri daca e cazul), daca se gaseste
//cel putin un nod care sa indeplineasca conditia
void APPEND_STYLE(programData program_data);

//functie care, in functie de selectorii primiti ca parametru din fisierul
//de comenzi, urmand ca mai apoi sa realizeze operatia de override
//pe style-ul unui nod (sau mai multor noduri daca e cazul), daca se gaseste
//cel putin un nod care sa indeplineasca conditia
void OVERRIDE_STYLE(programData program_data);

//functie care, in functie de selectorii primiti ca parametru din fisierul
//de comenzi, urmand ca mai apoi sa realizeze operatia de stergere a
//unui nod (sau mai multor noduri daca e cazul), daca se gaseste
//cel putin un nod care sa indeplineasca conditia
void DELETE_RECURSIVELY(programData program_data);

//functia care parseaza tot fisierul de comenzi, intrand pe diferite ramuri,
//in functie de comanda si de selectorul primit
void OperationSelection(programData program_data);

#endif