#include "random_graphs.h"
#include <time.h>
#include <stdlib.h>

AMATRIX* RandConnectedOr(AMATRIX *g, int edges_num, int nodes_num, int weight_min, int weight_max){ //connnected oriented graph
    if (edges_num < nodes_num - 1)
        return NULL;
    int *used = (int*)calloc(nodes_num, sizeof(int));
    srand(time(0));
    int od = rand() % nodes_num, dos = rand() % nodes_num;
    int first = od;
    used[od] = 1;
    int count = 1;
    while(count < nodes_num){
        while (g->adj[od][dos] || used[dos] || od == dos){
            dos = rand() % nodes_num;
        }
        g->adj[od][dos] = weight_min + rand() % (weight_max - weight_min + 1);
        count++;
        used[dos] = 1;
        od = dos;
    }

    if (edges_num >= nodes_num){
        count = edges_num - nodes_num ;
        g->adj[dos][first] = weight_min + rand() % (weight_max - weight_min + 1);
        for (int i = 0; i < count; i++){
            srand(time(0));
            while (g->adj[od][dos] || dos == od)
                dos = rand() % nodes_num;
            g->adj[od][dos] = weight_min + rand() % (weight_max - weight_min + 1);
            od = dos;
            dos = rand() % nodes_num;
        }
    }
    return g;
}

void dfs(AMATRIX *g, int nodes_num, int ost, int used[], int *count, int *comp, int *pr, int *cycle){
    used[nodes_num] = 1;
    comp[*count] = nodes_num; //save vertex in components
    *count += 1; //count vertex in 1 comp
    for (int i = 0; i < ost; i++ ){
        int to = i;
        if (used[to] == 0 && g->adj[nodes_num][to]) {
            pr[to] = nodes_num;
            dfs(g, to, ost, used, count, comp, pr, cycle);
        }
        else if (used[to] == 1 && to != pr[nodes_num] && g->adj[nodes_num][to]){
            cycle[0] = nodes_num;
            cycle[1] = to;
        }

    }
    used[nodes_num] = 2;
}

AMATRIX* ConnectGraph(AMATRIX *g, int edges_num, int nodes_num, int r1, int r2){ //just connected graph
    if (edges_num <= (nodes_num - 2) * (nodes_num - 1) / 2)
        return NULL;
    RandomGraph(edges_num, nodes_num, g, r1, r2);
    int *used = (int*)calloc(nodes_num, sizeof(int));
    int *comp = (int*)calloc(nodes_num, sizeof(int));
    int *pr = (int*)calloc(nodes_num, sizeof(int));
    int count = 0;
    int ost = nodes_num;
    int prevcycle = 0;
    int tmpprevcycle = 0;
    int cycle[2] = {0, 0};
    int neededg = 0;
    int last = 0;
    for (int i = 0; i < nodes_num; i++){
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
            comp = (int*)calloc(nodes_num, sizeof(int));
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
                g->adj[i-1][i] = r1 + rand()%(r2-r1+1);
                g->adj[i][i-1] = r1 + rand()%(r2-r1+1);
            }
            else if (cycle[0] || cycle[1])
                prevcycle = 1; //can give a connect
            else
                tmpprevcycle = 1; //need connect

            last = i;
        }
        count = 0;
        pr = (int*)calloc(nodes_num, sizeof(int));
    }
    if (tmpprevcycle == 1 && !(cycle[0] || cycle[1])) {
        neededg++;
        g->adj[last-1][last] = r1 + rand()%(r2-r1+1);
        g->adj[last][last-1] = r1 + rand()%(r2-r1+1);
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

    return g;
}

void PrintRandMatrix(AMATRIX *graph, int nodes_num){
    printf( "\n[adjacency matrix]: \n");
    if (graph) {
        for (int i = 0; i < nodes_num; i++) {
            for (int j = 0; j < nodes_num; j++) {
                printf("%d ", graph->adj[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

AMATRIX* RandomGraph(int edges_num, int nodes_num, AMATRIX *graph, int weight_min, int weight_max)
{
    int i, j;
    int counter = 0;
    srand(time(0));
    while (counter < edges_num){

        i = rand() % nodes_num;
        j = rand() % nodes_num;

        if ((!graph->adj[i][j]) && (i != j)){
            graph->adj[i][j] = weight_min + rand() % (weight_max - weight_min + 1);
            graph->adj[j][i] = graph->adj[i][j];
            counter++;
        }
    }
    return graph;
}

AMATRIX* RandomOrientedGraph(int edges_num, int nodes_num, AMATRIX *graph, int weight_min, int weight_max)
{
    int i = 0, j = 0;
    int counter = 0;
    srand(time(0));
    while (counter < edges_num){

        i = rand() % nodes_num;
        j = rand() % nodes_num;

        if ((!graph->adj[i][j]) && (i != j) && (!graph->adj[j][i])){
            graph->adj[i][j] = weight_min + rand() % (weight_max - weight_min + 1);
            counter++;
        }

        else if ((!graph->adj[j][i]) && (i != j) && (graph->adj[i][j])){
            graph->adj[j][i] = weight_min + rand() % (weight_max - weight_min + 1);
            counter++;
        }
    }
    return graph;
}

void ChoiceRand(AMATRIX *g, int oriented, int edges_num, int nodes_num, int weight_min, int weight_max)
{
    if (edges_num > nodes_num * (nodes_num - 1) / 2)
       ;
    else if (oriented)
        RandConnectedOr(g, edges_num, nodes_num, weight_min, weight_max);
    else
        ConnectGraph(g, edges_num, nodes_num, weight_min, weight_max);
}