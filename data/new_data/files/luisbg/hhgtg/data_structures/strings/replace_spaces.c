/* Replace all spaces with '%20'.                                             */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *
replace_spaces (char *str)
{
  int c;
  int n = 0;
  for (c = 0; c < strlen (str); c++) {
    if (str[c] == ' ')
      n++;
  }
  n *= 2;                       /* two extra characters per space switch */

  char *out = (char *) malloc ((strlen (str) + n) * sizeof (char));
  int d = 0;
  for (c = 0; c < strlen (str); c++) {
    if (str[c] != ' ') {
      out[d] = str[c];
      d++;
    } else {
      out[d] = '%';
      out[++d] = '2';
      out[++d] = '0';
      ++d;
    }
  }

  return out;
}

int
main ()
{
  char *url = "http://sample.com/foo   bar ish ";
  printf ("%s\n", url);
  printf ("%s\n", replace_spaces (url));
}
