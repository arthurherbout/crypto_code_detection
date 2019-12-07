/* FIFO Queue implemented with an array */

#include <stdlib.h>
#include <stdio.h>

#define MAX_SIZE 12

typedef struct queue
{
  int size;
  int head;
  int tail;
  int *arr;
} queue_t;


/* initialize the queue */
queue_t *
queue_init (int max_size)
{
  queue_t *q = malloc (sizeof (queue_t));
  q->size = max_size;
  q->head = 0;
  q->tail = 0;
  q->arr = malloc (max_size * sizeof (int));

  return q;
}

/* check if the queue is empty */
int
queue_is_empty (queue_t *q)
{
  if (q->head == q->tail)
    return 1;
  else
    return 0;
}

/* insert a value at the tail of the queue */
void
queue_put (queue_t *q, int num)
{
  q->arr[q->tail++] = num;
  q->tail = q->tail % q->size;    // wrap around
}

/* get the value at the head of the queue */
int
queue_get (queue_t *q)
{
  int num;

  q->head = q->head % q->size;

  if (!queue_is_empty (q))
    num = q->arr[q->head++];
  else
    num = -1;

  return num;
}

/* delete the memory used by the queue */
void
queue_destroy (queue_t *q)
{
  free (q->arr);
  free (q);
}


int
main ()
{
  queue_t *q = queue_init (MAX_SIZE);

  printf ("enqueue: %d\n", 0);
  queue_put (q, 0);
  printf ("enqueue: %d\n", 1);
  queue_put (q, 1);
  printf ("enqueue: %d\n", 2);
  queue_put (q, 2);
  printf ("enqueue: %d\n", 3);
  queue_put (q, 3);

  printf ("\n");

  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));
  printf ("\nenqueue: %d\n", 9);
  queue_put (q, 9);
  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));

  printf ("\n");

  printf ("enqueue: %d\n", 4);
  queue_put (q, 4);
  printf ("enqueue: %d\n", 5);
  queue_put (q, 5);

  printf ("\n");

  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));

  queue_destroy (q);

  return 0;
}
