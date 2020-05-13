#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include "algorithms.h"

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

typedef struct qu_node 
{
	NODE* node;
	struct qu_node* next;
} QU_NODE;
QU_NODE* QueuePush(QU_NODE* Q, NODE* V)
{
	QU_NODE* tmp = (QU_NODE*)malloc(sizeof(QU_NODE));
	if (!tmp) exit(EXIT_FAILURE);
	tmp->node = V;
	tmp->next = NULL;

	if (!Q) return tmp;
	QU_NODE* ptr = Q;
	while (ptr->next) ptr = ptr->next;
	ptr->next = tmp;
	return Q;
}
QU_NODE* QueuePop(QU_NODE* Q)
{
	QU_NODE* new_first = Q->next;
	free(Q);
	return new_first;
}
NODE* QueueGet(QU_NODE* Q)
{
	return Q != NULL ? Q->node : NULL;
}
int QueueIsEmpty(QU_NODE* Q)
{
	return Q == NULL ? 1 : 0;
}
void QueueDestroy(QU_NODE* Q)
{
	while (Q != NULL) Q = QueuePop(Q->next);
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
		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}	

	// --------------------------------------------------------
	// The algorithm:
	QU_NODE* first = QueuePush(NULL, start);

	printf("  Path: ");
	while (!QueueIsEmpty(first))
	{
		// Current node as poped from queue
		NODE* cur = QueueGet(first);
		first = QueuePop(first);
		int wave = CostGet(COST, cur, G->SIZE_N);

		// Get neighbors info
		NODE** neighbors_array = GraphGetNodeNeighbors(G, cur);
		if (!neighbors_array) continue;
		int neighbors_count = _msize(neighbors_array) / sizeof(neighbors_array[0]);

		// Update the costs for every neighbor
		for (int i = 0; i < neighbors_count; i++)
		{
			NODE* neighbor = neighbors_array[i];

			if ( (wave + 1) < CostGet(COST, neighbor, G->SIZE_N))
				CostSet(COST, neighbor, G->SIZE_N, wave + 1);
		}
	
		// If node not checked
		if (!Checker(checked, cur, check_num))
		{
			printf("%d ", cur->name); // Path

			// Append all the neighbors
			for (int i = 0; i < neighbors_count; i++)
					first = QueuePush(first, neighbors_array[i]);

			// Check the node
			checked[check_num++] = cur;
		}

		free(neighbors_array);
	}

	// --------------------------------------------------------
	// Result
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
	QueueDestroy(first);
	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);
}

//------------------------------------------------------------------------------------------------------
// DFS with STACK structure

typedef struct st_node
{ 
	NODE* node;
	struct st_node* next;
} ST_NODE;
ST_NODE* StackPush(ST_NODE* top, NODE* V)
{
	ST_NODE* tmp = (ST_NODE*)malloc(sizeof(ST_NODE));
	if (!tmp) exit(EXIT_FAILURE);
	tmp->node = V;
	tmp->next = NULL;

	// Connecting temp object
	if (top == NULL)
		top = tmp;
	else
	{
		tmp->next = top;
		top = tmp;
	}
	return top;
}
ST_NODE* StackPop(ST_NODE* top)
{
	if (top == NULL) return NULL;
	ST_NODE* tmp = top->next;
	free(top);
	top = tmp;
	return top;
}
NODE* StackGet(ST_NODE* top)
{
	return top != NULL ? top->node : NULL;
}
int StackIsEmpty(ST_NODE* top)
{
	return top == NULL ? 1 : 0;
}
void StackDestroy(ST_NODE* top)
{
	while (top != NULL) top = StackPop(top);
}

void DFS(GRAPH* G, NODE* start)
{
	// Checked node array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(8 + G->SIZE_N, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	// --------------------------------------------------------

	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// --------------------------------------------------------
	// The algorithm:
	ST_NODE* top = StackPush(NULL, start);

	printf("  Path: ");
	while (!StackIsEmpty(top))
	{
		NODE* cur = StackGet(top);
		top = StackPop(top);
		int wave = CostGet(COST, cur, G->SIZE_N);

		// Get neighbors info
		NODE** neighbors_array = GraphGetNodeNeighbors(G, cur);
		if (!neighbors_array) continue;
		int neighbors_count = _msize(neighbors_array) / sizeof(neighbors_array[0]);

		// Update cost for every neighbor
		for (int i = 0; i < neighbors_count; i++)
		{
			NODE* neighbor = neighbors_array[i];

			if ( (wave + 1) < CostGet(COST, neighbor, G->SIZE_N) )
				CostSet(COST, neighbor, G->SIZE_N, wave + 1);
		}

		// If node not checked
		if (!Checker(checked, cur, check_num))
		{
			printf("%d ", cur->name); // Path

			// Append all the neighbors
			for (int i = 0; i < neighbors_count; i++)
					top = StackPush(top, neighbors_array[i]);

			// Check the node
			checked[check_num++] = cur;
		}

		free(neighbors_array);
	}

	// --------------------------------------------------------
	// Result
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
	StackDestroy(top);
	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);
}

//------------------------------------------------------------------------------------------------------
// Deikstra with help-function

NODE* Deijkstra_FindLowestCostNode(EDGE** cost, NODE** processed, int size)
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

	// --------------------------------------------------------

	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// --------------------------------------------------------
	// The algorithm:
	int cur_cost = 0, new_cost = 0;
	NODE* cur = Deijkstra_FindLowestCostNode(COST, checked, G->SIZE_N);

	printf("  Path: ");
	while (cur != NULL)
	{
		printf("%d ", cur->name); // Path
		cur_cost = CostGet(COST, cur, G->SIZE_N);

		// Get neighbors info
		NODE** neighbors_array = GraphGetNodeNeighbors(G, cur);
		if (!neighbors_array)
		{
			checked[check_num++] = cur;
			cur = Deijkstra_FindLowestCostNode(COST, checked, G->SIZE_N);
			continue;
		}
		int neighbors_count = _msize(neighbors_array) / sizeof(neighbors_array[0]);

		for (int i = 0; i < neighbors_count; i++)
		{
			NODE* neighbor = neighbors_array[i];

			new_cost = cur_cost + GraphGetEdgeWeight(G, cur, neighbor);
			if (new_cost < CostGet(COST, neighbor, G->SIZE_N))
				CostSet(COST, neighbor, G->SIZE_N, new_cost);
		}

		checked[check_num++] = cur;
		cur = Deijkstra_FindLowestCostNode(COST, checked, G->SIZE_N);
		free(neighbors_array);
	}

	// --------------------------------------------------------
	// Result
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

void Bellman_Relax(EDGE** COST, EDGE* E, GRAPH* G, int size)
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
		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(start, n, 0);
		else
			COST[i] = EdgeSet(start, n, INT_MAX);
	}

	// --------------------------------------------------------
	// The algorithm:
	for (int iter = 1; iter < G->SIZE_N; iter++)
		for (int i = 0; i < G->SIZE_E; i++)
			Bellman_Relax(COST, G->edges[i], G, G->SIZE_N);

	// --------------------------------------------------------
	// Result
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

void Floyd_MatrixFree(GRAPH* G, EDGE*** matrix)
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
			NODE* target = G->nodes[j];
			if (i == j)
				matrix[i][j] = EdgeSet(source, target, 0);
			else
				matrix[i][j] = EdgeSet(source, target, GraphGetEdgeWeight(G, source, target));
		}
	}

	// --------------------------------------------------------
	// The algorithm:
	for (int k = 0; k < G->SIZE_N; k++)
		for (int i = 0; i < G->SIZE_N; i++)
			for (int j = 0; j < G->SIZE_N; j++)
			{
				int old_cost = matrix[i][j]->weight;
				
				// In case when we sum INT_MAX and INT_MAX
				long long new_cost = (long long)matrix[i][k]->weight + (long long)matrix[k][j]->weight;

				if (new_cost < old_cost)
					matrix[i][j]->weight = (int)new_cost;
			}

	// --------------------------------------------------------
	// Result (with output mode)
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
		Floyd_MatrixFree(G, matrix);
		return NULL;
	}
	else
		return matrix;
}

//------------------------------------------------------------------------------------------------------
// Minimum-spanning-tree (MST)

EDGE* Prim_FindRelevantEdge(GRAPH* G, GRAPH* tree)
{
	EDGE* relevant = NULL;
	for (int i = 0; i < tree->SIZE_N; i++)
	{
		NODE** neigbours = GraphGetNodeNeighbors(G, tree->nodes[i]);
		int neighbors_count = _msize(neigbours) / sizeof(neigbours[0]);

		for (int j = 0; j < neighbors_count; j++)
		{

			if (Checker(tree->nodes, neigbours[j], tree->SIZE_N)) continue;
			EDGE* cur = GraphGetEdge(G, tree->nodes[i], neigbours[j]);
			if (Checker(tree->edges, cur, tree->SIZE_E)) continue;

			if (relevant == NULL)
				relevant = cur;
			else if (cur->weight < relevant->weight)
				relevant = cur;
		}
		free(neigbours);
	}
	return relevant;
}
void Prim(GRAPH* G, NODE* root)
{
	if (G->directed)
	{
		printf("ERROR: This is directed graph\n");
		return;
	}

	// --------------------------------------------------------
	// Init the tree
	GRAPH* tree = (GRAPH*)malloc(sizeof(GRAPH));
	if (!tree) exit(EXIT_FAILURE);
	tree->nodes = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	tree->edges = (EDGE**)calloc(G->SIZE_E, sizeof(EDGE*)); 
	if (!tree->nodes || !tree->edges) exit(EXIT_FAILURE);
	tree->directed = 0;
	tree->SIZE_N = 0;
	tree->SIZE_E = 0;

	// --------------------------------------------------------
	// The algorithm
	tree->nodes[tree->SIZE_N++] = root;
	EDGE* new_edge = Prim_FindRelevantEdge(G, tree);
	while (new_edge)
	{
		tree->edges[tree->SIZE_E++] = new_edge;
		if (!Checker(tree->nodes, new_edge->source, tree->SIZE_N)) tree->nodes[tree->SIZE_N++] = new_edge->source;
		if (!Checker(tree->nodes, new_edge->target, tree->SIZE_N)) tree->nodes[tree->SIZE_N++] = new_edge->target;
		new_edge = Prim_FindRelevantEdge(G, tree);
	}

	// --------------------------------------------------------
	// Result
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
	if (G->directed)
	{
		printf("ERROR: This is directed graph\n");
		return;
	}

	// --------------------------------------------------------
	// Init the tree
	GRAPH* tree = (GRAPH*)malloc(sizeof(GRAPH));
	if (!tree) exit(EXIT_FAILURE);
	tree->nodes = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	tree->edges = (EDGE**)calloc(G->SIZE_E, sizeof(EDGE*));
	if (!tree->nodes || !tree->edges) exit(EXIT_FAILURE);
	tree->directed = 0;
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
	// Result
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

	free(old_values);
	free(edges);
	free(tree->edges);
	free(tree->nodes);
	free(tree);
	// return tree;
}

//------------------------------------------------------------------------------------------------------
// Flow network

void FordFalkerson(GRAPH* G, NODE* source, NODE* target)
{
	if (!G->directed)
	{
		printf("ERROR: This is undirected graph\n");
		return;
	}

	// --------------------------------------------------------
	// Preparation

	typedef struct flow_edge
	{
		EDGE* edge;
		 int  flow;
	}FLN;

	FLN** flow_net = (FLN**)calloc(G->SIZE_E, sizeof(FLN*));
	if (!flow_net) exit(EXIT_FAILURE);

	for (int i = 0; i < G->SIZE_E; i++)
	{
		flow_net[i] = (FLN*)malloc(sizeof(FLN));
		if (!flow_net[i]) exit(EXIT_FAILURE);

		flow_net[i]->edge = G->edges[i];
		flow_net[i]->flow = 0;
	}

	// Nodes of current flow path
	int path_len = 0;
	NODE** path = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	if (!path) exit(EXIT_FAILURE);

	// Remember old values of nodes and set the flags
	int* old_values = (int*)calloc(G->SIZE_N, sizeof(int));
	if (!old_values) exit(EXIT_FAILURE);
	for (int i = 0; i < G->SIZE_N; i++)
	{
		old_values[i] = G->nodes[i]->value;
		G->nodes[i]->value = 0;
	}
		

	// --------------------------------------------------------
	// The algorithm:

	ST_NODE* top = StackPush(NULL, source);
	while (!StackIsEmpty(top))
	{
		NODE* cur = StackGet(top);
		if (cur == target)
		{
			path[path_len++] = cur;
			printf("%d ", cur->name);

			// Remake node path to flow path
			FLN** path_fln = (FLN**)calloc(path_len - 1, sizeof(FLN*));
			if (!path_fln) exit(EXIT_FAILURE);			
			for (int i = 1; i < path_len; i++)
			{
				EDGE* path_edge_cur = GraphGetEdge(G, path[i - 1], path[i]);
				for (int j = 0; j < G->SIZE_E; j++)
					if (flow_net[j]->edge == path_edge_cur)
						path_fln[i - 1] = flow_net[j];
			}

			
			// Find the lowest flow capacity in the way
			int path_len_edge = path_len - 1;
			int min_flow_capacity = INT_MAX;
			for (int i = 0; i < path_len_edge; i++)
			{
				int residual_path = path_fln[i]->edge->weight - path_fln[i]->flow;
				if (residual_path < min_flow_capacity)
					min_flow_capacity = residual_path;
			}


			// Update flow in each edge
			for (int i = 0; i < path_len_edge; i++)
				path_fln[i]->flow += min_flow_capacity;


			// Return to source
			free(path_fln);
			StackDestroy(top);
			for (int i = 0; i < path_len; i++)
				path[i] = NULL;
			path_len = 0;

			top = StackPush(NULL, source);	
		}
		else
		{
			// Get neighbors info
			NODE** neighbors_array = GraphGetNodeNeighbors(G, cur);
			if (!neighbors_array) exit(EXIT_FAILURE);
			int neighbors_count = _msize(neighbors_array) / sizeof(neighbors_array[0]);


			// Edges with neighbors == targets
			FLN** neighbors_fln = (FLN**)calloc(neighbors_count, sizeof(FLN*));
			if (!neighbors_fln) exit(EXIT_FAILURE);
			for (int i = 0; i < neighbors_count; i++)
			{
				EDGE* negihbor_edge = GraphGetEdge(G, cur, neighbors_array[i]);
				for (int j = 0; j < G->SIZE_E; j++)
					if (flow_net[j]->edge == negihbor_edge)
						neighbors_fln[i] = flow_net[j];
			}
			free(neighbors_array);


			// Find max relevant edge
			NODE* neighbor_relevant = NULL;
			int max_flow_capacity = 0;
			for (int i = 0; i < neighbors_count; i++)
			{
				int residual_neighbor = neighbors_fln[i]->edge->weight - neighbors_fln[i]->flow;
				if (max_flow_capacity < residual_neighbor
					&& !neighbors_fln[i]->edge->target->value
					&& !Checker(path, neighbors_fln[i]->edge->target, path_len))
				{
					neighbor_relevant = neighbors_fln[i]->edge->target;
					max_flow_capacity = residual_neighbor;
				}
			}
			free(neighbors_fln);

			
			// Step back or step forward
			if (neighbor_relevant == NULL)
			{
				if (cur == source) break;
				cur->value = 1;
				top = StackPop(top);
				path[--path_len] = NULL;
			}
			else
			{
				top = StackPush(top, neighbor_relevant);
				path[path_len++] = cur;
			}
		}
	}

	// --------------------------------------------------------
	// Restore old values
	for (int i = 0; i < G->SIZE_N; i++)
		G->nodes[i]->value = old_values[i];


	// --------------------------------------------------------
	// Result
	for (int i = 0; i < G->SIZE_E; i++)
	{
		EdgePrint(flow_net[i]->edge);
		printf(" %d / %d \n\n", flow_net[i]->flow, flow_net[i]->edge->weight);
	}


	free(old_values);
	free(path);
	StackDestroy(top);
	for (int i = 0; i < G->SIZE_E; i++)
		free(flow_net[i]);
	free(flow_net);
}

//------------------------------------------------------------------------------------------------------
// Other algorithms


// Define the eccentricities of each node
void Eccentricity(GRAPH* G)
{
	EDGE*** matrix = FloydWarshall(G, 0);

	for (int i = 0; i < G->SIZE_N; i++)
	{
		int cur_ecc = INT_MIN;
		for (int j = 0; j < G->SIZE_N; j++)
			if (matrix[i][j]->weight > cur_ecc && matrix[i][j]->weight != INT_MAX)
				cur_ecc = matrix[i][j]->weight;
		matrix[i][0]->source->value = cur_ecc;
	}

	Floyd_MatrixFree(G, matrix);
}

// Define the radius and diameter of graph
void RadAndDiam(GRAPH* G, int* rad, int* diam)
{
	Eccentricity(G);	

	for (int i = 0; i < G->SIZE_N; i++)
	{
		if (G->nodes[i]->value < *rad)
			*rad = G->nodes[i]->value;
		if (G->nodes[i]->value > *diam)
			*diam = G->nodes[i]->value;
	}

}

// Define the center of graph
void Center(GRAPH* G)
{
	int rad = 0, diam = 0;
	RadAndDiam(G, &rad, &diam);

	for (int i = 0; i < G->SIZE_N; i++)
		if (G->nodes[i]->value == rad)
			continue;
}

// Define the density (thickness of graph)
double Density(GRAPH* G)
{
	return ((double)G->SIZE_E / (double)(2 * (double)G->SIZE_N * ((double)G->SIZE_N - 1)));
}