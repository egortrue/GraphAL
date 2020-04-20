#include <stdio.h>
#include "AMatrix.h"

int main()
{
    FILE *input;
    FILE *output;
    input = fopen("AdjMatrixInput.txt", "rt");
    output = fopen("AdjMatrixOutput.txt", "wt");
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

    AMATRIX *d = AMatrixSet(input);
    AMatrixPrint(d, output);
    fprintf(output, "\nDegree of v #%d is %d", 1, AMatrixDegree(d, 1));
    fprintf(output, "\nDensity is %.2lf", AMatrixDensity(d));

    return 0;
}

