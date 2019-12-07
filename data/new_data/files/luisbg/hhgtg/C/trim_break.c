#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* trim: remove trailing blanks, tabs, newlines */
int
trim (char s[])
{
  int n;
  for (n = strlen (s) - 1; n >= 0; n--)
    if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n')
      break;

  s[n + 1] = '\0';
  return n;
}

int
main ()
{
  // char s[11] = "test\n \n ";
  char *s = (char *) malloc (42 * sizeof (char));

  printf ("Enter a text to trim (max 42 char long): ");
  scanf ("%42s", s);

  trim (s);
  printf ("Trimmed: %s\n", s);

  return 0;
}
