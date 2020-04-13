#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printGraph(int **graph, int v, FILE *output){

    fprintf(output, "Adjacency matrix: \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            fprintf(output, "%d ", graph[i][j]);
        }
        fprintf(output, "\n");
    }

    int flag = 0;
    fprintf(output, "\nAdjacency list: \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            if (graph[i][j] == 1) {
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
            if (graph[i][j] == 1)
                fprintf(output, "%d %d \n", i, j);
        }
    }
}

void randomGraph(int e, int v, int **graph){
    int i, j;
    int counter = 0;
    srand(time(0));
    while (counter < e){

        i = rand() % v;
        j = rand() % v;

        if ((graph[i][j] != 1) && (i != j)){
            graph[i][j] = 1;
            graph[j][i] = 1;
            counter++;
        }
    }
}

void randomorientedGraph(int e, int v, int **graph){
    int i, j;
    int counter = 0;
    srand(time(0));
    while (counter < e){

        i = rand() % v;
        j = rand() % v;

        if ((graph[i][j] != 1) && (i != j) && (graph[j][i] == 0)){
            graph[i][j] = 1;
            counter++;
        }

        else if ((graph[i][j] != 1) && (i != j) && (graph[j][i] == 1)){
            graph[i][j] = 1;
        }
    }
}

int main() {
    int v=0, e=0;
    int **graph = NULL;
    FILE *input;
    FILE *output;
    input = fopen("argv[1].txt", "rt");
    output = fopen("argv[2].txt", "wt");
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
        fprintf(output, "wrong number of vertex");
        return 0;
    }

    graph = (int**)malloc(v * sizeof(int*));
    for(int i = 0; i < v; i++)
        graph[i] = (int*)malloc(sizeof(int*) * v);

    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            graph[i][j] = 0;
        }
    }

    randomGraph(e, v, graph);
    printGraph(graph, v, output);
    fprintf(output, "\nOriented graph:\n\n");

    graph = (int**)malloc(v * sizeof(int*));
    for(int i = 0; i < v; i++)
        graph[i] = (int*)malloc(sizeof(int*) * v);

    randomorientedGraph(e, v, graph);
    printGraph(graph, v, output);

    for(int i = 0; i < v; i++)
        free(graph[i]);
    free(graph);

    fclose(input);
    fclose(output);
}