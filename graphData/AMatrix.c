#include <stdio.h>
#include <stdlib.h>
#include "AMatrix.h"

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

//PARSER
AMATRIX *AMatrixRead(FILE *input, AMATRIX *adj_matrix)
{
    char str_new = fgetc(input);
    char *chr = NULL;
    chr = (char*)malloc(0*sizeof(char*));
    int line = 0, column = 0, j = 0;
    while (str_new != EOF) {
        if (str_new >= 48 && str_new <= 57){
            chr[j++] = str_new;
        }

        else if (str_new == ' ') {
            adj_matrix->adj[line][column] = atoi(chr);
            if (adj_matrix->adj[line][column] != 0)
                adj_matrix->e++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
            column++;
        }

        else if (str_new == '\n') {
            adj_matrix->adj[line][column] = atoi(chr);
            if (adj_matrix->adj[line][column] != 0)
                adj_matrix->e++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            column = 0;
            line++;
            j = 0;
        }

        str_new = fgetc(input);
    }
    adj_matrix->adj[line][column] = atoi(chr);
    if (adj_matrix->adj[line][column] != 0)
        adj_matrix->e++;

    return adj_matrix;
}

int AMatrixCountNodes(FILE *input)
{
    int count_lines = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c == '\n')
            count_lines++;
        c = fgetc(input);
    }
    count_lines++;
    rewind(input); //return back pointer in file
    return count_lines;
}

AMATRIX *AMatrixSet(int nodes)
{
    AMATRIX * tmp = (AMATRIX*)malloc(nodes * sizeof(AMATRIX));
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
