/* A stack where the smallest element can be peeked */

#include <stdlib.h>
#include <stdio.h>

#define MAX (sizeof(int) << 25)

// use a linked list to store a stack
typedef struct node
{
  int data;
  struct node *next;
} node;

typedef struct stack_min
{
  node *top;    // stack linked list
  node *min;    // minimum element linked list
} stack_min;

void stack_min_push (stack_min * s, int data);
int stack_min_pop (stack_min * s);
int stack_min_peek_top (stack_min s);
int stack_min_get_min (stack_min s);

// three regular stack functions: push, pop, free
void
stack_push (node ** stack, int data)
{
  node *new_node = (node *) malloc (sizeof (node));
  new_node->data = data;
  new_node->next = *stack;
  *stack = new_node;
}

int
stack_pop (node ** stack)
{
  int val;

  node *tmp = (*stack)->next;
  val = (*stack)->data;
  free (*stack);
  (*stack) = tmp;

  return val;
}

void
stack_free (node * head)
{
  node *tmp;

  while (head) {
    tmp = head->next;
    free (head);
    head = tmp;
  }
}

// min stack functions

/* push data to the top of the min stack */
void
stack_min_push (stack_min * s, int data)
{
  stack_push (&(s->top), data);

  // Only need to store if new minimum, the rest will never be minimum later on
  if (data < stack_min_get_min (*s)) {
    stack_push (&(s->min), data);
  }
}

/* get element from top of the min stack */
int
stack_min_pop (stack_min * s)
{
  int value = -1;

  if (s->top) {
    value = stack_pop (&(s->top));
  }
  // if value is the minimum, next minimum in stack->min is minimum now
  if (value == stack_min_get_min (*s)) {
    stack_pop (&(s->min));
  }

  return value;
}

/* peek at the smallest element contained in the min stack */
int
stack_min_get_min (stack_min s)
{
  if (!s.min) {
    return MAX;
  } else {
    return s.min->data;
  }
}

/* peek at top of the min stack */
int
stack_min_peek_top (stack_min s)
{
  if (s.top)
    return s.top->data;
  else
    return -1;
}

void
stack_min_free (stack_min * s)
{
  stack_free (s->top);
  stack_free (s->min);
}

stack_min
stack_min_init ()
{
  stack_min s;
  s.top = NULL;
  s.min = NULL;

  return s;
}

int
main ()
{
  stack_min s = stack_min_init ();

  printf ("push: %d\n", 2);
  stack_min_push (&s, 2);
  printf ("push: %d\n", 3);
  stack_min_push (&s, 3);
  printf ("push: %d\n", 1);
  stack_min_push (&s, 1);
  printf ("\n");

  printf ("min: %d\n", stack_min_get_min (s));
  printf ("pop: %d\n", stack_min_pop (&s));
  printf ("min: %d\n", stack_min_get_min (s));
  printf ("peek top: %d\n", stack_min_peek_top (s));
  printf ("pop: %d\n", stack_min_pop (&s));
  printf ("min: %d\n", stack_min_get_min (s));
  printf ("\n");

  printf ("push: %d\n", 4);
  stack_min_push (&s, 4);
  printf ("push: %d\n", 5);
  stack_min_push (&s, 5);
  printf ("\n");

  printf ("pop: %d\n", stack_min_pop (&s));
  printf ("min: %d\n", stack_min_get_min (s));

  stack_min_free (&s);

  return 0;
}
