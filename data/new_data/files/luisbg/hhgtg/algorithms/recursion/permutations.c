/* Implement a routine that prints all possible orderings of the characters in
 * a string. In other words, print all permutations that use all the characters
 * from the original string. For example, given the string “hat”, your function
 * should print the strings “tha”, “aht”, “tah”, “ath”, “hta”, and “hat”. Treat
 * each character in the input string as a distinct character, even if it is
 * repeated. Given the string “aaa”, your routine should print “aaa” six times.
 * You may print the permutations in any order you choose.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void
print (char *v, int size)
{
  int c;
  if (v) {
    for (c = 0; c < size; c++) {
      printf ("%c", v[c]);
    }
    printf ("\n");
  }
}

void
swap (char *i, char *j)
{
  char t;
  t = *i;
  *i = *j;
  *j = t;
}

// Based on Heap's Algorithm from
// Introduction to the Design and Analysis of Algorithms by Levitin
void
heap_permute (char **str, int n, int size)
{
  int i;
  if (n == 1)
    print (*str, size);
  else {
    for (i = 0; i < n; i++) {
      heap_permute (str, n - 1, size);
      if (n % 2 == 1)           // if n is odd
        swap (&(*str)[0], &(*str)[n - 1]);
      else                      // if n is even
        swap (&(*str)[i], &(*str)[n - 1]);
    }
  }
}

// Recursive solution
void do_permute (char * orig, char * out, int size, bool used[size], int level)
{
  // if past last level, add null character and print
  if (level == size) {
    out[size] = '\0';
    printf ("%s\n", out);
  }

  int i;

  // loop over yet unused characters
  for (i = 0; i < size; i++) {
    if (used[i])
      continue;

    // add unused character in current level and recurse one level up
    out[level] = orig[i];
    used[i] = true;
    do_permute (orig, out, size, used, level + 1);

    // set to unused for the next iteration
    used[i] = false;
  }
}

/* start the permutation. wrapper to add initializing parameters */
void permute (char * str, int size)
{
  bool used[size];
  char out[size];

  int i;
  for (i = 0; i < size; i++)
    used[i] = false;

  do_permute (str, out, size, used, 0);
}

/* iterative permutation */
void
permute_iterative_size_3 (char *str)
{
  int size = 3;
  char tmp[3];
  int m, n, o;

  for (m = 0; m < size; m++)
    for (n = 0; n < size; n++)
      if (n != m)
        for (o = 0; o < size; o++)
          if (o != n && o != m) {
            tmp[0] = str[m];
            tmp[1] = str[n];
            tmp[2] = str[o];

            printf ("%s\n", tmp);
          }
}


int
main ()
{
  int size = 4;
  char *str = (char *) malloc (size * sizeof (char));
  char *tmp = "hat";
  int c;
  for (c = 0; c < size; c++)
    str[c] = tmp[c];

  printf ("heap permute:\n \n");
  heap_permute (&str, size, size);

  printf (" \n");

  size = 4;
  tmp = "four";
  for (c = 0; c < size; c++)
    str[c] = tmp[c];

  heap_permute (&str, size, size);

  printf ("\ndirect:\n");

  size = 4;
  tmp = "0123";
  for (c = 0; c < size; c++)
    str[c] = tmp[c];

  permute (str, size);

  printf ("\niterative:\n");
  permute_iterative_size_3 ("abc");

  return 0;
}
