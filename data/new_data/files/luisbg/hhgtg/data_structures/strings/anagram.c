/* Given two strings, decide if one is an anagram of the other. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ASCII_SIZE 256

static int count;


void
string_sort (char *s, int length)
{
  char tmp;
  int i, j;

  for (i = 0; i < length - 1; i++) {
    for (j = i + 1; j < length; j++) {
      if (s[i] > s[j]) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
      }
    }
  }
}

bool
anagram_by_sort (char *first, char *second)
{
  int c;
  for (c = 0; first[c]; c++) {
  }
  int len = c;

  for (c = 0; second[c]; c++) {
  }
  int len_second = c;

  if (len != len_second)
    return false;

  char *sorted_first = (char *) malloc (len * sizeof (char));
  strcpy (sorted_first, first);
  char *sorted_second = (char *) malloc (len * sizeof (char));
  strcpy (sorted_second, second);

  string_sort (sorted_first, len);
  string_sort (sorted_second, len);

  for (c = 0; c < len; c++) {
    if (sorted_first[c] != sorted_second[c])
      return false;
  }

  return true;
}

bool
anagram_by_count (char *first, char *second)
{
  int i;
  int *count = (int *) calloc (ASCII_SIZE, sizeof (int));
  int len = strlen (first);

  if (len != strlen (second))
    return false;

  for (i = 0; i < len; i++) {
    count[(int) first[i]]++;
    count[(int) second[i]]--;
  }

  for (i = 0; i < len; i++) {
    if (count[(int) first[i]])
      return false;
  }

  free (count);

  return true;
}

bool
anagram (char *first, char *second)
{
  count++;

  if (count % 2 == 0)
    return anagram_by_sort (first, second);
  else
    return anagram_by_count (first, second);
}

int
main ()
{
  count = 0;

  char *a = "dad ";
  char *b = " add";
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  a = "troll";
  b = "hunter";
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  a = "same length ";
  b = "not the same";
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  a = "radar";
  b = "radar";
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
  printf ("%s:%s :: anagram? %s\n", a, b, anagram (a,
          b) ? "yes" : "no");
}
