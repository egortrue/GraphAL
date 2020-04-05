#ifndef GRAPHS_H
#define  GRAPHS_H

	#define  ORIENTED_GRAPH 0

	#include <stdlib.h>
	#include <stdio.h>

//------------------------------------------------------------------------------------------------------

	typedef struct node
	{
				int    name;	  // ���������� ID  
				int    value;     // ���� ��� ����� �� ������������
				
				// ������ != ���-�� 
				int	   SIZE_S;    // ������ �.� sources
				int	   SIZE_T;	  // ������ �.� targets

		struct node**  sources;	  // ������ ���������
		struct node**  targets;	  // ������ �����
	}NODE;

	NODE  NodeSet     (int name, int val, int SIZE_);
	NODE* NodeAddress (NODE** arr, int name, int arr_size);

	void  NodePrint   (const NODE* V);
	void  NodeLink    (NODE* source, NODE* target);
	void  NodeIsolate (NODE* V);  // ������� ������� ���������� � ����

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
		 int   SIZE_N;	// ���-�� �����
		 int   SIZE_E;  // ���-�� ����
		NODE** nodes;
		EDGE** edges;
	}GRAPH;

	 GRAPH  GraphSet        (int num_nodes, int num_edges);
	  void  GraphPrint      (const GRAPH* G);

	  void  GraphDestroy    (GRAPH* G);
	   int  GraphEdgeWeight (GRAPH* G, NODE* V1, NODE* V2);  // ���������� ��� ����� ����� ����� ������


//------------------------------------------------------------------------------------------------------

#endif