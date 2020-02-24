#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


bool
non_repeating_characters (char *s, int start, int end)
{
  bool *set = (bool *) calloc (256, sizeof (bool));
  for (int c = start; c <= end; c++) {
    if (set[(int) s[c]]) {
      free (set);
      return false;
    }

    set[(int) s[c]] = true;
  }

  free (set);
  return true;
}

int
length_of_longest_substr (char *s)
{
  for (int try_len = strlen (s); try_len > 0; try_len--) {
    int beg = 0;
    while (beg + try_len <= strlen (s)) {
      if (non_repeating_characters (s, beg, beg + try_len - 1))
        return try_len;

      beg++;
    }
  }

  return 0;
}

int
main ()
{
  char *str_a = "abracadabra";
  char *str_b = "peter";
  char *str_c = "ababababababababa";

  printf ("longest non-repeat substring of %s is %d char long\n", str_a,
      length_of_longest_substr (str_a));
  printf ("longest non-repeat substring of %s is %d char long\n", str_b,
      length_of_longest_substr (str_b));
  printf ("longest non-repeat substring of %s is %d char long\n", str_c,
      length_of_longest_substr (str_c));

  return 0;
}
