#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"

#define INF 999999

static int min_distance(int* dst, int* visited, int vertices)
{
    int min = INF;
    int min_index = -1;
    for(int v = 0; v < vertices; v++)
    {
        if(!visited[v] && dst[v] <= min)
        {
            min = dst[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph* g, int start)
{
    int* dst = malloc(g->vertices * sizeof(int));
    int* visited = calloc(g->vertices, sizeof(int));
    for(int i = 0; i < g->vertices; i++)
    {
        dst[i] = INF;
    }
    dst[start] = 0;
    for(int count = 0; count < g->vertices; count++)
    {
        int u = min_distance(dst, visited, g->vertices);
        visited[u] = 1;
        for(int v = 0; v < g->vertices; v++)
        {
            if(!visited[v] && g->adj_matrix[u][v] != INF && dst[u] != INF &&
               dst[u] + g->adj_matrix[u][v] < dst[v])
            {
                dst[v] = dst[u] + g->adj_matrix[u][v];
            }
        }
    }
    printf("Shortest distances from vertex %d:\n", start);
    for(int i = 0; i < g->vertices; i++)
    {
        if(dst[i] == INF)
        {
            printf("Vertex %d: Unreachable\n", i);
        }
        else
        {
            printf("Vertex %d: %d\n", i, dst[i]);
        }
    }
    free(dst);
    free(visited);
}
