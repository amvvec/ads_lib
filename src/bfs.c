#include <stdio.h>
#include <stdlib.h>

#include "bfs.h"

Queue* create_queue(int capacity)
{
    Queue* q = malloc(sizeof(Queue));

    q->item = malloc(capacity * sizeof(int));
    q->front = 0;
    q->rear = -1;
    q->capacity = capacity;

    return q;
}



void enqueue(Queue * q, int value)
{
    q->item[++q->rear] = value;
}


int dequeue(Queue * q)
{
    return q->item[q->front++];
}



void free_queue(Queue * q)
{
    free(q->item);
    free(q);
}





void bfs(Graph * g, int start)
{
    int * visited = calloc(g->vertices, sizeof(int));
    Queue * q = create_queue(g->vertices);
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



