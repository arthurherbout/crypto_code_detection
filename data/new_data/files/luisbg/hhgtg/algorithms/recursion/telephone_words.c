/* Telephone Words */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char
get_char_key (int telephoneKey, int place)
{
  switch (telephoneKey) {
    case 2:
      if (place == 1)
        return 'A';
      if (place == 2)
        return 'B';
      if (place == 3)
        return 'C';
    case 3:
      if (place == 1)
        return 'D';
      if (place == 2)
        return 'E';
      if (place == 3)
        return 'F';
    case 4:
      if (place == 1)
        return 'G';
      if (place == 2)
        return 'H';
      if (place == 3)
        return 'I';
    case 5:
      if (place == 1)
        return 'J';
      if (place == 2)
        return 'K';
      if (place == 3)
        return 'L';
    case 6:
      if (place == 1)
        return 'M';
      if (place == 2)
        return 'N';
      if (place == 3)
        return 'O';
    case 7:
      if (place == 1)
        return 'P';
      if (place == 2)
        return 'R';
      if (place == 3)
        return 'S';
    case 8:
      if (place == 1)
        return 'T';
      if (place == 2)
        return 'U';
      if (place == 3)
        return 'V';
    case 9:
      if (place == 1)
        return 'W';
      if (place == 2)
        return 'X';
      if (place == 3)
        return 'Y';
    default:
      return ' ';
  }
}

void
print_telephone_words (char *number, char *word, int level, int size)
{
  int n;

  // three new strings for the next level
  char *a = (char *) malloc ((level + 1) * sizeof (char));
  char *b = (char *) malloc ((level + 1) * sizeof (char));
  char *c = (char *) malloc ((level + 1) * sizeof (char));

  // copy word so far to them
  for (n = 0; n < level; n++) {
    strcpy (a, word);
    strcpy (b, word);
    strcpy (c, word);
  }

  // if 0 or 1, just one word
  if (number[n] - '0' == 0 || number[n] - '0' == 1) {
    a[level] = number[n];
    print_telephone_words (number, a, level + 1, size);

    // free unused new words
    free (b);
    free (c);

    if (level == size - 1) {
      printf ("%s\n", a);
      return;
    }
  } else {
    // add the 3 characters to the new words
    a[level] = get_char_key (number[n] - '0', 1);
    b[level] = get_char_key (number[n] - '0', 2);
    c[level] = get_char_key (number[n] - '0', 3);

    // base case. print words
    if (level == size - 1) {
      printf ("%s\n", a);
      printf ("%s\n", b);
      printf ("%s\n", c);
      return;
    }
    // not finished yet, create three more from current words
    print_telephone_words (number, a, level + 1, size);
    print_telephone_words (number, b, level + 1, size);
    print_telephone_words (number, c, level + 1, size);
  }
}

int
main ()
{
  char *number = "67012";
  // char *number = "8662665";

  char *tmp = (char *) malloc (7 * sizeof (char));
  print_telephone_words (number, tmp, 0, 5);

  return 0;
}
