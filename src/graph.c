#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes a graph with a given number of vertices.
 * @param g Pointer to the graph structure.
 * @param vertices Number of vertices (must be > 0 and <= MAX_VERTICES).
 * @note Exits program on invalid vertex count.
 * @complexity O(V²) where V is the number of vertices.
 */
void init_graph(Graph* g, int vertices)
{
    if(vertices <= 0 || vertices > MAX_VERTICES)
    {
        fprintf(stderr, "Invalid vertex count: %d\n", vertices);
        exit(1);
    }
    g->vertices = vertices;
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            g->adj_matrix[i][j] = 0;
        }
    }
}

/**
 * @brief Adds a directed edge to the graph.
 * @param g Pointer to the graph structure.
 * @param src Source vertex index.
 * @param dest Destination vertex index.
 * @return 0 on success, -1 on error (invalid graph or indices).
 * @complexity O(1).
 */
int add_edge(Graph* g, int src, int dst)
{
    if(!g)
    {
        fprintf(stderr, "Null graph pointer\n");
        return -1;
    }
    if(src < 0 || src >= g->vertices || dst < 0 || dst >= g->vertices)
    {
        fprintf(stderr, "Invalid edge: %d --> %d\n", src, dst);
        return -1;
    }
    g->adj_matrix[src][dst] = 1;
    return 0;
}

/**
 * @brief Prints the graph's adjacency list representation.
 * @param g Pointer to the graph structure.
 * @note Prints to stdout; does nothing if graph is NULL.
 * @complexity O(V²) where V is the number of vertices.
 */
void print_graph(Graph* g)
{
    if(!g)
    {
        fprintf(stderr, "Null graph pointer\n");
        return;
    }
    for(int i = 0; i < g->vertices; i++)
    {
        printf("Vertex %d: ", i);
        for(int j = 0; j < g->vertices; j++)
        {
            if(g->adj_matrix[i][j])
            {
                printf("%d", j);
            }
        }
        printf("\n");
    }
}
