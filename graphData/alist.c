#include <stdio.h>
#include <stdlib.h>
#include "alist.h"

//------------------------------------------------------------------------------------------------------
void AListAddValue(Alist **pphead, int val)
{
    Alist **pp = pphead, *pnew;

    while(*pp)
    {
        if(val < (*pp)->value)
            break;
        else
            pp = &((*pp)->pnext);
    }

    pnew = (Alist*)malloc(sizeof(Alist));
    pnew->value = val;
    pnew->pnext = *pp;
    *pp = pnew;
}

void AListPrint(Alist *phead, FILE *output)
{
    Alist* p = phead;
    while(p)
    {
        fprintf(output,"%d ", p->value);
        p = p->pnext;
    }
    fprintf(output, "\n");
}

void AListDelete(Alist *phead)
{
    if(phead)
    {
        AListDelete(phead->pnext);
        if(phead)
            free(phead);
    }
}

//format:
// 1-1 2 3
//4-3 1 5
void AListRead(FILE* input, FILE* output)
{
    Alist **graph= (Alist**)calloc(AListCountVertex(input), sizeof(Alist*));
    int j = 0;
    char *chr = (char*)malloc(sizeof(char*));
    int line = 0;
    int count_edges = 0;
    char str_new;
    str_new = fgetc(input);

    while (str_new != EOF) {
        if (str_new >= 48 && str_new <= 57){
            chr = realloc(chr, j+1 * sizeof(char*));
            chr[j++] = str_new;
        }

        else if (str_new == '-') {
            line = atoi(chr);
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (str_new == ' ' && chr != 0) {
            AListAddValue(&graph[line], atoi(chr));
            count_edges++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (str_new == '\n') {
            AListAddValue(&graph[line], atoi(chr));
            count_edges++;
            line = 0;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        str_new = fgetc(input);
    }
    AListAddValue(&graph[line], atoi(chr));

    for (int i = 0; i < AListCountVertex(input) + 1; i++) {
        if (graph[i]) {
            fprintf(output, "%d-", i);
            AListPrint(graph[i], output);
        }
    }

    fprintf(output, "Density: %.2lf", AListDensity(AListCountVertex(input), count_edges + 1));
    fprintf(output, "\nDegree of v #5: %d", AListDegree(graph[5]));

    for (int i = 0; i < AListCountVertex(input); ++i)
        AListDelete(graph[i]);
}

double AListDensity(int v, int e)
{
    return ((double)e/(double)(v*(v-1)));
}

int AListDegree(Alist *phead)
{
    Alist* p = phead;
    int count = 0;
    while(p)
    {
        count++;
        p = p->pnext;
    }
    return count;
}

int AListCountVertex(FILE *input)
{
    int lines_count = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c == '\n')
            lines_count++;
        c = fgetc(input);
    }
    lines_count++;
    rewind(input); //return back pointer in file
    return lines_count+1;
}
//------------------------------------------------------------------------------------------------------