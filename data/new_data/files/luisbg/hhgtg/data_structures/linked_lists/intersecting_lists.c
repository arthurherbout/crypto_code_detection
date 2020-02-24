#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Definition for singly-linked list.
typedef struct ListNode
{
  int val;
  struct ListNode *next;
} ListNode;


// unused but keeping it for future reference
bool
check_if_lists_intersect (ListNode * headA, ListNode * headB)
{
  if (!headA || !headB)
    return false;

  while (headA->next)
    headA = headA->next;

  while (headB->next)
    headB = headB->next;

  return headA->val == headB->val;
}

int
get_list_length (ListNode * head)
{
  int len = 0;

  while (head) {
    len++;
    head = head->next;
  }

  return len;
}

struct ListNode *
get_intersecting_node (ListNode * headA, ListNode * headB)
{
  ListNode *run_a = headA;
  ListNode *run_b = headB;

  int len_a = get_list_length (headA);
  int len_b = get_list_length (headB);

  if (len_a == 0 || len_b == 0)
    return NULL;

  while (len_a != len_b) {
    if (len_a > len_b) {
      run_a = run_a->next;
      len_a--;
    } else if (len_a < len_b) {
      run_b = run_b->next;
      len_b--;
    }
  }

  while (run_a != run_b) {
    run_a = run_a->next;
    run_b = run_b->next;

    if (!run_a || !run_b)
      return NULL;
  }

  return run_a;
}

bool
append_node_intersecting (ListNode ** list_a, ListNode ** list_b, int val,
    bool intersect)
{
  ListNode *run;
  ListNode *new_node = (ListNode *) malloc (sizeof (ListNode));
  if (!new_node)
    return false;

  new_node->val = val;
  new_node->next = NULL;

  if (*list_a) {
    run = *list_a;
    while (run->next)
      run = run->next;

    run->next = new_node;
  } else {
    *list_a = new_node;
  }

  if (intersect) {              // if intersecting same as above for b
    if (*list_b) {
      run = *list_b;
      while (run->next)
        run = run->next;

      run->next = new_node;
    } else {
      *list_b = new_node;
    }
  }

  return true;
}

int
main ()
{
  ListNode *list_a = NULL;
  ListNode *list_b = NULL;
  ListNode *intersect;

  printf ("created two empty lists\n");
  intersect = get_intersecting_node (list_a, list_b);
  if (intersect)
    printf ("intersection at: %d\n\n", intersect->val);
  else
    printf ("no intersection node\n\n");

  printf ("appending 0..2 in first list, and 5..10 in the second\n");
  for (int i = 0; i < 3; i++)
    append_node_intersecting (&list_a, NULL, i, false);
  for (int i = 5; i < 11; i++)
    append_node_intersecting (&list_b, NULL, i, false);

  intersect = get_intersecting_node (list_a, list_b);
  if (intersect)
    printf ("intersection at: %d\n\n", intersect->val);
  else
    printf ("no intersection node\n\n");

  printf ("creating an insterting node with value 42\n");
  append_node_intersecting (&list_a, &list_b, 42, true);

  intersect = get_intersecting_node (list_a, list_b);
  if (intersect)
    printf ("intersection at: %d\n\n", intersect->val);
  else
    printf ("no intersection node\n\n");

  printf ("appending 21..23 to the first list\n");
  for (int i = 21; i < 24; i++)
    append_node_intersecting (&list_a, NULL, i, false);

  intersect = get_intersecting_node (list_a, list_b);
  if (intersect)
    printf ("intersection at: %d\n", intersect->val);
  else
    printf ("no intersection node\n");

  return 0;
}
