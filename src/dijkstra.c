#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"

#define INF 999999

/**
 * @brief Finds the vertex with minimum distance that hasn’t been visited.
 * @param dist Array of distances.
 * @param visited Array of visited vertices.
 * @param vertices Number of vertices.
 * @return Index of minimum distance vertex.
 */
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

/**
 * @brief Computes shortest paths from a source vertex using Dijkstra’s
 * algorithm.
 * @param g Pointer to the graph structure.
 * @param start Starting vertex index.
 * @note Prints shortest distances to stdout.
 * @complexity O(V²) where V is the number of vertices.
 */
void dijkstra(Graph* g, int start)
{
    if(!g)
    {
        fprintf(stderr, "Null graph pointer\n");
        return;
    }
    if(start < 0 || start > g->vertices)
    {
        fprintf(stderr, "Invalid start vertex: %d\n", start);
        return;
    }
    int* dst = malloc(g->vertices * sizeof(int));
    int* visited = calloc(g->vertices, sizeof(int));
    if(!dst || !visited)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(dst);
        free(visited);
        return;
    }
    // Initialize distances
    for(int i = 0; i < g->vertices; i++)
    {
        dst[i] = INF;
    }
    dst[start] = 0;
    // Process all vertices
    for(int count = 0; count < g->vertices; count++)
    {
        int u = min_distance(dst, visited, g->vertices);
        if(u == -1)
        {
            break; // No reachable vertices left
        }
        visited[u] = 1;

        // Update distances for adjacent vertices
        for(int v = 0; v < g->vertices; v++)
        {
            if(!visited[v] && g->adj_matrix[u][v] != INF && dst[u] != INF &&
               dst[u] + g->adj_matrix[u][v] < dst[v])
            {
                dst[v] = dst[u] + g->adj_matrix[u][v];
            }
        }
    }
    // Print results
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
