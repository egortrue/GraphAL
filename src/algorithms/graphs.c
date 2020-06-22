#include <stdlib.h>
#include <stdio.h>

#include "graphs.h"

DLL_EXPORT NODE* NodeSet(int name, int val)
{
	NODE* V = (NODE*)malloc(sizeof(NODE));
	if (!V) exit(EXIT_FAILURE);
	V->name = name;
	V->value = val;
	return V;
}

//------------------------------------------------------------------------------------------------------

DLL_EXPORT EDGE* EdgeSet(NODE* source, NODE* target, int weight)
{
	EDGE* E = (EDGE*)malloc(sizeof(EDGE));
	if (!E) exit(EXIT_FAILURE);
	E->source = source;
	E->target = target;
	E->weight = weight;
	return E;
}

void EdgePrint(EDGE* E)
{
	printf("EDGE: (%d<->%d) ", E->source->name, E->target->name);
	printf("Weight: %d\n", E->weight);
}

//------------------------------------------------------------------------------------------------------

DLL_EXPORT GRAPH* GraphSet(int nodes_num, NODE** nodes, int edges_num, EDGE** edges, char info)
{
	GRAPH* G = (GRAPH*)malloc(sizeof(GRAPH));
	if (!G) exit(EXIT_FAILURE);
	G->nodes = (NODE**)calloc(nodes_num, sizeof(NODE*));
	if (!G->nodes) exit(EXIT_FAILURE);
	G->edges = (EDGE**)calloc(edges_num, sizeof(EDGE*));
	if (!G->nodes) exit(EXIT_FAILURE);

	if (nodes != NULL)
		for (int i = 0; i < nodes_num; i++)
			G->nodes[i] = nodes[i];

	if (edges != NULL)
		for (int i = 0; i < edges_num; i++)
			G->edges[i] = edges[i];

	G->SIZE_N = nodes_num;
	G->SIZE_E = edges_num;

	G->directed = info & 1;
	G->weighted = info & 2;

	return G;
}

void GraphPrint(GRAPH* G)
{
	int i = 0;
	printf("GRAPH (%s)\n", G->directed ? "directed" : "undirected");
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

DLL_EXPORT void GraphDestroy(GRAPH* G)
{
	for (int i = 0; i < G->SIZE_N; i++)
		free(G->nodes[i]);
	free(G->nodes);
	for (int i = 0; i < G->SIZE_E; i++)
		free(G->edges[i]);
	free(G->edges);
	free(G);
}

int GraphGetEdgeWeight(GRAPH* G, NODE* V1, NODE* V2)
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

EDGE* GraphGetEdge(GRAPH* G, NODE* V1, NODE* V2)
{
	for (int i = 0; i < G->SIZE_E; i++)
	{
		if (G->edges[i]->source == V1 && G->edges[i]->target == V2)
			return G->edges[i];
		if (G->edges[i]->source == V2 && G->edges[i]->target == V1 && G->directed == 0)
			return G->edges[i];
	}
	return NULL;
}

NODE** GraphGetNodeNeighbors(GRAPH* G, NODE* node)
{
	NODE** buffer = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	if (!buffer) exit(EXIT_FAILURE);

	int count = 0;
	for (int i = 0; i < G->SIZE_E; i++)
	{
		if (G->edges[i]->source == node)
			buffer[count++] = G->edges[i]->target;
		else if ((G->edges[i]->target == node && G->directed == 0))
			buffer[count++] = G->edges[i]->source;
	}

	if (!count)
	{
		free(buffer);
		return NULL;
	}
		
	NODE** neighbors = (NODE**)calloc(count, sizeof(NODE*));
	if (!neighbors) exit(EXIT_FAILURE);
	for (int i = 0; i < count; i++)
		neighbors[i] = buffer[i];
  	free(buffer);
	return neighbors;
}

//------------------------------------------------------------------------------------------------------
