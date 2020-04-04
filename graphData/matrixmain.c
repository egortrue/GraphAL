#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

int main(){
    FILE *input;
    FILE *output;
    input = fopen("matrixInput.txt", "rt");
    output = fopen("matrixOutput.txt", "wt");
    if (!input)
    {
        puts("Error open file for reading");
        return -2;
    }
    if (!output)
    {
        puts("Error open file for reading");
        return -2;
    }

    struct matrix *d = graph_create(countEdges(input), input);
    printGraph(d);
    printf("\n");
    printMatrixFile(d, output);
    printf("\nDegree of v #%d is %d", 1, degree(d, 1));
    printf("\nDensity is %.2lf", density(d));

    return 0;
}

