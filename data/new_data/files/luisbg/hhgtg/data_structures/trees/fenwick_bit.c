/* http://en.wikipedia.org/wiki/Fenwick_tree */

#include <stdio.h>
#include <stdlib.h>

int orig[10];
int bit[10];


/* update the value at index */
void
update (int index, int value, int len)
{
  while (index < len) {
    bit[index] += value;
    index |= (index + 1);
  }
}

/* get the value at index */
int
query (int index)
{
  int res = 0;
  while (index >= 0) {
    res += bit[index];
    index = (index & (index + 1)) - 1;
  }
  return res;
}

/* print the array of values */
void
print_array (int *arr, int len)
{
  int c;

  for (c = 0; c < len; c++) {
    printf ("%2d ", arr[c]);
  }
  printf ("\n");
}

/* calculate the summation of frequencies */
void
count_frequency (int *orig, int *freq, int len)
{
  int c;

  for (c = 0; c < len; c++) {
    freq[c] = 0;
  }

  for (c = 0; c < len; c++) {
    freq[orig[c]]++;
  }
}


int
main ()
{
  int c;

  printf ("original: \t\t");
  for (c = 0; c < 10; c++) {
    orig[c] = rand () % 10;
  }
  print_array (orig, 10);

  printf ("binary indexed tree: \t");
  for (c = 0; c < 10; c++) {
    update (c, orig[c], 10);
  }
  print_array (bit, 10);

  printf ("cummulative sum: \t");
  for (c = 0; c < 10; c++) {
    printf ("%2d ", query (c));
  }
  printf ("\n");

  printf ("read: \t\t\t");
  for (c = 0; c < 10; c++) {
    printf ("%2d ", query (c) - query (c - 1));
  }
  printf ("\n");

  return 0;
}
