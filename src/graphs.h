#ifndef GRAPHS_H
#define GRAPHS_H

//------------------------------------------------------------------------------------------------------

typedef struct node
{
	int name;  // ID  
	int value; // only Kruskal use it
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
void  EdgePrint (const EDGE* E);

//------------------------------------------------------------------------------------------------------

typedef struct graph 
{
	int SIZE_N;  // number of nodes
	int SIZE_E;  // number of edges

	NODE** nodes;
	EDGE** edges;

	unsigned char directed : 1;
}GRAPH;

GRAPH* GraphSet     (int nodes_num, int edges_num, int info);
void   GraphPrint   (const GRAPH* G);
void   GraphDestroy (GRAPH* G);

int    GraphGetEdgeWeight     (GRAPH* G, NODE* V1, NODE* V2);
EDGE*  GraphGetEdge           (GRAPH* G, NODE* V1, NODE* V2);
NODE** GraphGetNodeNeighbors  (GRAPH* G, NODE* node);

//------------------------------------------------------------------------------------------------------

#endif
