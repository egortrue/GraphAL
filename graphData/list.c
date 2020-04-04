#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void add2list(Node **pphead, int val){
    Node **pp = pphead, *pnew;

    while(*pp)
    {
        if(val < (*pp)->value)
            break;
        else
            pp = &((*pp)->pnext);
    }

    pnew = (Node*)malloc(sizeof(Node));
    pnew->value = val;
    pnew->pnext = *pp;
    *pp = pnew;
}

void print(Node *phead, FILE *output){
    Node* p = phead;
    while(p)
    {
        fprintf(output,"%d ", p->value);
        p = p->pnext;
    }
    fprintf(output, "\n");
}

void deleteList(Node *phead){
    if(phead)
    {
        deleteList(phead->pnext);
        if(phead)
            free(phead);
    }
}