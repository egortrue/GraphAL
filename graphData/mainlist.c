#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void readAList(FILE* input, FILE* output);
int countVertex(FILE *input);
double density(int v, int e);
int degree(Node *phead);

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
    readAList(input, output);
}

//format:
// 1-1 2 3
//4-3 1 5
void readAList(FILE* input, FILE* output){
    Node **graph= (Node**)calloc(countVertex(input), sizeof(Node));
    int j = 0;
    char *chr = (char*)malloc(sizeof(char*));
    int num = 0;
    int countedges = 0;
    char strnew;
    strnew = fgetc(input);

    while (strnew != EOF) {
        if (strnew >= 48 && strnew <= 57){
            chr = realloc(chr, j+1 * sizeof(char*));
            chr[j++] = strnew;
        }

        else if (strnew == '-') {
            num = atoi(chr);
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (strnew == ' ' && chr != 0) {
            add2list(&graph[num], atoi(chr));
            countedges++;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        else if (strnew == '\n') {
            add2list(&graph[num], atoi(chr));
            countedges++;
            num = 0;
            chr = 0;
            chr = (char*)realloc(chr, 0*sizeof(char*));
            j = 0;
        }

        strnew = fgetc(input);
    }
    add2list(&graph[num], atoi(chr));

    for (int i = 0; i < countVertex(input)+1; i++) {
        if (graph[i]) {
            fprintf(output, "%d-", i);
            print(graph[i], output);
        }
    }

    printf("Density: %.2lf", density(countVertex(input), countedges+1));
    printf("\nDegree of v #5: %d", degree(graph[5]));

    for (int i = 0; i < countVertex(input); ++i)
        deleteList(graph[i]);
}

double density(int v, int e){
    return ((double)e/(double)(v*(v-1)));
}

int degree(Node *phead){
    Node* p = phead;
    int count = 0;
    while(p)
    {
        count++;
        p = p->pnext;
    }
    return count;
}

int countVertex(FILE *input){
    int lines_count = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c == '\n')
            lines_count++;
        c = fgetc(input);
    }
    lines_count++;
    rewind(input); //return back pointer in file
    return lines_count+1;
}
