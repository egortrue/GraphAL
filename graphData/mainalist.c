#include <stdio.h>
#include <stdlib.h>
#include "alist.h"

int main(){
    FILE *input;
    FILE *output;
    input = fopen("adjListInput.txt", "rt");
    output = fopen("adjListOutput.txt", "wt");
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