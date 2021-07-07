#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//1->coada vida, 0 altfel
#define VIDA_Q(a) (((Queue)(a))->begin == NULL)

//1->stiva vida, 0 altfel
#define VIDA_S(a) (((Stack)(a))->top == NULL)

#define MAX_LINE_LEN 100 * 1024
#define MAX_COMMAND_LEN 1000

typedef void (*TFdelete)(void*);        //functie de stergere a unui element
typedef void (*TFprint)(void*);         //functie de afisare a unui element
typedef int (*TFCmp)(void*, void*);     //functie de comparare doua elemente

typedef struct TNodAttr {
	char *name;
	char *value;
	struct TNodAttr *next;
} TNodAttr, *TAttr;

typedef struct TNodInfo {
	char *type;
	char *id;
	TAttr style;
	TAttr otherAttributes;
	int isSelfClosing;
	char *contents;
} TNodInfo, *TInfo;

typedef struct TNodArb {
	TInfo info;
	struct TNodArb *nextSibling;
	struct TNodArb *firstChild;
    struct TNodArb *parent;
} TNodArb, *TArb;

typedef enum
{
    PARSE_ERROR             = 0,
    PARSE_CONTENTS          = 1,
    PARSE_OPENING_BRACKET   = 2,
    PARSE_TAG_TYPE          = 3,
    PARSE_CLOSING_TAG       = 4,
    PARSE_REST_OF_TAG       = 5,
    PARSE_ATTRIBUTE_NAME    = 6,
    PARSE_ATTRIBUTE_EQ      = 7,
    PARSE_ATTRIBUTE_VALUE   = 8,
    PARSE_SELF_CLOSING      = 9,
} TParseState;

TParseState Interpret(TParseState currentState, char c);

//structura ce reprezinta o lista simplu inlantuita
//folosita la definirea unei stive, respectiv a unei cozi
typedef struct cell
{
    struct cell *next;
    void* info;
}Cell, *SimpleList;

//structura ce reprezinta o coada
typedef struct
{
    SimpleList begin, end;      //inceputul, respectiv sfarsitul cozii
}TCoada, *Queue;

//structura ce reprezinta o stiva
typedef struct
{
    SimpleList top;         //varful stivei
}TStiva, *Stack;


typedef struct programData
{
    char** argv;        //parametrii pe care ii primeste programul la rulare
    TArb tree;             //radacina arborelui ce defineste programul
    TArb curr_node;     //nodul curent din arbore
    TParseState state;      //current state-ul din program e retinut aici
    FILE* html_file;         //fisierul ce contine codul HTML
    FILE* fin;          //fisierul ce contine comenzile de intrare
    FILE* fout;         //fisierul de iesire al programului
    char curr_id[1000];     //id-ul nodului curent din arbore
    char attr_name[1000];   //un string in care se "incarca", numele atributului
    char attr_value[1000];      //retine valoarea unui atribut
}TData, *programData;