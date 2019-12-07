#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


void insert (node ** leaf, int key);
void destroy_tree (struct node *leaf);
void traverse (struct node *leaf);

/* insert a key into the tree */
void
insert (node ** leaf, int key)
{
  if (!*leaf) {
    *leaf = (node *) malloc (sizeof (node));
    (*leaf)->key = key;

    /* initialize the children to null */
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;

  } else if (key <= (*leaf)->key) {
    /* if smaller recurse to the left branch */
    insert (&(*leaf)->left, key);

  } else if (key > (*leaf)->key) {
    /* if bigger recurse to the right branch */
    insert (&(*leaf)->right, key);
  }
}

/* destroy the tree and release all memory */
void
destroy_tree (struct node *leaf)
{
  if (leaf) {
    destroy_tree (leaf->left);
    destroy_tree (leaf->right);
    free (leaf);
  }
}

/* traverse the tree in order */
void
traverse (struct node *leaf)
{
  if (leaf) {
    traverse (leaf->left);
    printf ("%d ", leaf->key);
    traverse (leaf->right);
  }
}

/* check if both children of leaf have the same balanced height recursively */
// O(n)
static int
check_balanced_height (struct node *leaf)           /* cleaner than below */
{
  if (!leaf)
    return 0;                   // height of 0

  /* Check if left is balanced */
  int lefth = check_balanced_height (leaf->left);
  if (lefth == -1) {
    return -1;                  // not balanced
  }
  int righth = check_balanced_height (leaf->right);
  if (righth == -1) {
    return -1;                  // not balanced
  }

  /* check if current node is balanced */
  int hdiff = lefth - righth;
  if (hdiff > 1 || hdiff < -1)
    return -1;
  else {
    if (lefth < righth)
      return righth + 1;
    else
      return lefth + 1;
  }
}

/* get the height from the leaf to last level */
static int
get_height (struct node *leaf)
{
  if (!leaf)
    return 0;

  int lefth = get_height (leaf->left) + 1;
  int righth = get_height (leaf->right) + 1;

  return (lefth < righth) ? righth : lefth;
}

/* get the amount of nodes in the tree */
static int
get_size (struct node *leaf)
{
  if (!leaf)
    return 0;

  int lefts = get_size (leaf->left);
  int rights = get_size (leaf->right);

  return 1 + lefts + rights;
}

/* sum all the values stored in the tree */
int get_sum (node * leaf) {
    if (!leaf)
        return 0;

    int lefts = get_sum (leaf->left);
    int rights = get_sum (leaf->right);

    return leaf->key + lefts + rights;
}

int
number_of_half_nodes(struct node * root)
{
    if (!root)
        return 0;

    int lefts = number_of_half_nodes (root->left);
    int rights = number_of_half_nodes (root->right);

    if ((!root->left && root->right) || (root->left && !root->right))
        return 1 + lefts + rights;
    else
        return lefts + rights;
}

/* check if the leaf has balanced children */
// this function calls same nodes repeatedly, sub-optimal O(n^2)
bool
is_balanced_recursive (struct node * leaf)
{
  if (!leaf)
    return true;                // Base case

  int height_diff = get_height (leaf->left) - get_height (leaf->right);
  if (height_diff > 1 || height_diff < -1)
    return false;
  else
    return is_balanced_recursive (leaf->left)
        && is_balanced_recursive (leaf->right);
}

/* uses the check balanced height to check if it is balanced */
bool
is_balanced (struct node * leaf)
{
  if (check_balanced_height (leaf) != -1)
    return true;
  else
    return false;
}


int
main ()
{
  node *root = NULL;
  int nodes[10] = { 8, 9, 4, 6, 3, 10, 7, 5, 1, 2 };
  int c;

  for (c = 0; c < 6; c++) {
    printf ("inserting: %d\n", nodes[c]);
    insert (&root, nodes[c]);
  }
  printf ("max height is: %d\n", get_height (root));
  printf ("is it balanced? %s\n", is_balanced (root) ? "yes" : "no");

  for (; c < 10; c++) {
    printf ("inserting: %d\n", nodes[c]);
    insert (&root, nodes[c]);
  }
  printf ("max height is: %d\n", get_height (root));
  printf ("is it balanced? %s\n\n", is_balanced (root) ? "yes" : "no");

  printf ("tree size is: %d\n\n", get_size (root));

  printf ("number of half nodes (only one child): %d\n\n",
    number_of_half_nodes (root));

  printf ("sum of all values in the tree: %d\n", get_sum (root));

  destroy_tree (root);
  return 0;
}
