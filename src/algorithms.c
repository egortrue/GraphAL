#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>

// Common help-functions
int  Checker(void** arr, void* obj, int size_arr)
{
	for (int i = 0; i <= size_arr; i++)
		if (arr[i] == obj)
			return 1;
	return 0;
}
int  CostGet(EDGE** arr, NODE* obj, int size_arr)
{
	for (int i = 0; i < size_arr; i++)
		if (arr[i]->target == obj)
			return arr[i]->weight;
	return INT_MAX;
}
void CostSet(EDGE** arr, NODE* obj, int size_arr, int new_cost)
{
	for (int i = 0; i < size_arr; i++)
		if (arr[i]->target == obj)
			arr[i]->weight = new_cost;
}

//------------------------------------------------------------------------------------------------------
// BFS with QUEUE structure

typedef struct queue  
{
	// last - first empty place
	// if last == 0 --> queue is empty
	int last, SIZE_;
	NODE** queue;
} QUEUE;
QUEUE QueueSet   (int SIZE_)
{
	QUEUE Q = { 0, SIZE_, (NODE**)calloc(SIZE_, sizeof(NODE*)) };
	return Q;
}
void  QueuePrint (QUEUE* Q)
{
	printf("Queue: ");
	for (int i = 0; i < Q->SIZE_; i++)
		if (Q->queue[i] != 0)
			printf("%d ", Q->queue[i]->name);
	puts("");
}
void  QueueAdd   (QUEUE* Q, NODE* V)
{
	Q->queue[(Q->last)++] = V;
}
NODE* QueuePop   (QUEUE* Q)
{
	NODE* V = Q->queue[0];
	NODE** tmp = (NODE**)calloc(Q->SIZE_, sizeof(NODE*));
	if (!tmp) exit(EXIT_FAILURE);

	for (int i = 1; i < Q->SIZE_; i++)
		tmp[i - 1] = Q->queue[i];

	free(Q->queue);
	Q->queue = tmp;
	Q->last--;
	return V;
}

void BFS(GRAPH* G, NODE* start)
{
	// Checked nodes array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	// --------------------------------------------------------

	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}	

	// --------------------------------------------------------
	// The algorithm:
	QUEUE Q = QueueSet(G->SIZE_N * G->SIZE_N);
	QueueAdd(&Q, start);

	printf("  Path: ");
	while (Q.last != 0)
	{
		// Current node as poped from queue
		NODE* cur = QueuePop(&Q);
		int wave = CostGet(COST, cur, G->SIZE_N);

		// Update the costs for every neighbour
		for (int i = 0; i < cur->SIZE_T; i++)
		{
			NODE* neighbour = cur->targets[i];
			//if (neighbour == NULL || Checker(checked, neighbour, check_num)) continue;
			if (neighbour == NULL) continue;

			if ( (wave + 1) < CostGet(COST, neighbour, G->SIZE_N))
				CostSet(COST, neighbour, G->SIZE_N, wave + 1);
		}
	
		// If node not checked
		if (!Checker(checked, cur, check_num))
		{
			printf("%d ", cur->name); // Part of path

			// Append all the neighbours
			for (int i = 0; i < cur->SIZE_T; i++)
				if (cur->targets[i] != NULL)
					QueueAdd(&Q, cur->targets[i]);

			// Check the node
			checked[check_num++] = cur;
		}
	}

	// Rezult
	puts("");
	for (int i = 0; i < G->SIZE_N; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);		
		if (COST[i]->weight != INT_MAX)
			printf("%d step\n", COST[i]->weight);
		else
			printf("no way\n");
	}

	free(checked);
	free(Q.queue);
	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);
}

//------------------------------------------------------------------------------------------------------
// DFS with STACK structure

typedef struct stack 
{
	// last - first empty place
	// if last == 0 --> stack is empty
	int last, SIZE_;
	NODE** stack;
} STACK;
STACK StackSet  (int size) 
{
	STACK S = { 0, size, (NODE**)calloc(size, sizeof(NODE*)) };
	return S;			
}
void  StackPrint(STACK* S)
{
	printf("Stack: ");
	for (int i = 0; i < S->SIZE_; i++)
		if (S->stack[i] != 0)
			printf("%d ", S->stack[i]->name);
	puts("");
}
void  StackPush (STACK* S, NODE* V)
{
	S->stack[S->last++] = V;
}
NODE* StackPop  (STACK* S)
{
	S->last--;
	NODE* V = S->stack[S->last];
	S->stack[S->last] = NULL;
	return V;
}

void DFS(GRAPH* G, NODE* start)
{
	// Checked node array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(8 + G->SIZE_N, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}


	// The algorithm:
	STACK S = StackSet(G->SIZE_N * G->SIZE_N);
	StackPush(&S, start);

	printf("  Path: ");
	while (S.last != 0)
	{
		NODE* cur = StackPop(&S);
		int wave = CostGet(COST, cur, G->SIZE_N);

		// Update cost for every neighbour
		for (int i = 0; i < cur->SIZE_T; i++)
		{
			NODE* neighbour = cur->targets[i];
			if (neighbour == NULL) continue;

			if ( (wave + 1) < CostGet(COST, neighbour, G->SIZE_N) )
				CostSet(COST, neighbour, G->SIZE_N, wave + 1);
		}

		// If node not checked
		if (!Checker(checked, cur, check_num))
		{
			printf("%d ", cur->name); // Part of path

			// Append all the neighbours
			for (int i = 0; i < cur->SIZE_T; i++)
				if (cur->targets[i] != NULL)
					StackPush(&S, cur->targets[i]);

			// Check the node
			checked[check_num++] = cur;
		}
	}


	// Rezult
	puts("");
	for (int i = 0; i < G->SIZE_N; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);
		if (COST[i]->weight != INT_MAX)
			printf("%d step\n", COST[i]->weight);
		else
			printf("no way\n");;
	}


	free(checked);
	free(S.stack);
	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);
}

//------------------------------------------------------------------------------------------------------
// Deikstra with help-function

NODE* FindLowestCostNode(EDGE** cost, NODE** processed, int size)
{
	int lowest_cost = INT_MAX, cur_cost = 0;
	NODE* V = NULL;

	for (int i = 0; i < size; i++)
	{
		cur_cost = cost[i]->weight;
		if (cur_cost < lowest_cost && !Checker(processed, cost[i]->target, size))
		{
			lowest_cost = cur_cost;
			V = cost[i]->target;
		}
	}
	return V;
}
void Deijkstra(GRAPH* G, NODE* start)
{
	// Checked nodes array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(8 + G->SIZE_N, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// The algorithm:
	int cur_cost = 0, new_cost = 0;
	NODE* cur = FindLowestCostNode(COST, checked, G->SIZE_N);

	printf("  Path: ");
	while (cur != NULL)
	{
		printf("%d ", cur->name); // Part of path

		cur_cost = CostGet(COST, cur, G->SIZE_N);

		for (int i = 0; i < cur->SIZE_T; i++)
		{
			if (cur->targets[i] == NULL)
				continue;

			NODE* neighbour = cur->targets[i];

			new_cost = cur_cost + GraphEdgeWeight(G, cur, neighbour);
			if (new_cost < CostGet(COST, neighbour, G->SIZE_N))
				CostSet(COST, neighbour, G->SIZE_N, new_cost);

		}

		checked[check_num++] = cur;
		cur = FindLowestCostNode(COST, checked, G->SIZE_N);
	}

	// Rezult
	puts("");
	for (int i = 0; i < G->SIZE_N; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);
		if (COST[i]->weight != INT_MAX)
			printf("%d km\n", COST[i]->weight);
		else
			printf("no way\n");
	}

	free(checked);
	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);
}

//------------------------------------------------------------------------------------------------------
// Bellman-Ford / Relaxation

void Relax(EDGE** COST, EDGE* E, GRAPH* G, int size)
{
	int source_w = CostGet(COST, E->source, size);
	if (source_w == INT_MAX)
		return;

	int new_cost = source_w + E->weight;
	int old_cost = CostGet(COST, E->target, size);
	if (new_cost < old_cost)
		CostSet(COST, E->target, size, new_cost);
}
void BellmanFord(GRAPH* G, NODE* start)
{	
	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// The algorithm:
	for (int iter = 1; iter < G->SIZE_N; iter++)
		for (int i = 0; i < G->SIZE_E; i++)
			Relax(COST, G->edges[i], G, G->SIZE_N);


	// Rezult
	for (int i = 0; i < G->SIZE_N; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);
		if (COST[i]->weight != INT_MAX)
			printf("%d km\n", COST[i]->weight);
		else
			printf("no way\n");
	}


	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);
}

//------------------------------------------------------------------------------------------------------
// Floyd-Warshall

void FloydFree(GRAPH* G, EDGE*** matrix)
{
	for (int i = 0; i < G->SIZE_N; i++)
	{
		for (int j = 0; j < G->SIZE_N; j++)
			free(matrix[i][j]);
		free(matrix[i]);
	}
	free(matrix);
}
EDGE*** FloydWarshall(GRAPH* G, int output)
{
	// This is a matrix which includes weight between each two nodes
	EDGE*** matrix = (EDGE***)calloc(G->SIZE_N, sizeof(EDGE**));
	if (!matrix) exit(EXIT_FAILURE);
	for (int i = 0; i < G->SIZE_N; i++)
	{
		matrix[i] = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
		if (!matrix[i]) exit(EXIT_FAILURE);

		NODE* source = G->nodes[i];
		for (int j = 0; j < G->SIZE_N; j++)
		{
			matrix[i][j] = (EDGE*)malloc(sizeof(EDGE));
			if (!matrix[i][j]) exit(EXIT_FAILURE);

			NODE* target = G->nodes[j];
			if (i == j)
				matrix[i][j] = EdgeSet(source, target, 0);
			else
				matrix[i][j] = EdgeSet(source, target, GraphEdgeWeight(G, source, target));
		}
	}

	// --------------------------------------------------------
	// The algorithm:
	for (int k = 0; k < G->SIZE_N; k++)
		for (int i = 0; i < G->SIZE_N; i++)
			for (int j = 0; j < G->SIZE_N; j++)
			{
				int old_cost = matrix[i][j]->weight;
				long long new_cost = (long long)matrix[i][k]->weight + (long long)matrix[k][j]->weight;

				if (new_cost < old_cost)
					matrix[i][j]->weight = (int)new_cost;
			}

	// --------------------------------------------------------
	// Rezult
	if (output)
	{
		puts("");
		printf("       ");
		for (int i = 0; i < G->SIZE_N; i++)
			printf("%3d  ", matrix[i][0]->source->name);
		puts("");
		printf("     ");
		for (int i = 0; i < G->SIZE_N; i++)
			printf("-----");
		puts("");
		for (int i = 0; i < G->SIZE_N; i++)
		{
			printf("  %3d| ", matrix[i][0]->source->name);
			for (int j = 0; j < G->SIZE_N; j++)
			{
				if (matrix[i][j]->weight != INT_MAX)
					printf("%3d  ", matrix[i][j]->weight);
				else
					printf("inf  ");
			}
			puts("");
		}
		puts("\n    P.S. Name column equal name row / This's the square matrix");
		FloydFree(G, matrix);
		return NULL;
	}
	else
		return matrix;
}

//------------------------------------------------------------------------------------------------------
// Minimum-spanning-tree (MST)

EDGE* FindRelevantEdge(GRAPH* G, GRAPH* tree)
{
	NODE* source_general = NULL;
	NODE* target_general = NULL;
	 int  cost = INT_MAX;

	for (int i = 0; i < tree->SIZE_N; i++)
	{
		NODE* source = tree->nodes[i];
		NODE* target = NULL;
		int edge_cost_source = INT_MAX;

		for (int j = 0; j < source->SIZE_T; j++)
		{
			NODE* target_cur = source->targets[j];
			if (!target_cur) continue;
			if (Checker(tree->nodes, target_cur, tree->SIZE_N)) continue;

			int edge_cost_source_cur = GraphEdgeWeight(G, source, target_cur);
			if (edge_cost_source_cur < edge_cost_source)
			{
				edge_cost_source = edge_cost_source_cur;
				target = target_cur;
			}
		}

		if (edge_cost_source < cost)
		{
			source_general = source;
			target_general = target;
			cost = edge_cost_source;
		}
	}


	for (int i = 0; i < G->SIZE_E; i++)
	{
		EDGE* edge = G->edges[i];
		if (edge->target == target_general) return edge;
	}
	return NULL;
}
void Prim(GRAPH* G, NODE* root)
{
	// Init the tree
	GRAPH* tree = (GRAPH*)malloc(sizeof(GRAPH));
	if (!tree) exit(EXIT_FAILURE);
	tree->nodes = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	tree->edges = (EDGE**)calloc(G->SIZE_E, sizeof(EDGE*));
	if (!tree->nodes | !tree->edges) exit(EXIT_FAILURE);
	tree->SIZE_N = 0;
	tree->SIZE_E = 0;

	// --------------------------------------------------------
	// The algorithm
	tree->nodes[tree->SIZE_N++] = root;
	EDGE* new_edge = FindRelevantEdge(G, tree);
	while (new_edge)
	{
		tree->nodes[tree->SIZE_N++] = new_edge->target;
		tree->edges[tree->SIZE_E++] = new_edge;
		new_edge = FindRelevantEdge(G, tree);
	}

	// --------------------------------------------------------
	// Rezult
	if (tree->SIZE_N != G->SIZE_N)
		printf("\nThere are not exist spanning tree in this graph.\n");
	else
	{
		GraphPrint(tree);
		int weight = 0;
		for (int i = 0; i < tree->SIZE_E; i++)
			weight += tree->edges[i]->weight;
		printf("\n  Weight of tree: %d\n", weight);
	}

	free(tree->edges);
	free(tree->nodes);
	free(tree);
	//return tree;
}

int edges_compare(const void* val1, const void* val2) // for qsort
{
	const EDGE* e1 = *((const EDGE**)val1);
	const EDGE* e2 = *((const EDGE**)val2);

	if (e1->weight < e2->weight)  return -1;
	if (e1->weight == e2->weight) return  0;
	if (e1->weight > e2->weight)  return  1;
	return 0;
}
void Kruskal(GRAPH* G)
{
	// Init the tree
	GRAPH* tree = (GRAPH*)malloc(sizeof(GRAPH));
	if (!tree) exit(EXIT_FAILURE);
	tree->nodes = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	tree->edges = (EDGE**)calloc(G->SIZE_E, sizeof(EDGE*));
	if (!tree->nodes | !tree->edges) exit(EXIT_FAILURE);
	tree->SIZE_N = 0;
	tree->SIZE_E = 0;

	// --------------------------------------------------------
	// Remember old values of nodes
	int* old_values = (int*)calloc(G->SIZE_N, sizeof(int));
	if (!old_values) exit(EXIT_FAILURE);
	for (int i = 0; i < G->SIZE_N; i++)
		old_values[i] = G->nodes[i]->value;

	// --------------------------------------------------------
	// One node = one set with current id
	for (int i = 0; i < G->SIZE_N; i++)
		G->nodes[i]->value = i + 100;

	// --------------------------------------------------------
	// Sort the edges
	EDGE** edges = (EDGE**)calloc(G->SIZE_E, sizeof(EDGE*));
	if (!edges) exit(EXIT_FAILURE);
	for (int i = 0; i < G->SIZE_E; i++)
		edges[i] = G->edges[i];
	qsort(edges, G->SIZE_E, sizeof(EDGE*), edges_compare);

	// --------------------------------------------------------
	// Check the set's id
	for (int i = 0; i < G->SIZE_E; i++)
	{
		NODE* source = edges[i]->source;
		NODE* target = edges[i]->target;

		if (source->value != target->value)
		{
			// Update set's id
			for (int j = 0; j < G->SIZE_N; j++)
				if (G->nodes[j]->value == target->value)
					G->nodes[j]->value = source->value;

			// Append new edge to tree
			tree->edges[tree->SIZE_E++] = edges[i];

			// Append new nodes to tree
			int source_skip = 0, target_skip = 0;
			for (int j = 0; j < tree->SIZE_N; j++)
			{
				if (tree->nodes[j] == source) source_skip = 1;
				if (tree->nodes[j] == target) target_skip = 1;
			}
			if (tree->SIZE_N < G->SIZE_N)
			{
				if (!source_skip) tree->nodes[tree->SIZE_N++] = source;
				if (!target_skip) tree->nodes[tree->SIZE_N++] = target;
			}
		}
	}

	// --------------------------------------------------------
	// Restore old values
	for (int i = 0; i < G->SIZE_N; i++)
		G->nodes[i]->value = old_values[i];

	// --------------------------------------------------------
	// Rezult
	if (tree->SIZE_N != G->SIZE_N)
		printf("\nThere are not exist spanning tree in this graph.\n");
	else
	{
		GraphPrint(tree);
		int weight = 0;
		for (int i = 0; i < tree->SIZE_E; i++)
			weight += tree->edges[i]->weight;
		printf("\n  Weight of tree: %d\n", weight);
	}

	free(tree->edges);
	free(tree->nodes);
	free(tree);
	// return tree;
}

//------------------------------------------------------------------------------------------------------
// General Info about Graph (Radius, Diameter, Center, etc.)
void GeneralInfo(GRAPH* G)
{
	EDGE*** matrix = FloydWarshall(G, 0);
	int Radius = INT_MAX, Diameter = INT_MIN;

	typedef struct eccs { NODE* node; int ecc; } ECCs;
	ECCs* arr = (ECCs*)calloc(G->SIZE_N, sizeof(ECCs));
	if (!arr) exit(EXIT_FAILURE);

	// --------------------------------------------------------
	// Define the eccentricities of each node
	for (int i = 0; i < G->SIZE_N; i++)
	{
		int cur_ecc = INT_MIN;
		for (int j = 0; j < G->SIZE_N; j++)
			if (matrix[i][j]->weight > cur_ecc && matrix[i][j]->weight != INT_MAX)
				cur_ecc = matrix[i][j]->weight;
		arr[i] = (ECCs){ matrix[i][0]->source, cur_ecc };
	}

	// --------------------------------------------------------
	// Define the radius and diameter
	for (int i = 0; i < G->SIZE_N; i++)
	{
		if (arr[i].ecc < Radius)
			Radius = arr[i].ecc;
		if (arr[i].ecc > Diameter)
			Diameter = arr[i].ecc;
	}

	// --------------------------------------------------------
	// Rezult
	printf("  This is %s graph\n", (G->directed) ? "directed" : "undirected");
	printf("  Radius: %d\n  Diameter: %d\n", Radius, Diameter);
	printf("  Center: ");
	for (int i = 0; i < G->SIZE_N; i++)
		if (arr[i].ecc == Radius)
			printf("(%d) ", arr[i].node->name);
	puts("");

	free(arr);
	FloydFree(G, matrix);
}
