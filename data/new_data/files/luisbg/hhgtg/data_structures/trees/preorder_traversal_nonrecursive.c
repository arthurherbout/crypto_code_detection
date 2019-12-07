#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct tree
{
  int key;
  struct tree *left;
  struct tree *right;
} tree;

typedef struct stack
{
  struct stack *next;
  struct tree *data;
} stack;


void insert (tree ** leaf, int key);
bool preorder_traverse (tree *t);
static bool push (stack **q, tree *node);
static void pop (stack **q, tree **node);


/* traverse in preorder using a stack */
bool preorder_traverse (tree *t) {
  if (!t)
    return true;
  
  stack *q = (stack *) malloc (sizeof (stack));
  tree *tmp = NULL;
  
  push (&q, t);
  while (q) {
    pop (&q, &tmp);
    if (tmp) {
      printf ("%d -> ", tmp->key);
      if (tmp->right) {
        push (&q, tmp->right);
      }

      if (tmp->left) {
        push (&q, tmp->left);
      }
    }
  }

  printf ("\n"); 
 
  return true;
}

/* push node to the top of the stack */
static bool push (stack **t, tree *node)
{
  stack *new_element = (stack *) malloc (sizeof (stack));
  if (!new_element)
    return false;

  new_element->data = node;
  new_element->next = NULL;

  if (*t)
    new_element->next = *t;

  *t = new_element;
  return true;
}

/* get element from the top of teh stack */
static void pop (stack **t, tree **node)
{
  stack *tmp = *t;
  *node = (*t)->data;
  *t = (*t)->next;
  free (tmp);
}

/* insert a key into the tree */
void
insert (tree ** leaf, int key)
{
  if (!*leaf) {
    *leaf = (tree *) malloc (sizeof (tree));
    if (!*leaf)
      return;

    (*leaf)->key = key;
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;

    return;
  }

  if (key < (*leaf)->key) {
    insert (&(*leaf)->left, key);

  } else {
    insert (&(*leaf)->right, key);
  }
}


int
main ()
{
  tree *root = NULL;

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

  printf ("pre-order traverse: ");
  preorder_traverse (root);

  return 0;
}
