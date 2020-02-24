#include <stdlib.h>
#include <stdio.h>

#define MAX_DATA_IN_STACK 4

typedef struct stack
{
  int data[MAX_DATA_IN_STACK];
  int used;

  struct stack * next;
} stack;

typedef struct l_stacks
{
  struct stack * stack;
} l_stacks;

void push (l_stacks * ls, int data);
int pop (l_stacks * ls);


/* push data to the top of the last stack */
void push (l_stacks * ls, int data)
{
  printf("push: %d\n", data);
  stack *curr = ls->stack;

  // loop to last stack
  while (curr->used == MAX_DATA_IN_STACK && curr->next)
    curr = curr->next;

  // if last stack has space use it, if not create a new one and use it
  if (curr->used != MAX_DATA_IN_STACK) {
    curr->data[curr->used] = data;
    curr->used++;
  } else {
    stack *new = (stack *) malloc (sizeof(stack));
    new->data[0] = data;
    new->used = 1;
    new->next = NULL;
    curr->next = new;
  }
}

/* travel through stacks printing the data */
void travel (l_stacks ls) {
  stack *curr = ls.stack;

  printf("t: %d %d %d %d\n", curr->data[0], curr->data[1], curr->data[2],
         curr->data[3]);

  while (curr->next) {
    curr = curr->next;
    printf("t: %d %d %d %d\n", curr->data[0], curr->data[1], curr->data[2],
           curr->data[3]);
  }
}

/* get data at top of the last stack */
int pop (l_stacks * ls) {
  int value = -1;
  stack *curr = ls->stack;

  // loop to last stack
  while (curr->used == MAX_DATA_IN_STACK && curr->next)
    curr = curr->next;

  // stack is empty
  if (curr->used == 0)
    return value;

  // get top value in stack
  if (curr->used != 1) {
    value = curr->data[curr->used-1];
    curr->used--;

  // one element left in all stack
  } else if (ls->stack->used == 1) {
    value = curr->data[0];
    curr->used--;

  // last element of this stack get data and free stack
  } else {
    stack *tmp;
    curr = ls->stack;

    while (curr->next->next) {
      curr = curr->next;
    }
    value = curr->next->data[0];

    tmp = curr->next;
    curr->next = NULL;
    free(tmp);
  }

  return value;
}


int main ()
{
  l_stacks ls;
  stack *new = (stack *) malloc (sizeof(stack));
  new->used = 0;
  new->next = NULL;
  ls.stack = new;

  push (&ls, 1);
  push (&ls, 2);
  travel(ls);
  push (&ls, 3);
  push (&ls, 4);
  travel(ls);
  push (&ls, 5);
  push (&ls, 6);
  travel(ls);
  push (&ls, 7);
  push (&ls, 8);
  travel(ls);
  push (&ls, 9);
  push (&ls, 10);
  travel(ls);

  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));
  printf("pop: %d\n", pop (&ls));

  return 0;
}
