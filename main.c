#include "Operations.h"

//functie care initializeaza datele programului nostru
//Am realizat o structura de tipul programData, in care sa tinem evidenta
//mai multor variabile, cum ar fi cele 3 fisiere text, 2 de intrare,
//respectiv unul de iesire, arborele propriu-zis, dar si elemente precum id-ul
//curent, nodul curent, si current_state-ul din cadrul problemei
void CreateProgramData(programData program_data)
{
    
    program_data->state = PARSE_CONTENTS;
    program_data->tree = NULL;
    
    program_data->html_file = fopen(program_data->argv[1], "r");
    if(!program_data->html_file)
    {
        printf("Can't open %s file\n", program_data->argv[1]);
    }

    program_data->fin = fopen(program_data->argv[2], "r");
    if(!program_data->fin)
    {
        printf("Can't open %s file\n", program_data->argv[2]);
    }

    program_data->fout = fopen(program_data->argv[3], "w");
    if(!program_data->fout)
    {
        printf("Can't open %s file\n", program_data->argv[3]);
    }

    strcpy(program_data->curr_id, "0");
    strcpy(program_data->attr_name, "");
    strcpy(program_data->attr_value, "");
}

//Functie care in mare sterge toate datele din cadrul arborelui si inchide
//toate fisiere ce trebuie inchise
void DestroyProgramData(programData program_data)
{
    DeleteSubtree(program_data->tree);
    fclose(program_data->fin);
    fclose(program_data->fout);
    fclose(program_data->html_file);
}

//Functia main, care apeleaza toate functiile mari ale programului, pentru a
//indeplini toate cerintele
int main(int argc, char* argv[])
{
    //aceasta este structura in care tinem toate datele problemei
    TData program_data;
    program_data.argv = argv;

    //functia care intializeaza datele programului nostru
    CreateProgramData(&program_data);
    
    //functia care creeaza arborele
    ConstructTree(&program_data);

    //functia care indeplineste toate operatiile din fisierul de comenzi
    OperationSelection(&program_data);

    //functia care elibereaza toata memoria din cadrul programului
    DestroyProgramData(&program_data);

    return 0;
}