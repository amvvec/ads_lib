#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100

typedef struct
{
    int adj_matrix[MAX_VERTICES][MAX_VERTICES];
    int vertices;
} Graph;

void init_graph(Graph *g, int vertices);
int add_edge(Graph *g, int src, int dst);
void print_graph(Graph *g);

#endif // !GRAPH_H
