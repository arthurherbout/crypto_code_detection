#include <stdio.h>
#include <stdlib.h>

#define TRUE         1
#define FALSE        0

int N;
int inc[2001];
int dec[2001];
int num[2001];
int next;
int valid[2001];

void handlecase();

int main() {
   freopen ("c.in", "r", stdin);
   freopen ("c.out", "w", stdout);
   int n;
   scanf ("%d ", &n);
   int i;
   for (i = 0; i < n; i++) {
      printf ("Case #%d: ", i + 1);
      handlecase();
   }
   return 0;
}

void mark (int start, int place, int val) {
   int i;
   int preS = start;
   for (i = start; i < place; i++) {
      if (dec[i] == val) {
         num[i] = next;
         next++;
         mark (preS, i, val + 1);
         preS = i;
      }
   }
}

void handlecase() {
   scanf ("%d ", &N);
   int i;
   int b;
   for (i = 0; i < N; i++) {
      scanf ("%d ", &b);
   }
   for (i = 0; i < N; i++) {
      scanf ("%d ", &dec[i]);
      valid[i] = TRUE;
   }
   next = 1;
   mark (0, N, 1);
   for (i = 0; i < N; i++) {
      if (i != 0) {
         printf (" ");
      }
      printf ("%d", num[i]);
   }
   printf ("\n");
}

