/* Delete a node in the middle of a single-linked list,                       */
/* Given access to only that node.                                            */
/* Input: the node C from the linked list a->b->c->d->e                       */
/* Result: nothing is returned, but the new linked list looks like a->b->d->e */

#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
  int val;
  struct node *next;
} node;

/* Insert value into head of the list */
void
insert (node ** l, int val)
{
  node *new = (node *) malloc (sizeof (node));
  new->val = val;
  new->next = *l;
  *l = new;
}

/* Travel through list printing values */
void
travel (node * l)
{
  node *tmp = l;

  printf ("the list contains: ");
  while (tmp != NULL) {
    printf ("%d ", tmp->val);

    tmp = tmp->next;
  }
  printf ("\n\n");
}

/* Remove the node from the list */
void
remove_node (node * n)
{
  node *next = NULL;

  if (n == NULL || n->next == NULL)
    return;                     // Error

  next = n->next;
  n->val = next->val;
  n->next = next->next;
  free (next);
}

int
main ()
{
  // start with an empty list
  node *list = NULL;

  // add a few elements to the list
  insert (&list, 5);
  insert (&list, 4);
  insert (&list, 3);
  insert (&list, 2);
  insert (&list, 1);

  // travel the list
  travel (list);

  // remove item
  printf ("remove item %d\n", 3);
  node *curr = list;
  while (curr != NULL && curr->val != 3)
    curr = curr->next;

  remove_node (curr);

  // travel the new list
  travel (list);

  return 0;
}
