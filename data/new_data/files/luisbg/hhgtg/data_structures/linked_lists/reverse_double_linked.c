#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
  int data;
  struct Node *next;
  struct Node *prev;
} Node;


/* Reverse the linked list */
Node *
reverse_list (Node * head)
{
  Node *run = head;
  Node *tmp, *next, *last;

  while (run) {
    next = run->next;           // save next before changing the pointer

    tmp = run->prev;            // swap next and prev pointers
    run->prev = run->next;
    run->next = tmp;

    if (!next)                  // if next is NULL, we are at the last node
      last = run;               // which is going to be the new head
    run = next;
  }

  return last;                  // return new head
}

/* Insert a node at head of the list */
Node *
prepend_node (Node * head, int data)
{
  Node *tmp = (Node *) malloc (sizeof (Node));

  tmp->data = data;
  tmp->prev = NULL;

  if (!head) {
    tmp->next = NULL;
    return tmp;
  }

  head->prev = tmp;
  tmp->next = head;

  return tmp;
}

/* Insert a node at the tail of the list */
Node*  append_node (Node* head, int data){
    Node * new_node = (Node *) malloc (sizeof (Node));
    Node * runner;

    new_node->data = data;
    new_node->next = NULL;

    if (!head) {
        new_node->prev = NULL;
        return new_node;
    }

    runner = head;
    while (runner->next != NULL)
        runner = runner->next;

    runner->next = new_node;
    new_node->prev = runner;

    return head;
}

/* Print the values in the list */
void
print_list (Node * head)
{
  if (head == NULL) {
    printf ("empty list\n");
    return;
  }

  printf ("> ");
  while (head->next) {
    printf ("%d ", head->data);
    head = head->next;
  }
  printf ("%d ", head->data);

  printf ("< ");
  while (head->prev) {
    printf ("%d ", head->data);
    head = head->prev;
  }
  printf ("%d\n", head->data);
}

/* Remove the first node in the list with the given value */
void
remove_node (Node * head, int data)
{
  if (head == NULL)
    return;

  while (head->next) {
    if (head->data == data)
      break;
    head = head->next;
  }

  if (head->next) {             // node was found
    Node *tmp = head;

    head->next->prev = head->prev;
    head->prev->next = head->next;

    free (tmp);
  }
}


int
main ()
{
  Node *head = NULL;
  int c;
  int set[9] = { 0, 1, 2, 3, 4, 6, 7, 8, 9 };

  print_list (head);
  reverse_list (head);
  print_list (head);
  printf ("\n");

  head = prepend_node (head, 5);
  print_list (head);
  reverse_list (head);
  print_list (head);
  printf ("\n");

  c = 4;
  while (c >= 0) {
    head = prepend_node (head, set[c]);
    c--;
  }

  c = 5;
  while (c < 9) {
    head = append_node (head, set[c]);
    c++;
  }

  print_list (head);

  printf ("remove node 5 and reverse\n");
  remove_node (head, 5);

  head = reverse_list (head);
  print_list (head);

  return 0;
}
