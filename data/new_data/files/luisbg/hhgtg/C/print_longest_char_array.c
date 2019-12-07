#include <stdio.h>
#define MAXLINE 1000

/* maximum input line length */
int get_line (char line[], int maxline);
void copy (char to[], char from[]);


/* print the longest input */
int
main ()
{
  int len;                      /* currnet line length */
  int max;                      /* maximum length seen so far */

  char line[MAXLINE];           /* current line input */
  char longest[MAXLINE];        /* longest line saved here */
  max = 0;
  while ((len = get_line (line, MAXLINE)) > 0)
    if (len > max) {
      max = len;
      copy (longest, line);
    }
  if (max > 0)                  /* there was a line */
    printf ("%s", longest);

  return 0;
}

/* get_line: read a line into s, return length */
int
get_line (char s[], int lim)
{
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar ()) != EOF && c != '\n'; ++i)
    s[i] = c;
  if (c == '\n') {
    s[i] = c;
    ++i;
  }
  s[i] = '\0';
  return i;
}

/* copy: copy 'from' into 'to'; assume to is big enough */
void
copy (char to[], char from[])
{
  int i;
  i = 0;
  while ((to[i] = from[i]) != '\0')
    ++i;
}
