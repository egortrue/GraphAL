#ifndef GRAPHAL_RANDOM_GRAPHS_H
#define GRAPHAL_RANDOM_GRAPHS_H
#include "../src/graphs.h"
#include <stdio.h>

//ORIENTED GRAPHS
void RandomGraph(int e, int v, AMATRIX *graph, int r1, int r2);
void RandomOrientedGraph(int e, int v, AMATRIX *graph, int r1, int r2);

int RandConnectedOr(AMATRIX *g, int v, int n, int r1, int r2);
void dfs(AMATRIX *g, int v, int ost, int used[], int *count, int *comp, int *pr, int *cycle);
int ConnectGraph(AMATRIX *g, int v, int n);

//FOR TEST
void TestConnection(AMATRIX *graph, int e, int v, int r1, int r2);
void dfs2(AMATRIX *g, int v, int ost, int used[]);
void FindComp(AMATRIX *g, int v, int n);

//PRINT
void PrintRandMatrix(AMATRIX *graph, int v, FILE *output);
void PrintRandVertexList(AMATRIX *graph, int v, FILE *output);
void PrintRandAdjList(AMATRIX *graph, int v, FILE *output);
void PrintRandSNodEdg(AMATRIX *graph, int v, FILE *output);

#endif //GRAPHAL_RANDOM_GRAPHS_H
