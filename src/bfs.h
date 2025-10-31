#ifndef BFS_H
#define BFS_H

#include "graph.h"

typedef struct Queue
{
    int * item;
    int front, rear, capacity;
} Queue;

void bfs(Graph * g, int start);

#endif // !BFS_H
