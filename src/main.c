#include "stdio_sh.h"
#include "graphs_.h"
#include "algorithms_.h"


int main()
{
    // Подготовка

    FILE* fr = FileOpen("input.txt", "r");
    FILE* fw = FileOpen("output.txt", "w");

    /*
    // Debug version
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
        GraphDestroy(F);
    } 
    */


    ///* UX version
    // Считывание данных, создание графа
    GRAPH* F = FileRead(fr);
    NODE* start = F->nodes[0];

    // Немного интерактива:
    while (1)
    {
        system("cls");
        GraphPrint(F);
        puts("\nWhat you want to see:");
        puts("1) General Info\n2) BFS\n3) DFS\n4) Deijkstra\n5) Bellman-Ford\n6) Floyd-Warshall\n\n0) Exit");
        int c = (int)_getch() - 48;
        system("cls");
        GraphPrint(F);
        puts("");
        switch (c)
        {
        case 1:
            puts("General Info:");
            GeneralInfo(F);
            break;
        case 2:
            puts("BFS:");
            BFS(F, start);
            break;
        case 3:
            puts("DFS:");
            DFS(F, start);
            break;
        case 4:
            puts("Deijkstra:");
            Deijkstra(F, start);
            break;
        case 5:
            puts("Bellman-Ford:");
            BellmanFord(F, start);
            break;
        case 6:
            puts("Floyd-Warshall:");
            FloydWarshall(F, 1);
            break;
        case 0:
            system("cls");
            break;
        default:
            continue;
        }
        if (c == 0)
            break;
        printf("\nContinue?... ");
        c = _getch();
    }

    //Завершение работы
    GraphDestroy(F);
    //*/
    _fcloseall();
	return 0;
}