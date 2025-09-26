#include "../src/dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

void runDijkstraTests()
{
    // Test 1: Graph with edges 0->1, 0->2, 1->3
    printf(
        "Test 1: Shortest paths from 0 in graph with edges 0->1, 0->2, 1->3\n");
    Graph g;
    init_graph(&g, 4);
    add_edge(&g, 0, 1); // Weight = 1
    add_edge(&g, 0, 2); // Weight = 1
    add_edge(&g, 1, 3); // Weight = 1
    printf("Expected: 0:0, 1:1, 2:1, 3:2\nResult: \n");
    dijkstra(&g, 0);

    // Test 2: Graph with only 0->1, others unreachable
    printf("Test 2: Graph with only 0->1, others unreachable\n");
    Graph g_unreach;
    init_graph(&g_unreach, 5);
    add_edge(&g_unreach, 0, 1); // Weight = 1
    printf("Expected: 0:0, 1:1, 2:Unreachable, 3:Unreachable, "
           "4:Unreachable\nResult: \n");
    dijkstra(&g_unreach, 0);

    // Test 3: Empty graph
    printf("Test 3: Empty graph with 3 vertices\n");
    Graph g_empty;
    init_graph(&g_empty, 3);
    printf("Expected: 0:0, 1:Unreachable, 2:Unreachable\nResult: \n");
    dijkstra(&g_empty, 0);
}