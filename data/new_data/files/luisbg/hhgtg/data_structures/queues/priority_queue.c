/* Priority queue. Keeps the items in an unordered array stack.
 * Then biggest item is removed from the list
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct Item
{
  int priority;
  char label[32];
} Item_t;

typedef struct PQ
{
  Item_t *q;
  bool *used;
  int size;
  int max_size;
} PQ_t;


/* initialize priority queue */
PQ_t *
pq_init (int max_size)
{
  PQ_t * pq = (PQ_t *) malloc (sizeof (PQ_t));
  pq->q  = (Item_t *) malloc (max_size * sizeof (Item_t));
  pq->used = (bool *) calloc (max_size, sizeof (bool));
  pq->size = 0;
  pq->max_size = max_size;

  return pq;
}

/* check if queue is empty */
int
pq_empty (PQ_t * pq)
{
  return pq->size == 0;
}

/* insert value into priority queue */
void
pq_insert (PQ_t * pq, Item_t v)
{
  printf ("Insert: %d :: %s\n", v.priority, v.label);

  int i = 0;

  // insert v in the first unused spot
  if (pq->size < pq->max_size) {
    pq->size++;
    while (pq->used[i])
      i++;

    pq->q[i] = v;
    pq->used[i] = true;
  }
}

/* remove biggest value from priority queue */
Item_t
pq_del_max (PQ_t * pq)
{
  int j, top = -1,
    max = -1;
  Item_t tmp;

  /* find the top value */
  for (j = 0; j < pq->max_size; j++)
    if (pq->used[j] && max < pq->q[j].priority) {
      top = j;
      max = pq->q[top].priority;
    }

  /* return the top value */
  if (top != -1) {
    tmp = pq->q[top];
    pq->used[top] = false;
    pq->size--;
  }

  return tmp;
}

/* Delete the memory used by the priority queue */
void
pq_free (PQ_t * pq)
{
  free (pq->q);
  free (pq->used);
  free (pq);
}

int main ()
{
  int i, p;
  Item_t v[12];
  Item_t w;

  PQ_t *pq = pq_init (12);

  /* insert 12 random values */
  for (i = 0; i < 12; i++) {
    p = rand() % 100;
    v[i].priority = p;
    sprintf (v[i].label, "word %d", p);

    pq_insert (pq, v[i]);
  }

  /* get the top 6 */
  printf ("\nGet priorities:\n");
  for (i = 0; i < 6; i++) {
    w = pq_del_max (pq);
    printf ("%s\n", w.label);
  }

  /* insert 4 new values */
  for (i = 0; i < 4; i++) {
    p = rand() % 100;
    v[i].priority = p;
    sprintf (v[i].label, "word %d", p);

    pq_insert (pq, v[i]);
  }

  /* show all stored values in order */
  printf ("\nGet priorities:\n");
  while (!pq_empty (pq)) {
    w = pq_del_max (pq);
    printf ("%s\n", w.label);
  }

  /* free the memory */
  pq_free (pq);

  return 0;
}
