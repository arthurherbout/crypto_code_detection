/* If the numbers 1 to 5 are written out in words: one, two, three, four,
 * five, then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.
 *
 * If all the numbers from 1 to 1000 (one thousand) inclusive were written
 * out in words, how many letters would be used?
 *
 *
 * NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and
 * forty-two) contains 23 letters and 115 (one hundred and fifteen) contains
 * 20 letters. The use of "and" when writing out numbers is in compliance with
 * British usage.
 *
 */

#include <stdio.h>
#include <stdlib.h>


int to_english (int num, char singles[10][7], char tens[9][9],
                 char hundred[13], char excpt_teens[9][11])
{
  int c = 0;
  int orig_num = num;
  char tmp[100];
  int digit, x;

  tmp[0] = '\0';

  digit = num / 100;
  if (digit > 0) {
    for (x = 0; singles[digit][x] != '\0'; x++) {
      tmp[c] = singles[digit][x];
      c++;
    }

    for (x = 0; hundred[x] != '\0'; x++) {
      tmp[c] = hundred[x];
      c++;
    }

    if (num % 100 == 0)
      c -= 3;

    tmp[c] = '\0';
    num = num % 100;
  }

  digit = (num / 10) - 1;
  // printf ("%d\n", digit);
  if (digit >= 0) {
    if (digit != 0) {
      for (x = 0; tens[digit][x] != '\0'; x++) {
        tmp[c] = tens[digit][x];
        c++;
      }

      num = num % 10;
    } else {
      digit = num - 10;
      for (x = 0; excpt_teens[digit][x] != '\0'; x++) {
        tmp[c] = excpt_teens[digit][x];
        c++;
      }

      num = 0;
    }
    tmp[c] = '\0';
  }

  digit = num;
  if (digit > 0) {
    for (x = 0; singles[digit][x] != '\0'; x++) {
      tmp[c] = singles[digit][x];
      c++;
    }

    tmp[c] = '\0';
  }

  printf ("%d -> %s\n", orig_num, tmp);

  return c;
}

int main ()
{
  int i;
  int examples[10];

  for (i = 0; i < 3; i++)
    examples[i] = rand () % 10;
  for (i = 3; i < 6; i++)
    examples[i] = rand () % 100;
  for (i = 6; i < 10; i++)
    examples[i] = rand () % 1000;

  char singles[10][7] = {"zero ", "one ", "two ", "three ", "four ", "five ",
                          "six ", "seven ", "eight ", "nine "};
  char tens[9][9] = {"ten ", "twenty ", "thirty ", "forty ", "fifty ", "sixty ",
                          "seventy ", "eighty ", "ninety "};
  char hundred[13] = "hundred and ";
  char excpt_teens[10][11] = {"ten ", "eleven ", "twelve ", "thirteen ",
                            "fourteen ", "fifteen ", "sixteen ", "seventeen ",
                            "eighteen ", "nineteen "};

  //  int c;
  //  unsigned long sum = 0;
  //  for (c = 1; c < 1000; c++) {
  //    sum += to_english (c, singles, tens, hundred, excpt_teens);
  //  }
  //  printf("total: %d\n", sum);

  for (i = 0; i < 10; i++)
    to_english (examples[i], singles, tens, hundred, excpt_teens);

  return 0;
}
