#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int degree(struct matrix *m, int v){
    int countdegree = 0;
    for (int j = 0; j < m->n; j++){
        if (m->adj[v][j] != 0)
            countdegree++;
    }
    return countdegree;
}

double density(struct matrix *m){
    double dens = ((double)m->e/(double)(2*m->n*(m->n-1)));
    return dens;
}

void printGraph(struct matrix *m){
    for (int i = 0; i < m->n; i++){
        printf("\n");
        for (int j = 0; j < m->n; j++)
            printf("%d ", m->adj[i][j]);
    }
}

void printMatrixFile(struct matrix *m, FILE *output){

    for (int i = 0; i < m->n; i++){
        for (int j = 0; j < m->n; j++){
            fprintf(output, "%d ", m->adj[i][j]);
        }
        fprintf(output, "\n");
    }
}

//PARSER
void readMatrix(FILE *input){
    char strnew = fgetc(input);
    char *chr = NULL;
    chr = (char*)malloc(0*sizeof(char*));
    int line = 0, column = 0, j = 0;
    while (strnew != EOF) {
        if (strnew >= 48 && strnew <= 57){
            chr[j++] = strnew;
        }

        else if (strnew == ' ') {
            G.adj[line][column] = atoi(chr);
            if (G.adj[line][column] != 0)
                G.e++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
            column++;
        }

        else if (strnew == '\n') {
            G.adj[line][column] = atoi(chr);
            if (G.adj[line][column] != 0)
                G.e++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            column = 0;
            line++;
            j = 0;
        }

        strnew = fgetc(input);
    }
    G.adj[line][column] = atoi(chr);
    if (G.adj[line][column] != 0)
        G.e++;
}

struct matrix *graph_create(int nodes, FILE *input){

    struct matrix * tmp = &G;
    tmp->adj = (int**)malloc(nodes*sizeof(int*));
    tmp->n = nodes;
    for (int i = 0; i<G.n; i++){
        tmp->adj[i] = (int*)malloc(nodes*sizeof(int));
    }

    readMatrix(input);
    return tmp;
}

int countEdges(FILE *input){
    int lines_count = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c == '\n')
            lines_count++;
        c = fgetc(input);
    }
    lines_count++;
    rewind(input); //return back pointer in file
    return lines_count;
}

