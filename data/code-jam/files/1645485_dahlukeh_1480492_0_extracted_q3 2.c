#include <stdio.h>
#include <stdlib.h>


#define TRUE            1
#define FALSE           0

#define LEFT            0
#define RIGHT           1

#define EP              0.0000001

struct critical {
   double time;
   int carFront;
   int carBack;
};

void handlecase ();

int N;
int lanes[60];
double speed[60];
double distanceFront[60];
double distanceBack[60];

int storeLanes[60];
double storeFront[60];
double storeBack[60];

int amoP;
struct critical points[10000];

int blocked (int c1, int c2);

void sort (int start, int finish);

double absF (double n);

double bestTime (int choices);

int main () {
   freopen ("in.txt", "r", stdin);
   freopen ("out.txt", "w", stdout);
   int n;
   int i;
   scanf ("%d ", &n);
   for (i = 0; i < n; i++) {
      printf ("Case #%d: ", i + 1);
      handlecase ();
   }
   return 0;
}


void handlecase () {
   scanf ("%d ", &N);
   int i, j;
   for (i = 0; i < N; i++) {
      char lane;
      scanf ("%c %lf %lf ", &lane, &speed[i], &storeBack[i]);
      storeFront[i] = storeBack[i] + 5.0;
      if (lane == 'L') {
         storeLanes[i] = LEFT;
      } else {
         storeLanes[i] = RIGHT;
      }
   }
   amoP = 0;
   for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
         double dSpeed = speed[j] - speed[i];
         if (dSpeed > 0.0) {
            double dDist = storeBack[i] - storeFront[j];
            if (dDist >= 0.0) {
               points[amoP].time = (dDist / dSpeed);
               points[amoP].carFront = i;
               points[amoP].carBack = j;
               amoP++;
            }
         }
      }
   }
   sort (0, amoP - 1);
   double best = 0.0;
   for (i = 0; i < (1 << amoP); i++) {
      for (j = 0; j < N; j++) {
         lanes[j] = storeLanes[j];
         distanceBack[j] = storeBack[j];
         distanceFront[j] = storeFront[j];
      }
      double res = bestTime (i);
      if (res < 0.0 || (res > best && best >= 0.0)) {
         best = res;
      }
   }
   if (best < 0.0) {
      printf ("Possible\n");
   } else {
      printf ("%lf\n", best);
   }
}



double bestTime (int choices) {
   int i, j;
   int coll = FALSE;
   double lastTime = 0.0;
   for (i = 0; i < amoP && !coll; i++) {
      for (j = 0; j < N; j++) {
         double change = speed[j] * (points[i].time - lastTime);
         distanceBack[j] += change;
         distanceFront[j] += change;
      }
      // Now, check if they are in the same lane
      if (lanes[points[i].carBack] == lanes[points[i].carFront]) {
         // We need one to move over
         int canFront = TRUE;
         int canBack = TRUE;
         int laneLook = (lanes[points[i].carFront] + 1) % 2;
         for (j = 0; j < N; j++) {
            if (lanes[j] == laneLook) {
               if (blocked (j, points[i].carFront)) {
                  canFront = FALSE;
               }
               if (blocked (j, points[i].carBack)) {
                  canBack = FALSE;
               }
            }
         }
         if (canFront && canBack) {
            if (choices & (1 << i)) {
               lanes[points[i].carBack] = laneLook;
            } else {
               lanes[points[i].carFront] = laneLook;
            }
         } else if (canBack) {
            lanes[points[i].carBack] = laneLook;
         } else if (canFront) {
            lanes[points[i].carFront] = laneLook;
         } else {
            coll = TRUE;
         }
      }
      lastTime = points[i].time;
   }
   if (coll) {
      return points[i - 1].time;
   } else {
      return -1.0;
   }
}


void sort (int start, int finish) {
   if (finish <= start) {
      return;
   }
   int newFinish = (start + finish) / 2;
   sort (start, newFinish);
   sort (newFinish + 1, finish);
   int i;
   int place1 = start;
   int place2 = newFinish + 1;
   struct critical newP[finish - start + 1];
   for (i = 0; i < finish - start + 1; i++) {
      if (place1 > newFinish) {
         newP[i] = points[place2];
         place2++;
      } else if (place2 > finish) {
         newP[i] = points[place1];
         place1++;
      } else {
         if (points[place1].time < points[place2].time) {
            newP[i] = points[place1];
            place1++;
         } else {
            newP[i] = points[place2];
            place2++;
         }
      }
   }
   for (i = 0; i < finish - start + 1; i++) {
      points[start + i] = newP[i];
   }
}


int blocked (int c1, int c2) {
   double disB = distanceFront[c1] - distanceBack[c2];
   if (absF (disB) < EP) {
      if (speed[c1] > speed[c2]) {
         return TRUE;
      } else {
         return FALSE;
      }
   }
   if (absF (disB - 10.0) < EP) {
      if (speed[c1] < speed[c2]) {
         return TRUE;
      } else {
         return FALSE;
      }
   }
   if (disB > 0.0 && disB < 10.0) {
      return TRUE;
   }
   return FALSE;
}

double absF (double n) {
   if (n < 0.0) {
      return -n;
   } else {
      return n;
   }
}
