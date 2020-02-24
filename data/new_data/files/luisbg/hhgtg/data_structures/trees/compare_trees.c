#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


void insert (node ** leaf, int key);
void destroy_tree (struct node *leaf);

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

/* compare two trees to see if they are indentical in value */
int
is_identical (node * root1, node * root2)
{
  if (root1 == NULL && root2 == NULL)
    return 1;
  else if (root1 == NULL || root2 == NULL)
    return 0;

  if (root1->key != root2->key)
    return 0;

  return is_identical (root1->left, root2->left) &&
      is_identical (root1->right, root2->right);
}

int
main ()
{
  node *first = NULL;
  node *second = NULL;
  int nodes_same[10] = { 8, 9, 4, 6, 3, 10, 7, 5, 1, 2 };
  int nodes_diff[6] = { 1, 2, 3, 4, 5, 6 };
  int nodes_diff2[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  int c;

  printf ("inserting on first:");
  for (c = 0; c < 10; c++) {
    printf (" %d", nodes_same[c]);
    insert (&first, nodes_same[c]);
  }
  printf ("\n");

  printf ("inserting on second:");
  for (c = 0; c < 10; c++) {
    printf (" %d", nodes_same[c]);
    insert (&second, nodes_same[c]);
  }
  printf ("\n");

  printf ("are trees identical?: %s\n\n",
      is_identical (first, second) ? "yes" : "no");

  printf ("destroying second\n");
  destroy_tree (second);
  second = NULL;

  printf ("inserting on second:");
  for (c = 0; c < 6; c++) {
    printf (" %d", nodes_diff[c]);
    insert (&second, nodes_diff[c]);
  }
  printf ("\n");

  printf ("are trees identical?: %s\n\n",
      is_identical (first, second) ? "yes" : "no");

  printf ("destroying second\n");
  destroy_tree (second);
  second = NULL;

  printf ("inserting on second:");
  for (c = 0; c < 6; c++) {
    printf (" %d", nodes_diff2[c]);
    insert (&second, nodes_diff2[c]);
  }
  printf ("\n");

  printf ("are trees identical?: %s\n\n",
      is_identical (first, second) ? "yes" : "no");

  printf ("destroying second\n");
  destroy_tree (second);
  second = NULL;

  printf ("are trees identical?: %s\n",
      is_identical (first, second) ? "yes" : "no");

  destroy_tree (first);
  destroy_tree (second);

  return 0;
}
