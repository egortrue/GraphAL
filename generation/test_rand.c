#include "random_graphs.h"

void dfs2(AMATRIX *g, int v, int ost, int used[]){
    used[v] = 1;
    for (int i = 0; i < ost; i++ ){
        int to = i;
        if (used[to] == 0 && g->adj[v][to]) {
            dfs2(g, to, ost, used);
        }
    }
}
void FindComp(AMATRIX *g, int v, int n){
    int used[n];
    int counter = 0;
    for (int i = 0; i < n; i++){
        used[i] = 0;
    }
    for (int i = 0; i < n; i++){
        if (!used[i]){
            dfs2(g, i, n, used);
            counter++;
        }
    }
    if (counter > 1 )
        printf("here %d here", counter);

}
void TestConnection(AMATRIX *graph, int e, int v, int r1, int r2){
    for (int i = 0;  i < 1000; i++){
        graph = AMatrixSet(v);
        RandomGraph(e, v, graph, r1, r2);
        ConnectGraph(graph, e,  v);
        FindComp(graph, e, v);
        graph = AMatrixDelete(graph);
    }
}