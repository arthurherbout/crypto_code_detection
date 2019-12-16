/* Algorithm to determine if a string has all unique characters.              */
/* What if you cannot use additional data structures?                         */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ASCII_SIZE 256

/* O(n^2) but no need for extra memory */
bool
all_unique_characters (char *str)
{
  if (!str)
    return true;

  int c;
  int n;
  for (c = 0; str[c] != '\0'; c++) {
    for (n = c + 1; str[n] != '\0'; n++) {
      if (str[c] == str[n]) {
        return false;
      }
    }
  }

  return true;
}

/* O(n) and saving a 256 byte array */
bool
all_unique_characters_using_memory (char *str)
{
  int c;
  int *seen = (int *) calloc (ASCII_SIZE, sizeof (int));

  for (c = 0; str[c] != '\0'; c++) {
    if(seen[(int) str[c]])
      return false;
    seen[(int) str[c]] = 1;
  }

  return true;
}

int
main ()
{
  char *test = "";
  printf ("%s :: unique characters: %s\n", test,
      all_unique_characters (test) ? "yes" : "no");
  test = "hello";
  printf ("%s :: unique characters: %s\n", test,
      all_unique_characters (test) ? "yes" : "no");
  test = "world";
  printf ("%s :: unique characters: %s\n", test,
      all_unique_characters (test) ? "yes" : "no");

  test = "strings";
  printf ("%s :: unique characters: %s\n", test,
      all_unique_characters_using_memory (test) ? "yes" : "no");
  test = "chars";
  printf ("%s :: unique characters: %s\n", test,
      all_unique_characters_using_memory (test) ? "yes" : "no");
}
