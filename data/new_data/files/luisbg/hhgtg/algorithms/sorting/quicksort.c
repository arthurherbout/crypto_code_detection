#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct node
{
  int data;
  struct node *next;
} node;

typedef struct stack
{
  node *top;
} stack;


void quicksort (int *l, int low, int high);
void quicksort_non_recursive (int *l, int low, int high);
static void swap (int arr[], int a, int b);
// static int get_middle_pivot (int low, int high);
static int get_best_of_three_pivot (int *l, int low, int high);
static int partition (int *l, int low, int high);
static void print_list (int *l, int len);
static void push (stack * s, int data);
static int pop (stack * s);
static int stackempty (stack * s);


/* classic recursive quicksort */
void
quicksort (int *l, int low, int high)
{
  if (low >= high)
    return;

  int middle;

  middle = partition (l, low, high);
  quicksort (l, low, middle - 1);
  quicksort (l, middle + 1, high);
}

/* iterative quicksort using a stack */
void
quicksort_non_recursive (int *l, int low, int high)
{
  int middle;

  stack s;
  push (&s, high);
  push (&s, low);

  while (!stackempty (&s)) {
    low = pop(&s);
    high = pop(&s);

    if (low >= high)
      continue;

    middle = partition (l, low, high);
    if (middle - 1 > high - middle) {
      push (&s, middle - 1);
      push (&s, low);
      push (&s, high);
      push (&s, middle + 1);
    } else {
      push (&s, high);
      push (&s, middle + 1);
      push (&s, middle - 1);
      push (&s, low);
    }
  }
}

/* partition values smaller than k to the left and bigger than k to the right */
static int
partition (int *arr, int low, int high)
{
  int c;
  int middle;

  int k = get_best_of_three_pivot (arr, low, high);
  int pivot = arr[k];
  swap (arr, low, k);           // move pivot out of array to order

  c = low + 1;
  middle = high;
  while (c <= middle)           // run the array
  {
    while ((c <= high) && (arr[c] <= pivot))    // before pivot
      c++;
    while ((middle >= low) && (arr[middle] > pivot))    // after pivot
      middle--;
    if (c < middle)
      swap (arr, c, middle);
  }

  swap (arr, low, middle);      // bring the pivot to its place

  return middle;
}

/* swap values with auxiliary memory */
static void
swap (int arr[], int a, int b)
{
  int tmp = arr[a];
  arr[a] = arr[b];
  arr[b] = tmp;
}

/* push data to stack for iterative quicksort */
static void
push (stack * s, int data)
{
  node *new = (node *) malloc (sizeof (node));
  new->data = data;

  new->next = s->top;
  s->top = new;
}

/* get element from top of stack for iterative quicksort */
static int
pop (stack * s)
{
  int ret = -1;
  node *tmp = NULL;

  if (s->top) {
    ret = s->top->data;
    tmp = s->top;
    s->top = s->top->next;
    free (tmp);
  }

  return ret;
}

/* check if stack is empty */
static int
stackempty (stack * s)
{
  if (s->top)
    return 0;
  else
    return 1;
}

/*
static int
get_middle_pivot (int low, int high)
{
  return (low + high) / 2;
} */

/* try 3 pivots and chose the middle one */
static int
get_best_of_three_pivot (int *l, int low, int high)
{
  int a, b, c;
  int pivot = low;
  int len = high - low;

  if (len >= 3) {
    a = (rand() % len) + low;
    b = (rand() % len) + low;
    c = (rand() % len) + low;

    if (a <= b)
      if (b <= c)
        pivot = b;
      else
        if (a <= c)
          pivot = c;
        else
          pivot = a;
    else
      if (c <= b)
        pivot = b;
      else
        if (a <= c)
          pivot = a;
        else
          pivot = c;
  }

  return pivot;
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
  int l[50];
  int size = 50;
  int c;

  srand (time (NULL));

  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("out of order:\n");
  print_list (l, size - 1);

  printf ("in order (recursive quicksort):\n");
  quicksort (l, 0, size - 1);
  print_list (l, size - 1);

  printf ("\n\n");
  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("new out of order:\n");
  print_list (l, size - 1);

  printf ("in order (non-recursive quicksort):\n");
  quicksort_non_recursive (l, 0, size - 1);
  print_list (l, size - 1);

  return 0;
}
