#ifndef GRAPHAL_C_H
#define GRAPHAL_C_H

//------------------------------------------------------------------------------------------------------
typedef struct AList{
    int value;
    struct AList* pnext;
}ALIST;


void AListAddValue(ALIST **pphead, int val);
void AListPrint(ALIST *phead, FILE *output);
void AListDelete(ALIST *phead);
void AListRead(FILE* input, FILE* output);
int AListCountVertex(FILE *input);
double AListDensity(int v, int e);
int AListDegree(ALIST *phead);
//------------------------------------------------------------------------------------------------------

#endif //GRAPHAL_C_H
