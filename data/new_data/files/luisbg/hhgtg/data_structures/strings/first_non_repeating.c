#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
  bool seen;
  bool repeated;
  int pos;
} repeat_counter_t;


/* Find the first non-repeating character */
char
first_non_repeat (char *in)
{
  int *charcount = (int *) calloc (sizeof (char) << 8, sizeof (int));
  int len, i;

  // count the occurrences of each char
  for (i = 0; in[i]; i++) {
    charcount[(int) in[i]]++;
  }

  // find the first char in the string with one ocurrence
  len = i;
  for (i = 0; i < len; i++)
    if (charcount[(int) in[i]] == 1) {
      free (charcount);
      return in[i];
    }

  free (charcount);
  return '\0';
}

/* Find the first non-repeating character for long strings */
char
first_non_repeat_long (char *in)
{
  const int char_range = sizeof (char) << 8;
  repeat_counter_t *rep_count = (repeat_counter_t *) calloc (char_range,
      sizeof (repeat_counter_t));
  int lowest_pos = char_range;
  int i;

  // run through character type range checking if seen and repeated
  for (i = 0; in[i]; i++) {
    if (!rep_count[(int) in[i]].repeated) {
      if (!rep_count[(int) in[i]].seen) {
        rep_count[(int) in[i]].seen = true;
        rep_count[(int) in[i]].pos = i;
      } else
        rep_count[(int) in[i]].repeated = true;
    }
  }

  // go through range finding the unrepeated character with the lowest position
  for (i = 0; i < char_range; i++) {
    if (rep_count[i].seen && !rep_count[i].repeated &&
      rep_count[i].pos < lowest_pos) {
      lowest_pos = rep_count[i].pos;
    }
  }

  free (rep_count);
  if (lowest_pos != char_range)
    return in[lowest_pos];
  else
    return '\0';
}

int
main ()
{
  char *str = "this is either the worst or the best";
  printf ("the first non-repeating character of: %s\nis: %c\n",
      str, first_non_repeat (str));

  str = "this is a much longer string that might be longer than two hundred fifty six characters";
  printf ("the first non-repeating character of: %s\nis: %c\n",
      str, first_non_repeat_long (str));

  str = "should be last character .boud. x";
  printf ("the first non-repeating character of: %s\nis: %c\n",
      str, first_non_repeat (str));

  return 0;
}
