/* Reading in a stream of integers. Periodically, you wish to be able to look
 * up the rank of a number x (the number of values less than or equal to x).
 */

#include <stdlib.h>
#include <stdio.h>

#define MAX 1000

typedef struct node
{
  struct node *left;
  struct node *right;
  int value;
  int counter;
} node_t;


/* track ranks in a binary tree with the values and counter */
void
track (node_t ** root, int x)
{
  if (!*root) {
    node_t * new_node = (node_t *) malloc (sizeof (node_t));
    new_node->value = x;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->counter = 0;

    *root = new_node;
    return;
  }

  if ((*root)->value > x) {
    (*root)->counter++;
    track (&(*root)->left, x);
  } else {
    track (&(*root)->right, x);
  }
}

/* read the binary tree recursively to get the rank */
int get_rank (node_t * root, int x, int count)
{
  if (!root)
    return -MAX;

  if (root->value > x)
    return get_rank (root->left, x, count);

  count = count + 1 + root->counter;
  if (root->value == x)
    return count;

  if (root->value < x)
    return get_rank (root->right, x, count);

  return -1;
}


int main ()
{
  int size = 8;
  int i;
  int stream[8] = {20, 15, 25, 10, 5, 13, 23, 24};

  printf ("stream: ");
  for (i = 0; i < size; i++) {
    printf (" %d", stream[i]);
  }
  printf ("\n");

  node_t *tree = NULL;

  for (i = 0; i < size; i++) {
    track (&tree, stream[i]);
  }

  for (i = 0; i < size; i++) {
    printf ("%d has rank %d\n", stream[i], get_rank (tree, stream[i], 0));
  }

  return 0;
}
