/* Non-unique combinations of X values in Y positions
 * Also known as Multisubsets
 * or k-combination with repetitions, or k-multicombination
 *
 * https://en.wikipedia.org/wiki/Combination
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void
print_combo (int positions, int combo[positions])
{
  int i;

  for (i = 0; i < positions; i++) {
    (combo[i] < 10) ? printf ("%d ", combo[i]) : printf ("%c ", combo[i]);
  }
  printf ("\n");
}

void
generate_recurse (int positions, int combo[positions], int set_size,
    int set[set_size], int level)
{
  int i;

  if (level == positions) {
    print_combo (positions, combo);
    return;
  }

  for (i = 0; i < set_size; i++) {
    combo[level] = set[i];
    generate_recurse (positions, combo, set_size, set, level + 1);
  }
}

void
generate_combinations (int positions, int combo[positions], int set_size,
    int set[set_size])
{
  generate_recurse (positions, combo, set_size, set, 0);
}

void
iterative_combinations (int set_size, int set[set_size], int positions)
{
  int n, i;
  int val[positions];
  int c[positions];

  /* init c[] */
  for (n = 0; n < positions; ++n) {
    c[n] = 0;
    val[n] = 0;
  }

  /* generate combinations */
  for (n = 0; n < pow (set_size, positions); n++) {
    for (i = 0; i < positions; i++) {
      printf ("%c ", set[val[i]]);

      c[i]++;
      if (c[i] == pow (set_size, (positions - 1 - i))) {
        c[i] = 0;
        val[i]++;
        if (val[i] == set_size)
          val[i] = 0;
      }
    }
    printf ("\n");
  }
}

int
main ()
{
  int positions = 2;
  int combo[positions];
  int set_size = 8;
  int num_set[set_size];
  int i;

  for (i = 0; i < set_size; i++) {
    num_set[i] = i;
  }

  printf ("generate combination of octal numbers\n");
  generate_combinations (positions, combo, set_size, num_set);

  positions = 3;
  set_size = 4;
  int second_combo[positions];
  int letter_set[set_size];

  for (i = 0; i < set_size; i++) {
    letter_set[i] = 'A' + i;
  }

  printf ("\ngenerate padlock combinations when it is 4 wheels of A to C\n");
  generate_combinations (positions, second_combo, set_size, letter_set);

  for (i = 0; i < set_size; i++) {
    letter_set[i] = 'Q' + i;
  }

  printf ("\ngenerate padlock combinations iteratively\n");
  iterative_combinations (set_size, letter_set, positions);

  return 0;
}
