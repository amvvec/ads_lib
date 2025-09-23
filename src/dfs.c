#include <stdio.h>
#include <stdlib.h>

#include "dfs.h"

static void dfs_recursive(Graph* g, int vertex, int* visited)
{
    visited[vertex] = 1;
    printf("%d ", vertex);
    for(int i = 0; i < g->vertices; i++)
    {
        if(g->adj_matrix[vertex][i] && !visited[i])
        {
            dfs_recursive(g, i, visited);
        }
    }
}

void dfs(Graph* g, int start)
{
    if(!g)
    {
        fprintf(stderr, "Null graph pointer\n");
        return;
    }
    if(start < 0 || start >= g->vertices)
    {
        fprintf(stderr, "Invalid start vertext: %d\n", start);
        return;
    }
    int* visited = calloc(g->vertices, sizeof(int));
    if(!visited)
    {
        fprintf(stderr, "Visited array allocation failed\n");
        return;
    }
    dfs_recursive(g, start, visited);
    printf("\n");
    free(visited);
}
