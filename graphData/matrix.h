#ifndef TRAIN_GRAPH_H
#define TRAIN_GRAPH_H

struct matrix {
    int** adj;
    int n; //Number of nodes in matrix.
    int e; //Number of edges
}G;

int countEdges(FILE *input); //count the amount of nodes in matrix
struct matrix *graph_create(int nodes, FILE *input);
void readMatrix(FILE *input); //read matrix from file
void printGraph(struct matrix *m);
void printMatrixFile(struct matrix *m, FILE *output);
int degree(struct matrix *m, int v); //count degree of chosen vertex
double density(struct matrix *m); //calculate density
#endif //TRAIN_GRAPH_H
