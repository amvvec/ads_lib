#include <stdio.h>
#include <stdlib.h>

#include "../src/graph.h"

void runGraphTests()
{
    // Test 1: Initialization
    printf("Test 1: Initialize Graph with 5 vertices\n");
    Graph g;
    init_graph(&g, 5);
    if(g.vertices == 5 && g.adj_matrix[0][0] == 0)
    {
        printf("Result: Initialized successfully with 5 vertices\n");
    }
    else
    {
        printf("Result: Initialization failed\n");
    }

    // Test 2: Add valid edges
    printf("Test 2: Add edges (0->1, 1->2, 2->0)\n");
    add_edge(&g, 0, 1);
    add_edge(&g, 1, 2);
    add_edge(&g, 2, 0);
    if(g.adj_matrix[0][1] == 1 && g.adj_matrix[1][2] == 1 &&
       g.adj_matrix[2][0] == 1)
    {
        printf("Result: Edges added correctly\n");
    }
    else
    {
        printf("Result: Edge addition failed\n");
    }

    // Test 3: Add invalid edge
    printf("Test 3: Add invalid edge (5->5)\n");
    add_edge(&g, 5, 5); // Should print error and not modify matrix
    int invalid_edge = 0;
    for(int i = 0; i < g.vertices; i++)
        if(g.adj_matrix[5][i] != 0 || g.adj_matrix[i][5] != 0)
            invalid_edge = 1;
    if(!invalid_edge)
    {
        printf("Result: Invalid edge handled correctly\n");
    }
    else
    {
        printf("Result: Invalid edge handling failed\n");
    }

    // Test 4: Max vertices boundary
    printf("Test 4: Initialize graph with MAX_VERTICES\n");
    Graph g_max;
    init_graph(&g_max, MAX_VERTICES);
    if(g_max.vertices == MAX_VERTICES)
    {
        printf("Result: Initialized with MAX_VERTICES successfully\n");
    }
    else
    {
        printf("Result: Max vertices initialization failed\n");
    }
}
