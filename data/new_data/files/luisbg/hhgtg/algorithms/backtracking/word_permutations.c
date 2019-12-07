/* Implement a routine that prints all allowed orderings of the characters in
 * a string, without two consecutive vowels.
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

bool
is_vowel (char c)
{
  if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
    return true;

  return false;
}

bool
check_for_rule (int size, char *str)
{
  if (size < 2)
    return true;

  for (int i = 0; i < size - 1; i++) {
    if (is_vowel (str[i]) && is_vowel (str[i + 1])) {
      return false;
    }
  }

  return true;
}

// Recursive check for all permutations
void
do_permute (char *orig, char *out, int size, bool used[size], int level)
{
  // If past last level, it is a valid answer! Add null character and print
  if (level == size) {
    out[size] = '\0';
    printf ("%s\n", out);
  }
  // Loop over yet unused characters
  for (int i = 0; i < size; i++) {
    if (used[i])
      continue;

    // Add unused character in current level and if admissible so far,
    // recurse one level up
    out[level] = orig[i];
    used[i] = true;
    if (check_for_rule (level + 1, out)) {
      do_permute (orig, out, size, used, level + 1);
    }
    // Set to unused for the next iteration
    used[i] = false;
  }
}

// Start the permutation. Wrapper to add initializing parameters
void
permute (char *str, int size)
{
  bool used[size];
  char out[size];

  int i;
  for (i = 0; i < size; i++)
    used[i] = false;

  do_permute (str, out, size, used, 0);
}

int
main ()
{
  int size = 5;
  char *tmp = "outer";

  permute (tmp, size);

  return 0;
}
