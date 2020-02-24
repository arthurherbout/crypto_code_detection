// Luke Harrison
// 22/05/10

#include <stdio.h>
#include <stdlib.h>

#define TRUE         1
#define FALSE        0

int deleteCost, addCost, smoothness;
int numPixels;

int pixels[100];

int abs (int n);
int max (int n1, int n2);
int min (int n1, int n2);

int minCost2 (int n1, int n2);

int minCostNoDelete2 (int n1, int n2);

void handleCase (FILE *input, FILE* output);

int minCostAdd (int n1, int n2);
int main () {
   FILE* input = fopen ("input.txt", "r");
   FILE* output = fopen ("output.txt", "w");
   int i;
   int numCases;
   fscanf (input, "%d ", &numCases);
   for (i = 0; i < numCases; i++) {
      fprintf (output, "Case #%d: ", i + 1);
      handleCase (input, output);
   }   
   
   fclose (input);
   fclose (output);
   return 0;
}

void handleCase (FILE *input, FILE* output) {
   fscanf (input, "%d %d %d %d ", &deleteCost, &addCost, &smoothness, &numPixels);
   int i;
   for (i = 0; i < numPixels; i++) {
      fscanf (input, "%d ", &(pixels[i]));
   }
   if (numPixels == 3) {
      int minDelete = min (minCost2 (pixels[0], pixels[1]), minCost2 (pixels[1], pixels[2]));
      minDelete = min (minDelete, minCost2 (pixels[0], pixels[2])) + deleteCost;
      printf ("Here %d %d\n", deleteCost, minDelete);
      int costToChange;
      int costToAdd;
      int lowest;
      int newMiddle;
      int changeMiddle;
      int otherPixel;
      if (abs (pixels[0] - pixels[1]) > smoothness && abs (pixels[1] - pixels[2]) > smoothness) { // Both are out
         // Change one of the ends;
         costToChange = min ((abs (pixels[0] - pixels[1]) - smoothness) + minCostNoDelete2 (pixels[1], pixels[2]), (abs (pixels[1] - pixels[2]) - smoothness) + minCostNoDelete2 (pixels[0], pixels[1]));
         // Just adding
         if (smoothness > 0) {
            if (pixels[0] < pixels[1] && pixels[1] > pixels[2]) {
               newMiddle = pixels[1] - ((pixels[1] - max (pixels[0], pixels[2])) % smoothness); // Bring it to the nearest multiple
            } else if (pixels[0] > pixels[1] && pixels[1] < pixels[2]) {
               newMiddle = pixels[1] - ((min (pixels[0], pixels[2]) - pixels[1]) % smoothness); // Bring it to the nearest multiple
            } else {
               newMiddle = pixels[1];
            }
         }
         if (smoothness > 0) {
            costToAdd = ((abs (pixels[0] - pixels[1]) / smoothness) * addCost) + ((abs (pixels[1] - pixels[2]) / smoothness) * addCost);
            costToAdd = min (costToAdd, ((abs (pixels[0] - newMiddle) / smoothness) * addCost) + ((abs (newMiddle - pixels[2]) / smoothness) * addCost));
         } else {
            costToAdd = 10000000;
         }
         if (pixels[0] < pixels[1] && pixels[1] > pixels[2]) {
            newMiddle = (max (pixels[0], pixels[2])) - smoothness;
            otherPixel = min (pixels[0], pixels[2]);
         } else if (pixels[0] > pixels[1] && pixels[1] < pixels[2]) {
            newMiddle = (min (pixels[0], pixels[2])) + smoothness;
            otherPixel = max (pixels[0], pixels[2]);
         } else {
            if (abs (pixels[1] - pixels[0]) < abs (pixels[1] - pixels[2])) {// Find the closest
               if (pixels[0] > pixels[2]) {// 0 closer than 2
                  newMiddle = pixels[0] - smoothness;
                  otherPixel = pixels[2];
               } else {
                  newMiddle = pixels[0] + smoothness;
                  otherPixel = pixels[2];
               }
            } else {
               if (pixels[2] > pixels[0]) {// 0 closer than 2
                  newMiddle = pixels[2] - smoothness;
                  otherPixel = pixels[0];
               } else {
                  newMiddle = pixels[2] + smoothness;
                  otherPixel = pixels[0];
               }
            }
         }
         changeMiddle = abs (pixels[1] - newMiddle) + minCostNoDelete2 (newMiddle, otherPixel);
         printf ("%d %d %d\n", newMiddle, otherPixel, changeMiddle);
         lowest = min (costToChange, costToAdd);
         lowest = min (lowest, changeMiddle);
         printf ("%d %d %d %d\n", costToChange, costToAdd, changeMiddle, lowest);
      } else if (abs (pixels[0] - pixels[1]) > smoothness) { // First two are out
         lowest = minCostNoDelete2 (pixels[0], pixels[1]);
      } else if (abs (pixels[1] - pixels[2]) > smoothness) { // Last two
         lowest = minCostNoDelete2 (pixels[1], pixels[2]);
      } else {
         lowest = 0;
      }
      printf ("Here %d\n", deleteCost);
      fprintf (output, "%d\n", min (minDelete, lowest));
   } else if (numPixels == 2) {
      
      fprintf (output, "%d\n", minCost2 (pixels[0], pixels[1]));
   } else {
      fprintf (output, "0\n");
   }
}



int abs (int n) {
   if (n < 0) {
      return -n;
   } else {
      return n;
   }
}

int max (int n1, int n2) {
   if (n1 > n2) {
      return n1;
   } else {
      return n2;
   }
}


int min (int n1, int n2) {
   if (n1 < n2) {
      return n1;
   } else {
      return n2;
   }
}


int minCost2 (int n1, int n2) {
   int costChange = max (abs (n1 - n2) - smoothness, 0);
   int costDelete = deleteCost;
   int costAdd = 100000000;
   if (smoothness > 0) {
      costAdd = (abs (n1 - n2) / smoothness) * addCost;
   }
   int lowest = min (costChange, costDelete);
   lowest = min (lowest, costAdd);
   return lowest;
}


int minCostNoDelete2 (int n1, int n2) {
   int costToChange, costAdd;
   costToChange = abs (n1 - n2) - smoothness;
   costAdd = 100000000;
   if (smoothness > 0) {
      costAdd = (abs (n1 - n2) / smoothness) * addCost;
   }
   return min (costToChange, costAdd);
}


