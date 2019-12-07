/* Implement a single linked list */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
  struct node *next;
  int n;
} node;


/* Insert at the head of the list */
bool
insert_prepend (node ** l, int n)
{
  printf ("insert prepend %d\n", n);

  node *new_node = (node *) malloc (sizeof (node));
  if (!new_node)
    return false;

  new_node->n = n;
  new_node->next = *l;
  *l = new_node;

  return true;
}

/* Insert at the tail of the list */
bool
insert_append (node ** l, int n)
{
  printf ("insert append %d\n", n);

  node *run;
  node *new_node = (node *) malloc (sizeof (node));
  if (!new_node)
    return false;

  new_node->n = n;
  new_node->next = NULL;

  if (*l) {
    run = *l;
    while (run->next)           // go to last node
      run = run->next;

    run->next = new_node;       // append new_node as next of last
  } else {
    *l = new_node;
  }

  return true;
}

/* Insert after a certain element */
bool
insert_after (node ** l, int b, int n)
{
  printf ("insert %d after %d\n", n, b);

  node *run;
  node *new_node = (node *) malloc (sizeof (node));
  if (!new_node)
    return false;

  new_node->n = n;
  new_node->next = NULL;

  if (*l) {
    run = *l;
    while (run && run->n != b)  // find before element
      run = run->next;

    if (!run)
      return false;

    new_node->next = run->next;
    run->next = new_node;
  } else {
    return false;
  }

  return true;
}

/* Insert at position */
node *
insert_at_position (node * head, int data, int pos)
{
  node *run;
  node *new_node = (node *) malloc (sizeof (node));
  int c = 2;

  if (!new_node)
    return NULL;

  new_node->n = data;
  new_node->next = NULL;

  if (head) {
    if (pos == 1) {
      new_node->next = head;
      return new_node;
    }

    run = head;
    while (run && c < pos) {
      run = run->next;
      c++;
    }

    if (!run)
      return head;

    new_node->next = run->next;
    run->next = new_node;
  } else {
    return new_node;
  }

  return head;
}

/* Retrieve and remove the first item */
bool
dequeue (node ** l, int *value)
{
  node *tmp;

  if (*l) {
    *value = (*l)->n;

    tmp = *l;                   // pointer to first node
    *l = tmp->next;
    free (tmp);

    return true;
  } else {
    printf ("list is empty\n");
    return false;
  }
}

/* Retrieve and remove the last item */
bool
pop (node ** l, int *value)
{
  node *run;

  if (!*l) {
    printf ("list is empty\n");
    return false;
  }

  run = *l;

  if (!run->next) {             // only one element in the list
    *value = run->n;
    free (run);
    *l = NULL;
  } else {
    while (run->next->next) {   // make runner second to last element
      run = run->next;
    }

    *value = run->next->n;      // get value of last and remove
    free (run->next);
    run->next = NULL;
  }

  return true;
}

/* Travel through list printing the elements */
bool
travel (node * l)
{
  node *run = l;

  printf ("the list contains: ");
  while (run) {                 // run through all elements
    printf ("%d ", run->n);

    run = run->next;
  }
  printf ("\n\n");

  return true;
}

/* Remove the node with the value n */
bool
remove_node (node ** l, int n)
{
  node *run, *tmp;

  if (*l) {                     // not an empty list
    run = *l;

    if (run->n == n) {          // if first node
      *l = run->next;           // list points to the second
      free (run);
    } else {
      while (run->next->n != n) {       // find node
        if (!run->next->next)   // node isn't in the list
          return false;

        run = run->next;
      }

      tmp = run->next;
      run->next = run->next->next;      // make previous node point to next
      free (tmp);
    }
  }

  return true;
}

/* remove the nth node */
bool
remove_node_at_position (node ** l, int n)
{
  node *run, *tmp;
  int c = 2;

  if (*l) {                     // not an empty list
    run = *l;

    if (n == 1) {               // if first node
      *l = run->next;           // list points to the second
      free (run);
    } else {
      while (c < n) {           // find node
        if (!run->next)         // node isn't in the list
          return false;

        run = run->next;
        c++;
      }

      tmp = run->next;
      run->next = run->next->next;      // make previous node point to next
      free (tmp);
    }
  }

  return true;
}

/* remove the nth node from the end of the list */
node *
remove_nth_from_end (node * head, int n)
{
  node *run, *tmp, *forward;
  int c = 0;

  if (!head)                    // input checking
    return NULL;
  if (n <= 0)
    return head;

  run = head;
  forward = head;

  if (n == 1) {                 // remove last element
    while (run->next->next)
      run = run->next;
    free (run->next);
    run->next = NULL;
    return head;
  }

  while (c <= n) {              // run the forward runner ahead by n
    if (forward == NULL) {
      if (c == n) {             // first element is to be removed
        tmp = head;
        head = head->next;
        free (tmp);
      }
      return head;
    }

    forward = forward->next;
    c++;
  }

  while (forward != NULL) {     // find node
    run = run->next;
    forward = forward->next;
  }

  tmp = run->next;
  run->next = run->next->next;  // make previous node point to next
  free (tmp);

  return head;
}

/* Reverse the order of the list */
node *
reverse_order (node * curr)
{
  node *prev = NULL;            // set prev to NULL in case list is emtpy
  node *next_tmp;

  while (curr) {
    next_tmp = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next_tmp;
  }

  return prev;
}

/* return the number of elements in the list */
int list_length (node * head)
{
    int c = 1;

    while (head) {
        head = head->next;
        c++;
    }

    return c;
}


int
main ()
{
  // start with an empty list
  node *list = NULL;
  int n;
  travel (list);

  // add a few elements to the list
  insert_append (&list, 2);
  insert_append (&list, 3);
  insert_prepend (&list, 1);
  insert_prepend (&list, 0);
  insert_append (&list, 4);
  insert_append (&list, 5);
  insert_append (&list, 6);

  // travel the list
  travel (list);

  printf ("list length is now %d\n\n", list_length (list));

  // remove all elements
  dequeue (&list, &n);
  printf ("remove first item: %d\n", n);
  dequeue (&list, &n);
  printf ("remove first item: %d\n", n);
  travel (list);

  printf ("list length is now %d\n\n", list_length (list));

  // reverse order of list
  printf ("reverse order of list\n");
  list = reverse_order (list);
  travel (list);

  printf ("remove item with value 3\n");
  remove_node (&list, 3);
  travel (list);

  printf ("remove item with value 2\n");
  remove_node (&list, 2);
  travel (list);

  insert_after (&list, 4, 3);
  travel (list);

  printf ("remove item with value 7\n");
  remove_node (&list, 7);
  travel (list);

  printf ("add a 9 in position 3\n");
  list = insert_at_position (list, 9, 3);
  travel (list);

  printf ("add a 8 in position 1\n");
  list = insert_at_position (list, 8, 1);
  travel (list);

  printf ("list length is now %d\n\n", list_length (list));

  printf ("remove node at position 3\n");
  remove_node_at_position (&list, 3);
  travel (list);

  printf ("remove node at position 3 from end\n");
  list = remove_nth_from_end (list, 3);
  travel (list);

  pop (&list, &n);
  printf ("remove last item: %d\n", n);
  pop (&list, &n);
  printf ("remove last item: %d\n", n);
  dequeue (&list, &n);
  printf ("remove first item: %d\n", n);

  if (pop (&list, &n))
    printf ("remove last item: %d\n\n", n);

  printf ("list length is now %d\n\n", list_length (list));

  return 0;
}
