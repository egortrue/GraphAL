#ifndef GRAPHAL_C_H
#define GRAPHAL_C_H

//------------------------------------------------------------------------------------------------------
typedef struct AList{
    int value;
    struct AList* pnext;
}Alist;


void AListAddValue(Alist **pphead, int val);
void AListPrint(Alist *phead, FILE *output);
void AListDelete(Alist *phead);
void AListRead(FILE* input, FILE* output);
int AListCountVertex(FILE *input);
double AListDensity(int v, int e);
int AListDegree(Alist *phead);
//------------------------------------------------------------------------------------------------------

#endif //GRAPHAL_C_H
