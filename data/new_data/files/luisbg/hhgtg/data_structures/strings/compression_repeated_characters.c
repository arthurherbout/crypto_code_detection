/* Basic string compression using the counts of repeated characters.          */
/* For example the string aabccccaaa would become a5b1c4.                     */
/* If the 'compressed' string would not become smaller than the original      */
/* string, return the original string.                                        */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ASCII_SIZE 256


char *
compress_string (char *str, int len)
{
  int chars[ASCII_SIZE] = { 0 };

  int c, ascii_code;
  int nss = 0;
  for (c = 0; c < len; c++) {
    ascii_code = str[c];
    if (chars[ascii_code] == 0) {   /* a new character */
      nss += 2;                 /* needs to spaces in the compressed str */
    }

    chars[ascii_code]++;

    if (chars[ascii_code] >= 10)    /* one more if a two digit number */
      nss++;
  }

  if (nss < len) {
    char tmp;
    char *new = (char *) malloc (nss * sizeof (char));

    int pos = 0;
    for (c = 0; c < ASCII_SIZE; c++) {
      if (chars[c] > 0) {
        new[pos++] = c;
        if (chars[c] < 10) {
          tmp = chars[c] + '0';
          new[pos++] = tmp;
        } else {
          tmp = (chars[c] / 10) + '0';
          new[pos++] = tmp;
          tmp = (chars[c] % 10) + '0';
          new[pos++] = tmp;
        }
      }
    }
    new[pos] = '\0';

    return new;
  } else {
    return str;
  }
}

char *
compress_blocks (char *text)
{
  char *temp = (char *) malloc (sizeof (char) * strlen (text));
  int n, r;
  int m = 0;
  int pos = 0;

  while (m < strlen (text)) {
    n = m + 1;
    r = 1;
    for (r = 1; text[m] == text[n]; n++)
      r++;

    temp[pos++] = text[m];
    if (r > 1)
      temp[pos++] = r + '0';

    m = n;
  }

  return temp;
}

int
main ()
{
  char *test = "aabbbcddeeeeeeeeeeee";
  printf ("%s :: compressed :: %s\n", test, compress_string (test,
          strlen (test)));

  printf ("%s :: compressed blocks :: %s\n", test,
      compress_blocks (test));

  test = "abbccdde";
  printf ("%s :: compressed :: %s\n", test, compress_string (test,
          strlen (test)));

  test = "uuuseeerr";
  printf ("%s :: compressed blocks :: %s\n", test,
      compress_blocks (test));

  return 0;
}
