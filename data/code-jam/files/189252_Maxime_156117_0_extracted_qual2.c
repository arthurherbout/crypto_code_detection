#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <math.h>

typedef struct pnt {
  float x, y, z, vx, vy, vz;
} PNT;


PNT P[500];

float Distance(PNT *x) {
  register float d = (float)(x->x * x->x) + (float)(x->y * x->y) + (float)(x->z * x->z);
  d = sqrtf(d);
  return d;
}

main() {
  char str[4096];
  int t, i;
  int N, T;
  float dist, time, mind, distance, prevd, tim, mintim;
  PNT center;

  fgets(str, sizeof(str), stdin);
  sscanf(str, "%d", &T);

  for (t = 0; t < T; t++) {
    fgets(str, sizeof(str), stdin);
    sscanf(str, "%d", &N);
    for (i = 0; i < N; i++) {
      fgets(str, sizeof(str), stdin);
      sscanf(str, "%f %f %f %f %f %f", &P[i].x, &P[i].y, &P[i].z, &P[i].vx, &P[i].vy, &P[i].vz);
    }
    
    bzero(&center, sizeof(center));
    for (i = 0; i < N; i++) {
      center.x += P[i].x;
      center.y += P[i].y;
      center.z += P[i].z;
      fprintf(stderr, "P.%d %f %f %f %f %f %f\n", i, P[i].x, P[i].y, P[i].z, P[i].vx, P[i].vy, P[i].vz);
    }
    center.x /= N;
    center.y /= N;
    center.z /= N;
    mind = distance = prevd = Distance(&center);

    for (i = 0; i < N; i++) {
      P[i].vx /= (float)N;
      P[i].vy /= (float)N;
      P[i].vz /= (float)N;
    }
    mintim = 0.0;
    mind += 1000000000;
    for (tim = 1.0; /*(prevd > distance) &&*/ (tim < 5000.0); tim += 1.0) {
      for (i = 0; i < N; i++) {
	center.x += P[i].vx;
	center.y += P[i].vy;
	center.z += P[i].vz;
      }
      distance = Distance(&center);
/*      fprintf(stderr, " -- tim: %f mintim: %f-- mind: %f, distance: %f\n", tim, mintim, mind, distance);*/
      if (distance < mind) {
	mintim = tim;
	mind = distance;
      }
    }

    printf("Case #%d: %.8f %.8f \n", t + 1, mind, mintim);
  }

  return 0;
}
