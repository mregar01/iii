#ifndef QUEUE_INCLUDE
#define QUEUE_INCLUDED

typedef struct Queue_T *Queue_T;

/**********Queue_new********
 *
 * Creates and returns a queue
 * Inputs: A hint for the number of elements expected to be stored in the queue
 * Return: A Queue
 * 
 * Expects:
 *      Queue to be only store indices of a Bit2 array
 *
 * Notes:
 *      Essentially a modified Array with 2 variables to keep track of the
 *      front and rear of this FIFO structure. This implementation won't remove
 *      elements once popped as that would add to time complexity.
 *      This implementation only works with indices for this assignment
 ************************/
Queue_T Queue_new(int hint);

/**********Queue_inside********
 *
 * Returns the number of elements in the queue
 * Inputs: The queue
 * Return: Integer indicating number of elements in queue
 * 
 * Expects:
 *      Called on a valid queue
 *
 * Notes:
 *
 ************************/
int Queue_inside(Queue_T queue);

/**********Queue_enqueue********
 *
 * Adds an element to back of the queue
 * Inputs: The queue and the col and row of the enqueued element
 * Return: 
 * 
 * Expects:
 *      Called on a valid queue
 *
 * Notes:
 *
 ************************/
void Queue_enqueue(Queue_T queue, int col, int row);

/**********Queue_dequeue********
 *
 * Pops an element from front of queue
 * Inputs: The queue
 * Return: The popped element's indices in an int array
 * 
 * Expects:
 *      Called on a valid queue
 *
 * Notes:
 *
 ************************/
int* Queue_dequeue(Queue_T queue);

/**********Queue_free********
 *
 * Frees up all space allocated by a queue
 * Inputs: pointer to an instance of a queue
 * Return:
 * Expects: queue to be nonnull and not freed already
 *
 * Notes:
 *
 ************************/
void Queue_free(Queue_T *queue);

#undef Queue_T
#endif