#include <stdlib.h>
#include <stdio.h>

typedef struct queue
{
  int *q;                       // queue
  int n;                        // number of elements
} queue;


void heapsort (int *l, int n);
static void bubble_down (queue * q, int pos);
static void make_heap (queue * q, int *l, int n);
static int extract_min (queue * q);
static void q_swap (queue * q, int a, int b);


void
heapsort (int *l, int n)
{
  int c;                        // counter

  // heap for sorting
  queue q;
  make_heap (&q, l, n);

  for (c = 0; c < n; c++)
    l[c] = extract_min (&q);
}

static void
bubble_down (queue * q, int pos)
{
  int child;                    // child index
  int c;                        // counter
  int min_index;                // index of lightest child

  child = 2 * pos;
  min_index = pos;

  for (c = 0; c <= 1; c++) {
    if ((child + c) <= q->n)
      if (q->q[min_index] > q->q[child + c])
        min_index = child + c;
  }

  if (min_index != pos) {
    q_swap (q, pos, min_index);
    bubble_down (q, min_index);
  }
}

static void
make_heap (queue * q, int *l, int n)
{
  int c;                        // counter

  q->q = (int *) malloc (n * sizeof (int));
  q->n = n;

  for (c = 0; c < n; c++)
    q->q[c + 1] = l[c];

  for (c = q->n / 2; c >= 1; c--)
    bubble_down (q, c);
}

static int
extract_min (queue * q)
{
  int min = -1;                 // minimum value

  if (q->n <= 0)
    printf ("ERROR: empty queue\n");
  else {
    min = q->q[1];
    q->q[1] = q->q[q->n];
    q->n = q->n - 1;
    bubble_down (q, 1);
  }

  return min;
}

static void
q_swap (queue * q, int a, int b)
{
  int tmp;

  tmp = q->q[a];
  q->q[a] = q->q[b];
  q->q[b] = tmp;
}

void
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
  int l[50];
  int size = 50;
  int c;
  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("out of order:\n");
  print_list (l, size - 1);

  printf ("in order:\n");
  heapsort (l, size - 1);
  print_list (l, size - 1);

  return 0;
}
