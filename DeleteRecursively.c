#include "DeleteRecursively.h"

//functie care face stergerea recursiva a tuturor nodurilor din cadrul unui
//subarbore
void DeleteSubtree(TArb node)
{
    if(node == NULL)
    {
        return;
    }
    if(node->firstChild != NULL)
    {
        DeleteSubtree(node->firstChild);
    }
    if(node->nextSibling != NULL)
    {
        DeleteSubtree(node->nextSibling);
    }

    DeleteNode(node);
    node = NULL;
}

//functie care genereaza noile ID-uri pentru siblings ai nodului sters:
//de exemplu, daca node->nextSibling->id == 2.2.3 si se doreste stergerea 
//nodului node, cu id == 2.2.2, si care, sa zicem, are 3 "rude" dupa el,
//dupa stergerea acestuia, rudele vor avea 2.2.2, 2.2.3 si 2.2.4(in loc de
//2.2.3, 2.2.4, 2.2.5)
void NewIDs(TArb node, TArb next_node)
{
    if(next_node == NULL)
    {
        return;
    }
    if(next_node->nextSibling != NULL)
    {
        NewIDs(next_node, next_node->nextSibling);
    }

    strcpy(next_node->info->id, node->info->id);
}

//functie care face stergera a ultimului copil a unui nod parinte,
//intrucat la stergerea ultimului copil, nu mai e nevoie de reconstruirea
//id-urilor din cadrul arborelui, acestea ramanand neschimbate
void DeleteLastNode(programData program_data, TArb node)
{
    TArb p;
    TArb father = SearchingForFather(node, program_data->tree);
                
    for(p = father->firstChild; p->nextSibling != node; p = p->nextSibling)
    {
        if(p == node)
        {
            break;
        }
    }
    p->nextSibling = NULL;
    //cazul in care parintele are un singur fiu, dupa stergere, campul
    //firstChild va fi NULL
    if(father->firstChild == node)
    {
        father->firstChild = NULL;
    }
    DeleteSubtree(node);
    node = NULL;
}

//functie care face stergerea oricarui nod din lista de copii ai unui nod
//parinte, mai putin ultimul nod din cadrul acestei liste, caz in care va
//fi apelata functia DeleteLastNode
void DeleteMiddleNode(programData program_data, TArb node)
{
    Queue id_modified = InitQ();
    TArb aux1, prev_node, father, next_node, node_rename;
    //retinem nodul separat, intr-o alta variabila, pentru a nu o pierde
    aux1 = node;
    father = SearchingForFather(node, program_data->tree);

    //se cauta nodul precedent nodului de sters, si se retine
    for(prev_node = father->firstChild; prev_node->nextSibling != node; 
        prev_node = prev_node->nextSibling)
    {
        if(prev_node == node)
        {
            break;
        }
    }
    //retinem si nodul urmator
    next_node = node->nextSibling;
    //facem noua legatura intre nodul precedent si cel urmator
    prev_node->nextSibling = next_node;

    //se creeaza noile noduri pentru nextSibling-urile nodului de sters
    NewIDs(node, next_node);

    //printr-o parcurgere BFS, vom trece prin toti urmasii tutror nodurilor
    //nextSibling ale nodului de sters, si le vom schimba ID-ul vechi
    //intr-un ID corespunzator
    for(aux1 = next_node; aux1 != NULL; aux1 = aux1->nextSibling)
    {
        if(aux1->firstChild != NULL)
        {
            Enqueue(id_modified, aux1->firstChild);
        }
        while(!VIDA_Q(id_modified))
        {
            node_rename = Dequeue(id_modified);
            if(node_rename->firstChild != NULL)
            {
                TArb p = node_rename->firstChild;
                for(; p != NULL; p = p->nextSibling)
                {
                    Enqueue(id_modified, p);
                }
            }
            //creare si atribuire noul ID, care nu este altceva decat ID-ul 
            //parintelui si ultima chestie dupa punct de la ID-ul vechi
            father = node_rename->parent;
            int length = strlen(node_rename->info->id), dot_pos;
            char last_characters[100];
            strcpy(last_characters, "");
            for(int i = 0; i < length; i++)
            {
                if(node_rename->info->id[i] == '.')
                {
                    dot_pos = i;
                }
            }
            for(int i = dot_pos; i < length; i++)
            {
                sprintf(last_characters, "%s%c",
                    last_characters, node_rename->info->id[i]);
            }
            strcpy(node_rename->info->id, father->info->id);
            sprintf(node_rename->info->id, "%s%s",
                node_rename->info->id, last_characters);
        }
    }
    node->nextSibling = NULL;
    father = node->parent;
    node->parent = NULL;
    
    //in cazul in care nodul de sters este chiar nodul firstChild, atunci
    //father trebuie sa puncteze in alta zona de memorie, la next_node
    if(father->firstChild == node)
    {
        father->firstChild = next_node;
    }
    DeleteSubtree(node);
    aux1 = NULL;
    DestroyQ(id_modified, SimpleFree);
}

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu un anumit id; Cautarea nodului se face prin functia de cautare
//SmartNodeSearch
void DeleteRecursivelyID(programData program_data, char* command)
{
    int ok = 0;
    char ID[100];
    strcpy(ID, command + 1);
    TArb node = SmartNodeSearch(program_data->tree, ID);

    if(node->nextSibling == NULL)
    {
        DeleteLastNode(program_data, node);
        ok = 1;
    }    
    else
    {
        DeleteMiddleNode(program_data, node);
        ok = 1;
    }

    if(ok == 0)
    {
        fprintf(program_data->fout, 
        "Delete recursively failed: no node found for selector %s!\n",
        command);
    }
}

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu o anumit tag type;
//Parcurgerea nodurilor se face in latime, folosind o coada
void DeleteRecursivelyAllTags(programData program_data, char* command)
{
    int ok = 0;
    Queue bfs_tree = InitQ();

    TArb node, next_node;
    Enqueue(bfs_tree, program_data->tree);
    
    while(!VIDA_Q(bfs_tree))
    {     
        node = Dequeue(bfs_tree);

        //pentru nodurile care se afla in capat si respecta conditia
        if(node->nextSibling == NULL)
        {
            if(strcmp(node->info->type, command) == 0)
            {
                DeleteLastNode(program_data, node);
                ok = 1;
                continue;
            }
        }
        next_node = node;       
        
        //pentru restul nodurilor care respecta conditia
        if(strcmp(node->info->type, command) == 0)
        {
            DeleteMiddleNode(program_data, node);
            ok = 1;
            continue;
        }
        node = next_node;

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
    DestroyQ(bfs_tree, SimpleFree);

    if(ok == 0)
    {
        fprintf(program_data->fout, 
        "Delete recursively failed: no node found for selector %s!\n",
        command);
    }
}

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu o anumita clasa;
//Parcurgerea nodurilor se face in latime, folosind o coada
void DeleteRecursivelyAllClasses(programData program_data, char* command)
{
    int ok = 0;
    char class[100];
    strcpy(class, command + 1);

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
                    if(node->nextSibling == NULL)
                    {
                        DeleteLastNode(program_data, node);
                        ok = 1;
                    }    
                    else
                    {
                        DeleteMiddleNode(program_data, node);
                        ok = 1;
                    }
                    break;
                }
            }
        }
    }
    if(ok == 0)
    {
        fprintf(program_data->fout, 
        "Delete recursively failed: no node found for selector %s!\n",
        command);
    }
    DestroyQ(bfs_tree, SimpleFree);

}

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod cu o anumita clasa si un anumit tag type
//Parcurgerea nodurilor se face in latime, folosind o coada
void DeleteRecursivelyGivenClass(programData program_data, char* command)
{
    int ok = 0;
    char tag[100], class[100];
    
    strcpy(tag, "");
    strcpy(class, "");

//citirea tagului si a clasei
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
//parcurgerea listei de atribute, pentru a vedea in primul rand daca exista
//campul "class", si apoi daca acesta coincide cu cel care trebuie
        TAttr p = node->info->otherAttributes;
        for(; p != NULL; p = p->next)
        {
            if(strcmp(p->name, "class") == 0)
            {
                if(strcmp(p->value, class) == 0)
                {
                    if(node->nextSibling == NULL)
                    {
                        DeleteLastNode(program_data, node);
                        ok = 1;
                    }    
                    else
                    {
                        DeleteMiddleNode(program_data, node);
                        ok = 1;
                    }
                    break;
                }
            }
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout, 
        "Delete recursively failed: no node found for selector %s!\n",
        command);
    }
}

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod care este fiul unui anume nod parinte
//Parcurgerea nodurilor se face in latime, folosind o coada
void DeleteRecursivelyChild(programData program_data, char* command)
{
    int ok = 0;
    char parent[100], child[100];
    strcpy(child, "");
    strcpy(parent, "");

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
                    if(p->nextSibling == NULL)
                    {
                        DeleteLastNode(program_data, p);
                        ok = 1;
                    }    
                    else
                    {
                        DeleteMiddleNode(program_data, p);
                        ok = 1;
                    }
                    break;
                }
                Enqueue(bfs_tree, p);
            }
        }
    }
    DestroyQ(bfs_tree, SimpleFree);
    if(ok == 0)
    {
        fprintf(program_data->fout, 
        "Delete recursively failed: no node found for selector %s!\n",
        command);
    }
}

//Functie care realizeaza stergerea unui intreg subarbore, pornind de la un
//nod care este descendetul unui anumit stramos
//Parcurgerea nodurilor se face in latime, folosind o coada
void DeleteRecursivelyAllDescents(programData program_data, char* command)
{
    int ok = 0;
    char ancestor[100], descent[100];
    strcpy(ancestor, "");
    strcpy(descent, "");

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

//parcurgere BFS pentru gasirea stramosului cu tag type-ul dat;
//Dupa gasirea acestuia, vom face separat o parcurgere BFS pe el, pentru
//a-i gasi toti mostenitorii ce respecta conditia de a avea un anume tag type
    while(!VIDA_Q(bfs_tree))
    {   
        node = Dequeue(bfs_tree);
        if(node->firstChild != NULL)
        {
            program_data->curr_node = node;
            if(strcmp(node->info->type, ancestor) == 0)
            {
                Enqueue(bfs_descendents, node);
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
                        if(descent_node->nextSibling == NULL)
                        {
                            DeleteLastNode(program_data, descent_node);
                            ok = 1;
                        }    
                        else
                        {
                            DeleteMiddleNode(program_data, descent_node);
                            ok = 1;
                        }
                        continue;
                    }
                }
            }
            //aici se observa ca, pentru eficienta, vom sari deja peste nodurile
            //parcurse la BFS-ul mostenitorilor stramosului, intrucat ei deja
            //au fost "testati", asa ca nu mai are rost inca o data
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
        "Delete recursively failed: no node found for selector %s!\n",
        command);
    }
}