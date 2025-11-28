#include <stdio.h>
#include <stdlib.h>

#include "dfs.h"

/**
 * @brief Helper function for recursive DFS traversal.
 * @param g Pointer to the graph structure.
 * @param vertex Current vertex to explore.
 * @param visited Array to track visited vertices.
 * @complexity O(V + E) where V is vertices, E is edges.
 */
static void dfs_recursive(Graph *g, int vertex, int *visited)
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

/**
 * @brief Performs DFS on a graph starting from a given vertex.
 * @param g Pointer to the graph structure.
 * @param start Starting vertex index.
 * @note Prints visited vertices to stdout.
 * @complexity O(V + E) where V is vertices, E is edges.
 */
void dfs(Graph *g, int start)
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
    int *visited = calloc((size_t)g->vertices, sizeof(int));
    if(!visited)
    {
        fprintf(stderr, "Visited array allocation failed\n");
        return;
    }
    dfs_recursive(g, start, visited);
    printf("\n");
    free(visited);
}
