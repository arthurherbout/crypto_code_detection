/* Given a string s and string p, find all the start indices of p's anagrams
 * in s. Both strings contain just lowercase letters.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


bool
compare_letter_counts (int *letters_a, int *letters_b)
{
  // if different amount of any letter, then strings are not anagrams
  for (int c = 0; c < 26; c++) {
    if (letters_a[c] != letters_b[c])
      return false;
  }

  return true;
}

int *
count_letters (char *str, int start, int end)
{
  int *letters = (int *) calloc (26, sizeof (int));

  // count how many times each letter is in the string
  for (int c = start; c <= end; c++) {
    letters[str[c] - 'a']++;
  }

  return letters;
}

int* findAnagrams(char* s, char* p, int* returnSize) {
  int len_s = strlen (s);
  int len_p = strlen (p);
  int *count_p = NULL;
  int *count_s = NULL;
  int *res = NULL;
  *returnSize = 0;

  if (len_s == 0)
     return NULL;

  count_p = count_letters (p, 0, len_p - 1);
  for (int t = 0; t <= (len_s - len_p); t++) {
    if (count_s != NULL)
      free (count_s);
    // count letters in each substring of s that is same size as p
    count_s = count_letters (s, t, t + len_p - 1);

    // compare letter counts of substring of s and p
    if (compare_letter_counts (count_p, count_s)) {
      (*returnSize)++;
      res = realloc (res, *returnSize * sizeof (int));
      res[*returnSize - 1] = t;
    }
  }

  return res;
}

int
main ()
{
  char *s = "anticonstitutional";
  char *p = "it";
  int retSize;

  int *anagrams = findAnagrams (s, p, &retSize);

  printf ("%s :: %s\n", s, p);
  for (int i = 0; i < retSize; i++) {
    printf ("%d: ", anagrams[i]);

    for (int c = 0; c < strlen(s); c++) {
	if (c == anagrams[i])
	    printf ("(");
	else if (c == anagrams[i] + strlen (p))
            printf (")");

	printf ("%c", s[c]);
    }
    printf ("\n");
  }

  return 0;
}
