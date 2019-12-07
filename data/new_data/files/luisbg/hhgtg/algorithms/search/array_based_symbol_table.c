#include <stdlib.h>
#include <stdio.h>


#define Key int
#define maxKey 80
#define NULLItem -1

typedef struct Item {
  Key key;
  int value;
} Item;


static Item *st;
static int N;

static int key (Item item) { return item.key; }
static int less (Key A, Key B) { return A < B; }
static int eq (Key A, Key B) { return A == B; }


/* initialize array of items */
void st_init (int maxN)
{
  st = malloc ((maxN) * sizeof (Item));
  N = 0;
}

/* get the number of items stored */
int st_count ()
{
  return N;
}

/* insert item */
void st_insert (Item item)
{
  int j = N++;
  Key v = key (item);

  while (j > 0 && less (v, key(st[j - 1]))) {
    st[j] = st[j - 1];
    j--;
  }

  st[j] = item;
}

/* search for item with key */
Item st_search (Key v)
{
  int j;
  for (j = 0; j < N; j++) {
    if (eq (v, key (st[j])))
      return st[j];
    if (less (v, key (st[j])))
      break;
  }

  Item null_item;
  null_item.key = -1;
  return null_item;
}

/* select with index */
Item st_select (int k)
{
  return st[k];
}

/* sort all items */
void st_sort (void (*visit)(Item))
{
  int i;
  for (i = 0; i < N; i++)
    visit (st[i]);
}


int main ()
{
  int i;
  Item sample[50];
  Item tmp;

  st_init (maxKey);

  for (i = 0; i < 50; i++) {
    sample[i].key = rand () % maxKey;
    sample[i].value = sample[i].key * 4;

    st_insert (sample[i]);
  }

  printf ("Have %d items\n", st_count ());

  for (i = 0; i < 10; i++) {
    tmp = st_search (rand () % maxKey);

    if (tmp.key != -1)
      printf ("Found item %d, with value %d\n", tmp.key, tmp.value);
  }

  return 0;
}
