#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct hash_table
{                               /* has collision by wrapping around to */
  int *key;                  /* next free row                       */
  int *content;
  bool *used;
  int elements;
  int size;
} hash_table;


/* very simple hash to predict collisions */
int
hash (int key)
{
  return key % 32;
}

/* initialize hash table */
hash_table *
init_hash_table (int size)
{
  int i;

  hash_table *t = (hash_table *) malloc (sizeof (hash_table));
  t->key = (int *) malloc (size * sizeof (int));
  t->content = (int *) malloc (size * sizeof (int));
  t->used = (bool *) malloc (size * sizeof (bool));
  t->elements = 0;
  t->size = size;

  for (i = 0; i < size; i++) {
    t->key[i] = -1;
    t->content[i] = -1;
    t->used[i] = false;
  }

  return t;
}

/* insert (key, content) into the hash table */
void
insert (hash_table * t, int key, int cont)
{
  printf ("insert key: %d; content: %d\n", key, cont);

  if (t->elements == t->size) {
    printf ("hash table full\n");
    return;
  }

  int p = hash (key);
  while (t->used[p]) {
    p++;
    if (p == 32)
      p = 0;
  }

  t->key[p] = key;
  t->content[p] = cont;
  t->used[p] = true;
  t->elements++;
}

/* get the content matching the given key */
int
get (hash_table * t, int key)
{
  int p = hash (key);
  while (t->key[p] != key && t->key[p] != -1) {
    p++;
    if (p == 32)
      p = 0;
  }

  if (t->key[p] == -1)
    return -1;

  return t->content[p];
}


int
main ()
{
  int i;
  hash_table *t = init_hash_table (32);

  /* no collisions block */
  insert (t, 1, 100);
  insert (t, 2, 200);
  insert (t, 4, 400);
  insert (t, 8, 800);
  insert (t, 16, 1600);
  printf ("\n");

  /* collisions block */
  insert (t, 32, 3200);
  insert (t, 64, 6400);
  insert (t, 128, 12800);
  printf ("\n");

  /* checks */
  printf ("key: %d; content: %d\n", 2, get (t, 2));
  printf ("key: %d; content: %d\n", 8, get (t, 4));
  printf ("key: %d; content: %d\n", 16, get (t, 16));
  printf ("key: %d; content: %d\n", 32, get (t, 32));
  printf ("key: %d; content: %d\n", 64, get (t, 64));
  printf ("key: %d; content: %d\n", 128, get (t, 128));
  printf ("key: %d; content: %d\n", 33, get (t, 33));
  printf ("\n \n");

  printf ("number of stored elements: %d\n \n", t->elements);

  /* show entire hash table contents */
  for (i = 0; i < 32; i++) {
    printf ("row: %d; key: %d; content: %d\n", i, t->key[i], t->content[i]);
  }

  return 0;
}
