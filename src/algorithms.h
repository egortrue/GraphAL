#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graphs.h"

// Walk on the non-weighted graph
DLL_EXPORT NODE** BFS (GRAPH* G, NODE* start);
DLL_EXPORT NODE** DFS (GRAPH* G, NODE* start);

// Walk on the weighted graph
DLL_EXPORT NODE** Deijkstra (GRAPH* G, NODE* start);
void    BellmanFord   (GRAPH* G, NODE* start);
EDGE*** FloydWarshall (GRAPH* G, int output);

// Minimum-spanning-tree (MST)
void Prim    (GRAPH* G, NODE* root);
void Kruskal (GRAPH* G);

// Flow network
void FordFalkerson(GRAPH* G, NODE* source, NODE* target);

// Other algorithms
void Eccentricity(GRAPH* G);                    // Define the eccentricities for each node
void RadAndDiam(GRAPH* G, int* rad, int* diam); // Define the radius and diameter of graph
void Center(GRAPH* G);  		            	// Define the center of graph
double Density(GRAPH* G);                       // Define the density (thickness of graph)

#endif
