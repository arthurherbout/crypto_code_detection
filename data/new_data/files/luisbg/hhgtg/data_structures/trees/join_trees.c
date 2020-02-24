/* Join Two Binary Trees Together */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


node * join_trees (node *a, node *b);
node * insert_node (node ** leaf, int key);
void destroy_tree (struct node *leaf);
void traverse_in_order (struct node *leaf);
void traverse_in_postorder (struct node *leaf);
void traverse_in_preorder (struct node *leaf);
bool validate_tree (struct node * leaf);
void show (node * leaf, int h);


/* join two trees into one keeping the order rules */
node *
join_trees (node *a, node *b)
{
  if (a == NULL)
    return b;
  if (b == NULL)
    return a;

  b = insert_node (&b, a->key);
  b->left = join_trees (a->left, b->left);
  b->right = join_trees (a->right, b->right);

  free (a);
  return b;
}

/* insert a key into the tree */
node *
insert_node (node ** leaf, int key)
{
  node *run = NULL;
  if (!*leaf) {
    node *new_node = (node *) malloc (sizeof (node));
    if (!new_node)
      return NULL;

    new_node->key = key;
    /* initialize the children to null */
    new_node->left = NULL;
    new_node->right = NULL;
    *leaf = new_node;

    return new_node;
  }

  run = *leaf;
  if (key <= run->key) {
    /* if smaller recurse to the left branch */
    insert_node (&run->left, key);
  } else {
    /* if bigger recurse to the right branch */
    insert_node (&run->right, key);
  }

  return run;
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

/* validate if tree follows order rules */
bool
validate_tree (struct node * leaf)
{
  if (leaf) {
    if (leaf->left && leaf->key < leaf->left->key)
        return false;
    if (leaf->right && leaf->key > leaf->right->key)
        return false;

    if (!validate_tree (leaf->left))
      return false;
    if (!validate_tree (leaf->right))
      return false;
  }

  return true;
}

/* print the data at the node */
static void
print_node (int key, int h)
{
  int i;

  for (i = 0; i < h; i++)
    printf ("    ");

  printf ("%d\n", key);
}

/* show tree structure rotated 90 degrees */
void
show (node * leaf, int h)
{
  if (!leaf) {
    // print_node ('*', h);
    return;
  }

  show (leaf->right, h + 1);
  print_node (leaf->key, h);
  show (leaf->left, h + 1);
}


int
main ()
{
  node *a = NULL;
  node *b = NULL;

  insert_node (&a, 8);
  insert_node (&a, 10);
  insert_node (&a, 6);
  insert_node (&a, 4);
  insert_node (&a, 7);

  insert_node (&b, 5);
  insert_node (&b, 1);
  insert_node (&b, 9);
  insert_node (&b, 3);
  insert_node (&b, 2);

  printf ("show tree A:\n");
  show (a, 0);

  printf ("show tree B:\n");
  show (b, 0);

  printf ("validated A?: %s\n", validate_tree (a) ? "yes" : "no");
  printf ("validated B?: %s\n\n", validate_tree (b) ? "yes" : "no");

  join_trees (b, a);

  printf ("show joint tree:\n");
  show (a, 0);

  printf ("validated?: %s\n", validate_tree (a) ? "yes\n" : "no\n");

  destroy_tree (a);
  destroy_tree (b);

  return 0;
}
