/* Write an efficient function that deletes characters from a string.
 * Use the prototype
 * 
 * string removeChars( string str, string remove );
 * 
 * where any character existing in remove must be deleted from str. For
 * example, given a str of “Battle of the Vowels: Hawaii vs. Grozny” and a
 * remove of “aeiou”, the function should transform str to “Bttl f th Vwls:
 * Hw vs. Grzny”. Justify any design decisions you make and discuss the
 * efficiency of your solution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void remove_chars (char **str, char *remove);
bool set_char_map (char *remove, bool ** map);


void
remove_chars (char **str, char *remove)
{
  int c, pos;
  bool *map;
  set_char_map (remove, &map);

  pos = 0;
  for (c = 0; (*str)[c] != '\0'; c++) {
    if (!map[(int) (*str)[c]]) {
      if (pos != c) {
        (*str)[pos] = (*str)[c];
      }
      pos++;
    }
  }
  (*str)[pos] = '\0';

  return;
}

/* Would be better to use a Hash Table to save memory. Specially so unicode
 * can be supported. */
bool
set_char_map (char *remove, bool ** map)
{
  bool *tmp = (bool *) malloc (256 * sizeof (bool));
  if (!tmp)
    return false;
  int c;

  for (c = 0; c < 256; c++) {
    tmp[c] = false;
  }

  for (c = 0; remove[c] != '\0'; c++) {
    tmp[(int) remove[c]] = true;
  }

  *map = tmp;

  return true;
}


int
main ()
{
  // original has to be dynamic so it can be changed in place
  char *original = (char *) malloc (60 * sizeof (char));
  char *tmp = "Battle of the Vowels: Hawaii vs. Grozny";
  int c;
  for (c = 0; tmp[c] != '\0'; c++) {
    original[c] = tmp[c];
  }

  char *remove = "aeoiu";

  printf ("original: %s\n", original);
  remove_chars (&original, remove);
  printf ("removed:  %s\n", original);

  return 0;
}
