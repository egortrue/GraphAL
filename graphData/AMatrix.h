#ifndef GRAPH_H
#define GRAPH_H

//------------------------------------------------------------------------------------------------------
typedef struct AMatrix
{
    int** adj;
    int n; //Number of nodes in AMatrix.
    int e; //Number of edges
}AMATRIX;

AMATRIX *AMatrixSet(FILE *input);
void AMatrixRead(FILE *input, AMATRIX *adj_matrix); //read adjmatrix from file
void AMatrixPrint(AMATRIX *m, FILE *output);
int AMatrixDegree(AMATRIX *adj_matrix, int v); //count degree of chosen vertex
double AMatrixDensity(AMATRIX *adj_matrix); //calculate density
//------------------------------------------------------------------------------------------------------

#endif //GRAPH_H
