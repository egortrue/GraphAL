#ifndef GRAPHAL_RANDOM_GRAPHS_H
#define GRAPHAL_RANDOM_GRAPHS_H
#include <stdio.h>
//#define DLL_EXPORT __declspec(dllexport)
//Structures
//------------------------------------------------------------------------------------------------------

typedef struct aMatrix
{
    int** adj;
    int n; //Number of nodes in AMatrix.
    int e; //Number of edges
}AMATRIX;

AMATRIX *AMatrixSet           (int nodes);
AMATRIX *AMatrixDelete        (AMATRIX *m);

//------------------------------------------------------------------------------------------------------
typedef struct AListNode
{
    int weight;
    int dest;
    struct AListNode* next;
} ALISTNODE;

typedef struct AList
{
    struct AListNode *head;
} ALIST;

typedef struct AListG
{
    int V; //
    struct AList* array;
} ALISTG;

ALISTNODE* AListNewNode(int dest, int weight);
ALISTG* AListGSet(int V);
void AListEdgeAdd(ALISTG* graph, int src, int dest, int weight);
void AListPrint(ALISTG* graph, FILE* output);

//------------------------------------------------------------------------------------------------------
void ChoiceRand(AMATRIX* g, int oriented, int edges_num, int nodes_num, int weight_min, int weight_max);

//ORIENTED GRAPHS
AMATRIX* RandomGraph(int edges_num, int nodes_num, AMATRIX *graph, int weight_min, int weight_max);
AMATRIX* RandomOrientedGraph(int edges_num, int nodes_num, AMATRIX *graph, int weight_min, int weight_max);

AMATRIX* RandConnectedOr(AMATRIX *g, int edges_num, int nodes_num, int weight_min, int weight_max);
void dfs(AMATRIX *g, int nodes_num, int ost, int used[], int *count, int *comp, int *pr, int *cycle);
AMATRIX* ConnectGraph(AMATRIX *g, int edges_num, int nodes_num, int r1, int r2);

//FOR TEST
void TestConnection(AMATRIX *graph, int e, int v, int r1, int r2);
void dfs2(AMATRIX *g, int v, int ost, int used[]);
void FindComp(AMATRIX *g, int v, int n);

//PRINT
void PrintRandMatrix(AMATRIX *graph, int nodes_num);

#endif //GRAPHAL_RANDOM_GRAPHS_H
