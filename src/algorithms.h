#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graphs.h"

// Walk on the non-weighted graph
void  BFS (GRAPH* G, NODE* start);
void  DFS (GRAPH* G, NODE* start);

// Walk on the weighted graph
void    Deijkstra     (GRAPH* G, NODE* start);
void    BellmanFord   (GRAPH* G, NODE* start);
EDGE*** FloydWarshall (GRAPH* G, int output);

// Minimum-spanning-tree (MST)
void MST_Prim(GRAPH* G, NODE* root);

// Other algorithms
void  GeneralInfo(GRAPH* G);

#endif
