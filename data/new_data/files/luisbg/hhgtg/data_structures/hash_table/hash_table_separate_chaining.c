#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct linked_list
{
  char *key;
  char *word;
  struct linked_list *next;
} linked_list_t;

typedef struct hash_table
{
  int size;                     /* has collision by separate chaining */
  int elements;                 /* with head records in the bucket array */
  linked_list_t *buckets;
} hash_table_t;


/* simple hash to predict collisions */
int
hash (char *key)
{
  return key[0] % 16;
}

/* initialize hash table */
hash_table_t *
init_hash_table (int size)
{
  int i;
  hash_table_t *t = (hash_table_t *) malloc (sizeof (hash_table_t));

  t->size = size;
  t->elements = 0;
  t->buckets = (linked_list_t *) malloc (size * sizeof (linked_list_t));

  for (i = 0; i < size; i++) {
    t->buckets[i].key = (char *) malloc (16 * sizeof (char));
    strcpy(t->buckets[i].key, "");
    t->buckets[i].word = (char *) malloc (32 * sizeof (char));
  }

  return t;
}

/* insert (key, word) into the linked list */
void
insert_list (linked_list_t *l, char key[], char word[])
{
  printf ("insert %s->%s\n", key, word);

  linked_list_t * run;

  // if first element in list, insert in head
  if (strcmp (l->key, "") == 0) {
    strcpy (l->key, key);
    strcpy (l->word, word);
    l->next = NULL;
  } else {
    run = l;
    // if not first, create a new node and link from last
    while (run->next) {
      run = run->next;
    }

    linked_list_t *new_node;
    new_node = malloc (sizeof (linked_list_t));

    new_node->key = (char *) malloc (16 * sizeof (char));
    new_node->word = (char *) malloc (32 * sizeof (char));

    strcpy (new_node->key, key);
    strcpy (new_node->word, word);
    new_node->next = NULL;

    run->next = new_node;
  }
}

/* get word matching the key in the linked list */
char *
get_list (linked_list_t *l, char *key)
{
  linked_list_t *run;

  if (strcmp (l->key, key) == 0) {
    return l->word;
  } else {
    run = l->next;
    while (run) {
      if (strcmp (run->key, key) == 0) {
        return run->word;
      }

      run = run->next;
    }
  }

  return "";
}

/* insert (key, word) into the hash table */
void
insert_hash_table (hash_table_t * t, char key[], char word[])
{
  int h = hash (key);
  insert_list (&(t->buckets[h]), key, word);
}

/* the the word matching the key from the table */
char *
get_from_table (hash_table_t * t, char key[])
{
  int h = hash (key);
  return get_list (&(t->buckets[h]), key);
}


int
main ()
{
  hash_table_t *table = init_hash_table (32);

  printf ("created table\n");

  insert_hash_table (table, "first", "work it");
  insert_hash_table (table, "second", "make it");
  /* skip third for testing getting stored keys */
  insert_hash_table (table, "fourth", "make us");
  insert_hash_table (table, "fifth", "harder");
  insert_hash_table (table, "sixth", "better");
  insert_hash_table (table, "seventh", "faster");
  insert_hash_table (table, "eighth", "stronger");
  insert_hash_table (table, "ninth", "more than");
  insert_hash_table (table, "tenth", "hour");

  printf (" \n");

  char *k = "first";
  printf ("the key: %s -> has word: %s \n", k, get_from_table (table, k));
  k = "second";
  printf ("the key: %s -> has word: %s \n", k, get_from_table (table, k));
  k = "third";
  if (strcmp (get_from_table (table, k), ""))
    printf ("the key: %s -> isn't stored\n", k);
  k = "sixth";                  /* sixth collides with second */
  printf ("the key: %s -> has word: %s \n", k, get_from_table (table, k));
  k = "seventh";                /* seventh collides with second and sixth */
  printf ("the key: %s -> has word: %s \n \n", k, get_from_table (table, k));

  int c;
  for (c = 0; c < 32; c++) {
    if (strcmp (table->buckets[c].key, "") != 0) {
      printf ("table head records %d: %s->%s\n", c, table->buckets[c].key,
          table->buckets[c].word);
    }
  }

  return 0;
}
