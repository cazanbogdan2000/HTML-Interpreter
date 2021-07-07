//tree.h
//->functii care opereaza pe arbori
//->definitia unei asftel de structuri se poate gasi in fisierul
//header "lib.h"

#include "Stack.h"
#ifndef __TREE__
#define __TREE__

//functie care aloca memorie pentru un nod al arborelui, fara a seta insa
//valoarea acelui nod
//Retrurneaza nodul respectiv, alocat
TArb AllocNode();

//functie care primeste ca si parametru un nod parinte si returneaza
//un string ce reprezinta id-ul urmatorului copil
char* GenerateNodeID(TArb node);

//functie care realizeaza inserarea unui nod nou la sfarsitul listei de
//copii a-i tatalui la care trebuie adaugat
void InsertIntoTree(TArb tree, TArb new_node);

//functie care primeste un nod si, folosindu-se de id, cauta tatal
//nodului respectiv
TArb SearchingForFather(TArb node, TArb tree);

//functie care primeste ca si parametru un sir de caractere ce reprezinta
//id-ul unui nod, pe care il cauta in arbore; cautarea se face selectiv,
//neparcurgand tot arborele, ci doar cat timp id-ul pe parcurs coindice;
TArb SmartNodeSearch(TArb tree, char* id);

//Functie care aloca memoria necesara unui atribut, fie el de tipul style,
//fie el de tipul otherAttribute, dupa care formeaza campurile atributului,
//campul name si campul value
TAttr MakeAttribute(char* name, char* value);

//Functie care realizeaza inserarea unui singur atribut in cadrul listei
//de alte atribute ale unui nod, node->info->otherAttributes
void InsertAttribute(TArb node, TAttr attribute);

//Functie care creeaza o lista de atribute, atribute care, asa cum sugereaza
//si numele functiei, sunt de tipul campului style
void MakeStyleList(TArb node, char* list_string);

//Functie care realizeaza operatia de append asupra unei liste, adaugand
//astfel un nou element daca acesta nu exista, prin crearea acestuia,
//sau actualizarea unui element daca acesta deja exista
void AppendStyleList(TArb node, char* list_string);

//Functie care realizeaza stergerea unei intregi liste de style-uri
void DeleteStyleList(TArb node);

//Functie care realizeaza stergerea unei intregi liste de alte atribute
void DeleteOtherAttributeList(TArb node);

//functie care realizeaza stergerea unui intreg nod, cu toate campurile care
//fac parte din acesta, de la campul contents pana la campurile type si id
void DeleteNode(TArb node);

#endif