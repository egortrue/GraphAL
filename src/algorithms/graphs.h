#ifndef GRAPHS_H
#define GRAPHS_H

#define DLL_EXPORT __declspec(dllexport)

#define G_DIRECTED  0b00000001
#define G_WEIGHTED  0b00000010

//------------------------------------------------------------------------------------------------------

typedef struct node
{
	int name;  // ID  
	int value; 
}NODE;

DLL_EXPORT NODE* NodeSet(int name, int val);

//------------------------------------------------------------------------------------------------------

typedef struct edge
{
	int weight;
	NODE* source;
	NODE* target;
}EDGE;

DLL_EXPORT EDGE* EdgeSet(NODE* source, NODE* target, int weight);

//------------------------------------------------------------------------------------------------------

typedef struct graph 
{
	int SIZE_N;  // number of nodes
	int SIZE_E;  // number of edges

	NODE** nodes;
	EDGE** edges;

	unsigned char directed  : 1;
	unsigned char weighted  : 1;
}GRAPH;

DLL_EXPORT GRAPH* GraphSet(int nodes_num, NODE** nodes, int edges_num, EDGE** edges, char info);
void   GraphPrint   (GRAPH* G);
DLL_EXPORT void GraphDestroy (GRAPH* G);

int    GraphGetEdgeWeight     (GRAPH* G, NODE* V1, NODE* V2);
EDGE*  GraphGetEdge           (GRAPH* G, NODE* V1, NODE* V2);
NODE** GraphGetNodeNeighbors  (GRAPH* G, NODE* node);

//------------------------------------------------------------------------------------------------------

#endif
