#include "random_graphs.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

DLL_EXPORT AMATRIX* RandFlowNetwork(AMATRIX* g, int weight_min, int weight_max, int max_degree){
    //add extra nodes
    g->adj = (int**)realloc(g->adj, (g->nodes_num + 2)*sizeof(int**));
    for (int i = g->nodes_num; i < g->nodes_num + 2; i++){
        g->adj[i] = (int*)calloc(g->nodes_num + 2, sizeof(int));
    }
    g->nodes_num += 2;

    int source = g->nodes_num - 2;
    int sink = g->nodes_num - 1;

    //connect source & sink w/ graph
    int start = rand() % g->nodes_num, dir = rand() % g->nodes_num;
    while (g->adj[source][dir] || dir == source || dir == sink || AMatrixDegree(g, dir) >= max_degree)
        dir = rand() % g->nodes_num;
    while (g->adj[source][dir] == 0)
        g->adj[source][dir] = weight_min + rand() % (weight_max - weight_min + 1);

    while (g->adj[start][sink] || start == source || start == sink || AMatrixDegree(g, start) >= max_degree)
        start = rand() % g->nodes_num;
    while (g->adj[start][sink] == 0)
        g->adj[start][sink] = weight_min + rand() % (weight_max - weight_min + 1);

    return g;
}

AMATRIX* RandConnectedOrGraph(AMATRIX *g, int weight_min, int weight_max, int max_degree){
    if (g->edges_num < g->nodes_num - 1 || g->nodes_num <= 1 || g->edges_num > (g->nodes_num - 1)*g->nodes_num/2)
        return NULL;

    int *used = (int*)calloc(g->nodes_num, sizeof(int));
    int start = rand() % g->nodes_num;
    int dir = rand() % g->nodes_num;
    int first = start;
    used[start] = 1;
    int count = 1;

    while (count < g->nodes_num){
        while (g->adj[start][dir] || used[dir] || start == dir){
            dir = rand() % g->nodes_num;
        }
        while (g->adj[start][dir] == 0)
            g->adj[start][dir] = weight_min + rand() % (weight_max - weight_min + 1);
        count++;
        used[dir] = 1;
        start = dir;
    }

    if (g->edges_num >= g->nodes_num){
        count = g->edges_num - g->nodes_num;
        while (g->adj[dir][first] == 0)
            g->adj[dir][first] = weight_min + rand() % (weight_max - weight_min + 1);
        start = 0;
        dir = 0;
        for (int i = 0; i < count; i++){
            while (g->adj[start][dir] || dir == start ||
                    AMatrixDegree(g, start) >= max_degree || AMatrixDegree(g, dir) >= max_degree || g->adj[dir][start])
            {
                dir = rand() % g->nodes_num;
                start = rand() % g->nodes_num;
            }
            while (g->adj[start][dir] == 0)
                g->adj[start][dir] = weight_min + rand() % (weight_max - weight_min + 1);
        }
    }

    return g;
}

AMATRIX *RandConnectedGraph(AMATRIX *g, int weight_min, int weight_max, int max_degree){
    if (g->edges_num < g->nodes_num - 1 || g->nodes_num <= 1 || g->edges_num > (g->nodes_num - 1)*g->nodes_num/2)
        return NULL;
    RandomGraph(g, weight_min, weight_max, max_degree);

    int *pr = (int*)calloc(g->nodes_num, sizeof(int));
    int *used = (int*)calloc(g->nodes_num, sizeof(int));
    int *nodes_comp = (int*)calloc(g->nodes_num, sizeof(int));
    int *tmpnodes_comp = (int*)calloc(g->nodes_num, sizeof(int));

    int tmpcount_nodes = 0;
    int count_nodes = 0;
    int cycle[2] = {0, 0};
    int j = 0, k = 0;
    int new_edges = 0;
    int count_comps = 0;
    
    for (int i = 0; i < g->nodes_num; i++){
        if (!used[i]) {
            if ((cycle[0] || cycle[1])) {
                g->adj[cycle[0]][cycle[1]] = 0;
                g->adj[cycle[1]][cycle[0]] = 0;
                cycle[0] = 0;
                cycle[1] = 0;
                new_edges--;
            }
            
            for (int m = 0; m < g->nodes_num; m++)
                tmpnodes_comp[m] = nodes_comp[m];
            tmpcount_nodes = count_nodes;

            count_nodes = 0;
            nodes_comp = (int*)calloc(g->nodes_num, sizeof(int));
            dfs(g, i, used, &count_nodes, nodes_comp, pr, cycle);

            if (count_nodes != g->nodes_num && (cycle[0] || cycle[1])){
                g->adj[cycle[0]][cycle[1]] = 0;
                g->adj[cycle[1]][cycle[0]] = 0;
                cycle[0] = 0;
                cycle[1] = 0;
                new_edges--;
            }
            if (tmpcount_nodes > 0){
                k = tmpnodes_comp[rand() % tmpcount_nodes];
                j = nodes_comp[rand() % count_nodes];
                while (j == k || AMatrixDegree(g, j) >= max_degree || AMatrixDegree(g, k) >= max_degree ) {
                    k = tmpnodes_comp[rand() % tmpcount_nodes];
                    j = nodes_comp[rand() % count_nodes];
                }
                while (g->adj[k][j] == 0)
                    g->adj[k][j] = weight_min + rand()%(weight_max - weight_min + 1);
                g->adj[j][k] = g->adj[k][j];
                new_edges++;
            }
            count_comps++;
        }
    }
   if (new_edges < 0){
        j = rand() % g->nodes_num;
        k = rand() % g->nodes_num;
        while (j == k || AMatrixDegree(g, j) >= max_degree || AMatrixDegree(g, k) >= max_degree ) {
            j = rand() % g->nodes_num;
            k = rand() % g->nodes_num;
        }
        while (g->adj[k][j] == 0)
            g->adj[k][j] = weight_min + rand()%(weight_max - weight_min + 1);
        g->adj[j][k] =  g->adj[k][j];
        new_edges++;
    }

    int i = 0;
    cycle[0] = 0;
    cycle[1] = 0;
    used = (int*)calloc(g->nodes_num, sizeof(int));
    nodes_comp = (int*)calloc(g->nodes_num, sizeof(int));
    pr = (int*)calloc(g->nodes_num, sizeof(int));
    while (new_edges > 0){
        count_nodes = 0;
        dfs(g, i, used, &count_nodes, nodes_comp, pr, cycle);
        if (cycle[0] || cycle[1]) {
            g->adj[cycle[0]][cycle[1]] = 0;
            g->adj[cycle[1]][cycle[0]] = 0;
            new_edges--;
            cycle[0] = 0;
            cycle[1] = 0;
            used = (int*)calloc(g->nodes_num, sizeof(int));
            nodes_comp = (int*)calloc(g->nodes_num, sizeof(int));
            pr = (int*)calloc(g->nodes_num, sizeof(int));
        }
        i++;
        if (i >= g->nodes_num)
            i = 0;
    }

    free(pr);
    free(used);
    free(nodes_comp);

    return g;
}

void dfs(AMATRIX *g, int node_num, int used[], int *count, int *comp, int *pr, int *cycle)
{
    used[node_num] = 1;
    comp[*count] = node_num;
    *count += 1;
    for (int i = 0; i < g->nodes_num; i++ ){
        int dest = i;
        if (used[dest] == 0 && g->adj[node_num][dest]) {
            pr[dest] = node_num;
            dfs(g, dest, used, count, comp, pr, cycle);
        }
        else if (used[dest] == 1 && dest != pr[node_num] && g->adj[node_num][dest]){
            cycle[0] = node_num;
            cycle[1] = dest;
        }

    }
    used[node_num] = 2;
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

DLL_EXPORT AMATRIX* RandomGraph(AMATRIX *graph, int weight_min, int weight_max, int max_degree)
{
    int i = 0, j = 0, N = 0;
    double r = 0;
    int counter = 0;
    int fail = 0;
    while (counter < graph->edges_num){

       r = (double)(rand())/RAND_MAX;
        if (r == 0 || r == 1)
            continue;
        N = floor(pow(graph->nodes_num, 2) * r);

        i = floor(N/graph->nodes_num);
        j =  N - i  * (graph->nodes_num);

        if ((!graph->adj[i][j]) && (i != j))
        {
            if ((AMatrixDegree(graph, i) < max_degree) && (AMatrixDegree(graph, j) < max_degree))
            {
                while (graph->adj[i][j] == 0)
                    graph->adj[i][j] = weight_min + rand() % (weight_max - weight_min + 1);
                graph->adj[j][i] = graph->adj[i][j];
                counter++;
                fail = 0;
            }
        }
        else if (fail > 10 && i!=j){
            graph->adj[j][i] = 0;
            graph->adj[i][j] = 0;
            fail = 0;
            counter--;
        }
        else
            fail++;
    }
    return graph;
}

DLL_EXPORT AMATRIX *ChoiceRand(AMATRIX *g, int oriented, int weight_min, int weight_max, int max_degree)
{
    if (!oriented && g->nodes_num * max_degree < 2 * g->edges_num)
        return 0;
    if (oriented && g->nodes_num * max_degree < g->edges_num)
        return 0;
    if (oriented)
        g = RandConnectedOrGraph(g,  weight_min, weight_max, max_degree);
    else
        g = RandConnectedGraph(g, weight_min, weight_max, max_degree);

    return g;
}