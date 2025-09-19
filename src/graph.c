#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

void init_graph(Graph* g, int vertices)
{
    g->vertices = vertices;
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(Graph* g, int src, int dst)
{
    g->adj_matrix[src][dst] = 1;
}

void print_graph(Graph* g)
{
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
