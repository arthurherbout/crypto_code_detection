#include <stdlib.h>
#include <stdio.h>


#define maxKey 80
#define NULLitem -1
#define Key int


typedef struct Item {
  int key;
  int value;
} Item;


static Item *st;
static int M = maxKey;
static int key (Item item);


void st_init (int MaxN)
{
  int i;
  st = malloc ((M + 1) * sizeof (Item));

  for (i = 0; i <= M; i++) {
    st[i].key = -1;           // st[i] = NULLitem;
    st[i].value = -1;
  }
}

int st_count ()
{
  int i, N = 0;
  for (i = 0; i < M; i++)
    if (st[i].key != -1)      // if (st[i] != NULLitem)
      N++;

  return N;
}

void st_insert (Item item)
{
  st[key (item)] = item;
}

Item st_search (Key v)
{
  return st[v];
}

void st_delete (Item item)
{
  st[key (item)].key = -1;        // st[key (item)] = -1;
}

/*
void st_sort (void (*vist)(Item))
{
  int i;
  for (i = 0; i < M; i++)
    if (st[i].key != -1)     // if (st[i] != NULLitem)
      visit (st[i]);
} */

static int key (Item item)
{
  return item.key;
}

int main ()
{
  int i;
  Item sample[50];
  Item tmp;

  st_init (M);

  for (i = 0; i < 50; i++) {
    sample[i].key = rand () % M;
    sample[i].value = sample[i].key * 4;

    st_insert (sample[i]);
  }

  printf ("Have %d items\n", st_count ());

  for (i = 0; i < 10; i++) {
    tmp = st_search (rand () % M);

    if (tmp.key != -1)
      printf ("Found item %d, with value %d\n", tmp.key, tmp.value);
  }

  return 0;
}
