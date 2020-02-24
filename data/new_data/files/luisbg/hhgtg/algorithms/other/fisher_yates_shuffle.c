#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static void swap (int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void shuffle (int size, int set[size]) {
  int i;

  for (i = 0; i < (size - 1); i++) {
    swap (&set[i], &set[rand () % size]);
  }
}

/* simultaneously initialize and shuffle the set */
void inside_out (int size, int set[size]) {
  int i, j;

  for (i = 0; i < size; i++) {
    j = rand() % (i + 1);
    if (j != i)
      swap (&set[i], &set[j]);

    /* value passed to set[j] could come from an external source.next */
    set[j] = i;
  }
}

int main () {
  int size = 16;
  int set[16];
  int io[16];
  int i;

  /* use time as seed for pseud-randomness */
  srand(time(NULL));

  /* new sorted list, only set and not io */
  for (i = 0; i < size; i++)
    set[i] = i + 1;

  printf ("sorted set: \t\t\t");
  for (i = 0; i < size; i++)
    printf (" %2d", set[i]);
  printf ("\n");

  printf ("fisher yates shuffled set: \t");

  shuffle (size, set);
  for (i = 0; i < size; i++)
    printf (" %2d", set[i]);
  printf ("\n");

  printf ("inside out shuffled set: \t");

  inside_out (size, io);
  for (i = 0; i < size; i++)
    printf (" %2d", io[i]);
  printf ("\n");

  return 0;
}

