#include "Operations.h"

//Functia care face adaugarea unui nou nod in cadrul arborelui, in cazul
//in care se gaseste nodul care sa poata fi parintele lui
void ADD(programData program_data)
{
    char command[100], new_command[100];
    //alocarea arborelui
    TArb new_node = AllocNode();
    fscanf(program_data->fin, "%s", command);
    strcpy(program_data->curr_id, command + 3);

    program_data->curr_node = SmartNodeSearch(program_data->tree,
        program_data->curr_id);
    
    //daca nu s-a gasit nodul la care ar trebui adaugat noul nod,
    //atunci nu se mai adauga si se elibereaza alocarea facuta anterior
    if(program_data->curr_node == NULL)
    {
        fprintf(program_data->fout,
            "Add tag failed: node with id %s not found!\n",
            program_data->curr_id);
        DeleteNode(new_node);
        fscanf(program_data->fin, "%[^\n]", command);
        return;
    }

    //in cazul in care se gaseste nodul, i se creeaza un ID
    new_node->info->id = GenerateNodeID(program_data->curr_node);
    new_node->parent = program_data->curr_node;

    //si se adauga in cadrul arborelui
    InsertIntoTree(program_data->curr_node, new_node);
    program_data->curr_node = new_node;

    //dupa care, daca are content, i se pune si acesta
    fscanf(program_data->fin, "%[^\n]", command);
    strcpy(new_command, command + 11);
    int length = strlen(new_command), ok = 0;
    //o simpla parsare a contentului + append la node-.info->contents
    for(int i = 0; i < length; i++)
    {
        if(new_command[i] == '>')
        {
            ok = 1;
            continue;
        }
        else if(new_command[i] == '<')
        {
            ok = 2;
            continue;
        }
        
        if(ok == 0)
        {
            sprintf(program_data->curr_node->info->type, "%s%c",
                program_data->curr_node->info->type, new_command[i]);
        }
        else if(ok == 1)
        {
            if(new_command[i] != ' ')
            {
                sprintf(program_data->curr_node->info->contents, "%s%c",
                    program_data->curr_node->info->contents, new_command[i]);
            }
        }
        else
        {
            break;
        }   
    }
}

//Functie care face afisarea unui tag intreg, cu numarul de taburi necesare;
//printarea consta in afisarea numelui tagului (a.k.a type), a listei de 
//atribute specifice style-ului, apoi afisarea restului de atribute, iar,
//la final, contents
void PrintTag(programData program_data, TArb node, int nr_tabs)
{
    for(int i = 0; i < nr_tabs; i++)
    {
        fprintf(program_data->fout, "\t");
    }
    if(node->info->style == NULL && node->info->otherAttributes == NULL)
    {
        fprintf(program_data->fout, "<%s>", node->info->type);
    }
    else
    {
        fprintf(program_data->fout, "<%s", node->info->type);
    
        if(node->info->style != NULL)
        {
            fprintf(program_data->fout," style=\"");
            for(TAttr p = node->info->style; p != NULL; p = p->next)
        
            {
                fprintf(program_data->fout,"%s: %s;", p->name, p->value);
        
                if(p->next != NULL)
                {
                    fprintf(program_data->fout, " ");
                }
            }
            fprintf(program_data->fout, "\"");
        }

        if(node->info->otherAttributes != NULL)
        {
            for(TAttr p = node->info->otherAttributes; p != NULL; p = p->next)
        
            {
                fprintf(program_data->fout," %s=\"%s\"", p->name, p->value);
            }
        }
        fprintf(program_data->fout, ">");
    }
    
    fprintf(program_data->fout, "\n");
    if(strcmp(node->info->contents, "") != 0)
    {
        for(int i = 0; i < nr_tabs + 1; i++)
        {
            fprintf(program_data->fout, "\t");
        }
        fprintf(program_data->fout, "%s\n", node->info->contents);
    }
}

//functie care realizeaza printarea unui tag de tipul self closing,
//intrucat acestea nu prezinta un element de inchidere dupa printarea
//tuturor atributelor, si de asemenea nici campul contents (deoarece se
//inchid inainte de acesta)
void PrintSelfClosingTag(programData program_data, TArb node, int nr_tabs)
{
    for(int i = 0; i < nr_tabs; i++)
    {
        fprintf(program_data->fout, "\t");
    }
    if(node->info->style == NULL && node->info->otherAttributes == NULL)
    {
        fprintf(program_data->fout, "<%s>", node->info->type);
    }
    else
    {
        fprintf(program_data->fout, "<%s", node->info->type);
    
        if(node->info->style != NULL)
        {
            fprintf(program_data->fout," style=\"");
            for(TAttr p = node->info->style; p != NULL; p = p->next)
        
            {
                fprintf(program_data->fout,"%s: %s;", p->name, p->value);
        
                if(p->next != NULL)
                {
                    fprintf(program_data->fout, " ");
                }
            }
            fprintf(program_data->fout, "\"");
        }

        if(node->info->otherAttributes != NULL)
        {
            for(TAttr p = node->info->otherAttributes; p != NULL; p = p->next)       
            {
                fprintf(program_data->fout," %s=\"%s\"", p->name, p->value);
            }
        }
        fprintf(program_data->fout, "/>");
    }
    fprintf(program_data->fout, "\n");
}

//Se afiseaza tagurile "normale", care nu sunt self Closing
//Pentru a evita un posibil Stack Overflow, am evitat printarea recursiva 
//a codului html, folosind o stiva pentru elementul de inchidere; s-ar putea
//sa fi fost mai mult cod scris, dar aceasta varianta nu mai depinde de
//memoria statica a programului, stiva generica personala fiind
//alocata dinamic
void NormalTags(programData program_data, int* nr_tabs, Stack ending_formula)
{
    PrintTag(program_data, program_data->curr_node, *nr_tabs);
    if(program_data->curr_node->firstChild != NULL)
    {
        (*nr_tabs)++;
        program_data->curr_node = program_data->curr_node->firstChild;
        Push(ending_formula, program_data->curr_node);
    }
    else
    {
        if(program_data->curr_node->nextSibling != NULL)
        {
            for(int i = 0; i < *nr_tabs; i++)
            {
                fprintf(program_data->fout,"\t");
            }
            
            fprintf(program_data->fout,"</%s>\n", 
                program_data->curr_node->info->type);
            Pop(ending_formula);
            program_data->curr_node = program_data->curr_node->nextSibling;
            Push(ending_formula, program_data->curr_node);
        }
        else
        {
            TArb eliminated_node = Pop(ending_formula);

            for(int i = 0; i < *nr_tabs; i++)
            {
                fprintf(program_data->fout,"\t");
            }
            fprintf(program_data->fout,"</%s>\n", 
                eliminated_node->info->type);
    
            TArb father = eliminated_node->parent;

            while(father->nextSibling == NULL)
            {
                if(strcmp(father->info->id, "0") == 0)
                {
                    Pop(ending_formula);
                    fprintf(program_data->fout, "</%s>",
                        father->info->type);
                    return;
                }
                program_data->curr_node = father;
                eliminated_node = Pop(ending_formula);
                for(int i = 0; i < *nr_tabs - 1; i++)
                {
                    fprintf(program_data->fout,"\t");
                }
                fprintf(program_data->fout,"</%s>\n", 
                    eliminated_node->info->type);
                (*nr_tabs)--;
                father = SearchingForFather(eliminated_node, program_data->tree);
            }
            for(int i = 0; i < *nr_tabs - 1; i++)
            {
                fprintf(program_data->fout,"\t");
            }
            Pop(ending_formula);
            fprintf(program_data->fout,"</%s>\n", 
                father->info->type);
            program_data->curr_node = father->nextSibling;
            Push(ending_formula, program_data->curr_node);
            (*nr_tabs)--;
        }
    }
}

//Functia care formateaza tot textul html, afisand cu tot cu taburi toate
//tagurile arborelui, fie ele self closing sau nu
void FORMAT(programData program_data)
{
    int nr_tabs = 0;
    Stack ending_formula = InitS();
    Push(ending_formula, program_data->curr_node);
    while(!VIDA_S(ending_formula))
    {   
        if(program_data->curr_node->info->isSelfClosing == 0)
        {
            NormalTags(program_data, &nr_tabs,ending_formula);
        }
        else
        {   
            PrintSelfClosingTag(program_data, program_data->curr_node, nr_tabs);
            if(program_data->curr_node->nextSibling != NULL)
            {
                Pop(ending_formula);
                program_data->curr_node = program_data->curr_node->nextSibling;
                Push(ending_formula, program_data->curr_node);
            }
            else
            {
                TArb father = SearchingForFather(
                    program_data->curr_node, program_data->tree);
                while(father->nextSibling == NULL)
                {
                    if(strcmp(father->info->id, "0") == 0)
                    {
                        Pop(ending_formula);
                        fprintf(program_data->fout, "</%s>",
                            father->info->type);
                        break;
                    }
                    program_data->curr_node = father;
                    for(int i = 0; i < nr_tabs - 1; i++)
                    {
                        fprintf(program_data->fout,"\t");
                    }
                    fprintf(program_data->fout,"</%s>\n", 
                        father->info->type);
                    Pop(ending_formula);       

                    father = SearchingForFather(father, program_data->tree);
                    nr_tabs--;
                }
                Pop(ending_formula);
                program_data->curr_node = father->nextSibling;
                
                nr_tabs--;
            }
        }
    }
    DestroyS(ending_formula, SimpleFree);
}

//functie care, in functie de selectorii primiti ca parametru din fisierul
//de comenzi, urmand ca mai apoi sa realizeze operatia de append
//pe style-ul unui nod (sau mai multor noduri daca e cazul), daca se gaseste
//cel putin un nod care sa indeplineasca conditia
void APPEND_STYLE(programData program_data)
{
    char character = '\0', command[100];
    strcpy(command, "");
    while(character != '\"')
    {
        fscanf(program_data->fin, "%c", &character);
    }
    fscanf(program_data->fin, "%c", &character);
    while(character != '\"')
    {
        sprintf(command, "%s%c", command, character);
        fscanf(program_data->fin, "%c", &character);
    }

    if(command[0] == '#')
    {
        AppendStyleID(program_data, command);
    }
    else if(command[0] == '.')
    {
        AppendStyleAllClasses(program_data, command);
    }
    else
    {
        int length = strlen(command);
        for(int i = 0; i < length; i++)
        {
            if(command[i] == ' ')
            {
                AppendStyleAllDescents(program_data, command);
                return;
            }
            else if(command[i] == '>')
            {
                AppendStyleChild(program_data, command);
                return;
            }
            else if(command[i] == '.')
            {
                AppendStyleGivenClass(program_data, command);
                return;
            }
        }
        AppendStyleAllTags(program_data, command);
    }
}

//functie care, in functie de selectorii primiti ca parametru din fisierul
//de comenzi, urmand ca mai apoi sa realizeze operatia de override
//pe style-ul unui nod (sau mai multor noduri daca e cazul), daca se gaseste
//cel putin un nod care sa indeplineasca conditia
void OVERRIDE_STYLE(programData program_data)
{
    char character = '\0', command[100];
    strcpy(command, "");
    while(character != '\"')
    {
        fscanf(program_data->fin, "%c", &character);
    }
    fscanf(program_data->fin, "%c", &character);
    while(character != '\"')
    {
        sprintf(command, "%s%c", command, character);
        fscanf(program_data->fin, "%c", &character);
    }

    if(command[0] == '#')
    {
        OverrideStyleID(program_data, command);
    }
    else if(command[0] == '.')
    {
        OverrideStyleAllClasses(program_data, command);
    }
    else
    {
        int length = strlen(command);
        for(int i = 0; i < length; i++)
        {
            if(command[i] == ' ')
            {
                OverrideStyleAllDescents(program_data, command);
                return;
            }
            else if(command[i] == '>')
            {
                OverrideStyleChild(program_data, command);
                return;
            }
            else if(command[i] == '.')
            {
                OverrideStyleGivenClass(program_data, command);
                return;
            }
        }
        OverrideStyleAllTags(program_data, command);
    }
}

//functie care, in functie de selectorii primiti ca parametru din fisierul
//de comenzi, urmand ca mai apoi sa realizeze operatia de stergere a
//unui nod (sau mai multor noduri daca e cazul), daca se gaseste
//cel putin un nod care sa indeplineasca conditia
void DELETE_RECURSIVELY(programData program_data)
{

    char character = '\0', command[100];
    strcpy(command, "");
    while(character != '\"')
    {
        fscanf(program_data->fin, "%c", &character);
    }
    fscanf(program_data->fin, "%c", &character);
    while(character != '\"')
    {
        sprintf(command, "%s%c", command, character);
        fscanf(program_data->fin, "%c", &character);
    }

    if(command[0] == '#')
    {
        DeleteRecursivelyID(program_data, command);
    }
    else if(command[0] == '.')
    {
        DeleteRecursivelyAllClasses(program_data, command);
    }
    else
    {
        int length = strlen(command);
        for(int i = 0; i < length; i++)
        {
            if(command[i] == ' ')
            {
                DeleteRecursivelyAllDescents(program_data, command);
                return;
            }
            else if(command[i] == '>')
            {
                DeleteRecursivelyChild(program_data, command);
                return;
            }
            else if(command[i] == '.')
            {
                DeleteRecursivelyGivenClass(program_data, command);
                return;
            }
        }
        DeleteRecursivelyAllTags(program_data, command);
    }
}

//functia care parseaza tot fisierul de comenzi, intrand pe diferite ramuri,
//in functie de comanda si de selectorul primit
void OperationSelection(programData program_data)
{
    int nr_operations;
    char command[20];
    fscanf(program_data->fin, "%d", &nr_operations);
    for(int i = 0; i < nr_operations; i++)
    {
        program_data->curr_node = program_data->tree;
        fscanf(program_data->fin, "%s", command);
        if(strcmp(command, "add") == 0)
        {
            ADD(program_data);
        }
        else if(strcmp(command, "format") == 0)
        {

            FORMAT(program_data);
            fprintf(program_data->fout, "\n");
        }
        else if(strcmp(command, "deleteRecursively") == 0)
        {
            DELETE_RECURSIVELY(program_data);
        }
        else if(strcmp(command, "appendStyle") == 0)
        {
            APPEND_STYLE(program_data);
        }
        else if(strcmp(command, "overrideStyle") == 0)
        {
            OVERRIDE_STYLE(program_data);
        }
        else
        {
            continue;
        }   
    }
}