#include "Override.h"

//Functie care face override unui anumit style primit ca parametru, la un
//nod cu un anumit id; Cautarea nodului se face prin functia de cautare
//SmartNodeSearch
void OverrideStyleID(programData program_data, char* command)
{
    char id[1000], *aux, style[1000];
    strcpy(id, command + 1);
    aux = readStyle(program_data);
    strcpy(style, aux);
    free(aux);

    TArb node = SmartNodeSearch(program_data->tree, id);
    
    DeleteStyleList(node);
    AppendStyleList(node, style);
    if(node == NULL)
    {
        fprintf(program_data->fout,
        "Override style failed: no node found for selector %s!\n", command);
    }
}

//functie care face override cu un anumit style tuturor nodurilor care au
//in cadrul campului node->info->type o anumita valoare;
//Parcurgerea nodurilor se face in latime, folosind o coada
void OverrideStyleAllTags(programData program_data, char* command)
{
    int ok = 0;
    char  *aux, style[1000];
    aux = readStyle(program_data);
    strcpy(style, aux);
    free(aux);

    Queue bfs_tree = InitQ();
    TArb node;
    Enqueue(bfs_tree, program_data->tree);
    while(!VIDA_Q(bfs_tree))
    {
        node = Dequeue(bfs_tree);
        if(node->firstChild != NULL)
        {
            program_data->curr_node = node->firstChild;
            TArb p = program_data->curr_node;
            for(; p != NULL; p = p->nextSibling)
            {
                Enqueue(bfs_tree, p);
            }
        }
        if(strcmp(node->info->type, command) == 0)
        {
            DeleteStyleList(node);
            AppendStyleList(node, style);
            ok = 1;
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout,
        "Override style failed: no node found for selector %s!\n", command);
    }
}

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
//Parcurgerea nodurilor se face in latime, folosind o coada
void OverrideStyleAllClasses(programData program_data, char* command)
{
    int ok = 0;
    char  *aux, style[1000], class[100];
    aux = readStyle(program_data);
    strcpy(style, aux);
    strcpy(class, command + 1);
    free(aux);

    Queue bfs_tree = InitQ();
    TArb node;
    Enqueue(bfs_tree, program_data->tree);
    while(!VIDA_Q(bfs_tree))
    {
        node = Dequeue(bfs_tree);
        if(node->firstChild != NULL)
        {
            program_data->curr_node = node->firstChild;
            TArb p = program_data->curr_node;
            for(; p != NULL; p = p->nextSibling)
            {
                Enqueue(bfs_tree, p);
            }
        }
//parcurgerea listei de atribute, pentru a vedea in primul rand daca exista
//campul "class", si apoi daca acesta coincide cu cel care trebuie
        TAttr p = node->info->otherAttributes;
        for(; p != NULL; p = p->next)
        {
            if(strcmp(p->name, "class") == 0)
            {
                if(strcmp(p->value, class) == 0)
                {
                    DeleteStyleList(node);
                    AppendStyleList(node, style);
                    ok = 1;
                    break;
                }
            }
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout,
        "Override style failed: no node found for selector %s!\n", command);
    }
}

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
//Parcurgerea nodurilor se face in latime, folosind o coada
void OverrideStyleGivenClass(programData program_data, char* command)
{
    int ok = 0;
    char *aux, tag[100], class[100], style[1000];
    strcpy(tag, "");
    strcpy(class, "");
    aux = readStyle(program_data);
    strcpy(style, aux);

    free(aux);

    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == '.')
        {
            strcpy(class, command + i + 1);
            break;
        }
        sprintf(tag, "%s%c",tag, command[i]);
    }

    Queue bfs_tree = InitQ();
    TArb node;
    Enqueue(bfs_tree, program_data->tree);
    while(!VIDA_Q(bfs_tree))
    {
        node = Dequeue(bfs_tree);
        if(node->firstChild != NULL)
        {
            program_data->curr_node = node->firstChild;
            TArb p = program_data->curr_node;
            for(; p != NULL; p = p->nextSibling)
            {
                Enqueue(bfs_tree, p);
            }
        }
        if(strcmp(node->info->type, tag) != 0)
        {
            continue;
        }

        TAttr p = node->info->otherAttributes;
        for(; p != NULL; p = p->next)
        {
            if(strcmp(p->name, "class") == 0)
            {
                if(strcmp(p->value, class) == 0)
                {
                    DeleteStyleList(node);
                    AppendStyleList(node, style);
                    ok = 1;
                    break;
                }
            }
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout,
        "Override style failed: no node found for selector %s!\n", command);
    }
}

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru
//Parcurgerea nodurilor se face in latime, folosind o coada
void OverrideStyleChild(programData program_data, char* command)
{
    int ok = 0;
    char *aux, parent[100], child[100], style[1000];
    strcpy(child, "");
    strcpy(parent, "");
    aux = readStyle(program_data);
    strcpy(style, aux);
    free(aux);

//citirea parintelui si a copilului
    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == '>')
        {
            strcpy(child, command + i + 1);
            break;
        }
        sprintf(parent, "%s%c", parent, command[i]);
    }

    Queue bfs_tree = InitQ();
    TArb node;
    Enqueue(bfs_tree, program_data->tree);
    while(!VIDA_Q(bfs_tree))
    {
        node = Dequeue(bfs_tree);
        if(node->firstChild != NULL)
        {
            program_data->curr_node = node->firstChild;
            TArb p = program_data->curr_node;
            for(; p != NULL; p = p->nextSibling)
            {
                if(strcmp(node->info->type, parent) == 0 &&
                    strcmp(p->info->type, child) == 0)
                {
                    DeleteStyleList(p);
                    AppendStyleList(p, style);
                    ok = 1;
                }
                Enqueue(bfs_tree, p);
            }
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout,
        "Override style failed: no node found for selector %s!\n", command);
    }
}

//functie care face override cu un anumit style tuturor nodurilor care au
//o anumita clasa, primita ca si parametru, nodurilor cu un anumit tag type
//Parcurgerea nodurilor se face in latime, folosind o coada
void OverrideStyleAllDescents(programData program_data, char* command)
{
    int ok = 0;
    char *aux, ancestor[100], descent[100], style[1000];
    strcpy(ancestor, "");
    strcpy(descent, "");
    aux = readStyle(program_data);
    strcpy(style, aux);
    free(aux);

//aici se realizeaza citirea numelui stramosului, respectiv a descendentului
    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == ' ')
        {
            strcpy(descent, command + i + 1);
            break;
        }
        sprintf(ancestor, "%s%c", ancestor, command[i]);
    }

    Queue bfs_tree = InitQ();
    Queue bfs_descendents = InitQ();
    TArb node, descent_node;
    Enqueue(bfs_tree, program_data->tree);
    while(!VIDA_Q(bfs_tree))
    {
        node = Dequeue(bfs_tree);
        if(node->firstChild != NULL)
        {
            program_data->curr_node = node;
            if(strcmp(node->info->type, ancestor) == 0)
            {
                Enqueue(bfs_descendents, node);

//parcurgere BFS pentru gasirea stramosului cu tag type-ul dat;
//Dupa gasirea acestuia, vom face separat o parcurgere BFS pe el, pentru
//a-i gasi toti mostenitorii ce respecta conditia de a avea un anume tag type
                while(!VIDA_Q(bfs_descendents))
                {
                    descent_node = Dequeue(bfs_descendents);
                    if(descent_node->firstChild != NULL)
                    {
                        program_data->curr_node = descent_node->firstChild;
                        TArb p = program_data->curr_node;
                        for(; p != NULL; p = p->nextSibling)
                        {
                            Enqueue(bfs_descendents, p);
                        }
                    }
                    if(descent_node == node)
                    {
                        continue;
                    }
                    if(strcmp(descent_node->info->type, descent) == 0)
                    {
                        DeleteStyleList(descent_node);
                        AppendStyleList(descent_node, style);
                        ok = 1;
                    }
                }
            }
            else
            {
                if(node->firstChild != NULL)
                {
                    program_data->curr_node = node->firstChild;
                    TArb p = program_data->curr_node;
                    for(; p != NULL; p = p->nextSibling)
                    {
                        Enqueue(bfs_tree, p);
                    }
                }
            }
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    DestroyQ(bfs_descendents, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout,
        "Override style failed: no node found for selector %s!\n", command);
    }
}