#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random_graphs.h"

int main() {
    int v=5, e=5;


    srand(time(0));
    int r1 = rand() % 100;
    int r2 = rand() % 100;

    AMATRIX *graph = AMatrixSet(v, e);

    graph = ChoiceRand(graph, 0, r1, r2);
    PrintRandMatrix(graph, v);
   // graph = AMatrixDelete(graph);

    AMATRIX *graph1 = AMatrixSet(10, 10);
    graph1 = ChoiceRand(graph1, 0, r1, r2);
    PrintRandMatrix(graph1, 10);

   /* RandomGraph(graph, r1, r2);
    PrintRandMatrix(graph, v);
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v, e);
    RandomOrientedGraph( graph, r1, r2);
    PrintRandMatrix(graph, v);
    graph = AMatrixDelete(graph);


    graph = AMatrixSet(v, e);
    graph = ConnectGraph(graph, r1, r2);
    PrintRandMatrix(graph, v);
    graph = AMatrixDelete(graph);

    graph = AMatrixSet(v, e);
    RandConnectedOr(graph, r1, r2);
    PrintRandMatrix(graph, v);
    graph = AMatrixDelete(graph);*/

    //TestConnection(graph, e, v, r1, r2);


}