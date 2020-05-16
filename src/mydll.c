
#include <windows.h>
#include <stdio.h>
#define DLL_EXPORT __declspec(dllexport)

//====================================================================================


typedef struct node
{
    int name;  // ID  
    int value; // only Kruskal use it
}NODE;

DLL_EXPORT NODE* NodeSet(int name, int value)
{
    NODE* v = (NODE*)malloc(sizeof(NODE));
    if (!v) exit(EXIT_FAILURE);
   
    v->name = name;
    v->value = value;

    return v;
}

//====================================================================================

typedef struct edge
{
    int weight;
    NODE* source;
    NODE* target;
}EDGE;

DLL_EXPORT EDGE* EdgeSet(int weight, NODE* source, NODE* target)
{
    EDGE* e = (EDGE*)malloc(sizeof(EDGE));
    if (!e) exit(EXIT_FAILURE);

    e->weight = weight;
    e->source = source;
    e->target = target;

    return e;
}

//====================================================================================

typedef struct graph
{
    int SIZE_N;  // number of nodes
    int SIZE_E;  // number of edges

    NODE** nodes;
    EDGE** edges;
}GRAPH;

DLL_EXPORT GRAPH* GraphSet(int nodes, int edges)
{
    GRAPH* G = (GRAPH*)malloc(sizeof(GRAPH));
    if (!G) exit(EXIT_FAILURE);
    G->nodes = (NODE**)calloc(nodes, sizeof(NODE*));
    if (!G->nodes) exit(EXIT_FAILURE);
    G->edges = (EDGE**)calloc(edges, sizeof(EDGE*));
    if (!G->nodes) exit(EXIT_FAILURE);

    G->SIZE_N = nodes;
    G->SIZE_E = edges;

    return G;
}

DLL_EXPORT void GraphSetNodes(GRAPH* G, NODE** nodes, int nodes_num)
{
    for (int i = 0; i < nodes_num; i++)
        G->nodes[i] = nodes[i];
}

DLL_EXPORT void GraphSetEdges(GRAPH* G, EDGE** edges, int edges_num)
{
    for (int i = 0; i < edges_num; i++)
        G->edges[i] = edges[i];
}



//====================================================================================


int GraphGetEdgeWeight(GRAPH* G, NODE* V1, NODE* V2)
{
	for (int i = 0; i < G->SIZE_E; i++)
	{
		if (G->edges[i]->source == V1 && G->edges[i]->target == V2)
			return G->edges[i]->weight;
		if (G->edges[i]->source == V2 && G->edges[i]->target == V1)
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
		if (G->edges[i]->source == V2 && G->edges[i]->target == V1)
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
		else if ((G->edges[i]->target == node))
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

DLL_EXPORT NODE** BFS(GRAPH* G, NODE* start)
{
	// Checked nodes array
	int check_num = 0;
	NODE** checked = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	if (!checked) exit(EXIT_FAILURE);

	// --------------------------------------------------------

	NODE** path = (NODE**)calloc(G->SIZE_N, sizeof(NODE*));
	if (!path) exit(EXIT_FAILURE);
	int path_len = 0;

	// --------------------------------------------------------

	EDGE** COST = (EDGE**)calloc(G->SIZE_N, sizeof(EDGE*));
	if (!COST) exit(EXIT_FAILURE);

	// begin -> any = inf | begin -> begin = 0
	for (int i = 0; i < G->SIZE_N; i++)
	{
		NODE* n = G->nodes[i];
		if (n == start)
			COST[i] = EdgeSet(0, start, n);
		else
			COST[i] = EdgeSet(INT_MAX, start, n);
	}

	// --------------------------------------------------------
	// The algorithm:
	QU_NODE* first = QueuePush(NULL, start);

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

			if ((wave + 1) < CostGet(COST, neighbor, G->SIZE_N))
				CostSet(COST, neighbor, G->SIZE_N, wave + 1);
		}

		// If node not checked
		if (!Checker(checked, cur, check_num))
		{
			path[path_len++] = cur;

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
	free(checked);
	QueueDestroy(first);
	for (int i = 0; i < G->SIZE_N; i++)
		free(COST[i]);
	free(COST);

	for (int i = 0; i < path_len; i++)
		printf("%d ", path[i]->name);
	printf("\n");

	return path;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: // Подключение DLL
        printf("DLL was attached:\n");
        printf("|- %s mode\n", lpReserved ? "implicit (compile)" : "explicit (runtime)");
        printf("|- HMODULE = %p\n", hModule);
        break;
    case DLL_PROCESS_DETACH: // Отключение DLL
        printf("DLL was detached\n");
        break;


    case DLL_THREAD_ATTACH: // Создание нового потока
        break;
    case DLL_THREAD_DETACH: // Завершение потока
        break;
    }
    return TRUE;
}
