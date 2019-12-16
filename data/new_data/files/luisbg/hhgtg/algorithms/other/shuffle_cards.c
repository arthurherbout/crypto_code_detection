#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static int rand_number (int start, int end) {
  return (rand () % (end - start)) + start;
}

static void swap_cards (int size, int deck[size], int a, int b) {
  int tmp = deck[a];
  deck[a] = deck[b];
  deck[b] = tmp;
}

/* shuffle cards based on Sattolo's suffling algorithm */
void shuffle_cards (int size, int deck[size]) {
  int i, r;

  for (i = 1; i < size; i++) {
    r = rand_number (0, i);
    swap_cards (size, deck, i, r);
  }
}


int main () {
  int size = 20;
  int deck[20];
  int i;

  /* use time as seed for pseud-randomness */
  srand(time(NULL));

  /* new sorted deck */
  for (i = 0; i < size; i++)
    deck[i] = i + 1;

  printf ("sorted deck:   ");
  for (i = 0; i < size; i++)
    printf (" %2d", deck[i]);
  printf ("\n");

  shuffle_cards (size, deck);

  printf ("shuffled deck: ");
  for (i = 0; i < size; i++)
    printf (" %2d", deck[i]);
  printf ("\n");

  return 0;
}

