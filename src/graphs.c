#include "graphs.h"
#include <stdlib.h>
#include <stdio.h>

NODE  NodeSet(int name, int val, int SIZE_)
{
	NODE V = { name,
			   val,
			   SIZE_,
			   SIZE_,
			   (NODE**)calloc(SIZE_, sizeof(NODE*)),
			   (NODE**)calloc(SIZE_, sizeof(NODE*)) };

	return V;
}

NODE* NodeAddress(NODE** arr, int name, int arr_size)
{
	int ok = 0;
	NODE* cur_node = NULL;
	for (int i = 0; i < arr_size; i++)
	{
		cur_node = arr[i];
		if (cur_node->name == name)
		{
			ok = 1;
			break;
		}
	}
	if (ok)
		return cur_node;
	
	printf("\nERROR *NodeAddress* --> Node doesn't define in [nodes]: (%d)\n", name);
	exit(EXIT_FAILURE);
}

void  NodeIsolate(NODE* V)
{
	for (int i = 0; i < V->SIZE_S; i++)
		V->sources[i] = NULL;
	for (int i = 0; i < V->SIZE_T; i++)
		V->targets[i] = NULL;
	return ;
}

void  NodePrint(const NODE* V)
{
	printf("NODE %d\n", V->name);
	printf("  Value:   %d\n", V->value);
	printf("  Sources: ");

	int k = 0;
	for (int i = 0; i < V->SIZE_S; i++)
		if (V->sources[i] != 0)
		{
			printf("%d ", V->sources[i]->name);
			k++;
		}
	printf("%s", k == 0 ? "None\n" : "\n");
	printf("  Targets: ");

	k = 0;
	for (int i = 0; i < V->SIZE_T; i++)
		if (V->targets[i] != 0)
		{
			printf("%d ", V->targets[i]->name);
			k++;
		}
	printf("%s", k == 0 ? "None\n" : "\n");
}

void NodeLink(NODE* source, NODE* target)
{

	int err1 = 0, err2 = 0;

	// source already know about target
	for (int k = 0; k < (source->SIZE_T); k++)
		if (source->targets[k] == target)
			err1 = 1;

	// target already know about source
	for (int k = 0; k < (target->SIZE_S); k++)
		if (target->sources[k] == source)
			err2 = 1;

	// define first empty place in array
	int i = 0, j = 0;
	while (source->targets[i] != 0) i++;
	while (target->sources[j] != 0) j++;

	// link the nodes
	if (!(err1 || err2))
	{
		source->targets[i] = target;
		target->sources[j] = source;
	}
	else if ((err1 == 0) && (err2 == 1))
		source->targets[i] = target;
	else if ((err1 == 1) && (err2 == 0))
		target->sources[j] = source;
}

//------------------------------------------------------------------------------------------------------

EDGE EdgeSet(NODE* source, NODE* target, int weight)
{
	EDGE E = { weight, source, target };
	return E;
}

void EdgePrint(const EDGE* E)
{
	printf("EDGE: (%d->%d) ", E->source->name, E->target->name);
	printf("Weight: %d\n", E->weight);
}

//------------------------------------------------------------------------------------------------------

GRAPH GraphSet(int num_nodes, int num_edges)
{
	GRAPH G = {	num_nodes,
				num_edges,
				(NODE**)calloc(num_nodes, sizeof(NODE*)),
			    (EDGE**)calloc(num_edges, sizeof(EDGE*)) };
	return G;
}

void  GraphPrint(const GRAPH* G)
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
			printf("(%2d->%2d)=%-2d%s", G->edges[i]->source->name, G->edges[i]->target->name,
										G->edges[i]->weight,
										((i+1) % 5 != 0) ? " " : "\n\t ");
	printf("%s", i == 0 ? "None\n\n" : "\n");
}

void GraphDestroy(GRAPH* G)
{
	for (int i = 0; i < G->SIZE_N; i++)
	{
		NodeIsolate(G->nodes[i]);
		free(G->nodes[i]->sources);
		free(G->nodes[i]->targets);
		free(G->nodes[i]);
	}
	free(G->nodes);
	for (int i = 0; i < G->SIZE_E; i++)
		free(G->edges[i]);
	free(G->edges);

	free(G);
}

int  GraphEdgeWeight(GRAPH* G, NODE* V1, NODE* V2)
{
	for (int i = 0; i < G->SIZE_E; i++)
		if (G->edges[i]->source == V1 && G->edges[i]->target == V2)
			return G->edges[i]->weight;
	return INT_MAX;
} 
