/* Display all paths in a binary tree */

#include <stdlib.h>
#include <stdio.h>


typedef struct tree_node
{
  int key;
  struct tree_node *left;
  struct tree_node *right;
} tree_node;

typedef struct node
{
  int data;
  struct node *next;
} node;

typedef struct queue
{
  node *head;
} queue;


void display_all_paths (tree_node * leaf, queue * q);
void display_paths_to_leaves (tree_node * leaf, queue * q);
void insert_node (tree_node ** leaf, int key);
void destroy_tree (struct tree_node *leaf);
void show (tree_node * leaf, int h, char direction);
static queue * init_queue ();
static void enqueue (queue * q, int data);
static int pop (queue * q);
static void print_queue (queue *q);


/* display all paths of the tree */
void
display_all_paths (tree_node * leaf, queue * q)
{
  // traverse adding elements to a queue pre-order
  if (leaf) {
    enqueue (q, leaf->key);

    display_all_paths (leaf->left, q);
    display_all_paths (leaf->right, q);

    // displaying the queue and remove last leaf when backtracking
    print_queue (q);
    pop (q);

  }
}

/* display all paths of the tree */
void
display_paths_to_leaves (tree_node * leaf, queue * q)
{
  // traverse adding elements to a queue pre-order
  if (leaf) {
    enqueue (q, leaf->key);

    display_paths_to_leaves (leaf->left, q);
    display_paths_to_leaves (leaf->right, q);

    // when backtracking if at a child-less leaf, display path
    if (!leaf->left && !leaf->right)
      print_queue (q);

    pop (q);
  }
}

/* create and initialize the queue */
queue *
init_queue ()
{
  queue *q = (queue *) malloc (sizeof (queue));
  q->head = NULL;

  return q;
}

/* add data to the tail of the queue */
void
enqueue (queue * q, int data)
{
  node *run;
  node *new = (node *) malloc (sizeof (node));
  new->data = data;
  new->next = NULL;

  if (!q->head) {
    q->head = new;
    return;
  }

  run = q->head;
  while (run->next)
    run = run->next;

  run->next = new;
}

/* get data from tail of the queue */
int
pop (queue * q)
{
  int d = -1;
  node *run = q->head;

  if (!q->head)
    return -1;

  if (!run->next) {
    d = run->data;
    free (run);
    q->head = NULL;

  } else {
    while (run->next->next)
      run = run->next;

    d = run->next->data;
    free (run->next);
    run->next = NULL;
  }

  return d;
}

/* display all nodes in the queue */
static void
print_queue (queue * q)
{
  node *run = q->head;
  int first = 1;

  while (run) {
    if (first) {
      printf ("%d", run->data);
      first = 0;
    } else
      printf (" -> %d", run->data);

    run = run->next;
  }
  printf ("\n");
}

/* insert a key into the tree */
void
insert_node (tree_node ** leaf, int key)
{
  if (!*leaf) {      // recurse until an empty leaf and introduce new node there
    tree_node *new_node = (tree_node *) malloc (sizeof (tree_node));

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

/* destroy the tree and release all memory */
void
destroy_tree (struct tree_node *leaf)
{
  if (leaf) {
    destroy_tree (leaf->left);
    destroy_tree (leaf->right);
    free (leaf);
  }
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
show (tree_node * leaf, int h, char direction)    // h is the depth level
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
  tree_node *root = NULL;
  queue * q;

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

  printf ("\ndisplay all paths\n");
  q = init_queue ();
  display_all_paths (root, q);
  free (q);

  printf ("\ndisplay all paths to leaves\n");
  q = init_queue ();
  display_paths_to_leaves (root, q);
  free (q);

  return 0;
}
