#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node
{
  struct node *next;
  int value;
} node;

typedef struct stack
{
  node *top;
} stack;


static stack *stack_init ();
static void push ();
static int pop (stack * s);
static bool empty (stack * s);
static int peek (stack * s);


void
sorting_w_stacks (int size, int list[size])
{
  stack *s_a = stack_init ();
  stack *s_b = stack_init ();
  int i;

  for (i = 0; i < size; i++) {
    while (peek (s_a) <= list[i]) {
      push (s_b, pop (s_a));
    }

    push (s_a, list[i]);
    while (!empty (s_b))
      push (s_a, pop (s_b));
  }

  for (i = 0; i < size; i++) {
    list[i] = pop (s_a);
  }
}

static stack *
stack_init ()
{
  stack *s = (stack *) malloc (sizeof (stack));
  s->top = NULL;

  return s;
}

static void
push (stack * s, int val)
{
  node *new_node = (node *) malloc (sizeof (node));

  new_node->value = val;
  new_node->next = s->top;
  s->top = new_node;
}

static int
pop (stack * s)
{
  if (empty (s))
    return 100;

  int ret = s->top->value;
  node *tmp = s->top;
  s->top = s->top->next;
  free (tmp);

  return ret;
}

static bool
empty (stack * s)
{
  return (s->top == NULL);
}

static int
peek (stack * s)
{
  if (!empty (s))
    return s->top->value;
  else
    return 100;
}


int
main ()
{
  int list[10] = { 10, 8, 6, 4, 3, 5, 7, 9, 1, 2 };
  int size = 10;
  int i;

  printf ("orig list: ");
  for (i = 0; i < size; i++)
    printf ("%d ", list[i]);
  printf ("\n");

  sorting_w_stacks (size, list);

  printf ("sorted list: ");
  for (i = 0; i < size; i++)
    printf ("%d ", list[i]);
  printf ("\n");

  return 0;
}
