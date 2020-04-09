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
	int V_NUM = G->SIZE_N;

	// checked nodes array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(8 + V_NUM, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	EDGE** COST = (EDGE**)calloc(V_NUM, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < V_NUM; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			*COST[i] = EdgeSet(start, n, 0);
		else
			*COST[i] = EdgeSet(start, n, INT_MAX);
	}	

	// The algorithm:
	QUEUE Q = QueueSet(V_NUM * V_NUM);
	QueueAdd(&Q, start);

	printf("  Path: ");
	while (Q.last != 0)
	{
		// current node as poped from queue
		NODE* cur = QueuePop(&Q);
		int wave = CostGet(COST, cur, V_NUM);

		// Update the costs for every neighbour
		for (int i = 0; i < cur->SIZE_T; i++)
		{
			NODE* neighbour = cur->targets[i];
			//if (neighbour == NULL || Checker(checked, neighbour, check_num)) continue;
			if (neighbour == NULL) continue;

			if ( (wave + 1) < CostGet(COST, neighbour, V_NUM))
				CostSet(COST, neighbour, V_NUM, wave + 1);
		}
	
		// if node not checked
		if (!Checker(checked, cur, check_num))
		{
			printf("%d ", cur->name); // Part of path

			// append all the neighbours
			for (int i = 0; i < cur->SIZE_T; i++)
				if (cur->targets[i] != NULL)
					QueueAdd(&Q, cur->targets[i]);

			// check the node
			checked[check_num++] = cur;
		}
	}

	// Rezult
	puts("");
	for (int i = 0; i < V_NUM; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);		
		if (COST[i]->weight != INT_MAX)
			printf("%d step\n", COST[i]->weight);
		else
			printf("no way\n");
	}

	free(checked);
	free(Q.queue);
	for (int i = 0; i < V_NUM; i++)
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
	int V_NUM = G->SIZE_N;

	// checke node array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(8 + V_NUM, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	EDGE** COST = (EDGE**)calloc(V_NUM, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < V_NUM; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			*COST[i] = EdgeSet(start, n, 0);
		else
			*COST[i] = EdgeSet(start, n, INT_MAX);
	}


	// The algorithm:
	STACK S = StackSet(V_NUM * V_NUM);
	StackPush(&S, start);

	printf("  Path: ");
	while (S.last != 0)
	{
		NODE* cur = StackPop(&S);
		int wave = CostGet(COST, cur, V_NUM);

		// update cost for every neighbour
		for (int i = 0; i < cur->SIZE_T; i++)
		{
			NODE* neighbour = cur->targets[i];
			if (neighbour == NULL) continue;

			if ( (wave + 1) < CostGet(COST, neighbour, V_NUM) )
				CostSet(COST, neighbour, V_NUM, wave + 1);
		}

		// if node not checked
		if (!Checker(checked, cur, check_num))
		{
			printf("%d ", cur->name); // Part of path

			// append all the neighbours
			for (int i = 0; i < cur->SIZE_T; i++)
				if (cur->targets[i] != NULL)
					StackPush(&S, cur->targets[i]);

			// check the node
			checked[check_num++] = cur;
		}
	}


	// Rezult
	puts("");
	for (int i = 0; i < V_NUM; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);
		if (COST[i]->weight != INT_MAX)
			printf("%d step\n", COST[i]->weight);
		else
			printf("no way\n");;
	}


	free(checked);
	free(S.stack);
	for (int i = 0; i < V_NUM; i++)
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
	int V_NUM = G->SIZE_N;

	// checked nodes array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(8 + V_NUM, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	EDGE** COST = (EDGE**)calloc(V_NUM, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < V_NUM; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			*COST[i] = EdgeSet(start, n, 0);
		else
			*COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// The algorithm:
	int cur_cost = 0, new_cost = 0;
	NODE* cur = FindLowestCostNode(COST, checked, V_NUM);

	printf("  Path: ");
	while (cur != NULL)
	{
		printf("%d ", cur->name); // Part of path

		cur_cost = CostGet(COST, cur, V_NUM);

		for (int i = 0; i < cur->SIZE_T; i++)
		{
			if (cur->targets[i] == NULL)
				continue;

			NODE* neighbour = cur->targets[i];

			new_cost = cur_cost + GraphEdgeWeight(G, cur, neighbour);
			if (new_cost < CostGet(COST, neighbour, V_NUM))
				CostSet(COST, neighbour, V_NUM, new_cost);

		}

		checked[check_num++] = cur;
		cur = FindLowestCostNode(COST, checked, V_NUM);
	}

	// Rezult
	puts("");
	for (int i = 0; i < V_NUM; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);
		if (COST[i]->weight != INT_MAX)
			printf("%d km\n", COST[i]->weight);
		else
			printf("no way\n");
	}

	free(checked);
	for (int i = 0; i < V_NUM; i++)
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
	int V_NUM = G->SIZE_N;

	EDGE** COST = (EDGE**)calloc(V_NUM, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < V_NUM; i++)
	{
		COST[i] = (EDGE*)malloc(sizeof(EDGE));
		if (!COST[i]) exit(EXIT_FAILURE);

		NODE* n = G->nodes[i];
		if (n == start)
			*COST[i] = EdgeSet(start, n, 0);
		else
			*COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// The algorithm:
	for (int iter = 1; iter < V_NUM; iter++)
		for (int i = 0; i < G->SIZE_E; i++)
			Relax(COST, G->edges[i], G, V_NUM);


	// Rezult
	for (int i = 0; i < V_NUM; i++)
	{
		printf("  %d->%d = ", start->name, COST[i]->target->name);
		if (COST[i]->weight != INT_MAX)
			printf("%d km\n", COST[i]->weight);
		else
			printf("no way\n");
	}


	for (int i = 0; i < V_NUM; i++)
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
	int V_NUM = G->SIZE_N;

	EDGE*** matrix = (EDGE***)calloc(V_NUM, sizeof(EDGE**));
	if (!matrix) exit(EXIT_FAILURE);

	for (int i = 0; i < V_NUM; i++)
	{
		matrix[i] = (EDGE**)calloc(V_NUM, sizeof(EDGE*));
		if (!matrix[i]) exit(EXIT_FAILURE);

		NODE* source = G->nodes[i];
		for (int j = 0; j < V_NUM; j++)
		{
			matrix[i][j] = (EDGE*)malloc(sizeof(EDGE));
			if (!matrix[i][j]) exit(EXIT_FAILURE);

			NODE* target = G->nodes[j];
			if (i == j)
				*matrix[i][j] = EdgeSet(source, target, 0);
			else
				*matrix[i][j] = EdgeSet(source, target, GraphEdgeWeight(G, source, target));
		}
	}


	// The algorithm:
	for (int k = 0; k < V_NUM; k++)
		for (int i = 0; i < V_NUM; i++)
			for (int j = 0; j < V_NUM; j++)
			{
				int old_cost = matrix[i][j]->weight;
				long long new_cost = (long long)matrix[i][k]->weight + (long long)matrix[k][j]->weight;

				if (new_cost < old_cost)
					matrix[i][j]->weight = (int)new_cost;
			}

	// Rezult
	if (output)
	{
		puts("");
		for (int i = 0; i < V_NUM; i++)
		{
			printf("  %3d| ", matrix[i][0]->source->name);
			for (int j = 0; j < V_NUM; j++)
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
		if (edge->source == source_general &&
			edge->target == target_general)
			return edge;
	}
	return NULL;
}

void MST_Prim(GRAPH* G, NODE* root)
{
	// Init the tree
	GRAPH* tree = (GRAPH*)malloc(sizeof(GRAPH));
	if (!tree) exit(EXIT_FAILURE);
	tree->nodes = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	tree->edges = (EDGE**)calloc(G->SIZE_E, sizeof(EDGE*));
	if (!tree->nodes | !tree->edges) exit(EXIT_FAILURE);
	tree->SIZE_N = 0;
	tree->SIZE_E = 0;

	tree->nodes[tree->SIZE_N++] = root;
	EDGE* new_edge = FindRelevantEdge(G, tree);

	while (new_edge)
	{
		tree->nodes[tree->SIZE_N++] = new_edge->target;
		tree->edges[tree->SIZE_E++] = new_edge;
		new_edge = FindRelevantEdge(G, tree);
	}

	GraphPrint(tree);

	free(tree->edges);
	free(tree->nodes);
	free(tree);
	//return tree;
}

//------------------------------------------------------------------------------------------------------
// General Info about Graph (Radius, Diameter, Center, etc.)

void GeneralInfo(GRAPH* G)
{
	int V_NUM = G->SIZE_N;

	EDGE*** matrix = FloydWarshall(G, 0);
	int Radius = INT_MAX, Diametr = INT_MIN;

	typedef struct eccs { NODE* node; int ecc; } ECCs;
	ECCs* arr = (ECCs*)calloc(V_NUM, sizeof(ECCs));
	if (!arr) exit(EXIT_FAILURE);

	for (int i = 0; i < V_NUM; i++)
	{
		int cur_ecc = INT_MIN;
		for (int j = 0; j < V_NUM; j++)
			if (matrix[i][j]->weight > cur_ecc && matrix[i][j]->weight != INT_MAX)
				cur_ecc = matrix[i][j]->weight;
		arr[i] = (ECCs){ matrix[i][0]->source, cur_ecc };
	}

	for (int i = 0; i < V_NUM; i++)
	{
		//printf("%d: %d\n", arr[i].node->name, arr[i].ecc);
		if (arr[i].ecc < Radius)
			Radius = arr[i].ecc;
		if (arr[i].ecc > Diametr)
			Diametr = arr[i].ecc;
	}

	printf("  This is %s\n", (ORIENTED_GRAPH) ? "Oriented Graph" : "Non-Oriented Graph");
	printf("  Radius: %d\n  Diameter: %d\n", Radius, Diametr);
	printf("  Center: ");
	for (int i = 0; i < V_NUM; i++)
	{
		if (arr[i].ecc == Radius)
			printf("(%d) ", arr[i].node->name);
	}
	puts("");

	free(arr);
	FloydFree(G, matrix);
}
