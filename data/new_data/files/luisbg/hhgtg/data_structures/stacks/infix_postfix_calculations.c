/* Evaluate an infix calculation with stacks by converting it to postfix
 *
 * example: ./infix_postfix_calculations "((5+7)*9)"
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct stack
{
  int *s;
  int top;
} stack_t;


/* Initialize stack type */
stack_t *
stack_init (int size)
{
  // printf ("int init: %d\n", size);

  stack_t *stack = malloc (sizeof (stack_t));;
  stack->top = -1;
  stack->s = (int *) malloc (size * sizeof (int));

  return stack;
} 

/* Push num to top element of the stack */
void
stack_push (stack_t *stack, int num)
{
  // printf ("int push: %d\n", num);

  stack->top++;
  stack->s[stack->top] = num;
}

/* Pop the top element of the stack */
int
stack_pop (stack_t *stack)
{
  int r = stack->s[stack->top];
  stack->top--;

  // printf ("int pop: %d\n", r);

  return r;
}

/* Evaluate the postfix expression */
int postfix_evaluation (char *postfix)
{
  int i = 0;
  stack_t *stack = stack_init (strlen (postfix));

  while (postfix[i]) {
    if (postfix[i] == '+')
      stack_push (stack, stack_pop (stack) + stack_pop (stack));

    if (postfix[i] == '*')
      stack_push (stack, stack_pop (stack) * stack_pop (stack));

    if ((postfix[i] >= '0') && (postfix[i] <= '9'))
      stack_push (stack, postfix[i++] - '0');
    while ((postfix[i] >= '0') && (postfix[i] <= '9'))
      stack_push (stack, 10 * stack_pop (stack) + (postfix[i++]-'0'));

    i++;
  }

  return stack_pop (stack);
}

/* Convert the infix expression into a postfix expression */
char * infix_to_postfix (char *expr)
{
  int N = strlen (expr);
  char *postfix = (char *) malloc (N * sizeof (char));
  int i, p = 0;

  printf ("operation: %s\n", expr);

  stack_t *stack = stack_init (N);

  for (i = 0; i < N; i++) {
    if (expr[i] == ')') {
      postfix[p++] = stack_pop (stack);
      postfix[p++] = ' ';
    }
    if ((expr[i] == '+') || (expr[i] == '*'))
      stack_push (stack, expr[i]);
    if ((expr[i] >= '0') && (expr[i] <= '9')) {
      postfix[p++] = expr[i];
      postfix[p++] = ' ';
    }
  }
  postfix[++p] = '\0';

  return postfix;
}


int main (int argc, char *argv[])
{
  int result;
  if (argc != 2) {
    printf ("usage: ./infix_postfix_calculations \"expression\"\n");
    return 1;
  }

  char * expr = argv[1];

  char * postfix = infix_to_postfix (expr);
  printf ("postfix: %s\n", postfix);

  result = postfix_evaluation (postfix);
  printf ("result: %d\n", result);

  return 0;
} 
