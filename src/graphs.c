#include "graphs.h"
#include <stdlib.h>
#include <stdio.h>

NODE* NodeSet(int name, int val)
{
	NODE* V = (NODE*)malloc(sizeof(NODE));
	if (!V) exit(EXIT_FAILURE);

	V->name = name;
	V->value = val;

	return V;
}

NODE* NodeGetAddress(NODE** arr, int name, int arr_size)
{
	for (int i = 0; i < arr_size; i++)
		if (arr[i]->name == name)
			return arr[i];
	
	printf("\nNODE NOT FOUND: (%d)\n", name);
	exit(EXIT_FAILURE);
}

//------------------------------------------------------------------------------------------------------

EDGE* EdgeSet(NODE* source, NODE* target, int weight)
{
	EDGE* E = (EDGE*)malloc(sizeof(EDGE));
	if (!E) exit(EXIT_FAILURE);
	E->source = source;
	E->target = target;
	E->weight = weight;
	return E;
}

void EdgePrint(const EDGE* E)
{
	printf("EDGE: (%d<->%d) ", E->source->name, E->target->name);
	printf("Weight: %d\n", E->weight);
}

//------------------------------------------------------------------------------------------------------

GRAPH* GraphSet(int nodes_num, int edges_num, int directed)
{
	GRAPH* G = (GRAPH*)malloc(sizeof(GRAPH));
	if (!G) exit(EXIT_FAILURE);
	G->nodes = (NODE**)calloc(nodes_num, sizeof(NODE*));
	if (!G->nodes) exit(EXIT_FAILURE);
	G->edges = (EDGE**)calloc(edges_num, sizeof(EDGE*));
	if (!G->nodes) exit(EXIT_FAILURE);
	
	G->SIZE_N = nodes_num;
	G->SIZE_E = edges_num;
	G->directed = directed & 1;

	return G;
}

void GraphPrint(const GRAPH* G)
{
	int i = 0;
	printf("GRAPH\n");
	printf("  Nodes: ");
	for (; i < G->SIZE_N; i++)
		if (G->nodes[i] != 0)
			printf("(%2d)=%2d%s", G->nodes[i]->name, G->nodes[i]->value, ((i+1) % 7 != 0) ? " " : "\n\t ");
	printf("%s", i == 0 ? "None\n" : "\n");
	i = 0;
	printf("  Edges: ");
	for (; i < G->SIZE_E; i++)
		if (G->edges[i] != 0)
			printf("(%2d<->%2d)=%-2d%s", G->edges[i]->source->name, G->edges[i]->target->name,
										 G->edges[i]->weight,
										 ((i+1) % 5 != 0) ? " " : "\n\t ");
	printf("%s", i == 0 ? "None\n\n" : "\n");
}

void GraphDestroy(GRAPH* G)
{
	for (int i = 0; i < G->SIZE_N; i++)
		free(G->nodes[i]);
	free(G->nodes);
	for (int i = 0; i < G->SIZE_E; i++)
		free(G->edges[i]);
	free(G->edges);
	free(G);
}

int GraphEdgeWeight(GRAPH* G, NODE* V1, NODE* V2)
{
	for (int i = 0; i < G->SIZE_E; i++)
	{
		if (G->edges[i]->source == V1 && G->edges[i]->target == V2)
			return G->edges[i]->weight;
		if (G->edges[i]->source == V2 && G->edges[i]->target == V1 && G->directed == 0)
			return G->edges[i]->weight;
	}
	return INT_MAX;
} 
