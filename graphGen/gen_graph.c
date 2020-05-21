#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/graphs.c"

int connectedGraph(AMATRIX *g, int v, int n, int r1, int r2){ //connnected oriented graph
    if (v < n-1)
        return 0;
    int *used = (int*)calloc(n,sizeof(int));
    srand(time(0));
    int od = rand() % n, dos = rand() % n;
    int first = od;
    used[od] = 1;
    int count = 1;
    while(count < n){
        while (g->adj[od][dos] || used[dos] || od == dos){
            dos = rand() % n;
        }
        g->adj[od][dos] = r1 + rand()%(r2-r1+1);
        count++;
        used[dos] = 1;
        od = dos;
    }

    if (v >= n){
         count = v - n - 1;
         g->adj[dos][first] = r1 + rand()%(r2-r1+1);
        for (int i = 0; i < count; i++){
            srand(time(0));
            while (g->adj[od][dos] || dos == od)
                dos = rand() % n;
            g->adj[od][dos] = r1 + rand()%(r2-r1+1);
           // g->adj[dos][od] = g->adj[od][dos];
            od = dos;
            dos = rand() % n;
        }
    }
    return 0;
}

void dfs(AMATRIX *g, int v, int ost, int used[], int *count, int *comp, int *pr, int *cycle){
    used[v] = 1;
    comp[*count] = v; //save vertex in components
    *count += 1; //count vertex in 1 comp
    for (int i = 0; i < ost; i++ ){
        int to = i;
        if (used[to] == 0 && g->adj[v][to]) {
            pr[to] = v;
            dfs(g, to, ost, used, count, comp, pr, cycle);
        }
        else if (used[to] == 1 && to != pr[v] && g->adj[v][to]){
            cycle[0] = v;
            cycle[1] = to;
        }

    }
    used[v] = 2;
}

int ConnectGraph(AMATRIX *g, int v, int n){ //just connected graph
    int *used = (int*)calloc(n,sizeof(int));
    int *comp = (int*)calloc(n,sizeof(int));
    int *pr = (int*)calloc(n,sizeof(int));
    int count = 0;
    int ost = n;
    int prevcycle = 0;
    int tmpprevcycle = 0;
    int cycle[2] = {0, 0};
    int neededg = 0;
    int last = 0;
    for (int i = 0; i < n; i++){
        if (!used[i]){
            if (prevcycle && (cycle[0] || cycle[1])) {
                g->adj[cycle[0]][i] = g->adj[cycle[0]][cycle[1]];
                g->adj[i][cycle[0]] = g->adj[cycle[0]][i];
                g->adj[cycle[0]][cycle[1]] = 0;
                g->adj[cycle[1]][cycle[0]] = 0;
                cycle[0] = 0;
                cycle[1] = 0;
                prevcycle = 0;
            }
            comp = (int*)calloc(n, sizeof(int));
            dfs(g, i, ost, used, &count, comp, pr, cycle);
            if ((cycle[0] || cycle[1]) && tmpprevcycle == 1){
                g->adj[cycle[0]][i-1] = g->adj[cycle[0]][cycle[1]];
                g->adj[i-1][cycle[0]] = g->adj[cycle[0]][i-1];
                g->adj[cycle[0]][cycle[1]] = 0;
                g->adj[cycle[1]][cycle[0]] = 0;
                cycle[0] = 0;
                cycle[1] = 0;
                tmpprevcycle = 0;
            }
            else if (tmpprevcycle == 1 && !(cycle[0] || cycle[1])) {
                neededg++;
                //tmpprevcycle = 0;
                g->adj[i-1][i] = 787;
                g->adj[i][i-1] = 787;
            }
            else if (cycle[0] || cycle[1])
                prevcycle = 1; //can give a connect
            else
                tmpprevcycle = 1; //need connect

            last = i;
        }
        count = 0;
        pr = (int*)calloc(n, sizeof(int));
    }
    if (tmpprevcycle == 1 && !(cycle[0] || cycle[1])) {
        neededg++;
        g->adj[last-1][last] = 787;
        g->adj[last][last-1] = 787;
    }
    int i = 0;
    while (neededg != 0 && i < ost){
        count = 0;
        dfs(g, i, ost, used, &count, comp, pr, cycle);
        if (cycle[0] || cycle[1]) {
            g->adj[cycle[0]][cycle[1]] = 0;
            g->adj[cycle[1]][cycle[0]] = 0;
            neededg--;
        }
        i++;
    }
    free(pr);
    free(used);
    free(comp);
}

void PrintRandMatrix(AMATRIX *graph, int v, FILE *output){
    fprintf(output, "\n[adjacency matrix]: \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            fprintf(output, "%d ", graph->adj[i][j]);
        }
        fprintf(output, "\n");
    }
}

void PrintRandVertexList(AMATRIX *graph, int v, FILE *output){
    fprintf(output, "\n[vertex list] \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            if (graph->adj[i][j])
                fprintf(output, "%d %d \n", i, j);
        }
    }
}

void PrintRandAdjList(AMATRIX *graph, int v, FILE *output){
    int flag = 0;
    fprintf(output, "\n[adjacency list]: \n");
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
}

void PrintRandSNodEdg(AMATRIX *graph, int v, FILE *output){
    fprintf(output, "\n[nodes]: \n");
    for (int i = 0; i < v; i++){
        fprintf(output,"%d %d\n", i, 1);
    }
    fprintf(output, "\n[edges]: \n");
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            if (graph->adj[i][j])
                fprintf(output, "%d %d %d \n", i, j, graph->adj[i][j]);
        }
    }
}

void RandomGraph(int e, int v, AMATRIX *graph, int r1, int r2)
{
    int i, j;
    int counter = 0;
    srand(time(0));
    while (counter < e){

        i = rand() % v;
        j = rand() % v;

        if ((!graph->adj[i][j]) && (i != j)){
            graph->adj[i][j] = r1 + rand()%(r2-r1+1);
            graph->adj[j][i] = graph->adj[i][j];
            counter++;
        }
    }
}

void RandomOrientedGraph(int e, int v, AMATRIX *graph, int r1, int r2)
{
    int i = 0, j = 0;
    int counter = 0;
    srand(time(0));
    while (counter < e){

        i = rand() % v;
        j = rand() % v;

        if ((!graph->adj[i][j]) && (i != j) && (!graph->adj[j][i])){
            graph->adj[i][j] = r1 + rand()%(r2-r1+1);
            counter++;
        }

        else if ((!graph->adj[j][i]) && (i != j) && (graph->adj[i][j])){
            graph->adj[j][i] = r1 + rand()%(r2-r1+1);
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
    connectedGraph(graph, e, v, r1, r2);
    fprintf(output, "\nConnected oriented graph:\n\n");
    PrintRandMatrix(graph, v, output);
    PrintRandVertexList(graph, v, output);
    PrintRandAdjList(graph, v, output);
    PrintRandSNodEdg(graph, v, output);
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v);
    RandomGraph(e, v, graph, r1, r2);
    ConnectGraph(graph, e,  v);
    fprintf(output, "\nConnected non-oriented graph:\n\n");
    PrintRandMatrix(graph, v, output);
    PrintRandVertexList(graph, v, output);
    PrintRandAdjList(graph, v, output);
    PrintRandSNodEdg(graph, v, output);
    graph = AMatrixDelete(graph);


    fclose(input);
    fclose(output);
}