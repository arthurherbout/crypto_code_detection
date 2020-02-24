/*
 * Merge two sorted lists A and B as one linked list
 */

// TODO: if we keep track of tail of new list, we avoid traversing it
// for each Append()

#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
  int data;
  struct Node *next;
} Node;


Node *
RemoveHead (Node ** l)
{
  Node *tmp;

  if (*l) {                     // not an empty list
    tmp = *l;
    *l = tmp->next;             // list points to the second
  }

  return tmp;
}

/* Append data to tail of list */
static void
Append (Node ** head, Node * new_node)
{
  new_node->next = NULL;

  Node *run = *head;
  if (run) {                    // go to last element and insert after it
    while (run->next)
      run = run->next;

    run->next = new_node;
  } else {                      // empty list, new node is head
    *head = new_node;
  }
}

/* Print the list */
static void
Print (Node * head)
{
  while (head != NULL) {
    printf ("%d ", head->data);
    head = head->next;
  }
  printf ("\n");
}

/* Merge the two sorted lists into one */
Node *
MergeLists (Node * headA, Node * headB)
{
  Node *head = NULL;
  Node *tmp = NULL;

  while (headA && headB) {      // run through both lists until one is empty
    if (headA->data < headB->data) {    // appending the smallest new head
      tmp = RemoveHead (&headA);
      headA = tmp->next;
      Append (&head, tmp);
    } else {
      tmp = RemoveHead (&headB);
      headB = tmp->next;
      Append (&head, tmp);
    }
  }

  while (headA) {               // one of the two lists isn't empty yet
    tmp = RemoveHead (&headA);
    headA = tmp->next;
    Append (&head, tmp);
  }
  while (headB) {
    tmp = RemoveHead (&headB);
    headB = tmp->next;
    Append (&head, tmp);
  }

  return head;
}


int
main ()
{
  Node *A = NULL;
  Node *B = NULL;

  for (int i = 0; i < 10; i = i + 2) {
    Node *tmp = (Node *) malloc (sizeof (Node));
    tmp->data = i;
    tmp->next = NULL;
    Append (&A, tmp);
  }

  for (int i = 1; i < 15; i++) {
    if (i % 2 == 0)
      continue;

    Node *tmp = (Node *) malloc (sizeof (Node));
    tmp->data = i;
    tmp->next = NULL;
    Append (&B, tmp);
  }

  Print (A);
  Print (B);

  // Merge the lists
  A = MergeLists (A, B);
  Print (A);

  return 0;
}
