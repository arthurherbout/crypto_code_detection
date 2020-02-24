/* Inspired by Project Euler problem 28 */

#include <stdlib.h>
#include <stdio.h>


/* Sum of numbers in both down-right and up-right diagonals */
int sum_of_diagonals (int size, int matrix[size][size])
{
  int i, j;
  int sum = 0;

  for (i = 0, j = 0; i < size; i++, j++)   // numbers in the down-right diagonal
    sum += matrix[i][j];

  for (i = size -1, j = 0; j < size; i--, j++)    // up-right diagonal
    sum += matrix[i][j];

  sum -= 1; // we added the 1 in the center twice

  return sum;
}

/* Print the two-dimensional array of numbers */
void print_matrix (int size, int matrix[size][size])
{
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf ("%3d ", matrix[i][j]);
    }
    printf ("\n");
  }

  printf ("\n");
}

/* Generate the spiral of numbers in the two-dimensional array */
void generate_matrix (int size, int matrix[size][size])
{
  int c, i, j, level, step;

  // starting position is middle point
  i = size / 2;
  j = size / 2;

  level = 0;
  c = 1;
  matrix[i][j] = 1;

  // spiral out
  while (c < (size * size)) {
    level++;    // step increments happen before going right and going left
    step = 0;
    while (step < level) {    // write numbers going right
      step++;
      j++;
      matrix[i][j] = ++c;
    }
    if (c >= (size * size))
        break;

    step = 0;
    while (step < level) {    // write numbers going down
      step++;
      i++;
      matrix[i][j] = ++c;
    }

    level++;    // the going left step increment
    step = 0;
    while (step < level) {    // write numbers going left
      step++;
      j--;
      matrix[i][j] = ++c;
    }

    step = 0;
    while (step < level) {    // write numbers going up
      step++;
      i--;
      matrix[i][j] = ++c;
    }
  }
}


int main ()
{
  int size = 21;
  int matrix[size][size];

  generate_matrix(size, matrix);
  print_matrix (size, matrix);

  printf ("sum of diagonals: %d\n", sum_of_diagonals (size, matrix));

  return 0;
}
