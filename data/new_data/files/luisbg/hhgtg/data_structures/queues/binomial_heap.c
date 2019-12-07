/* Binomial Heap Tree
 *
 * An implementation of a priority queue class backed by a binomial heap.  A
 * descripton of how such heaps work can be found in "Introduction to
 * Algorithms, Second Edition" by Cormen, Leisserson, Rivest, and Stein.
 *
 * Operation             | Runtime
 * ----------------------+----------
 * Create empty heap     | O(1)
 * Insert single element | O(lg N)
 * Query min value       | O(lg N)
 * Merge heaps           | O(lg N)
 * Delete/pop min        | O(lg N)
 *
 */


#include "stdio.h"
#include "stdlib.h"

typedef struct bh_node
{
  int key;
  char *label;
  int degree;

  struct bh_node *p;
  struct bh_node *child;
  struct bh_node *sibling;
} bh_node_t;

typedef struct binomial_heap
{
  bh_node_t *head;
} bh_t;


bh_t *bh_init ();
void bh_push (bh_t * H, int k, char *label);
bh_node_t *bh_min (bh_t * H);
bh_node_t *bh_pop_min (bh_t * H);
void bh_delete (bh_t * H, bh_node_t * x);

void bn_link (bh_node_t * y, bh_node_t * z);
bh_node_t *bh_merge (bh_t * H1, bh_t * H2);
bh_node_t *bh_union (bh_t * H1, bh_t * H2);
bh_node_t *bh_reverse (bh_node_t * x);
void bh_decrease_key (bh_t * H, bh_node_t * x, int k);


bh_t *
bh_init ()
{
  bh_t *H = (bh_t *) malloc (sizeof (bh_t));

  H->head = NULL;
  return H;
}

bh_node_t *
bh_min (bh_t * H)
{
  bh_node_t *x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *y = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *z = (bh_node_t *) malloc (sizeof (bh_node_t));
  int min;

  if (H->head == NULL)
    return NULL;

  x = H->head;
  min = 2147483646;
  while (x) {
    if (x->key < min) {
      min = x->key;
      y = x;
    }
    z = x->sibling;
    x = z;
  }

  return y;
}

void
bn_link (bh_node_t * y, bh_node_t * z)
{
  y->p = z;
  y->sibling = z->child;
  z->child = y;
  z->degree++;
}

bh_node_t *
bh_merge (bh_t * H1, bh_t * H2)
{
  bh_t *H = (bh_t *) malloc (sizeof (bh_t));
  bh_node_t *x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *y = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *z = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *tmp = (bh_node_t *) malloc (sizeof (bh_node_t));

  H = bh_init ();
  x = H1->head;
  y = H2->head;
  z = NULL;
  tmp = NULL;

  if (x && y) {
    if (x->degree <= y->degree) {
      z = x;
      tmp = x->sibling;
      x = tmp;
    } else {
      z = y;
      tmp = y->sibling;
      y = tmp;
    }
  } else if (y) {
    z = y;
    tmp = y->sibling;
    y = tmp;
  } else if (x) {
    z = x;
    tmp = x->sibling;
    x = tmp;
  }

  H->head = z;
  while (x && y) {
    if (x->degree <= y->degree) {
      z->sibling = x;
      tmp = x->sibling;
      x = tmp;
    } else {
      z->sibling = y;
      tmp = y->sibling;
      y = tmp;
    }
    tmp = z->sibling;
    z = tmp;
  }

  if (x) {
    z->sibling = x;
  } else if (y) {
    z->sibling = y;
  }

  return H->head;
}

bh_node_t *
bh_union (bh_t * H1, bh_t * H2)
{
  bh_t *H = (bh_t *) malloc (sizeof (bh_t));
  bh_node_t *prev_x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *next_x = (bh_node_t *) malloc (sizeof (bh_node_t));

  H = bh_init ();
  H->head = bh_merge (H1, H2);

  if (H->head == NULL) {
    return NULL;
  }

  prev_x = NULL;
  x = H->head;
  next_x = x->sibling;
  while (next_x) {
    if ((x->degree != next_x->degree) || ((next_x->sibling != NULL)
            && next_x->sibling->degree == x->degree)) {
      prev_x = x;
      x = next_x;
    } else if (x->key <= next_x->key) {
      x->sibling = next_x->sibling;
      bn_link (next_x, x);
    } else {
      if (prev_x == NULL) {
        H->head = next_x;
      } else {
        prev_x->sibling = next_x;
      }
      bn_link (x, next_x);
      x = next_x;
    }
    next_x = x->sibling;
  }

  return H->head;
}

void
bh_push (bh_t * H, int k, char *label)
{
  bh_t *H_ = (bh_t *) malloc (sizeof (bh_t));
  bh_node_t *x = (bh_node_t *) malloc (sizeof (bh_node_t));

  x->child = NULL;
  x->degree = 0;
  x->key = k;
  x->label = label;
  x->p = NULL;
  x->sibling = NULL;

  H_->head = x;
  H->head = bh_union (H, H_);
}

bh_node_t *
bh_pop_min (bh_t * H)
{
  bh_node_t *x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *y = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *z = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *prev_x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *prev_y = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_t *H_ = (bh_t *) malloc (sizeof (bh_t));
  int min;

  if (H->head == NULL)
    return NULL;

  x = H->head;
  prev_x = NULL;
  prev_y = NULL;
  min = 2147483647;

  while (x) {
    if (x->key < min) {
      prev_y = prev_x;
      min = x->key;
      y = x;
    }
    prev_x = x;
    z = x->sibling;
    x = z;
  }

  if (prev_y) {
    prev_y->sibling = y->sibling;
  } else {
    H->head = y->sibling;
  }

  H_ = bh_init ();
  H_->head = bh_reverse (y->child);
  H->head = bh_union (H, H_);

  return y;
}

bh_node_t *
bh_reverse (bh_node_t * x)
{
  bh_node_t *prev_x = (bh_node_t *) malloc (sizeof (bh_node_t));
  bh_node_t *next_x = (bh_node_t *) malloc (sizeof (bh_node_t));

  if (x == NULL)
    return NULL;

  prev_x = NULL;
  while (x) {
    x->p = NULL;
    next_x = x->sibling;
    x->sibling = prev_x;
    prev_x = x;
    x = next_x;
  }

  return prev_x;
}

void
bh_decrease_key (bh_t * H, bh_node_t * x, int k)
{
  bh_node_t *y;
  bh_node_t *z;
  int tmp;

  if (k > x->key) {
    printf ("Error! Value can't be bigger than current");
  } else {
    x->key = k;
    y = x;
    z = y->p;

    while ((z != NULL) && (y->key < z->key)) {
      tmp = y->key;
      y->key = z->key;
      z->key = tmp;
      y = z;
      z = y->p;
    }
  }
}

void
bh_delete (bh_t * H, bh_node_t * x)
{
  bh_decrease_key (H, x, -2147483646);
  bh_pop_min (H);
}


int
main ()
{
  bh_t *BH;
  bh_node_t *node;
  int i, rnd_key, n = 10;
  char label[32];

  BH = bh_init ();
  for (i = 0; i < n; i++) {
    rnd_key = rand () % 100;
    sprintf (label, "key %d", rnd_key);
    bh_push (BH, rnd_key, label);

    printf ("inserted %s\n", label);
    // printf ("the head is now %d\n\n", BH->head->key);
  }

  for (i = 0; i < (n / 2); i++) {
    node = bh_min (BH);
    printf ("minimum value is %d\n", node->key);

    if ((rand () % 10) < 6) {
      node = bh_pop_min (BH);
      printf ("%s is extracted\n", node->label);
      // if (BH->head)
      //   printf ("the head is now %d\n\n", BH->head->key);
    }
  }

  for (i = 0; i < n; i++) {
    rnd_key = rand () % 100;
    sprintf (label, "key [2] %d", rnd_key);
    bh_push (BH, rnd_key, label);

    printf ("inserted %d\n", rnd_key);
    // printf ("the head is now %d\n\n", BH->head->key);
  }

  while (BH->head) {
    node = bh_pop_min (BH);
    printf ("Extracted minimum value which is %d\n", node->key);
  }

  return 0;
}
