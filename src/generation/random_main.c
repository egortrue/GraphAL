#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random_graphs.h"

int main() {

    srand(time(0));
    int r1 = rand() % 100;
    int r2 = rand() % 100;

    AMATRIX *graph1 = AMatrixSet(5, 5);
    graph1 = ChoiceRand(graph1, 0, r1, r2,2);
    PrintRandMatrix(graph1, 5);
    for (int i = 0; i < graph1->nodes_num; i++)
        printf("%d ", AMatrixDegree(graph1, i));



}