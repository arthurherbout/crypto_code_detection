#include <stdio.h>

/* check check whether a string has a balanced number of left and right () */
/* parentheses. */

int
balanced (char *str)
{
  int c = 0, balance = 0;

  while (str[c] != '\0') {
    if (str[c] == '(')
      balance++;
    if (str[c] == ')')
      balance--;

    if (balance < 0)
      return 0;

    c++;
  }

  return balance == 0;
}


int
main ()
{
  printf ("is '%s' balanced? %s\n", "(a)", balanced ("(a)") ? "yes" : "no");
  printf ("is '%s' balanced? %s\n", "((b))", balanced ("((b))") ? "yes" : "no");
  printf ("is '%s' balanced? %s\n", "(c)(d)(e)",
      balanced ("(c)(d)(e)") ? "yes" : "no");
  printf ("is '%s' balanced? %s\n", "(f))(", balanced ("(f))(") ? "yes" : "no");
  printf ("is '%s' balanced? %s\n", "", balanced ("") ? "yes" : "no");

  return 0;
}
