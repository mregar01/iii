#include <assert.h>
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

struct Queue_T {
        int size;
        int front;
        int rear;
        int* elems;
};

Queue_T Queue_new(int hint) {
        int* arr;
        arr = malloc((2*hint)+1); //Do I malloc this and the queue?
        assert(arr != NULL);
        Queue_T queue = malloc(sizeof(struct Queue_T));
        queue->elems = arr;

        queue->front = 0;
        queue->rear = 0;
        queue->size = hint;
        
        return queue;
}

int Queue_inside(Queue_T queue) {
        assert(queue);
        return queue->rear - queue->front;
}

void Queue_enqueue(Queue_T queue, int col, int row) {
        assert(queue);
        if((queue->rear + 1) == queue->size) {
                queue->size *= 2;
                queue->elems = (int *)realloc(queue->elems, queue->size);
        }
        int val[2] = {col, row};
        queue->elems[queue->rear] = *val;
        queue->rear += 1;
}

int* Queue_dequeue(Queue_T queue) {
        assert(queue);
        int *popped = queue->elems[queue->front];
        queue->front+=1;
        return popped;
}

void Queue_free(Queue_T *queue) {
        assert(queue && *queue);
        free((*queue)->elems); //how to free the array in the struct
        free(*queue);
}