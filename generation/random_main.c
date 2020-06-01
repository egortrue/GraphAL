#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random_graphs.h"

int main() {
    int v=0, e=0;
    FILE *input;
    FILE *output;
    input = fopen("RandomGrInput.txt", "rt");
    output = fopen("RandomGrOutput.txt", "wt");
    if (!input)
    {
        puts("Error open file for reading");
        return -2;
    }
    if (!output)
    {
        puts("Error open file for writing");
        return -2;
    }

    fscanf(input, "%d %d", &v, &e);
    if (e > v*(v - 1)/2){
        fprintf(output, "Wrong number of vertex");
        return 0;
    }

    srand(time(0));
    int r1 = rand() % 100;
    int r2 = rand() % 100;

    AMATRIX *graph = AMatrixSet(v);
    RandomGraph(e, v, graph, r1, r2);

    PrintRandMatrix(graph, v, output);
    PrintRandVertexList(graph, v, output);
    PrintRandAdjList(graph, v, output);
    PrintRandSNodEdg(graph, v, output);
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v);
    RandomOrientedGraph(e, v, graph, r1, r2);
    fprintf(output, "\nOriented graph:\n\n");
    PrintRandMatrix(graph, v, output);
    PrintRandVertexList(graph, v, output);
    PrintRandAdjList(graph, v, output);
    PrintRandSNodEdg(graph, v, output);
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v);
    RandConnectedOr(graph, e, v, r1, r2);
    fprintf(output, "\nConnected oriented graph:\n\n");
    PrintRandMatrix(graph, v, output);
    PrintRandVertexList(graph, v, output);
    PrintRandAdjList(graph, v, output);
    PrintRandSNodEdg(graph, v, output);
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v);
    graph = ConnectGraph(graph, e,  v, r1, r2);
    fprintf(output, "\nConnected non-oriented graph:\n\n");
    PrintRandMatrix(graph, v, output);
    PrintRandVertexList(graph, v, output);
    PrintRandAdjList(graph, v, output);
    PrintRandSNodEdg(graph, v, output);
    graph = AMatrixDelete(graph);

    //TestConnection(graph, e, v, r1, r2);

    fclose(input);
    fclose(output);
}