/* B-Tree (work in progress) */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define M 5
#define Key int

typedef struct {
  int key;
  char txt[32];
} Item;

typedef struct st_node * link;

typedef struct {
  Key key;
  union {
    link next;
    Item item;
  } ref;
} entry;

struct st_node {
  entry b[M];
  int m;
};


void insert (Item item);


static link head;
static int H, N;


static int key (Item item) {
  return item.key;
}

static int less (int a, int b) {
  return a < b;
}

static int eq (int a, int b) {
  return a == b;
}

static link NEW()
{
  link x = malloc (sizeof *x);
  x->m = 0;
  return x;
}

static link split (link h) {
  int j;
  link t = NEW();

  for (j = 0; j < M/2; j++)
    t->b[j] = h->b[M/2+j];

  h->m = M/2;
  t->m = M/2;

  return t;
}

void st_init (int maxN) {
  head = NEW();
  H = 0;
  N = 0;
}

static link insert_r (link h, Item item, int H)
{
  int i, j;
  Key v = key(item);
  entry x;
  link t, u;

  x.key = v;
  x.ref.item = item;

  if (H == 0)
    for (j = 0; j < h->m; j++)
      if (less (v, h->b[j].key))
        break;

  if (H != 0)
    for (j = 0; j < h->m; j++)
      if ((j+1 == h->m) || less (v, h->b[j+1].key)) {
        t = h->b[j++].ref.next;
        u = insert_r (t, item, H-1);
        if (u == NULL)
          return NULL;

        x.key = u->b[0].key;
        x.ref.next = u;
        break;
      }

  for (i = (h->m)++; i > j; i--)
    h->b[i] = h->b[i-1];
  h->b[j] = x;

  if (h->m < M)
    return NULL;
  else
    return split(h);
}

void insert (Item item)
{
  link t;
  link u = insert_r (head, item, H);

  if (u == NULL)
    return;

  t = NEW();
  t->m = 2;
  t->b[0].key = head->b[0].key;
  t->b[0].ref.next = head;
  t->b[1].key = u->b[0].key;
  t->b[1].ref.next = u;
  head = t;
  H++;
}

Item search_r (link h, Key v, int H)
{
  int j;
  if (H == 0)
    for (j = 0; j < h->m; j++)
      if (eq (v, h->b[j].key))
        return h->b[j].ref.item;

  if (H != 0)
    for (j = 0; j < h->m; j++)
      if ((j+1 == h->m) || less (v, h->b[j+1].key))
        return search_r (h->b[j].ref.next, v, H - 1);

  printf ("Item %d not found\n", (int) v);

  Item null_item;
  null_item.key = 0;
  strcpy (null_item.txt, "");
  return null_item;
}

Item search (Key v)
{
  return search_r (head, v, H);
}


int main ()
{
  int i;
  Item itms[30];
  Item tmp_item;

  st_init (5);

  for (i = 0; i < 30; i++) {
    itms[i].key = i;
    sprintf (itms[i].txt, "this is item %d", i);

    insert (itms[i]);
  }

  for (i = 3; i < 30; i += 3) {
    tmp_item = search (i);
    printf ("%s\n", tmp_item.txt);
  }

  return 0;
}
