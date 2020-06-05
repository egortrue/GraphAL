#ifndef GRAPHAL_RANDOM_GRAPHS_H
#define GRAPHAL_RANDOM_GRAPHS_H
#include <stdio.h>
#define DLL_EXPORT __declspec(dllexport)
//#define DLL_EXPORT
//Structures
//------------------------------------------------------------------------------------------------------

typedef struct aMatrix
{
    int** adj;
    int nodes_num; //Number of nodes in AMatrix.
    int edges_num; //Number of edges
}AMATRIX;

DLL_EXPORT AMATRIX *AMatrixSet           (int nodes_num, int edges_num);
DLL_EXPORT AMATRIX *AMatrixDelete        (AMATRIX *graph);

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
DLL_EXPORT AMATRIX *ChoiceRand(AMATRIX* g, int oriented, int weight_min, int weight_max);

//ORIENTED GRAPHS
DLL_EXPORT AMATRIX* RandomGraph(AMATRIX *graph, int weight_min, int weight_max);
AMATRIX* RandomOrientedGraph(AMATRIX *graph, int weight_min, int weight_max);

AMATRIX* RandConnectedOr(AMATRIX *g, int weight_min, int weight_max);
void dfs(AMATRIX *g, int nodes_num, int ost, int used[], int *count, int *comp, int *pr, int *cycle);
AMATRIX* ConnectGraph(AMATRIX *g, int weight_min, int weight_max);

//FOR TEST
void TestConnection(AMATRIX *graph, int e, int v, int r1, int r2);
void dfs2(AMATRIX *g, int v, int ost, int used[]);
void FindComp(AMATRIX *g, int v, int n);

//PRINT
void PrintRandMatrix(AMATRIX *graph, int nodes_num);

#endif //GRAPHAL_RANDOM_GRAPHS_H
