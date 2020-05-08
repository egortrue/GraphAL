#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>

#include "stdio_sh.h"
#include "graphs.h"
#include "algorithms.h"

int main()
{
    //Preparation
  //  FILE* fr = FileOpen("data/input.txt", "r");
  //  FILE* fw = FileOpen("data/output.txt", "w");

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
 /*   GRAPH* F = FileRead(fr);
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
    _fcloseall();
*/

    FILE *input;
    FILE *output;
    input = fopen("../data/AdjListInput.txt", "rt");
    output = fopen("../data/AdjListOutput.txt", "wt");

    if (!input)
    {
        puts("Error open file for reading");
        return -2;
    }
    if (!output)
    {
        puts("Error open file for writing");
        return -2;
    }
    int count_edges = 0;
    aListg *grap = AListReads(input, output, &count_edges);
    AListPrints(grap, output);

   // fprintf(output, "Density: %.2lf", AListDensity(AListCountVertex(input), count_edges + 1));
   // fprintf(output, "\nDegree of v #5/: %d", AListDegree(grap[5].g));
   // for (int i = 0; i < AListCountVertex(input); ++i)
        AListDelete(grap[1].g);


    input = fopen("../data/input.txt", "rt");
    output = fopen("../data/AdjMatrixOutput.txt", "wt");
    if (!input)
    {
        puts("Error open file for reading");
        return -2;
    }
    if (!output)
    {
        puts("Error open file for writing");
        return -2;
    }

    AMATRIX *d = AMatrixRead(input);
    AMatrixPrint(d, output);
    fprintf(output, "\nDegree of v #%d is %d", 1, AMatrixDegree(d, 1));
    fprintf(output, "\nDensity is %.2lf", AMatrixDensity(d));

	return 0;
}
