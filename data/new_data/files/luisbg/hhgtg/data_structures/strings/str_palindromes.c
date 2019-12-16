/* Given a string, check if it is a palindrome */
/* examples: "radar", "madam", "racecar", "was it a car or a cat I saw" */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
is_palindrome (char *str)
{
  int a, b;

  for (a = 0, b = strlen (str) - 1; a < b; a++, b--) {
    if (str[a] != str[b])
      return 0;
  }

  return 1;
}

int
is_palindrome_ignore_spaces (char *str)
{
  int a, b;

  for (a = 0, b = strlen (str) - 1; a < b; a++, b--) {
    while (str[a] == ' ')
      a++;
    while (str[b] == ' ')
      b--;

    if (str[a] != str[b]) {
      return 0;
    }
  }

  return 1;
}

int
main ()
{
  printf ("is '%s' an palindrome? %s\n", "radar",
      is_palindrome ("radar") ? "yes" : "no");
  printf ("is '%s' an palindrome? %s\n", "racecar",
      is_palindrome ("racecar") ? "yes" : "no");
  printf ("is '%s' an palindrome? %s\n", "notreally",
      is_palindrome ("notreally") ? "yes" : "no");
  printf ("is '%s' an palindrome? %s\n", "step on no pets",
      is_palindrome ("step on no pets") ? "yes" : "no");
  printf ("is '%s' an palindrome? %s\n", "no",
      is_palindrome ("no") ? "yes" : "no");
  printf ("is '%s' an palindrome? %s\n\n", "",
      is_palindrome ("") ? "yes" : "no");


  printf ("is '%s' an palindrome? %s\n", "was it a car or a cat i saw",
      is_palindrome_ignore_spaces ("was it a car or a cat i saw") ? "yes" :
      "no");
  printf ("is '%s' an palindrome? %s\n", "was it a car or a dog i saw",
      is_palindrome_ignore_spaces ("was it a a car or a dog i saw") ? "yes" :
      "no");

  return 0;
}
