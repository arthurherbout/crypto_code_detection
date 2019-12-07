/* Binary Tree, base functions */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node
{
  int key;
  struct node *left;
  struct node *right;
} node;


void insert_node (node ** leaf, int key);
void delete_node (node ** leaf, int key);
int count (struct node *leaf);
node *search_key (node * leaf, int key);
void destroy_tree (struct node *leaf);
void traverse_in_order (struct node *leaf);
void traverse_in_postorder (struct node *leaf);
void traverse_in_preorder (struct node *leaf);
bool validate_tree (struct node * leaf);
void show (node * leaf, int h, char direction);


/* insert a key into the tree */
void
insert_node (node ** leaf, int key)
{
  if (!*leaf) {      // recurse until an empty leaf and introduce new node there
    node *new_node = (node *) malloc (sizeof (node));

    if (!new_node)
      return;

    new_node->key = key;
    /* initialize the children to null */
    new_node->left = NULL;
    new_node->right = NULL;
    *leaf = new_node;

    return;
  }

  if (key <= (*leaf)->key) {
    /* if smaller recurse to the left branch */
    insert_node (&(*leaf)->left, key);
  } else {
    /* if bigger recurse to the right branch */
    insert_node (&(*leaf)->right, key);
  }
}

/* returns the address of the node to be deleted, address of its parent and */
/* whether the node is found or not                                         */
static bool
search_parent (struct node **root, int num, struct node **parent, struct
    node **x)
{
  struct node *curr = *root;
  *parent = NULL;

  while (curr) {
    /* if the node to be deleted is found */
    if (curr->key == num) {
      *x = curr;
      return true;
    }

    *parent = curr;

    if (curr->key > num)
      curr = curr->left;
    else
      curr = curr->right;
  }

  return false;
}

/* delete the specified node from the binary tree */
void
delete_node (node ** leaf, int key)
{
  node *parent, *head, *succesor;

  if (!*leaf)
    return;

  search_parent (leaf, key, &parent, &head);

  if (!search_parent (leaf, key, &parent, &head)) {
    printf ("ERROR: %d isn't in the tree", key);
    return;
  }

  if (head->right && head->left) {      //have both childs
    parent = head;              // replace with left child's tree biggest
    succesor = head->right;     // then remove the old location of that value below
                                // (that location had no childs, or just left child)
    while (succesor->left) {
      parent = succesor;
      succesor = succesor->left;
    }

    head->key = succesor->key;
    head = succesor;
  }

  if (!head->right && head->left) {     // only has left child
    if (parent->left == head)   // link past the node
      parent->left = head->left;        // can also be previous case
    else                        // after we switched values
      parent->right = head->left;

    free (head);
    return;
  }

  if (head->right && !head->left) {     // only has right child
    if (parent->left == head)   // link past the node
      parent->left = head->right;
    else
      parent->right = head->right;

    free (head);
    return;
  }

  if (!head->right && !head->left) {    // no children
    if (parent->right == head)
      parent->right = NULL;
    else
      parent->left = NULL;

    free (head);
    return;
  }
}

/* count the number of nodes in the tree */
int
count (struct node *leaf)
{
  if (!leaf)
    return 0;

  return count (leaf->left) + count (leaf->right) + 1;
}

/* height of the tree */
int
height (struct node *leaf)
{
  int u, v;

  if (!leaf)
    return -1;

  u = height (leaf->left);
  v = height (leaf->right);

  if (u > v)
    return u + 1;
  else
    return v + 1;
}

/* search for a key in the tree */
node *
search_key (node * leaf, int key)
{
  if (!leaf)
    return NULL;

  printf ("%d ->", leaf->key);

  if (key == leaf->key) {
    printf ("\n");
    return leaf;
  } else if (key < leaf->key) {
    return search_key (leaf->left, key);
  } else {
    return search_key (leaf->right, key);
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
traverse_in_order (struct node *leaf)
{
  if (leaf) {
    traverse_in_order (leaf->left);
    printf ("%d ", leaf->key);
    traverse_in_order (leaf->right);
  }
}

/* traverse the tree in pre-order */
void
traverse_in_preorder (struct node * leaf)
{
  if (leaf) {
    printf ("%d ", leaf->key);
    traverse_in_preorder (leaf->left);
    traverse_in_preorder (leaf->right);
  }
}

/* traverse the tree in post-order */
void
traverse_in_postorder (struct node * leaf)
{
  if (leaf) {
    traverse_in_postorder (leaf->left);
    traverse_in_postorder (leaf->right);
    printf ("%d ", leaf->key);
  }
}

/* check the tree is a valid binary tree */
bool
validate_tree_helper (struct node *leaf, struct node *prev)
{
  if (leaf == NULL)
    return true;

  if (!validate_tree_helper (leaf->left, prev))
    return false;
  if (prev != NULL && prev->key >= leaf->key)
    return false;

  prev = leaf;
  return validate_tree_helper (leaf->right, prev);
}

bool
validate_tree (struct node * leaf)
{
  struct node *prev = NULL;
  return validate_tree_helper (leaf, prev);
}

/* print the value and direction of the node based on the parent */
static void
print_node (int key, int h, char direction)
{
  int i;

  for (i = 0; i < h; i++)
    printf ("    ");

  printf ("%c %d\n", direction, key);
}

/* show tree left to right (rotated 90 degrees) */
void
show (node * leaf, int h, char direction)    // h is the depth level
{
  if (!leaf) {
    return;
  }

  show (leaf->right, h + 1, '/');
  print_node (leaf->key, h, direction);    // in order from biggest, to smallest
  show (leaf->left, h + 1, '\\');
}


int
main ()
{
  node *root = NULL;

  insert_node (&root, 8);
  insert_node (&root, 10);
  insert_node (&root, 4);
  insert_node (&root, 6);
  insert_node (&root, 7);
  insert_node (&root, 9);
  insert_node (&root, 5);
  insert_node (&root, 3);
  insert_node (&root, 1);
  insert_node (&root, 2);

  printf ("show tree: (rotated 90 degrees)\n");
  show (root, 0, ' ');

  printf ("search 10\n");
  search_key (root, 10);
  printf ("search 5\n");
  search_key (root, 5);
  printf ("search 2\n");
  search_key (root, 2);
  printf ("search 9\n");
  search_key (root, 9);

  printf ("\n");
  traverse_in_order (root);
  printf ("\n\n");

  printf ("deleting 5\n");
  delete_node (&root, 5);
  traverse_in_order (root);
  printf ("\n");

  printf ("deleting 4\n");
  delete_node (&root, 4);
  traverse_in_order (root);
  printf ("\n");

  printf ("validated?: %s\n", validate_tree (root) ? "yes" : "no");
  printf ("\n");

  printf ("traverse tree in order:\n");
  traverse_in_order (root);
  printf ("\n");

  printf ("traverse tree in pre-order:\n");
  traverse_in_preorder (root);
  printf ("\n");

  printf ("traverse tree in post-order:\n");
  traverse_in_postorder (root);
  printf ("\n\n");

  printf ("number of nodes in the tree: ");
  printf ("%d\n\n", count (root));

  printf ("height of the tree: ");
  printf ("%d\n", height (root));

  return 0;
}
