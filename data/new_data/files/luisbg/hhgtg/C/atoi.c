#include <stdio.h>
#include <ctype.h>

/* atoi: convert s to integer */
int
a_to_i (char s[])
{
  int i, n, sign;
  for (i = 0; isspace (s[i]); i++)      /* skip white spaces */
    ;

  sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-')       /* skip sign */
    i++;

  for (n = 0; isdigit (s[i]); i++)
    n = 10 * n + (s[i] - '0');
  return sign * n;
}

int
main ()
{
  char *number = "  +348";

  printf ("%d\n", a_to_i (number));

  return 0;
}
