#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/graphs.c"
int used[7];

int dfs(AMATRIX *g, int ot, int v, int n, int *cnt){
    used[ot] = 1;
    (*cnt)++;
    if (*cnt < n) {
        srand(time(0));
        int i = rand() % n;
        while (g->adj[ot][i] || used[i])
            i = rand() % n;
        g->adj[ot][i] = 7;
        g->adj[i][ot] = 7;
        dfs(g, i, v, n, cnt);
    }
    else
        while (*cnt <= v){
            srand(time(0));
            int i = rand() % n;
            ot = rand() % n;
            if (!g->adj[ot][i]){
                g->adj[ot][i] = 7;
                g->adj[i][ot] = 7;
                (*cnt)++;
            }

        }

    return 0;
}

void RandomF(AMATRIX *G, int v, int e){
    int i = 0;


}

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
            if (graph->adj[i][j]) {
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
            if (graph->adj[i][j])
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
  /*  RandomGraph(e, v, graph);
    RandomGraphPrint(graph, v, output);
    fprintf(output, "\nOriented graph:\n\n");
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v);
    RandomOrientedGraph(e, v, graph);
    RandomGraphPrint(graph, v, output);
    graph = AMatrixDelete(graph);*/

    if (e<v-1)
        return 0;
    graph = AMatrixSet(v);
    int cnt = 0;
    dfs(graph, 0, e, v, &cnt);
    RandomGraphPrint(graph, v, output);
    graph = AMatrixDelete(graph);
    fclose(input);
    fclose(output);
}