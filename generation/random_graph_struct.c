#include <stdlib.h>
#include <stdio.h>
#include "random_graphs.h"

//------------------------------------------------------------------------------------------------------

DLL_EXPORT AMATRIX *AMatrixSet(int nodes, int edges)
{
    AMATRIX * tmp = (AMATRIX*)calloc(0, sizeof(AMATRIX));
    tmp->adj = (int**)malloc(nodes* sizeof(int*));
    tmp->nodes_num = nodes;
    tmp->edges_num = edges;
    for (int i = 0; i < tmp->nodes_num; i++){
        tmp->adj[i] = (int*)calloc(nodes, sizeof(int));
    }

    return tmp;
}

DLL_EXPORT AMATRIX *AMatrixDelete(AMATRIX *m){

    if (m) {
        for (int i = 0; i < m->edges_num; i++)
            for (int j = 0; j < m->edges_num; j++)
                m->adj[i][j] = 0;

    }
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
