#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../graphData/AMatrix.h"

void RandomGraphPrint(AMATRIX *graph, int v, FILE *output)
{
    fprintf(output, "Adjacency matrix: \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            fprintf(output, "%d ", graph->adj[i][j]);
        }
        fprintf(output, "\n");
    }

    int flag = 0;
    fprintf(output, "\nAdjacency list: \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            if (graph->adj[i][j] == 1) {
                if (flag == 0) {
                    fprintf(output, "%d - ", i);
                    flag = 1;
                }
                fprintf(output, "%d ", j);
            }
        }
        if (flag == 1) {
            flag = 0;
            fprintf(output, "\n");
        }
    }

    fprintf(output, "\nVertex List \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            if (graph->adj[i][j] == 1)
                fprintf(output, "%d %d \n", i, j);
        }
    }
}

void RandomGraph(int e, int v, AMATRIX *graph)
{
    int i, j;
    int counter = 0;
    srand(time(0));
    while (counter < e){

        i = rand() % v;
        j = rand() % v;

        if ((graph->adj[i][j] != 1) && (i != j)){
            graph->adj[i][j] = 1;
            graph->adj[j][i] = 1;
            counter++;
        }
    }
}

void RandomOrientedGraph(int e, int v, AMATRIX *graph)
{
    int i = 0, j = 0;
    int counter = 0;
    srand(time(0));
    while (counter < e){

        i = rand() % v;
        j = rand() % v;

        if ((graph->adj[i][j] != 1) && (i != j) && (graph->adj[j][i] == 0)){
            graph->adj[i][j] = 1;
            counter++;
        }

        else if ((graph->adj[j][i] != 1) && (i != j) && (graph->adj[i][j] == 1)){
            graph->adj[j][i] = 1;
            counter++;
        }
    }
}

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

    AMATRIX *graph = AMatrixSet(v);

    RandomGraph(e, v, graph);
    RandomGraphPrint(graph, v, output);
    fprintf(output, "\nOriented graph:\n\n");
    graph = AMatrixDelete(graph);

    RandomOrientedGraph(e, v, graph);
    RandomGraphPrint(graph, v, output);
    graph = AMatrixDelete(graph);

    fclose(input);
    fclose(output);
}