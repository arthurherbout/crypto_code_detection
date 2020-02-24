#include <stdlib.h>
#include <stdio.h>


typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


void tree_insert (node ** leaf, int key);
void traverse (struct node *leaf);
void sorted_array_to_tree (struct node **leaf, int arr[], int start, int end);
static int check_balanced_height (struct node *leaf);
node *create_tree_selfcontained (int arr[], int start, int end);

/* insert a key into the tree */
void
tree_insert (node ** leaf, int key)
{
  if (!*leaf) {
    *leaf = (node *) malloc (sizeof (node));
    (*leaf)->key = key;

    /* initialize the children to null */
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;

  } else if (key <= (*leaf)->key)
    /* if smaller recurse to the left branch */
    tree_insert (&(*leaf)->left, key);

  else if (key > (*leaf)->key)
    /* if bigger recurse to the right branch */
    tree_insert (&(*leaf)->right, key);
}

/* check if the tree is balanced */
static int
check_balanced_height (struct node *leaf)
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

/* convert sorted array into a balanced tree */
void
sorted_array_to_tree (struct node **leaf, int arr[], int start, int end)
{
  if (end < start)
    return;

  int middle = (start + end) / 2;

  printf ("%d: ", arr[middle]);
  tree_insert (leaf, arr[middle]);
  sorted_array_to_tree (leaf, arr, start, middle - 1);
  sorted_array_to_tree (leaf, arr, middle + 1, end);
}

/* convert sorted array into a balanced tree without auxiliary functions */
node *
create_tree_selfcontained (int arr[], int start, int end)
{
  if (end < start)
    return NULL;

  int middle = (start + end) / 2;
  node *new_leaf = (node *) malloc (sizeof (node));
  printf ("%d: ", arr[middle]);

  new_leaf->key = arr[middle];
  new_leaf->left = create_tree_selfcontained (arr, start, middle - 1);
  new_leaf->right = create_tree_selfcontained (arr, middle + 1, end);

  return new_leaf;
}


int
main ()
{
  node *root = NULL;
  int *arr = (int *) malloc (10 * sizeof (int));
  int c;
  for (c = 0; c < 10; c++)
    arr[c] = c;

  sorted_array_to_tree (&root, arr, 0, 9);
  printf ("\nleft tree height: %d\n", check_balanced_height (root->left));
  printf ("right tree height: %d\n", check_balanced_height (root->right));

  arr = (int *) realloc (arr, 33 * sizeof (int));
  for (c = 10; c < 33; c++)
    arr[c] = c;
  node *second = create_tree_selfcontained (arr, 0, 32);
  printf ("\nleft tree height: %d\n", check_balanced_height (second->left));
  printf ("right tree height: %d\n", check_balanced_height (second->right));

  return 0;
}
