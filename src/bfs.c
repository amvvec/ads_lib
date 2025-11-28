#include <stdio.h>
#include <stdlib.h>

#include "bfs.h"

Queue *create_queue(int capacity)
{
    Queue *q = malloc(sizeof(Queue));
    if(!q)
    {
        fprintf(stderr, "Queue allocation failed\n");
        exit(1);
    }
    // TODO: add safety check for (int) capacity
    // q->item = malloc(capacity * sizeof(int)); // WARNING: int -> long long
    q->item = malloc(sizeof(int) * (size_t)capacity);
    if(!q->item)
    {
        fprintf(stderr, "Queue allocation failed\n");
        exit(1);
    }
    q->front = 0;
    q->rear = -1;
    q->capacity = capacity;
    return q;
}

void enqueue(Queue *q, int value)
{
    if(q->rear >= q->capacity - 1)
    {
        return;
    }
    q->item[++q->rear] = value;
}

int dequeue(Queue *q)
{
    if(q->front > q->rear)
    {
        return -1;
    }
    return q->item[q->front++];
}

void free_queue(Queue *q)
{
    free(q->item);
    free(q);
}

/**
 * @brief Performs BFS on a graph starting from a given vertex.
 * @param g Pointer to the graph structure.
 * @param start Starting vertex index.
 * @note Prints visited vertices to stdout.
 * @complexity O(V + E) where V is vertices, E is edges.
 */
void bfs(Graph *g, int start)
{
    if(!g)
    {
        fprintf(stderr, "Null graph pointer\n");
        return;
    }
    if(start < 0 || start >= g->vertices)
    {
        fprintf(stderr, "Invalid start vertex: %d\n", start);
        return;
    }
    // WARNING: int -> size-t
    // int * visited = calloc(g->vertices, sizeof(int));
    int *visited = calloc((size_t)g->vertices, sizeof(int));
    if(!visited)
    {
        fprintf(stderr, "Visited array allocation failed\n");
        return;
    }
    Queue *q = create_queue(g->vertices);
    visited[start] = 1;
    enqueue(q, start);
    while(q->front <= q->rear)
    {
        int current = dequeue(q);
        printf("%d", current);

        for(int i = 0; i < g->vertices; i++)
        {
            if(g->adj_matrix[current][i] && !visited[i])
            {
                visited[i] = 1;
                enqueue(q, i);
            }
        }
    }
    printf("n");
    free_queue(q);
    free(visited);
}
