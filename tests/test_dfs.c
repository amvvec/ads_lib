#include "../src/dfs.h"
#include <stdio.h>
#include <stdlib.h>

void runDFSTests()
{
    // Test 1: Simple graph traversal
    printf("Test 1: DFS on graph with edges 0->1, 0->2, 1->3\n");
    Graph g;
    init_graph(&g, 4);
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    printf("Expected: 0 1 3 2 (or valid DFS order)\nResult: ");
    dfs(&g, 0); // Manual check for now
    printf("Check output manually for valid DFS order\n");

    // Test 4: Empty graph (no edges)
    printf("Test 4: Empty graph with 3 vertices\n");
    Graph g_empty;
    init_graph(&g_empty, 3);
    printf("Expected: 0\nResult: ");
    dfs(&g_empty, 0);
    printf("Check output manually for 0\n");

    // Test 5: Disconnected vertices
    printf("Test 5: Disconnected vertex (2->3, start at 0)\n");
    Graph g_disconnect;
    init_graph(&g_disconnect, 4);
    add_edge(&g_disconnect, 2, 3);
    printf("Expected: 0\nResult: ");
    dfs(&g_disconnect, 0);
    printf("Check output manually for 0\n");
}
