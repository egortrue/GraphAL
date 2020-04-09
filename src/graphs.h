#ifndef GRAPHS_H
#define GRAPHS_H

#define  ORIENTED_GRAPH 0

//------------------------------------------------------------------------------------------------------

typedef struct node
{
			int    name;	  // ID  
			int    value;     // Kruskal use it
				
			int	   SIZE_S;    // size of sources
			int	   SIZE_T;	  // size of targets

	struct node**  sources;	  // parents
	struct node**  targets;	  // children
}NODE;

NODE  NodeSet     (int name, int val, int SIZE_);
NODE* NodeAddress (NODE** arr, int name, int arr_size);

void  NodePrint   (const NODE* V);
void  NodeLink    (NODE* source, NODE* target);
void  NodeIsolate (NODE* V);  // clear node's sources and targets

//------------------------------------------------------------------------------------------------------

typedef struct edge
{
		 int   weight;
		NODE*  source;
		NODE*  target;
}EDGE;

EDGE EdgeSet   (NODE* source, NODE* target, int val);
void EdgePrint (const EDGE* E);

//------------------------------------------------------------------------------------------------------

typedef struct graph 
{
	 int   SIZE_N;  // number of nodes
	 int   SIZE_E;  // number of edges
	NODE** nodes;
	EDGE** edges;
}GRAPH;

GRAPH  GraphSet        (int num_nodes, int num_edges);
 void  GraphPrint      (const GRAPH* G);
 void  GraphDestroy    (GRAPH* G);
  int  GraphEdgeWeight (GRAPH* G, NODE* V1, NODE* V2);  // return weight between two nodes if it exist

//------------------------------------------------------------------------------------------------------

#endif