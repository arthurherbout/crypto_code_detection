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
void traverse_preorder (struct node *leaf);

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

/* insert a key into the tree with inverse order */
void
insert_reverse (node ** leaf, int key)
{
  if (!*leaf) {
    *leaf = (node *) malloc (sizeof (node));
    (*leaf)->key = key;

    /* initialize the children to null */
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;

  } else if (key > (*leaf)->key) {
    /* if bigger recurse to the left branch */
    insert (&(*leaf)->left, key);

  } else {
    /* if smaller recurse to the right branch */
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
traverse_preorder (struct node *leaf)
{
  if (leaf) {
    printf ("%d ", leaf->key);
    traverse_preorder (leaf->left);
    traverse_preorder (leaf->right);
  }
}

/* check if first tree is a mirror of the second */
static int
is_mirror (node * root1, node * root2)
{
  if (!root1 && !root2)
    return 1;
  if ((!root1 && root2) || (root1 && !root2))
    return 0;

  if (root1->key == root2->key) {
    return is_mirror (root1->left, root2->right) & is_mirror (root1->right,
        root2->left);
  }

  return 0;
}

int
main ()
{
  node *root1 = NULL, *root2 = NULL, *root3 = NULL;
  int nodes1[6] = { 1, 2, 3, 4, 5, 6 };
  int nodes2[6] = { 8, 10, 2, 1, 5, 3 };
  int c;

  for (c = 0; c < 6; c++) {
    // printf ("inserting tree 1: %d\n", nodes1[c]);
    insert (&root1, nodes1[c]);
  }

  for (c = 0; c < 6; c++) {
    // printf ("inserting tree 2: %d\n", nodes1[c]);
    insert_reverse (&root2, nodes1[c]);
  }

  for (c = 0; c < 6; c++) {
    // printf ("inserting tree 3: %d\n", nodes2[c]);
    insert (&root3, nodes2[c]);
  }

  printf ("tree 1: \n");
  traverse_preorder (root1);
  printf ("\ntree2: \n");
  traverse_preorder (root2);
  printf ("\ntree3: \n");
  traverse_preorder (root3);
  printf ("\n");

  printf ("are 1 and 2 mirrors? %s\n", is_mirror (root1, root2) ? "yes" : "no");
  printf ("are 1 and 3 mirrors? %s\n", is_mirror (root1, root3) ? "yes" : "no");

  destroy_tree (root1);
  return 0;
}
