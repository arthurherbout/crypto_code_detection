/* Given a singly linked list, determine if it is a palindrome. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
  struct node *next;
  int n;
} node;


bool
insert_prepend (node ** l, int n)
{
  node *new_node = (node *) malloc (sizeof (node));
  if (!new_node)
    return false;

  new_node->n = n;
  new_node->next = *l;
  *l = new_node;

  return true;
}

bool
print_list (node * l)
{
  node *run = l;

  printf ("the list contains: ");
  while (run) {
    printf ("%d ", run->n);
    run = run->next;
  }
  printf ("\n\n");

  return true;
}

/* Return a reversed copy of the list */
node *
reverse_order_copy (node * curr)
{
  node *new_curr = NULL;
  node *prev = NULL;

  while (curr) {
    new_curr = (node *) malloc (sizeof (node));
    new_curr->n = curr->n;
    new_curr->next = prev;

    prev = new_curr;
    curr = curr->next;
  }

  return new_curr;
}

/* are two linked lists equal? */
bool
equal_linked_list (node * head_a, node * head_b)
{
  while (head_a && head_b) {
    if (head_a->n != head_b->n)
      return false;

    head_a = head_a->next;
    head_b = head_b->next;
  }

  if (!head_a && !head_b)
    return true;
  else
    return false;
}

/* is a linked list a palindrome? */
bool
isPalindrome (node * head)
{
  node *rev = reverse_order_copy (head);
  return equal_linked_list (head, rev);
}

int
main ()
{
  bool palindrome;
  node *l = NULL;
  node *p = NULL;

  printf ("inserting 0..9 into a list\n");
  for (int i = 0; i < 10; i++) {
    insert_prepend (&l, i);
  }
  print_list (l);

  palindrome = isPalindrome (l);
  printf ("list is %sa palindrome\n", palindrome ? "" : "not ");

  printf ("inserting 0..5..0 into a new list\n");
  for (int i = 0; i < 6; i++) {
    insert_prepend (&p, i);
  }
  for (int i = 4; i >= 0; i--) {
    insert_prepend (&p, i);
  }
  print_list (p);

  palindrome = isPalindrome (p);
  printf ("list is %sa palindrome\n", palindrome ? "" : "not ");

  return 0;
}
