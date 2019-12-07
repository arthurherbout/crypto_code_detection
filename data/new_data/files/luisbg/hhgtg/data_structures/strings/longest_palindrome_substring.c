#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


bool
is_palindrome (char *str, int start, int end)
{
  int a, b;

  for (a = start, b = end; a < b; a++, b--) {
    if (str[a] != str[b])
      return false;
  }

  return true;
}

char *
longest_palindrome_try_all (char *s)
{
  int len = strlen (s);

  // try all substring sizes starting from big
  for (int c = len; c >= 0; c--) {
    for (int sub = 0; sub <= (len - c); sub++) {
      if (is_palindrome (s, sub, sub + c - 1)) {
        char *out = (char *) calloc (c + 1, sizeof (char));
        int m = 0;
        for (int n = sub; n < sub + c; n++)
          out[m++] = s[n];

        return out;
      }
    }
  }

  return NULL;
}

int
expand_around_center (char *s, int left, int right)
{
  int l = left;
  int r = right;
  while (l >= 0 && r < strlen (s) && s[l] == s[r]) {
    r++;
    l--;
  }

  // return length of biggest palindrome with left and right at the center
  return r - l - 1;
}

char *
longest_palindrome_expanding (char *s)
{
  int max_sub = 0;
  int max_start;
  int tmp_sub;

  if (strlen (s) == 0)
    return "";

  for (int c = 0; c < strlen (s); c++) {
    tmp_sub = expand_around_center (s, c, c);
    if (tmp_sub > max_sub) {
      max_sub = tmp_sub;
      max_start = c - (max_sub / 2);
    }
  }

  for (int c = 0; c < strlen (s) - 1; c++) {
    tmp_sub = expand_around_center (s, c, c + 1);
    if (tmp_sub > max_sub) {
      max_sub = tmp_sub;
      max_start = c - (max_sub / 2) + 1;
    }
  }

  char *out = (char *) calloc (max_sub + 1, sizeof (char));
  for (int c = 0; c < max_sub; c++)
    out[c] = s[max_start++];

  return out;
}


int
main ()
{
  char *a = "racecar";
  char *b = "not_a_palindrome";
  char *c = "aaaaaaaaaaaaaaaaaaa";
  char *d = "";
  char *tmp;

  tmp = longest_palindrome_try_all (a);
  printf ("longest palindrome in '%s' is: '%s'\n", a, tmp);

  free (tmp);
  tmp = longest_palindrome_expanding (b);
  printf ("longest palindrome in '%s' is: '%s'\n", b, tmp);

  free (tmp);
  tmp = longest_palindrome_try_all (c);
  printf ("longest palindrome in '%s' is: '%s'\n", c, tmp);

  free (tmp);
  tmp = longest_palindrome_expanding (d);
  printf ("longest palindrome in '%s' is: '%s'\n", d, tmp);

  return 0;
}
