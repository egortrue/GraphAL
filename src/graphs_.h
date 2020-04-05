#ifndef GRAPHS_H
#define  GRAPHS_H

	#define  ORIENTED_GRAPH 0

	#include <stdlib.h>
	#include <stdio.h>

//------------------------------------------------------------------------------------------------------

	typedef struct node
	{
				int    name;	  // Уникальный ID  
				int    value;     // Пока что нигде не используется
				
				// Размер != кол-во 
				int	   SIZE_S;    // Размер д.м sources
				int	   SIZE_T;	  // Размер д.м targets

		struct node**  sources;	  // Массив родителей
		struct node**  targets;	  // Массив детей
	}NODE;

	NODE  NodeSet     (int name, int val, int SIZE_);
	NODE* NodeAddress (NODE** arr, int name, int arr_size);

	void  NodePrint   (const NODE* V);
	void  NodeLink    (NODE* source, NODE* target);
	void  NodeIsolate (NODE* V);  // Очищает массивы указателей у узла

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
		 int   SIZE_N;	// Кол-во узлов
		 int   SIZE_E;  // Кол-во рёбер
		NODE** nodes;
		EDGE** edges;
	}GRAPH;

	 GRAPH  GraphSet        (int num_nodes, int num_edges);
	  void  GraphPrint      (const GRAPH* G);

	  void  GraphDestroy    (GRAPH* G);
	   int  GraphEdgeWeight (GRAPH* G, NODE* V1, NODE* V2);  // Возвращает вес ребра между двумя узлами


//------------------------------------------------------------------------------------------------------

#endif