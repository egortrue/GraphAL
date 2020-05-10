#include <stdlib.h>
#include <stdio.h>

#include "graphs.h"

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
	printf("EDGE: (%c<->%c) ", E->source->name + 64, E->target->name + 64);
	printf("Weight: %d\n", E->weight);
}

//------------------------------------------------------------------------------------------------------

GRAPH* GraphSet(int nodes_num, int edges_num, int info)
{
	GRAPH* G = (GRAPH*)malloc(sizeof(GRAPH));
	if (!G) exit(EXIT_FAILURE);
	G->nodes = (NODE**)calloc(nodes_num, sizeof(NODE*));
	if (!G->nodes) exit(EXIT_FAILURE);
	G->edges = (EDGE**)calloc(edges_num, sizeof(EDGE*));
	if (!G->nodes) exit(EXIT_FAILURE);
	
	G->SIZE_N = nodes_num;
	G->SIZE_E = edges_num;
	G->directed = info & 1;

	return G;
}

void GraphPrint(const GRAPH* G)
{
	int i = 0;
	printf("GRAPH (%s)\n", G->directed ? "directed" : "undirected");
	printf("  Nodes: ");
	for (; i < G->SIZE_N; i++)
		if (G->nodes[i] != 0)
			printf("(%2c)=%2d%s", G->nodes[i]->name + 64, G->nodes[i]->value, ((i+1) % 7 != 0) ? " " : "\n\t ");
	printf("%s", i == 0 ? "None\n" : "\n");
	i = 0;
	printf("  Edges: ");
	for (; i < G->SIZE_E; i++)
		if (G->edges[i] != 0)
			printf("(%2c<->%2c)=%-2d%s", G->edges[i]->source->name + 64, G->edges[i]->target->name + 64,
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
		
	NODE** neighbours = (NODE**)calloc(count, sizeof(NODE*));
	if (!neighbours) exit(EXIT_FAILURE);
	for (int i = 0; i < count; i++)
		neighbours[i] = buffer[i];
	free(buffer);
	return neighbours;
}
//------------------------------------------------------------------------------------------------------
int AMatrixDegree(AMATRIX *adj_matrix, int v)
{
    int countdegree = 0;
    for (int j = 0; j < adj_matrix->n; j++){
        if (adj_matrix->adj[v][j] != 0)
            countdegree++;
    }
    return countdegree;
}

double AMatrixDensity(AMATRIX *adj_matrix)
{
    double dens = ((double)adj_matrix->e / (double)(2 * adj_matrix->n * (adj_matrix->n - 1)));
    return dens;
}

void AMatrixPrint(AMATRIX *m, FILE *output)
{
    for (int i = 0; i < m->n; i++){
        for (int j = 0; j < m->n; j++){
            fprintf(output, "%d ", m->adj[i][j]);
        }
        fprintf(output, "\n");
    }
}

int AMatrixCountNodes(FILE *input)
{
    int count_lines = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c == '\n')
            count_lines++;
        c = fgetc(input);
    }
    count_lines++;
    rewind(input); //return back pointer in file
    return count_lines;
}

AMATRIX *AMatrixSet(int nodes)
{
    AMATRIX * tmp = (AMATRIX*)malloc(nodes * sizeof(AMATRIX));
    tmp->adj = (int**)malloc(nodes*sizeof(int*));
    tmp->n = nodes;
    tmp->e = 0;
    for (int i = 0; i < tmp->n; i++){
        tmp->adj[i] = (int*)calloc(nodes, sizeof(int));
    }

    return tmp;
}

AMATRIX *AMatrixDelete(AMATRIX *m){
    for (int i = 0; i < m->e; i++)
        for (int j = 0; j < m->e; j++)
            m->adj[i][j] = 0;
    return m;
}
//------------------------------------------------------------------------------------------------------
void AListAddValue(ALIST **pphead, int val)
{
    ALIST **pp = pphead, *pnew;

    while(*pp)
    {
        if(val < (*pp)->value)
            break;
        else
            pp = &((*pp)->pnext);
    }

    pnew = (ALIST*)malloc(sizeof(ALIST));
    pnew->value = val;
    pnew->pnext = *pp;
    *pp = pnew;
}

void AListPrints(aListg *graph, FILE *output) {
    for (int i = 0; i < graph->v; i++) {
        if (graph[i].g) {
            fprintf(output, "%d-", i);
            ALIST *p = graph[i].g;
            while (p) {
                fprintf(output, "%d ", p->value);
                p = p->pnext;
            }
            fprintf(output, "\n");
        }
    }
}

void AListDelete(ALIST *phead)
{
    if(phead)
    {
        AListDelete(phead->pnext);
        if(phead)
            free(phead);
    }
}
double AListDensity(int v, int e)
{
    return ((double)e/(double)(v*(v-1)));
}

int AListDegree(ALIST *phead)
{
    ALIST* p = phead;
    int count = 0;
    while(p)
    {
        count++;
        p = p->pnext;
    }
    return count;
}

int AListCountVertex(FILE *input)
{
    int lines_count = 0;
    char c = fgetc(input);
    while (c != EOF)
    {
        if (c == '\n')
            lines_count++;
        c = fgetc(input);
    }
    lines_count++;
    rewind(input);//return back pointer in file
    return lines_count+1;
}
//------------------------------------------------------------------------------------------------------
