#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int data;
  struct node *next;
} node;

typedef struct stack
{
  node *top;
} stack;


int pop (stack * s);
void push (stack * s, int data);
int peek (stack s);
bool empty (stack s);
stack init ();


/* initialize the stack */
stack
init ()
{
  stack * s = (stack *) malloc (sizeof (stack));
  s->top = NULL;

  return *s;
}

/* push data to the top of the stack */
void
push (stack * s, int data)
{
  node *new = (node *) malloc (sizeof (node));
  new->data = data;

  new->next = s->top;
  s->top = new;
}

/* get data at top of the stack and remove */
int
pop (stack * s)
{
  int ret = -1;
  node *tmp;

  if (s->top) {
    ret = s->top->data;
    tmp = s->top;
    s->top = s->top->next;
    free (tmp);
  }

  return ret;
}

/* get the data at the top of the stack but don't remove */
int
peek (stack s)
{
  if (s.top)
    return s.top->data;
  else
    return -1;
}

/* check if the stack is empty */
bool
is_empty (stack s)
{
  if (s.top)
    return false;
  else
    return true;
}


int
main ()
{
  stack s = init();
  s.top = NULL;

  printf ("push: %d\n", 1);
  push (&s, 1);
  printf ("push: %d\n", 2);
  push (&s, 2);
  printf ("push: %d\n\n", 3);
  push (&s, 3);

  printf ("empty stack? %s\n\n", is_empty (s)? "yes": "no");

  printf ("pop: %d\n", pop (&s));

  printf ("push: %d\n", 4);
  push (&s, 4);
  printf ("push: %d\n\n", 5);
  push (&s, 5);

  printf ("peek: %d\n\n", peek (s));
  printf ("pop: %d\n", pop (&s));
  printf ("pop: %d\n", pop (&s));
  printf ("pop: %d\n", pop (&s));
  printf ("pop: %d\n\n", pop (&s));

  printf ("empty stack? %s\n", is_empty (s)? "yes": "no");

  return 0;
}
