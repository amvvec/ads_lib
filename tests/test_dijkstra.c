#include "../src/dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

void runDijсkstraTests()
{
    // Test 1: Simple graph with unweighted edges (weight=1)
    printf(
        "Test 1: Shortest paths from 0 in graph with edges 0->1, 0->2, 1->3\n");
    Graph g;
    init_graph(&g, 4);
    add_edge(&g, 0, 1); // Weight = 1
    add_edge(&g, 0, 2); // Weight = 1
    add_edge(&g, 1, 3); // Weight = 1
    printf("Expected: 0:0, 1:1, 2:1, 3:2\nResult: ");
    dijkstra(&g, 0); // Manual check for distances

    // Test 2: Unreachable vertex
    printf("Test 2: Unreachable vertex (add isolated 4)\n");
    Graph g_unreach;
    init_graph(&g_unreach, 5);
    add_edge(&g_unreach, 0, 1); // Weight = 1
    printf("Expected: 0:0, 1:1, 2:INF, 3:INF, 4:INF\nResult: ");
    dijkstra(&g_unreach, 0); // Manual check

    // Test 5: Empty graph (no edges)
    printf("Test 5: Empty graph with 3 vertices\n");
    Graph g_empty;
    init_graph(&g_empty, 3);
    printf("Expected: 0:0, 1:INF, 2:INF\nResult: ");
    dijkstra(&g_empty, 0);
    printf("Check output manually for correct distances\n");
}
