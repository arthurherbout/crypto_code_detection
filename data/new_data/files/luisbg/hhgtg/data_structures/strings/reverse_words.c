/* Write a function that reverses the order of the words in a string. For
 * example, your function should transform the string “Do or do not, there is
 * no try.” to “try. no is there not, do or Do”. Assume that all words are space
 * delimited and treat punctuation the same as letters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_order_words (char *phrase);
static void reverse_string (char *phrase, int start, int end);


void
reverse_order_words (char *phrase)
{
  reverse_string (phrase, 0, strlen (phrase) - 1);
  for (int i = 0, j = 0; i < strlen (phrase); i = j + 1) {
    for (j = i; j < strlen (phrase) && phrase[j] != ' '; j++);
    reverse_string (phrase, i, j - 1);
  }
}

static void
reverse_string (char *phrase, int start, int end)
{
  char tmp;
  while (end > start) {
    tmp = phrase[start];
    phrase[start] = phrase[end];
    phrase[end] = tmp;

    start++;
    end--;
  }
}


int
main ()
{
  char *phrase = (char *) malloc (30 * sizeof (char));
  char *tmp = "Do or do not, there is no try.";
  strcpy (phrase, tmp);

  printf ("original: %s\n", phrase);
  reverse_order_words (phrase);
  printf ("reversed: %s\n", phrase);

  return 0;
}
