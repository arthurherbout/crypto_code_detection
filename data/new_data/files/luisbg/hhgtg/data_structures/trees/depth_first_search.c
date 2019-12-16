#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


bool visit[10] = { 0 };

void insert (node ** leaf, int key);
void traverse (node * leaf);
void depth_first_traverse (node * leaf);
bool depth_first_search (node * leaf, int key);


/* insert a key into the tree */
void
insert (node ** leaf, int key)
{
  if (!*leaf) {
    *leaf = (node *) malloc (sizeof (node));
    (*leaf)->key = key;

    (*leaf)->left = NULL;
    (*leaf)->right = NULL;

  } else if (key < (*leaf)->key) {
    insert (&(*leaf)->left, key);

  } else if (key > (*leaf)->key) {
    insert (&(*leaf)->right, key);
  }
}

/* print the values in the tree in order */
void
traverse (node * leaf)
{
  if (leaf) {
    traverse (leaf->left);
    printf ("%d .", leaf->key);
    traverse (leaf->right);
  }
}

/* traverse the list pre-order */
void
depth_first_traverse (node * leaf)
{
  if (leaf) {
    printf ("%d .", leaf->key);
    depth_first_traverse (leaf->left);
    depth_first_traverse (leaf->right);
  }
}

/* search for a key by going to the leftest node first */
bool
depth_first_search (node * leaf, int key)
{
  if (!leaf)
    return false;

  if (leaf->key == key)
    return true;

  if (key < leaf->key)
    depth_first_search (leaf->left, key);
  else
    depth_first_search (leaf->right, key);

  return false;
}


int
main ()
{
  node *root = NULL;

  int c;
  for (c = 0; c < 10; c++) {
    visit[c] = false;
  }

  insert (&root, 5);
  insert (&root, 2);
  insert (&root, 8);
  insert (&root, 1);
  insert (&root, 3);
  insert (&root, 4);
  insert (&root, 6);
  insert (&root, 9);
  insert (&root, 10);
  insert (&root, 7);

  printf ("in-order traverse: ");
  traverse (root);
  printf ("\n");

  printf ("depth first traverse: ");
  depth_first_traverse (root);
  printf ("\n");

  printf ("depth first search: ");
  printf ("%d %s\n", 5, depth_first_search (root, 5) ? "found" : "not found");

  printf ("depth first search: ");
  printf ("%d %s\n", 12, depth_first_search (root, 12) ? "found" : "not found");

  return 0;
}
