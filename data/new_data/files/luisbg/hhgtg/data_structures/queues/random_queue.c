/* Queue that gets a random element instead of first */

#include <stdlib.h>
#include <stdio.h>

#define maxN 12

typedef enum
{
  UNUSED, USED
} use;

typedef struct queue
{
  int *elements;     // array of elements
  int *used;         // array of which positions are used

  int size;          // max size of the queue
  int num_elements;  // number of stored elements
} queue_t;


/* Initialize queue */
queue_t *
queue_init (int max_num)
{
  queue_t *q = malloc (sizeof (queue_t));
  q->num_elements = 0;
  q->size = max_num;
  q->elements = malloc (max_num * sizeof (int));
  q->used = malloc (max_num * sizeof (use));

  return q;
}

/* Check if the queue is empty */
int
queue_is_empty (queue_t *q)
{
  if (q->num_elements == 0)
    return 1;
  else
    return 0;
}

/* Check if the queue is full */
int
queue_is_full (queue_t *q)
{
  if (q->num_elements == q->size)
    return 1;
  else
    return 0;
}

/* Insert a number in the first empty position of the queue */
void
queue_put (queue_t *q, int num)
{
  int pos = 0;
  if (!queue_is_full (q)) {
    // loop until empty position
    while (q->used[pos] == USED && pos < q->size)
      pos++;

    q->elements[pos] = num;
    q->used[pos] = USED;
    q->num_elements++;
  }
}

/* Get an element from a random used position of the queue */
int
queue_get (queue_t *q)
{
  int num = -1;
  int pos;

  if (!queue_is_empty (q)) {
    // loop until a random used position
    do {
      pos = rand() % q->size;
    } while (q->used[pos] == UNUSED);

    num = q->elements[pos];
    q->used[pos] = UNUSED;
    q->num_elements--;
  }

  return num;
}

/* Delete the queue from memory */
void
queue_destroy (queue_t *q)
{
  free(q->elements);
  free(q->used);
}


int
main ()
{
  queue_t *q = queue_init (maxN);

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
  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));
  printf ("\n");

  printf ("enqueue: %d\n", 4);
  queue_put (q, 4);
  printf ("enqueue: %d\n", 5);
  queue_put (q, 5);
  printf ("enqueue: %d\n", 6);
  queue_put (q, 6);
  printf ("enqueue: %d\n", 7);
  queue_put (q, 7);
  printf ("enqueue: %d\n", 8);
  queue_put (q, 8);
  printf ("enqueue: %d\n", 9);
  printf ("\n");

  printf ("dequeue: %d\n", queue_get (q));
  printf ("\n");

  queue_put (q, 9);
  printf ("enqueue: %d\n", 10);
  queue_put (q, 10);
  printf ("enqueue: %d\n", 11);
  queue_put (q, 11);
  printf ("enqueue: %d\n", 12);
  queue_put (q, 12);
  printf ("\n");

  printf ("dequeue: %d\n", queue_get (q));
  printf ("dequeue: %d\n", queue_get (q));

  printf ("\n");

  queue_destroy (q);

  return 0;
}
