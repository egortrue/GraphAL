#include <stdlib.h>
#include "random_graphs.h"

//------------------------------------------------------------------------------------------------------
int AMatrixDegree(AMATRIX *adj_matrix, int v)
{
    int count_degree = 0;
    for (int j = 0; j < adj_matrix->nodes_num; j++){
        if (adj_matrix->adj[v][j] != 0) {
            count_degree++;
        }
    }
    return count_degree;
}

DLL_EXPORT AMATRIX *AMatrixSet(int nodes, int edges)
{
    AMATRIX *tmp = malloc(sizeof(AMATRIX));
    tmp->adj = (int**)calloc(nodes, sizeof(int*));
    tmp->nodes_num = nodes;
    tmp->edges_num = edges;
    for (int i = 0; i < tmp->nodes_num; i++){
        tmp->adj[i] = (int*)calloc(nodes, sizeof(int));
    }

    return tmp;
}

DLL_EXPORT AMATRIX *AMatrixDelete(AMATRIX *g){
    if (g) {
        g->edges_num = 0;
        for (int i = 0; i < g->nodes_num; i++)
                free(g->adj[i]);
        g->nodes_num = 0;
        free(g->adj);
        free(g);
    }
    return g;
}
//------------------------------------------------------------------------------------------------------
