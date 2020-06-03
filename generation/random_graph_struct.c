#include <stdlib.h>
#include <stdio.h>
#include "random_graphs.h"

//------------------------------------------------------------------------------------------------------

 AMATRIX *AMatrixSet(int nodes, int edges)
{
    AMATRIX * tmp = malloc(sizeof(AMATRIX));
    tmp->adj = (int**)malloc(nodes* sizeof(int*));
    tmp->nodes_num = nodes;
    tmp->edges_num = edges;
    for (int i = 0; i < tmp->nodes_num; i++){
        tmp->adj[i] = calloc(nodes, sizeof(int));
    }

    return tmp;
}

 AMATRIX *AMatrixDelete(AMATRIX *m){

    if (m) {
        m->edges_num = 0;
        for (int i = 0; i < m->nodes_num; i++)
                free(m->adj[i]);
        m->nodes_num = 0;
        free(m->adj);
        free(m);
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
