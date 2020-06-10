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
#include <stdint.h>

// Choices must conform with Hull-Dobell Theorem
// Specifically:
// rand_new = (mult * rand_old + add) mod m
// 1.  "add" cannot be zero
// 2.  "add" and "mult" must be relatively prime
// 3.  "add" - 1 must be divisible by all prime factors of "mult"
// 4.  "add"  must be a multiple of 4, if "mult" is a multiple of 4

static uint64_t rand64 = 2395040858963409; // seed for my RNG routine
static uint64_t add  = 1442695040888963407; // Knuth MMIX
static uint64_t mult =  6364136223846793005; // Knuth MMIX
static uint64_t MAXINT_U64 = 0xFFFFFFFFFFFFFFFF; // 16 Fs

// notes:
// 0xFFFFFFFFFFFFFFC5 = largest 64 bit prime
// 0xFFFFFFFFFFFFFFFF = largest 64 bit number (unsigned)

double random64() {
    // implictly  m = 64 bits by using uint64_t
    rand64 = mult * rand64 + add;
    return (double) rand64/ MAXINT_U64;
    // result should be between 0 and 1
}
