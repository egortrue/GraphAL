#include <stdio.h>
#include <stdlib.h>
#include "AList.h"

int main(){
    FILE *input;
    FILE *output;
    input = fopen("AdjListInput.txt", "rt");
    output = fopen("AdjListOutput.txt", "wt");
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
    AListRead(input, output);
}