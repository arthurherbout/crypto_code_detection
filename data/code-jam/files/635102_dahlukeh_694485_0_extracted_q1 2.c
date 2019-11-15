// Luke Harrison
// 06/06/10

#include <stdio.h>
#include <stdlib.h>

#define TRUE               1
#define FALSE              0

int curDiamond[500][500];

int size;

void handleCase (FILE *input, FILE *output);

int max (int n1, int n2);

int sizeOfDiamond (int size);


int main () {
   FILE *input = fopen ("input.txt", "r");
   FILE *output = fopen ("output.txt", "w");
   int i;
   int numCases;
   fscanf (input, "%d ", &numCases);
   for (i = 1; i <= numCases; i++) {
      fprintf (output, "Case #%d: ", i);
      handleCase (input, output);
      printf ("\n\n");
   }
   return 0;
}


void handleCase (FILE *input, FILE *output) {
   int i, j;
   fscanf (input, "%d ", &size);
   for (i = 0; i < size; i++) {
      for (j = 0; j <= i; j++) {
         fscanf (input, "%d ", &curDiamond[i][j]);
      }
   }
   for (i = 0; i < size - 1; i++) {
      for (j = 0; j < size - i - 1; j++) {
         fscanf (input, "%d ", &curDiamond[i + size][j]);
      }
   }
   
   for (i = 0; i < (size * 2) - 1; i++) {
      for (j = 0; j < size; j++) {
         printf ("%d", curDiamond[i][j]);
      }
      printf ("\n");
   }
   
   int biggestDif = 0;
   int curDif;
   int rowDif;
   for (i = 0; i < size; i++) {
      curDif = 0;
      rowDif = 0;
      for (j = 0; j <= i; j++) {
         printf ("Comparing %d %d and %d %d ----- %d %d\n", i, j, i, i - j, curDiamond[i][j], curDiamond[i][i - j]);
         if (curDiamond[i][j] != curDiamond[i][i - j]) {
            printf ("Updated %d\n", i - (2 * j));
            rowDif++;
         }
         printf ("Comparing %d %d and %d %d ----- %d %d\n", i, j, ((size - 1) * 2) - i, j, curDiamond[i][j], curDiamond[((size - 1) * 2) - i][j]);
         if (curDiamond[i][j] != curDiamond[((size - 1) * 2) - i][j]) {
            printf ("Updated %d\n", (size * 2) - 1);
            curDif = max (curDif, (size * 2) - 1);
         }
      }
      biggestDif = max (biggestDif, (rowDif / 2) + size);
      biggestDif = max (biggestDif, curDif);
   }
   
   fprintf (output, "%d\n", (biggestDif * biggestDif) - (size * size));
}


int max (int n1, int n2) {
   if (n1 > n2) {
      return n1;
   } else {
      return n2;
   }
}


int sizeOfDiamond (int size) {
   return (size * (((size * 2) - 1) + 1) / 2);
}
