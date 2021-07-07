#include "ConstructTree.h"

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse contents; in functie de urmatorul caracter, se schimba
//current state cu next state;
void ParseContents(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
    
    if(program_data->state == PARSE_CONTENTS)
    {
        if(character == '\n')
        {
            return;
        }
        //se adauga prima litera in campul contents
        sprintf(program_data->curr_node->info->contents, "%s%c",
           program_data->curr_node->info->contents, character);
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse opening bracket; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseOpeningBracket(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);

    if(program_data->state == PARSE_TAG_TYPE)
    {
        //se creeaza noul nod
        TArb new_node = AllocNode();
        //daca este nodul radacinia
        if(program_data->tree == NULL)
        {
            program_data->tree = new_node;
            program_data->tree->info->id = calloc(MAX_COMMAND_LEN,sizeof(char));
            strcpy(program_data->tree->info->id, program_data->curr_id);
            program_data->curr_node = program_data->tree;
            
            sprintf(program_data->curr_node->info->type, "%c", character);
            return;
        }
        //pentru orice alt nod, generare ID si inserare in arbore
        new_node->info->id = GenerateNodeID(program_data->curr_node);
        new_node->parent = program_data->curr_node;
        InsertIntoTree(program_data->curr_node, new_node);
        program_data->curr_node = new_node;
        strcpy(program_data->curr_id, new_node->info->id);

        sprintf(program_data->curr_node->info->type, "%c", character);
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse tag type; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseTagType(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
    //se incepe citirea tag type-ului
    if(program_data->state == PARSE_TAG_TYPE)
    {
        sprintf(program_data->curr_node->info->type, "%s%c",
            program_data->curr_node->info->type, character);
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse closing tag; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseClosingTag(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
    
    //se inchide tagul curent, noul tag curent devine tatal lui
    if(program_data->state == PARSE_CONTENTS)
    {
        TArb curr_node_father = SearchingForFather(program_data->curr_node,
            program_data->tree);
        
        program_data->curr_node = curr_node_father;
        strcpy(program_data->curr_id, curr_node_father->info->id);
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse rest of tag; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseRestOfTag(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);

    //se citeste primul caracter al atributului
    if(program_data->state == PARSE_ATTRIBUTE_NAME)
    {
        sprintf(program_data->attr_name, "%s%c",
    program_data->attr_name, character);
    
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse attribute name; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseAttributeName(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
    
    //se citeste tot numele atributului
    if(program_data->state == PARSE_ATTRIBUTE_NAME)
    {
        sprintf(program_data->attr_name, "%s%c",
            program_data->attr_name, character);
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse attribute eq; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseAttributeEq(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
}


//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse attribute value; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseAttributeValue(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
    //se citeste valoarea atributului
    if(program_data->state == PARSE_ATTRIBUTE_VALUE)
    {
        sprintf(program_data->attr_value, "%s%c",
            program_data->attr_value, character);
    }
    else
    {
        //daca se incadreaza la campul style, intra aici
        if(strcmp(program_data->attr_name, "style") == 0)
        {
            MakeStyleList(program_data->curr_node, program_data->attr_value);
        }
        //pentru campul otherAttributes
        else
        {
            TAttr attr = MakeAttribute(program_data->attr_name,
                program_data->attr_value);
            InsertAttribute(program_data->curr_node, attr);
            attr = program_data->curr_node->info->otherAttributes;
        }
        strcpy(program_data->attr_name, "");
        strcpy(program_data->attr_value, "");
    }
}

//Functie apelata in momentul in care current state-ul din cadrul problemei
//a ajuns pe parse self closing; in functie de urmatorul caracter,
//se schimba current state cu next state;
void ParseSelfClosing(programData program_data, char character)
{
    program_data->state = Interpret(program_data->state, character);
    if(program_data->state == PARSE_CONTENTS)
    {
        //marcare nod care se inchide singur
        program_data->curr_node->info->isSelfClosing = 1;
        //se inchide tagul curent, noul tag curent devine tatal lui
        TArb curr_node_father = SearchingForFather(program_data->curr_node,
            program_data->tree);
        
        program_data->curr_node = curr_node_father;
        strcpy(program_data->curr_id, curr_node_father->info->id);
    }
}

//Functie care citeste rand pe rand fiecare caracter din fisierul html,
//pana la finalizarea acestuia, si il prelucreaza in functie de
//state-ul pe care acesta intra
void ConstructTree(programData program_data)
{   
    while(!feof(program_data->html_file))
    {
        char character;
        fscanf(program_data->html_file, "%c", &character);
        if(program_data->state == PARSE_CONTENTS)
        {
            ParseContents(program_data, character);
        }
        else if(program_data->state == PARSE_OPENING_BRACKET)
        {
            ParseOpeningBracket(program_data, character);
        }
        else if(program_data->state == PARSE_TAG_TYPE)
        {
            ParseTagType(program_data, character);
        }
        else if(program_data->state == PARSE_CLOSING_TAG)
        {
            ParseClosingTag(program_data, character);
        }
        else if(program_data->state == PARSE_REST_OF_TAG)
        {
            ParseRestOfTag(program_data, character);
        }
        else if(program_data->state == PARSE_ATTRIBUTE_NAME)
        {
            ParseAttributeName(program_data, character);
        }
        else if(program_data->state == PARSE_ATTRIBUTE_EQ)
        {
            ParseAttributeEq(program_data, character);
        }
        else if(program_data->state == PARSE_ATTRIBUTE_VALUE)
        {
            ParseAttributeValue(program_data, character);
        }
        else if(program_data->state == PARSE_SELF_CLOSING)
        {
            ParseSelfClosing(program_data, character);
        }
        else
        {
            break;
        }      
    }
    strcpy(program_data->tree->info->contents, "");
}
