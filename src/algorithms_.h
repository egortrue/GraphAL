#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graphs_.h"
#include <stdio.h>
#include <stdlib.h>

// Walk on the non-weighted graph
void  BFS (GRAPH* G, NODE* start);
void  DFS (GRAPH* G, NODE* start);

// Walk on the weighted graph
void    Deijkstra     (GRAPH* G, NODE* start);
void    BellmanFord   (GRAPH* G, NODE* start);
EDGE*** FloydWarshall (GRAPH* G, int output);


void  GeneralInfo (GRAPH* G);

#endif
