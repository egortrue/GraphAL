#ifndef GRAPHAL_C_H
#define GRAPHAL_C_H

typedef struct node{
    int value;
    struct node* pnext;
} Node;


void add2list(Node **pphead, int val);
void print(Node *phead, FILE *output);
void deleteList(Node *phead);

#endif //GRAPHAL_C_H
