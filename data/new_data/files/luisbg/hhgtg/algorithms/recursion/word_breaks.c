/* Given a non-empty string s and a dictionary wordDict containing a list of
 * non-empty words, determine if s can be segmented into a space-separated
 * sequence of one or more dictionary words. You may assume the dictionary does
 * not contain duplicate words.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


bool
starts_with (char *str, char *sub)
{
  if (strlen (sub) > strlen (str))
    return false;

  for (int c = 0; c < strlen (sub); c++) {
    if (str[c] != sub[c])
      return false;
  }

  return true;
}

bool
wordBreak_recur (char *s, char **wordDict, int wordDictSize)
{
  for (int w = 0; w < wordDictSize; w++) {
    if (starts_with (s, wordDict[w])) {
      printf ("%s starts with %s\n", s, wordDict[w]);

      char *new_s = (char *) calloc (strlen (s), sizeof (char));
      strcpy (new_s, s + strlen (wordDict[w]));

      if (wordBreak_recur (new_s, wordDict, wordDictSize))
        return true;
    }
  }

  // printf ("s %s\n", s);

  return (strlen (s) == 0);
}

bool
word_break (char *s, char **wordDict, int wordDictSize)
{
  return wordBreak_recur (s, wordDict, wordDictSize);
}

int
main ()
{
  char *word_a = "racecar";
  char *dict_a[8] = { "one", "two", "three", "four", "car", "race", "seven",
    "eight"
  };
  char *word_b = "nothappening";
  char *dict_b[8] = { "enni", "hap", "not", "nothap", "ning", "six", "seven",
    "eight"
  };

  printf ("%s can %sbe made from dictionary:\n",
      word_a, (word_break (word_a, dict_a, 8)) ? "" : "*not* ");
  for (int w = 0; w < 8; w++)
    printf ("%s, ", dict_a[w]);

  printf ("\n\n");
  printf ("%s can %sbe made from dictionary:\n",
      word_b, (word_break (word_b, dict_b, 8)) ? "" : "*not* ");
  for (int w = 0; w < 8; w++)
    printf ("%s, ", dict_b[w]);
  printf ("\n");

  return 0;
}
