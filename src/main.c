#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "stdio_sh.h"
#include "graphs.h"
#include "algorithms.h"

int main()
{
    // Preparation
    FILE* fr = FileOpen("data/input.txt", "r");
    FILE* fw = FileOpen("data/output.txt", "w");

    /* Memory leaks test
    while (1) 
    {
        GRAPH* F = FileRead(fr);
        NODE* start = F->nodes[0];

        GeneralInfo(F);
        BFS(F, start);
        DFS(F, start);
        Deijkstra(F, start);
        BellmanFord(F, start);
        FloydWarshall(F, 1);
        Prim(F, start);
        Kruskal(F);
        GraphDestroy(F);
    } 
    */

    ///* UX version
    GRAPH* F = FileRead(fr);
    NODE* start = F->nodes[0];
    enum {exit, info, bfs, dfs, deijkstra, bellman, floyd, prim, kruskal} c;
    while (1)
    {
        system("cls");
        GraphPrint(F);

        puts("\nWhat you want to see:");
        printf("1) General Info\n2) BFS\n3) DFS\n4) Deijkstra\n5) Bellman-Ford\n6) Floyd-Warshall\n");
        printf("7) Prim's MST algorithm\n8) Kruskal's MST algorithm\n\n0)Exit");

        c = (int)_getch() - 48;
        system("cls");
        GraphPrint(F);
        puts("");

        switch (c)
        {
        case info:
            puts("General Info:");
            GeneralInfo(F);
            break;
        case bfs:
            puts("BFS:");
            BFS(F, start);
            break;
        case dfs:
            puts("DFS:");
            DFS(F, start);
            break;
        case deijkstra:
            puts("Deijkstra:");
            Deijkstra(F, start);
            break;
        case bellman:
            puts("Bellman-Ford:");
            BellmanFord(F, start);
            break;
        case floyd:
            puts("Floyd-Warshall:");
            FloydWarshall(F, 1);
            break;
        case prim:
            puts("Prim's minimum-spanning-tree:");
            Prim(F, start);
            break;
        case kruskal:
            puts("Kruskal's minimum-spanning-tree:");
            Kruskal(F);
            break;
        case exit:
            system("cls");
            break;
        default:
            continue;
        }

        if (c == exit)
            break;

        printf("\nContinue?... ");
        c = _getch();
    }

    GraphDestroy(F);
    //*/
    
    _fcloseall();
	return 0;
}
