#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

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
