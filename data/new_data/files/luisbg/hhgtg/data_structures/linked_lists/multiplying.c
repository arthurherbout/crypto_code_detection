/* You have two numbers represented by a linked list, where each node         */
/* contains a single digit. With the MSB at the head of the list.             */
/* Write code to multiply the two numbers                                     */


#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
  int d;
  struct node *next;
} node;


/* Travel through the list printing the elements */
static void
print_number (node * l)
{
  while (l) {
    printf ("%d ", l->d);
    l = l->next;
  }
  printf ("\n");
}

/* Insert value at the tail of the list */
static void
append (node ** l, int d)
{
  node *new = (node *) malloc (sizeof (node));
  node *head = *l;

  new->d = d;

  if (!head)
    *l = new;
  else {
    while (head->next) {
      head = head->next;
    }
    head->next = new;
  }
}

/* Multiply the two lists */
int
multiply (node * a, node * b)
{
  node *run_a, *run_b;
  int product = 0;
  int tmp;

  run_a = a;
  while (run_a) {        // run through a multiplying it with each digit of ...
    tmp = 0;
    run_b = b;
    while (run_b) {      // b, and for each digit of b increase digit factor
      tmp *= 10;
      tmp += run_a->d * run_b->d;

      run_b = run_b->next;
    }
    if (product)            // increase the factor of each tmp sum
      product *= 10;        // but not for the first digit
    product += tmp;

    run_a = run_a->next;
  }

  return product;
}


int
main ()
{
  node *a = NULL;
  node *b = NULL;
  // node *sum = NULL;
  int sum;

  append (&a, 6);
  append (&a, 1);
  append (&a, 7);

  append (&b, 2);
  append (&b, 9);
  append (&b, 5);
  append (&b, 0);

  print_number (a);
  print_number (b);

  sum = multiply (a, b);
  printf ("sum: %d\n", sum);

  return 0;
}
