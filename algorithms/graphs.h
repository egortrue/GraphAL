#ifndef GRAPHS_H
#define GRAPHS_H

#define G_DIRECTED  0b00000001
#define G_WEIGHTED  0b00000010
#define G_CONNECTED 0b00000100

//------------------------------------------------------------------------------------------------------

typedef struct node
{
	int name;  // ID  
	int value; 
}NODE;

NODE* NodeSet        (int name, int val);
NODE* NodeGetAddress (NODE** arr, int name, int arr_size);

//------------------------------------------------------------------------------------------------------

typedef struct edge
{
	int weight;
	NODE* source;
	NODE* target;
}EDGE;

EDGE* EdgeSet   (NODE* source, NODE* target, int weight);
void  EdgePrint (EDGE* E);

//------------------------------------------------------------------------------------------------------

typedef struct graph 
{
	int SIZE_N;  // number of nodes
	int SIZE_E;  // number of edges

	NODE** nodes;
	EDGE** edges;

	unsigned char directed  : 1;
	unsigned char weighted  : 1;
	unsigned char connected : 1;
}GRAPH;

GRAPH* GraphSet     (int nodes, int edges, char info);
void   GraphPrint   (GRAPH* G);
void   GraphDestroy (GRAPH* G);

int    GraphGetEdgeWeight     (GRAPH* G, NODE* V1, NODE* V2);
EDGE*  GraphGetEdge           (GRAPH* G, NODE* V1, NODE* V2);
NODE** GraphGetNodeNeighbors  (GRAPH* G, NODE* node);

//------------------------------------------------------------------------------------------------------

typedef struct aMatrix
{
	int nodes; // number of nodes
	int edges; // number of edges
    int** adj;
}AMATRIX;

AMATRIX* aMatrixSet       (int nodes);
AMATRIX* aMatrixGenerate  (int nodes, int edges, int directed);
void     aMatrixDestroy   (AMATRIX* matrix);

//------------------------------------------------------------------------------------------------------

#endif
