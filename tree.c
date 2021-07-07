#include "tree.h"

//functie care aloca memorie pentru un nod al arborelui, fara a seta insa
//valoarea acelui nod
TArb AllocNode()
{
    //initializarea nodului, precum si a elementelor directe din cadrul
    //acestuia + verificare eroare de alocare
    TArb tree = calloc(1, sizeof(TNodArb));
    if(!tree)
    {
        return NULL;
    }
    tree->info = calloc(1, sizeof(TNodInfo));
    if(!tree->info)
    {
        free(tree);
        return NULL;
    }
    tree->nextSibling = NULL;
    tree->firstChild = NULL;
    
    //initializarea elementelor directe din cadrul campului info, aflat
    //in interiorul arborelui
    tree->info->type = calloc(MAX_COMMAND_LEN, sizeof(char));
    if(!tree->info->type)
    {
        free(tree->info);
        free(tree);
        return NULL;
    }

    tree->info->contents = calloc(MAX_COMMAND_LEN, sizeof(char));
    if(!tree->info->contents)
    {   
        free(tree->info->type);
        free(tree->info);
        free(tree);
        return NULL;
    }
    tree->info->style = NULL;
    tree->info->otherAttributes = NULL;
    tree->info->isSelfClosing = 0;
    tree->parent = NULL;
    return tree;
}

//functie care primeste ca si parametru un nod parinte si returneaza
//un string ce reprezinta id-ul urmatorului copil
char* GenerateNodeID(TArb node)
{
    char* curr_id = calloc(MAX_COMMAND_LEN, sizeof(char));
    if(!curr_id)
    {
        printf("Alocare esuata\n");
        return NULL;
    }
    if(strcmp(node->info->id, "0") == 0)
    {
        if(node->firstChild == NULL)
        {
            strcpy(curr_id, "1");
            return curr_id;
        }
        else
        {
            strcpy(curr_id, "2");
            return curr_id;
        }       
    }
    strcpy(curr_id, node->info->id);
    if(node->firstChild == NULL)
    {
        sprintf(curr_id, "%s.%d", curr_id, 1);
        return curr_id;
    }
    else
    {
        TArb last_children = node->firstChild;
        for(; last_children->nextSibling != NULL; )
        {
            last_children = last_children->nextSibling;
        }
        strcpy(curr_id,last_children->info->id);
        int last_index = 0, last_dot_index, length = strlen(curr_id);
        last_dot_index = length;
        for(int i = 0; i < length; i++)
        {
            if(curr_id[i] == '.')
            {
                last_dot_index = i;
            }
        }
        if(last_dot_index != length)
        {
            last_index = atoi(curr_id + last_dot_index + 1);
        }
        curr_id[last_dot_index] = '\0';
        last_index++;
        sprintf(curr_id, "%s.%d",curr_id, last_index);
    }    
    return curr_id;
}

//functie care realizeaza inserarea unui nod nou la sfarsitul listei de
//copii a-i tatalui la care trebuie adaugat
void InsertIntoTree(TArb father, TArb new_node)
{
    if(father->firstChild == NULL)
    {
        father->firstChild = new_node;
    }
    else
    {
        TArb last_child = father->firstChild;
        for(; last_child->nextSibling != NULL; )
        {
            last_child = last_child->nextSibling;
        }
        last_child->nextSibling = new_node;
    }
}

//functie care primeste un nod si, folosindu-se de id, cauta tatal
//nodului respectiv
TArb SearchingForFather(TArb node, TArb tree)
{
    char father_id[1000];
    strcpy(father_id, node->info->id);
    if(strcmp(father_id, "1") == 0 || strcmp(father_id, "2") == 0)
    {
        return tree;
    }
    int length = strlen(father_id), last_dot_index = 0;
    if(length == 1)
    {
        return tree;
    }
    for(int i = 0; i < length; i++)
    {
        if(father_id[i] == '.')
        {
            last_dot_index = i;
        }
    }
    father_id[last_dot_index] = '\0';
    return SmartNodeSearch(tree, father_id);
}

//functie care primeste ca si parametru un sir de caractere ce reprezinta
//id-ul unui nod, pe care il cauta in arbore; cautarea se face selectiv,
//neparcurgand tot arborele, ci doar cat timp id-ul pe parcurs coindice;
//Astfel, nu se va intra niciodata pe un nod care nu este stramos al nodului
//care trebuie cautat
TArb SmartNodeSearch(TArb tree, char* id)
{
    TArb result = tree;
    int index = 0;
    if(id[index] == '1')
    {
        result = result->firstChild;
    }
    else if(id[index] == '2')
    {
        result = result->firstChild->nextSibling;
    }

    while(strcmp(result->info->id, id) != 0)
    {
        if(result->info->id[index] == id[index])
        {
            index++;
        }
        else
        {
            if(id[index] == '.')
            {
                result = result->firstChild;
            }
            else if(id[index - 1] != '.')
            {
                if(index == 0)
                {
                    result = result->firstChild;
                }
                else if(index == 1)
                {
                    result = result->nextSibling;
                }
                else
                {
                    result = result->nextSibling;
                    index--;
                }
            }
            else
            {
                result = result->nextSibling;
            }
        }
        if(result == NULL)
        {
            return NULL;
        }
    }
    return result;
}

//Functie care aloca memoria necesara unui atribut, fie el de tipul style,
//fie el de tipul otherAttribute, dupa care formeaza campurile atributului,
//campul name si campul value
TAttr MakeAttribute(char* name, char* value)
{
    TAttr attribute = calloc(1, sizeof(TNodAttr));
    if(!attribute)
    {
        printf("Alocarea nu a fost realizata cu succes\n");
        return NULL;
    }
    attribute->name = calloc(MAX_COMMAND_LEN, sizeof(char));
    if(!attribute->name)
    {
        printf("Alocarea nu a fost realizata cu succes\n");
        free(attribute);
        return NULL;
    }
    attribute->value = calloc(MAX_COMMAND_LEN, sizeof(char));
    if(!attribute->value)
    {
        printf("Alocarea nu a fost realizata cu succes\n");
        free(attribute->name);
        free(attribute);
        return NULL;
    }
    attribute->next = NULL;
    strcpy(attribute->name, name);
    strcpy(attribute->value, value);
    return attribute;
}

//Functie care realizeaza inserarea unui singur atribut in cadrul listei
//de alte atribute ale unui nod, node->info->otherAttributes
void InsertAttribute(TArb node, TAttr attribute)
{
    TAttr final_attribute = node->info->otherAttributes;
    if(final_attribute == NULL)
    {
        node->info->otherAttributes = attribute;
        return;
    }
    for( ; final_attribute->next != NULL; )
    {
        final_attribute = final_attribute->next;
    }
    final_attribute->next = attribute;
}

//Functie care creeaza o lista de atribute, atribute care, asa cum sugereaza
//si numele functiei, sunt de tipul campului style
void MakeStyleList(TArb node, char* list_string)
{
    char name[MAX_LINE_LEN], value[MAX_LINE_LEN];
    int length = strlen(list_string), ok = 0;
    TAttr attribute = node->info->style, last_attribute = NULL;
    strcpy(name, "");
    strcpy(value, "");
    for(int i = 0; i < length; i++)
    {
        if(list_string[i] == ':')
        {
            ok = 1;
            continue;
        }
        if(list_string[i] == ' ')
        {
            continue;
        }
        if(ok == 0)
        {
            sprintf(name, "%s%c", name, list_string[i]);
        }
        else if(strchr(";: ", list_string[i]) == NULL)
        {
            sprintf(value, "%s%c", value, list_string[i]);
        }
        if(list_string[i] == ';')
        {
            ok = 0;
            
            if(attribute == NULL)
            {
                node->info->style = MakeAttribute(name, value);
                attribute = node->info->style;
            }
            else
            {
                for(; attribute != NULL;
                    attribute = attribute->next)
                {
                    last_attribute = attribute;
                }
                if(attribute == NULL)
                {
                    if(strcmp(name, last_attribute->name) == 0)
                    {
                        strcpy(last_attribute->value, value);
                        attribute = node->info->style;
                    }
                    else
                    {
                        last_attribute->next = MakeAttribute(name, value);
                        attribute = node->info->style;
                    }
                    
                }
            }
            strcpy(name, "");
            strcpy(value, "");
        }
    }
}

//Functie care realizeaza operatia de append asupra unei liste, adaugand
//astfel un nou element daca acesta nu exista, prin crearea acestuia,
//sau actualizarea unui element daca acesta deja exista
void AppendStyleList(TArb node, char* list_string)
{
    char name[MAX_LINE_LEN], value[MAX_LINE_LEN];
    int length = strlen(list_string), ok = 0;
    TAttr attribute = node->info->style, last_attribute = NULL;
    strcpy(name, "");
    strcpy(value, "");
    for(int i = 0; i < length; i++)
    {
        if(list_string[i] == ':')
        {
            ok = 1;
            continue;
        }
        if(list_string[i] == ' ')
        {
            continue;
        }
        if(ok == 0)
        {
            sprintf(name, "%s%c", name, list_string[i]);
        }
        else if(strchr(";: ", list_string[i]) == NULL)
        {
            sprintf(value, "%s%c", value, list_string[i]);
        }
        if(list_string[i] == ';')
        {
            ok = 0;
            
            if(attribute == NULL)
            {
                node->info->style = MakeAttribute(name, value);
                attribute = node->info->style;
            }
            else
            {
                for(; attribute != NULL;
                    attribute = attribute->next)
                {
                    if(strcmp(name, attribute->name) == 0)
                    {
                        strcpy(attribute->value, value);
                        attribute = node->info->style;
                        break;
                    }
                    last_attribute = attribute;
                }
                if(attribute == NULL)
                {
                    if(strcmp(name, last_attribute->name) == 0)
                    {
                        strcpy(last_attribute->value, value);
                        attribute = node->info->style;
                    }
                    else
                    {
                        last_attribute->next = MakeAttribute(name, value);
                        attribute = node->info->style;
                    }
                    
                }
            }
            strcpy(name, "");
            strcpy(value, "");
        }
    }
}

//Functie care realizeaza stergerea unei intregi liste de style-uri
void DeleteStyleList(TArb node)
{
    TAttr attribute = node->info->style, aux;
    for(; attribute != NULL;)
    {
        node->info->style = attribute->next;
        aux = attribute;
        attribute = attribute->next;
        free(aux->name);
        free(aux->value);
        free(aux);
    }
    node->info->style = NULL;
}

//Functie care realizeaza stergerea unei intregi liste de alte atribute
void DeleteOtherAttributeList(TArb node)
{
    TAttr attribute = node->info->otherAttributes, aux;
    for(; attribute != NULL;)
    {
        node->info->otherAttributes = attribute->next;
        aux = attribute;
        attribute = attribute->next;
        free(aux->name);
        free(aux->value);
        free(aux);
    }
    node->info->otherAttributes = NULL;
}

//functie care realizeaza stergerea unui intreg nod, cu toate campurile care
//fac parte din acesta, de la campul contents pana la campurile type si id
void DeleteNode(TArb node)
{
    DeleteStyleList(node);
    DeleteOtherAttributeList(node);
    char* contents = node->info->contents;
    char* id = node->info->id, *type = node->info->type;
    if(contents)
    {
        free(contents);
    }
    free(id);
    node->info->contents = NULL;
    node->info->id = NULL;
    node->info->type = NULL;
    free(type);
    node->firstChild = NULL;
    node->nextSibling = NULL;
    free(node->info);
    free(node);
    node = NULL;
}