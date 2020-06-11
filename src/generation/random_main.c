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

    AMATRIX *graph1 = AMatrixSet(50, 51);
    graph1 = ChoiceRand(graph1, 0, r1, r2,10);
    PrintRandMatrix(graph1, 50);
    for (int i = 0; i < graph1->nodes_num; i++)
        printf("%d ", AMatrixDegree(graph1, i));
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