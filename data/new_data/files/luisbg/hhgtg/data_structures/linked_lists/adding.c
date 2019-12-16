/* You have two numbers represented by a linked list, where each node         */
/* contains a single digit. The digits are stored in reverse order, such that */
/* the 1's digit is at the head of the list. Write a function that adds the   */
/* two numbers and returns the sum as a linked list.                          */

#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
  int val;
  struct node *next;
} node;


/* Travel through the list printing the elements */
static void
travel (node * l)
{
  while (l) {
    printf ("%d ", l->val);
    l = l->next;
  }
  printf ("\n");
}

/* Insert value at the head of the list */
static void
prepend (node ** l, int d)
{
  node *new = (node *) malloc (sizeof (node));
  new->val = d;
  new->next = *l;
  *l = new;
}

/* Insert value at the tail of the list */
static void
append (node ** l, int d)
{
  node *new = (node *) malloc (sizeof (node));
  node *head = *l;

  new->val = d;

  if (!head)
    *l = new;
  else {
    while (head->next) {
      head = head->next;
    }
    head->next = new;
  }
}

/* Convert a list of digits to an integer */
static int
list_to_number (node * l)
{
  int base = 1;
  int sum = 0;
  while (l) {
    sum += l->val * base;
    l = l->next;
    base *= 10;
  }

  return sum;
}

/* Convert an integer into a list of digits */
static void
number_to_list (node ** l, int n)
{
  while (n != 0) {
    append (l, n % 10);
    n = n / 10;
  }
}

/* Add the two lists converting them to numbers and back */
void
add (node * a, node * b, node ** sum)
{
  int sum_a = list_to_number (a);
  int sum_b = list_to_number (b);

  printf ("sum is %d\n", sum_a + sum_b);
  number_to_list (sum, sum_a + sum_b);
}

void
add_node (node ** res, node ** prev, node * new_node)
{
  if (*res && *prev)
    (*prev)->next = new_node;
  else
    *res = new_node;
  *prev = new_node;
}

int
check_carry (node ** res_node, int tmp)
{
  int carry;

  if (tmp >= 10) {
    tmp -= 10;
    carry = 1;
  } else {
    carry = 0;
  }

  (*res_node)->val = tmp;
  return carry;
}

int
loop_remaining_side (node ** res, node ** prev, node * lr, int carry)
{
  int tmp;

  while (lr) {
    node *res_node = (node *) calloc (1, sizeof (node));
    tmp = lr->val + carry;
    carry = check_carry (&res_node, tmp);

    add_node (res, prev, res_node);
    lr = lr->next;
  }

  return carry;
}

/* Sum two linked lists into one */
node *
add_with_lists (node * a, node * b)
{
  int tmp;
  int carry = 0;
  node *res = NULL;
  node *prev = NULL;

  //  add both lists until one finishes
  while (a && b) {
    node *res_node = (node *) calloc (1, sizeof (node));
    tmp = a->val + b->val + carry;
    carry = check_carry (&res_node, tmp);

    add_node (&res, &prev, res_node);
    a = a->next;
    b = b->next;
  }

  // one list might be longer than the other, add it to the sum
  carry = loop_remaining_side (&res, &prev, a, carry);
  carry = loop_remaining_side (&res, &prev, b, carry);

  // we might need one more digit (ex, 1 + 99)
  if (carry) {
    node *res_node = (node *) calloc (1, sizeof (node));
    res_node->val = 1;
    add_node (&res, &prev, res_node);
  }

  return res;
}


int
main ()
{
  node *a = NULL;
  node *b = NULL;
  node *c = NULL;
  node *sum = NULL;
  node *second_sum = NULL;

  prepend (&a, 6);
  prepend (&a, 1);
  prepend (&a, 7);

  prepend (&b, 2);
  prepend (&b, 9);
  prepend (&b, 5);

  travel (a);
  travel (b);

  add (a, b, &sum);
  travel (sum);

  printf ("\n");

  prepend (&c, 7);
  prepend (&c, 0);
  prepend (&c, 5);

  travel (b);
  travel (c);

  second_sum = add_with_lists (b, c);
  travel (second_sum);

  return 0;
}
