#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


node * root_insert_node (node * run, int key);
static node * rotate_right (node * r);
static node * rotate_left (node * r);
bool validate_tree (struct node * leaf);
void show (node * leaf, int h);


/* insert a key into the root of a tree */
node *
root_insert_node (node * run, int key)
{
  if (!run) {
    node *new_node = (node *) malloc (sizeof (node));
    if (!new_node)
      return NULL;

    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
  }

  if (key < run->key) {
    run->left = root_insert_node (run->left, key);
    run = rotate_right (run);
  } else {
    run->right = root_insert_node (run->right, key);
    run = rotate_left (run);
  }

  return run;
}

/* rotate parent and left children */
static node *
rotate_right (node * r)
{
  node *x = r->left;
  if (x) {
    r->left = x->right;
    x->right = r;
  }

  return x;
}

/* rotate parent and right children */
static node *
rotate_left (node * r)
{
  node *x = r->right;
  if (x) {
    r->right = x->left;
    x->left = r;
  }

  return x;
}

/* check if the tree is a valid binary search tree */
bool
validate_tree (struct node * leaf)
{
  if (leaf) {
    if (leaf->left)
      if (leaf->key < leaf->left->key)
        return false;
    if (leaf->right)
      if (leaf->key > leaf->right->key)
        return false;

    if (!validate_tree (leaf->left))
      return false;
    if (!validate_tree (leaf->right))
      return false;
  }

  return true;
}

/* print the data in the node */
void
print_node (int key, int h)
{
  int i;

  for (i = 0; i < h; i++)
    printf ("    ");

  printf ("%d\n", key);
}

/* show the tree structure */
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
  int i;
  int sample_list[10] = {8, 10, 4, 6, 7, 9, 5, 3, 1, 2};

  node *r = NULL;

  for (i = 0; i < 5; i++) {
    printf ("node insert %d\n", sample_list[i]);
    r = root_insert_node (r, sample_list[i]);
  }

  printf ("\nshow tree:\n");
  show (r, 0);
  printf ("\nvalidated?: %s\n", validate_tree (r) ? "yes\n" : "no\n");

  for (i = 5; i < 10; i++) {
    printf ("node insert %d\n", sample_list[i]);
    r = root_insert_node (r, sample_list[i]);
  }

  printf ("\nshow tree:\n");
  show (r, 0);
  printf ("\nvalidated?: %s\n", validate_tree (r) ? "yes\n" : "no\n");

  return 0;
}
