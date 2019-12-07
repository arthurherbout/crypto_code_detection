#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct queue
{
  int value;
  struct queue *next;
} queue;


void mergesort (int *l, int low, int high);
void mergesort_bottom_up (int *l, int low, int high);
static void enqueue (queue ** q, int value);
static int dequeue (queue ** q);
static int headq (queue * q);
static bool empty_queue (queue * q);
static void merge (int *l, int low, int middle, int high);
static void print_list (int *l, int len);


/* up-down merge sort */
void
mergesort (int *l, int low, int high)
{
  int middle;

  if (low < high) {             // divide the list recursively in halves, merge the halves
    middle = (low + high) / 2;
    mergesort (l, low, middle);
    mergesort (l, middle + 1, high);
    merge (l, low, middle, high);
  }
}

/* bottom up merge sort */
void
mergesort_bottom_up (int *l, int low, int high)
{
  int i, m;
  int x, min;

  for (m = 1; m <= high - low; m = m + m)
    for (i = low; i <= high - m; i += m + m) {
      x = i + m + m - 1;
      if (x < high)
        min = x;
      else
        min = high;

      merge (l, i, i + m - 1, min);
    }
}

/* merging step of merge sort */
static void
merge (int *l, int low, int middle, int high)
{
  int i;                        // counter
  queue *buffer1 = NULL;
  queue *buffer2 = NULL;        // buffers to hold elements for merging

  for (i = low; i <= middle; i++)       // queue first half in buffer 1
    enqueue (&buffer1, l[i]);
  for (i = middle + 1; i <= high; i++)  // queue second half in buffer 2
    enqueue (&buffer2, l[i]);

  i = low;                      // run through the buffers getting the lowest number in either head
  while (!empty_queue (buffer1) && !empty_queue (buffer2)) {
    if (headq (buffer1) <= headq (buffer2))
      l[i++] = dequeue (&buffer1);
    else
      l[i++] = dequeue (&buffer2);
  }

  while (!empty_queue (buffer1))        // one of the buffers still has elements
    l[i++] = dequeue (&buffer1);
  while (!empty_queue (buffer2))
    l[i++] = dequeue (&buffer2);
}

/* add value to the queue */
static void
enqueue (queue ** q, int value)
{
  queue *node = (queue *) malloc (sizeof (queue));
  node->value = value;
  node->next = NULL;

  if (!*q) {
    *q = node;

  } else {
    queue *curr = *q;
    while (curr->next != NULL) {
      curr = curr->next;
    }

    curr->next = node;
  }
}

/* get head element from the queue */
static int
dequeue (queue ** q)
{
  if (*q) {
    queue *node = *q;
    int value = node->value;
    *q = node->next;
    free (node);

    return value;
  } else {
    return -1;
  }
}

/* see the value at the head of the queue, but don't remove it */
static int
headq (queue * q)
{
  return q->value;
}

/* check if queue is empty */
static bool
empty_queue (queue * q)
{
  if (!q)
    return true;
  else
    return false;
}

/* print the list of values */
static void
print_list (int *l, int len)
{
  int c;
  for (c = 0; c < len; c++) {
    printf ("%d ", l[c]);
  }
  printf ("\n");
}


int
main ()
{
  int l[42];
  int size = 42;
  int c;
  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("out of order:\n");
  print_list (l, size);

  mergesort (l, 0, size - 1);
  printf ("in order mergesort:\n");
  print_list (l, size);

  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("\nout of order:\n");
  print_list (l, size);

  mergesort_bottom_up (l, 0, size - 1);
  printf ("in order bottom-up mergesort:\n");
  print_list (l, size);

  return 0;
}
