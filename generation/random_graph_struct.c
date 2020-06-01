#include <stdlib.h>
#include <stdio.h>
#include "random_graphs.h"

//------------------------------------------------------------------------------------------------------
int AMatrixDegree(AMATRIX *adj_matrix, int v)
{
    int countdegree = 0;
    for (int j = 0; j < adj_matrix->n; j++){
        if (adj_matrix->adj[v][j] != 0)
            countdegree++;
    }
    return countdegree;
}

double AMatrixDensity(AMATRIX *adj_matrix)
{
    double dens = ((double)adj_matrix->e / (double)(2 * adj_matrix->n * (adj_matrix->n - 1)));
    return dens;
}

void AMatrixPrint(AMATRIX *m, FILE *output)
{
    for (int i = 0; i < m->n; i++){
        for (int j = 0; j < m->n; j++){
            fprintf(output, "%d ", m->adj[i][j]);
        }
        fprintf(output, "\n");
    }
}

DLL_EXPORT AMATRIX *AMatrixSet(int nodes)
{
    AMATRIX * tmp = (AMATRIX*)malloc(sizeof(AMATRIX));
    tmp->adj = (int**)malloc(nodes*sizeof(int*));
    tmp->n = nodes;
    tmp->e = 0;
    for (int i = 0; i < tmp->n; i++){
        tmp->adj[i] = (int*)calloc(nodes, sizeof(int));
    }

    return tmp;
}

AMATRIX *AMatrixDelete(AMATRIX *m){
    for (int i = 0; i < m->e; i++)
        for (int j = 0; j < m->e; j++)
            m->adj[i][j] = 0;
    return m;
}
//------------------------------------------------------------------------------------------------------
ALISTNODE* AListNewNode(int dest, int weight)
{
    ALISTNODE* newNode = (ALISTNODE*)malloc(sizeof(ALISTNODE));
    newNode->weight = weight;
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

ALISTG* AListGSet(int V)
{
    ALISTG* graph = (ALISTG*)malloc(sizeof(ALISTG));
    graph->V = V;
    graph->array = (ALIST*)calloc(V, sizeof(ALIST));
    return graph;
}

void AListEdgeAdd(ALISTG* graph, int src, int dest, int weight)
{
    ALISTNODE* newNode = AListNewNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

}

void AListPrint(ALISTG* graph, FILE* output)
{
    fprintf(output, "[nodes]\n");
    for (int v = 0; v < graph->V; v++) {
        ALISTNODE *pointer = graph->array[v].head;
        if (pointer)
            fprintf(output, "%d %d\n", v, 1);
    }

    fprintf(output, "\n[edges]\n");
    for (int v = 0; v < graph->V; v++) {
        ALISTNODE *pointer = graph->array[v].head;
        if (pointer) {
            while (pointer) {
                fprintf(output, "%d %d %d\n", v,  pointer->dest, pointer->weight);
                pointer = pointer->next;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------
