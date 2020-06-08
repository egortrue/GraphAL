#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graphs.h"

// Walk on a non-weighted graph
DLL_EXPORT NODE** BFS (GRAPH* G, NODE* start);
DLL_EXPORT NODE** DFS (GRAPH* G, NODE* start);

// Walk on a weighted graph
DLL_EXPORT NODE**  Dijkstra      (GRAPH* G, NODE* start);
EDGE**  BellmanFord   (GRAPH* G, NODE* start);
EDGE*** FloydWarshall (GRAPH* G);

// Minimum-spanning-tree (MST)
DLL_EXPORT EDGE** Prim    (GRAPH* G, NODE* root);
DLL_EXPORT EDGE** Kruskal (GRAPH* G);

// Flow network
void FordFalkerson(GRAPH* G, NODE* source, NODE* target);

// Other algorithms
void Eccentricity(GRAPH* G);                    // Define the eccentricities for each node
void RadAndDiam(GRAPH* G, int* rad, int* diam); // Define the radius and diameter of graph
void Center(GRAPH* G);  		            	// Define the center of graph
double Density(GRAPH* G);                       // Define the density (thickness of graph)

#endif
