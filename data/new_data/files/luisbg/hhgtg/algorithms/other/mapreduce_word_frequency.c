/* Copyright © 2014 Boris Faure
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define NB_THREADS 10

/* Task: use 10 threads to do a kind of "map-reduce" to count word frequencies
 * in a given file.
 *
 * Solution:
 * -  mmap the file
 * -  split the file into 10 segments
 *    each threads works on a segment and produces a binary tree containing
 *    the words it has encountered and their frequency. They first start by
 *    finding better boundaries (start on the next word if they start "in the
 *    middle" of a word.
 * -  As a reduce function, merge all the trees together. This is done in the
 *    main thread.
 *
 * Notes/Limitations:
 * -  a Red-Black tree would be more efficient to do look-ups but would had
 *    more complex code. I considered a simple binary tree to be efficient
 *    enough.
 * -  The use of mmap() avoids dealing with the issue of words being cut when
 *    reading the file into a buffer. It also means less allocation as words
 *    are not duplicated but simple pointers to the mmap.
 * -  The reduce part could have been done in separate threads merging
 *    trees two-by-two until there is only one tree.
 * -  isalpha() is used instead of !isspace() not to include ',' or '.' in
 *    words. 'b42' would not be considered a word but 'b' would.
 * -  Composed words like 'foo-bar' are considered two words.
 * -  In order to speed-up comparisons in the tree, each node contains the
 *    hash of the word it contains. A slight variation of the jenkins hash
 *    algorith is used: the addition of tolower() makes the hash
 *    case-insentive and thus 'Lorem' and 'lorem' are considered the same
 *    word. This only works with a locale 'C' or 'POSIX' since the hash
 *    function works on bytes and not codepoints. Without that hashing
 *    function, the code should work with any locale:
 *    isalpha()/strncasecmp() correctly handles the locale.
 * -  The output is not sorted alphabetically but sorted by the hash function.
 * -  Most recursions are written using the call stack. It should be wiser to
 *    write a simple stack to be able to write iterative versions.
 */



typedef struct word_cell_t word_cell_t;

struct word_cell_t
{
  uint32_t hash;
  uint32_t len;
  const char *word_start;
  uint32_t count;

  /* Simple binary tree */
  word_cell_t *left;
  word_cell_t *right;
};

/* Globals */
static struct
{
  const char *mem;
  struct stat st;
} _G;


/* hash the word ignoring case using the jenkins hash function */
/* http://en.wikipedia.org/wiki/Jenkins_hash_function */
uint32_t
jenkins_ignore_case (const char *key, size_t len)
{
  uint32_t hash, i;

  for (hash = i = 0; i < len; ++i) {
    hash += tolower (key[i]);
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

/* create a new tree cell to store the word */
static word_cell_t *
word_cell_new (uint32_t hash, const char *word_start, uint32_t len)
{
  word_cell_t *wc = calloc (1, sizeof (word_cell_t));

  if (!wc) {
    perror ("calloc");
    exit (EXIT_FAILURE);
  }
  wc->hash = hash;
  wc->word_start = word_start;
  wc->len = len;
  wc->count = 1;

  return wc;
}

/* extract the cell from the tree */
static void
word_cell_extract (word_cell_t ** parent)
{
  word_cell_t *cell = *parent;

  if (cell) {
    if (cell->right && cell->left) {
      /* cell has 2 children, replace it by its predecessor: the node on
       * the right-most node on its left */
      word_cell_t *new_node = cell->left;
      word_cell_t **new_parent = NULL;

      while (new_node->right) {
        new_parent = &new_node->right;
        new_node = new_node->right;
      }

      if (new_parent) {
        *new_parent = new_node->left;
        new_node->left = cell->left;
      }
      new_node->right = cell->right;
      *parent = new_node;

      cell->right = NULL;
      cell->left = NULL;

      return;
    }

    if (cell->right) {
      *parent = cell->right;
    } else {
      *parent = cell->left;
    }

    cell->right = NULL;
    cell->left = NULL;
  }
}

/* free the memory allocated to the tree */
static void
tree_delete (word_cell_t * tree)
{
  if (tree) {
    tree_delete (tree->left);
    tree_delete (tree->right);
    free (tree);
  }
}

/* compare the cell with the given word */
static int
word_cell_cmp (word_cell_t * wc, uint32_t hash,
    const char *word_start, uint32_t len)
{
  if (wc->hash == hash) {
    if (wc->len == len) {
      return strncasecmp (wc->word_start, word_start, len);
    } else {
      return wc->len - len;
    }
  } else {
    return wc->hash - hash;
  }
}

/* print information in the tree in order */
static void
tree_print (word_cell_t * tree)
{
  uint32_t i;
  if (tree) {
    tree_print (tree->left);

    for (i = 0; i < tree->len; i++) {
      putchar (tolower (tree->word_start[i]));
    }
    printf (": %d\n", tree->count);

    tree_print (tree->right);
  }
}

/* check if the cell is stored in the tree and return it */
static word_cell_t *
search_tree (word_cell_t ** treep, uint32_t hash, const char *word_start,
    uint32_t len, word_cell_t *** parent)
{
  int cmp;
  word_cell_t *cell = *treep;

  *parent = treep;

  if (!cell)
    return NULL;

  while (42) {
    cmp = word_cell_cmp (cell, hash, word_start, len);

    if (cmp == 0) {
      return cell;
    } else {
      if (cmp < 0) {
        if (cell->left) {
          *parent = &cell->left;
          cell = cell->left;
        } else {
          *parent = &cell->left;
          return NULL;
        }
      } else {
        if (cell->right) {
          *parent = &cell->right;
          cell = cell->right;
        } else {
          *parent = &cell->right;
          return NULL;
        }
      }
    }
  }

  /* NEVER REACHED */
  abort ();
  return NULL;
}

/* insert hash and word in the tree, if it exists increment counter */
static word_cell_t *
insert_tree (word_cell_t * tree, uint32_t hash,
    const char *word_start, uint32_t len)
{
  if (tree) {
    word_cell_t *cell;
    word_cell_t **insert;

    /* search and insert if needed */
    cell = search_tree (&tree, hash, word_start, len, &insert);

    if (cell) {
      cell->count++;
    } else {
      cell = word_cell_new (hash, word_start, len);
      *insert = cell;
    }
  } else {
    tree = word_cell_new (hash, word_start, len);
  }

  return tree;
}

/* merge two trees combining counters of each word */
static void
word_cell_tree_merge (word_cell_t ** tree1, word_cell_t ** parent2)
{
  word_cell_t *cell, *cell2 = *parent2;
  word_cell_t **parent;

  if (!cell2)
    return;

  /* Recursion on tree2 to look for a simbling in tree1 and add it */
  cell = search_tree (tree1, cell2->hash, cell2->word_start, cell2->len,
      &parent);

  if (cell) {
    cell->count += cell2->count;
    word_cell_tree_merge (tree1, &cell2->left);
    word_cell_tree_merge (tree1, &cell2->right);
  } else {
    *parent = cell2;

    word_cell_extract (parent2);

    word_cell_tree_merge (tree1, parent2);
  }
}

/* merge the two trees into the first and free the memory of the second */
static void
reduce_trees (word_cell_t ** tree1, word_cell_t ** tree2)
{
  if (*tree1) {
    word_cell_tree_merge (tree1, tree2);

    tree_delete (*tree2);
    *tree2 = NULL;
  } else {
    *tree1 = *tree2;
    *tree2 = NULL;
  }
}

/* generate a tree with frequency of words in the file split */
static void *
counter_function (void *arg)
{
  int id = (intptr_t) arg;
  off_t split_size = _G.st.st_size / NB_THREADS;
  const char *start;
  const char *end;              /* excluded */
  word_cell_t *tree = NULL;

  if (id == 0) {
    start = _G.mem;
  } else {
    start = _G.mem + id * split_size;
    /* find an improved start */
    while (isalpha (*start)) {
      start++;
    }
  }
  if (id == (NB_THREADS - 1)) {
    end = _G.mem + _G.st.st_size;
  } else {
    end = _G.mem + (id + 1) * split_size;
    /* find a correct end */
    while (isalpha (*end) && end < (_G.mem + _G.st.st_size)) {
      end++;
    }
  }

  /* cycle through the file split getting words */
  while (start < end) {
    const char *word_start;
    const char *word_end;
    uint32_t hash;
    uint32_t len;

    /* skip non alpha to find start of the word*/
    while (!isalpha (*start) && start < end)
      start++;

    if (start >= end)
      break;

    word_start = start;
    word_end = word_start + 1;

    /* find the end of the word */
    while (word_end < end && isalpha (*word_end))
      word_end++;

    len = word_end - word_start;
    hash = jenkins_ignore_case (word_start, len);
    /* insert word in tree, if it already exists it will increase the count */
    tree = insert_tree (tree, hash, word_start, len);

    /* set start to end to cycle to next word */
    start = word_end + 1;
  }

  return tree;
}

/* print the usage information */
static void
usage (void)
{
  fputs ("Usage: word_counter file\n"
      "Count word occurences in given file.\n", stderr);
  exit (EXIT_FAILURE);
}


int
main (int argc, char **argv)
{
  int i, fd = -1, res = EXIT_SUCCESS;
  intptr_t ip;
  pthread_t threads[NB_THREADS];
  word_cell_t *trees[NB_THREADS];

  if (argc <= 1)
    usage ();

  if (stat (argv[1], &_G.st) < 0) {
    fprintf (stderr, "unable to stat '%s': %m\n", argv[1]);
    exit (EXIT_FAILURE);
  }

  if (_G.st.st_size == 0)
    goto end;

  fd = open (argv[1], O_RDONLY);
  if (fd < 0) {
    fprintf (stderr, "unable to open '%s': %m\n", argv[1]);
    exit (EXIT_FAILURE);
  }

  _G.mem = (const char *) mmap (NULL, _G.st.st_size, PROT_READ, MAP_PRIVATE,
      fd, 0);
  if (_G.mem == MAP_FAILED) {
    fprintf (stderr, "unable to mmap '%s': %m\n", argv[1]);
    res = EXIT_FAILURE;
    goto end;
  }
  close (fd);
  fd = -1;

  /* MAP */
  for (ip = 0; ip < NB_THREADS; ip++) {
    trees[ip] = NULL;
    /* create thread to generate word tree for each slice of the file */
    if (pthread_create (&threads[ip], NULL, &counter_function, (void *) ip)) {
      fprintf (stderr, "unable to create thread %zd: %m\n", ip + 1);
      exit (EXIT_FAILURE);
    }
  }
  for (i = 0; i < NB_THREADS; i++) {
    /* join threads to get all the generated word frequency trees */
    if (pthread_join (threads[i], (void **) &trees[i])) {
      fprintf (stderr, "unable to join thread %d: %m\n", i + 1);
      exit (EXIT_FAILURE);
    }
  }

  /* REDUCE */
  for (i = 1; i < NB_THREADS; i++) {
    reduce_trees (&trees[0], &trees[i]);
  }

  /* print result */
  tree_print (trees[0]);

  /* other trees are cleaned up already */
  tree_delete (trees[0]);

end:
  if (_G.mem) {
    munmap ((void *) _G.mem, _G.st.st_size);
  }
  if (fd != -1) {
    close (fd);
  }
  return res;
}
