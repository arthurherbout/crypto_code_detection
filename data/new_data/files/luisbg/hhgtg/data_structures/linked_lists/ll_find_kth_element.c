/* Find the kth to last element of a singly linked list */

#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
  int n;
  struct node *next;
} node;


/* Insert value at the head of the list */
void
prepend (node ** l, int n)
{
  node *new = (node *) malloc (sizeof (node));
  new->n = n;
  new->next = *l;
  *l = new;
}


/* Find the kth element with a naive method */
int
find_kth_element_brute (node ** l, int pos)
{
  int c = 0;
  node *head;

  head = *l;
  while (head) {
    c++;
    head = head->next;
  }

  head = *l;
  for (c -= pos; c > 0; c--)
    head = head->next;

  return head->n;
}

/* Find the kth element using a runner */
int
find_kth_element_iteratively (node ** l, int k)
{
  node *p1 = NULL;
  node *p2 = *l;
  int c;

  // move p2 forward pos times
  for (c = 0; c < k; c++) {
    if (!p2)
      return 0;                 // List shorter than k
    p2 = p2->next;
  }

  // now have p1 moving behind
  p1 = *l;
  while (p2) {
    p1 = p1->next;
    p2 = p2->next;
  }

  return p1->n;
}


int
main ()
{
  node *l = (node *) malloc (sizeof (node));
  l = NULL;

  prepend (&l, 10);
  prepend (&l, 9);
  prepend (&l, 8);
  prepend (&l, 7);
  prepend (&l, 6);
  prepend (&l, 5);
  prepend (&l, 4);
  prepend (&l, 3);
  prepend (&l, 2);
  prepend (&l, 1);

  // brute force
  printf ("%d th last element: %d\n", 1, find_kth_element_brute (&l, 1));
  printf ("%d th last element: %d\n", 2, find_kth_element_brute (&l, 2));
  printf ("%d th last element: %d\n", 3, find_kth_element_brute (&l, 3));

  // iteratively
  printf ("%d th last element: %d\n", 4, find_kth_element_iteratively (&l, 4));
  printf ("%d th last element: %d\n", 5, find_kth_element_iteratively (&l, 5));
  printf ("%d th last element: %d\n", 6, find_kth_element_iteratively (&l, 6));
  printf ("%d th last element: %d\n", 12, find_kth_element_iteratively (&l, 12));

  return 0;
}
