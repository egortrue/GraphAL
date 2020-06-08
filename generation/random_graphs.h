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
int AMatrixDegree                        (AMATRIX *adj_matrix, int v);

//------------------------------------------------------------------------------------------------------
DLL_EXPORT AMATRIX *ChoiceRand           (AMATRIX *g, int oriented, int weight_min, int weight_max, int max_degree);

//RANDOM GRAPHS
DLL_EXPORT AMATRIX* RandomGraph          (AMATRIX *graph, int weight_min, int weight_max, int max_degree);

AMATRIX* RandConnectedOrGraph            (AMATRIX *g, int weight_min, int weight_max, int max_degree);
AMATRIX* RandConnectedGraph              (AMATRIX *g, int weight_min, int weight_max, int max_degree);

void     dfs                             (AMATRIX *g, int node_num, int used[], int *count, int *comp, int *pr,
                                          int *cycle);

//PRINT
void     PrintRandMatrix                  (AMATRIX *graph, int nodes_num);

#endif //GRAPHAL_RANDOM_GRAPHS_H
