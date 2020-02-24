/* Determine wether a given list is circular or if the list has an ending    */
/* node. Without modifying the list.                                         */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int d;
  struct node *next;
} node;


/* Insert value at the head of the list */
static void
lineal_prepend (node ** l, int d)
{
  node *new = (node *) malloc (sizeof (node));
  new->d = d;
  new->next = *l;
  *l = new;
}

static node *
circular_prepend (node * head, int d)
{
  node *new_node = (node *) malloc (sizeof (node));
  node *run;

  new_node->d = d;
  new_node->next = head;

  if (head == NULL) {
    new_node->next = new_node;
    return new_node;
  }

  run = head->next;
  while (run->next != head)
    run = run->next;
  run->next = new_node;

  return new_node;
}

/* Insert value at the tail of the circular list
 * Return head */
static node *
circular_append (node * head, int data)
{
  node *curr = NULL;
  node *new = (node *) malloc (sizeof (node));
  new->d = data;
  new->next = head;

  if (!head) {                  // empty list
    new->next = new;
    return new;
  }

  curr = head->next;
  if (curr == head) {           // one element
    head->next = new;
    return head;
  }

  // more than one element, find the last one
  while (curr->next != head)
    curr = curr->next;
  curr->next = new;

  return head;
}

static node *
circular_remove_last (node * head)
{
  node *curr = NULL;

  if (!head) {                  // empty list
    return NULL;
  }

  curr = head->next;
  if (curr == head) {           // one element
    free (curr);
    return NULL;
  }

  // more than one element, find the last one
  while (curr->next->next != head) {
    curr = curr->next;
  }
  free (curr->next);
  curr->next = head;

  return head;
}

static node *
circular_remove_first (node * head)
{
  node *curr;

  if (!head)                    // empty list
    return NULL;

  if (head == head->next) {     // one element
    free (head);
    return NULL;
  }
  // more than one element, find last to make it point to the new head
  curr = head->next;
  while (curr->next != head) {
    curr = curr->next;
  }
  curr->next = head->next;
  free (head);

  return curr->next;
}

/* Turn the list circular by having tail element point to the head */
static void
make_circular (node ** l)
{
  node *first = *l;
  node *head = *l;

  while (head->next) {
    head = head->next;
  }

  head->next = first;
}

/* Check if a list is circular */
bool
find_cyclical (node * head)
{
  node *faster = head;

  while (true) {
    // if the faster pointer encounters a NULL element
    if (!faster || !faster->next)
      return false;

    // advance the pointers
    head = head->next;
    faster = faster->next->next;

    // if faster pointer ever equals slower then it's a circular list
    if (faster == head)
      return true;
  }
}

void
print_cyclical (node * head)
{
  node *run = head->next;

  if (!head) {
    printf ("empty list");
    return;
  }

  printf ("%d :", head->d);
  while (run != head) {
    printf ("%d :", run->d);
    run = run->next;
  }

  printf ("\n\n");
  return;
}

int
main ()
{
  node *l = NULL;
  node *m = NULL;
  lineal_prepend (&l, 9);
  lineal_prepend (&l, 8);
  lineal_prepend (&l, 7);
  lineal_prepend (&l, 6);
  lineal_prepend (&l, 5);
  lineal_prepend (&l, 4);
  lineal_prepend (&l, 3);
  lineal_prepend (&l, 2);
  lineal_prepend (&l, 1);
  lineal_prepend (&l, 0);

  printf ("is it circular? %s\n", find_cyclical (l) ? "yes" : "no");

  make_circular (&l);
  printf ("is it circular? %s\n\n", find_cyclical (l) ? "yes" : "no");
  print_cyclical (l);

  m = circular_append (m, 11);
  m = circular_append (m, 12);
  m = circular_append (m, 13);
  m = circular_append (m, 14);
  m = circular_prepend (m, 10);
  printf ("is it circular? %s\n\n", find_cyclical (m) ? "yes" : "no");
  print_cyclical (m);

  printf ("remove last\n");
  circular_remove_last (m);
  print_cyclical (m);

  printf ("remove first\n");
  m = circular_remove_first (m);
  printf ("is it circular? %s\n", find_cyclical (m) ? "yes" : "no");
  print_cyclical (m);


  return 0;
}
