#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random_graphs.h"
#include <math.h>

int main() {

    srand(time(0));
    int r1 = rand() % 100;
    int r2 = rand() % 100;
    int n = 30, e = 35;
    AMATRIX *graph1 = AMatrixSet(n, e);
    graph1 = ChoiceRand(graph1, 0, r1, r2,10);


  /*  graph1->adj[0][9] = 1;
    graph1->adj[9][0] = 1;
    graph1->adj[11][9] = 1;
    graph1->adj[9][11] = 1;
    graph1->adj[11][0] = 1;
    graph1->adj[0][11] = 1;
    graph1->adj[10][8] = 1;
    graph1->adj[8][10] = 1;
    graph1->adj[8][7] = 1;
    graph1->adj[7][8] = 1;
    graph1->adj[7][1] = 1;
    graph1->adj[1][7] = 1;
    graph1->adj[1][6] = 1;
    graph1->adj[6][1] = 1;
    graph1->adj[6][5] = 1;
    graph1->adj[5][6] = 1;
    graph1->adj[5][3] = 1;
    graph1->adj[3][5] = 1;
    graph1->adj[3][4] = 1;
    graph1->adj[4][3] = 1;
    graph1->adj[4][2] = 1;
    graph1->adj[2][4] = 1;

    PrintRandMatrix(graph1, 12);*/

    //graph1 = RandConnectedGraph(graph1, 1,1,2);
    for (int i = 0; i < graph1->nodes_num; i++)
        printf("%d ", AMatrixDegree(graph1, i));

    PrintRandMatrix(graph1, n);

    int *pr = (int*)calloc(n, sizeof(int));
    int *used = (int*)calloc(n, sizeof(int));
    int *nodes_comp = (int*)calloc(n, sizeof(int));
    int *tmpnodes_comp = (int*)calloc(n, sizeof(int));

    int tmpcount_nodes = 0;
    int count_nodes = 0;
    int cycle[2] = {0, 0};

    for (int i = 0; i < n; i++)
        dfs(graph1, i, used, &count_nodes, nodes_comp, pr, cycle);

    printf("\n%d", count_nodes);

    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (graph1->adj[i][j])
                count++;
            printf("\n%d", count);

   /* int a[25];
    for (int i = 0; i < 25; i++)
        a[i] = 0;
    int i =0, j =0;
    for (int k = 0; k < 10000; k++) {
        double r = (double) (rand()) / RAND_MAX * (1 - 0) + 0;
        int N = floor(25 * r);
        a[N]++;
    }
    for (int i = 0; i < 25; i++)
        printf("%d %d \n", i, a[i] );

    printf("\n");
    for (int i = 0; i < 25; i++)
        a[i] = 0;

     j =0;  i =0;
    for (int k = 0; k < 10000; k++) {
        double r = (double) (rand()) / RAND_MAX * (1 - 0) + 0;
        i = floor(5 * r);
        r = (double) (rand()) / RAND_MAX * (1 - 0) + 0;
        j = floor(5*r);
        int N = 5*j + i;
        a[N]++;
    }
    for (int i = 0; i < 25; i++)
        printf("%d %d \n", i, a[i] );*/

}