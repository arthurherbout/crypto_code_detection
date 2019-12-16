#include <stdlib.h>
#include <stdio.h>

typedef struct trie_node
{
  char key;
  int value;
  struct trie_node *next;
  struct trie_node *prev;
  struct trie_node *children;
  struct trie_node *parent;
} trie_node_t;

trie_node_t *create_node (char key, int data);
void add_trie (trie_node_t ** root, char *key, int data);
trie_node_t *search (trie_node_t * root, const char *key);


/* create a node for the trie */
trie_node_t *
create_node (char key, int data)
{
  trie_node_t *node = (trie_node_t *) malloc (sizeof (trie_node_t));

  if (!node) {
    printf ("ERROR: malloc failed\n");
    return node;
  }

  node->key = key;
  node->value = data;
  node->next = NULL;
  node->prev = NULL;
  node->children = NULL;
  node->parent = NULL;

  return node;
}

/* add a key in the trie */
void
add_trie (trie_node_t ** root, char *key, int data)
{
  trie_node_t *curr;

  printf ("Inserting key: %s\n", key);
  if (!*root) {
    printf ("Empty trie\n");
    return;
  }
  curr = (*root)->children;

  if (!curr) {           /* first node */
    for (curr = *root; *key; curr = curr->children) { // add all nodes
      curr->children = create_node (*key, 0);       // create node for this char

      printf ("Inserting first node: %c\n", *key);

      curr->children->parent = curr;
      key++;                     // loop through string with pointer arithmetic
    }

    curr->children = create_node ('\0', data); // create end node
    curr->children->parent = curr;
    return;
  }

  if (search (curr, key)) {     // check if key is already contained
    printf ("Duplicate!\n");
    return;
  }

  while (*key != '\0') {        /* for strings with the beginning already stored */
    if (*key == curr->key) {
      curr = curr->children;
      key++;
    } else
      break;
  }

  while (curr->next) {          /* find if the beginning is already stored */
    if (*key == curr->next->key) {
      key++;
      add_trie (&curr->next, key, data);
      return;
    }
    curr = curr->next;
  }

  curr->next = create_node (*key, 0);   /* start new branch */
  curr->next->parent = curr->parent;

  printf ("Inserting %c as neighbour of %c \n", curr->next->key, curr->key);

  key++;

  for (curr = curr->next; *key; curr = curr->children) {
    curr->children = create_node (*key, 0);     /* populate children of new */
    curr->children->parent = curr;      /* branch */
    key++;
    printf ("Inserting: %c\n", curr->children->key);
  }

  curr->children = create_node ('\0', data);
  curr->children->parent = curr;

  return;
}

/* search for a key in the trie */
trie_node_t *
search (trie_node_t * root, const char *key)
{
  trie_node_t *level = root->children;
  trie_node_t *curr = NULL;
  trie_node_t *found;

  while (1) {
    found = NULL;

    for (curr = level; curr; curr = curr->next) {
      if (curr->key == *key) {
        found = curr;
        break;
      }
    }

    if (!found)
      return NULL;

    if (*key == '\0') {
      return curr;
    }

    level = found->children;
    key++;
  }
}

int
main ()
{
  trie_node_t *root = NULL;
  trie_node_t *tmp = NULL;

  root = create_node ('\0', 0);
  add_trie (&root, "alice", 10);
  add_trie (&root, "barbara", 11);
  add_trie (&root, "charlie", 12);
  add_trie (&root, "alfred", 13);
  add_trie (&root, "bar", 14);
  add_trie (&root, "batman", 15);

  char *names[4] = { "alice", "alfred", "batman", "negan" };
  int c;
  for (c = 0; c < 4; c++) {
    tmp = search (root, names[c]);
    printf ("is %s there? %s\n", names[c], tmp ? "yes" : "no");
    if (tmp)
      printf ("%s is: %d\n", names[c], tmp->value);
  }

  return 0;
}
