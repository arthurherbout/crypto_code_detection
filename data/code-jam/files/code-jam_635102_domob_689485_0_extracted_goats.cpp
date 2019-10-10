/* CodeJam solution goats in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <cmath>


/* Solve a single case.  */

void
solve_case ()
{
  int n, m;
  scanf ("%d %d", &n, &m);
  assert (n == 2);

  double px[2], py[2];
  scanf ("%lf %lf %lf %lf", &px[0], &py[0], &px[1], &py[1]);
  
  double ofx, ofy;
  ofx = px[0];
  ofy = py[0];
  px[0] -= ofx;
  px[1] -= ofx;
  py[0] -= ofy;
  py[1] -= ofy;

  for (int i = 1; i <= m; ++i)
    {
      double qx, qy;
      scanf ("%lf %lf", &qx, &qy);
      qx -= ofx;
      qy -= ofy;

      // Distance is just to that water point.
      double l[2];
      for (int k = 0; k < 2; ++k)
        l[k] = sqrt (std::pow (px[k] - qx, 2) + std::pow (py[k] - qy, 2));
      
      double x = std::sqrt (std::pow (px[0] - px[1], 2)
                           + std::pow (py[0] - py[1], 2));

      double c = px[1] / x;
      double s = py[1] / x;

      double newPx, newPy;
      newPx = c * px[1] + s * py[1];
      newPy = -s * px[1] + c * py[1];
      assert (std::abs (newPy) < 1.0e-6);
      assert (newPx >= 0.0);

      double newQx, newQy;
      newQx = c * qx + s * qy;
      newQy = -s * qx + c * qy;

      double yHalf = std::abs (newQy);

      // Find angles.
      double alpha[2];
      alpha[0] = 2 * std::asin (yHalf / l[0]);
      alpha[1] = 2 * std::asin (yHalf / l[1]);

      // Find area as - (deltoid - 2 circle sections);
      double a = - x * yHalf;
      for (int k = 0; k < 2; ++k)
        a += alpha[k] / (2 * M_PI) * M_PI * l[k] * l[k];

      if (newQx < 0.0 || newQx > newPx)
        {
          double r = std::min (l[0], l[1]);

          double moons[2];
          for (int k = 0; k < 2; ++k)
            moons[k] = alpha[k] / (2 * M_PI) * M_PI * l[k] * l[k];
          moons[0] -= yHalf * (std::abs (newQx));
          moons[1] -= yHalf * (std::abs (newQx - newPx));

          a = M_PI * r * r - std::abs (moons[0] - moons[1]);
        }

      printf (" %lf", a);
    }
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      printf ("Case #%d: ", i);
      solve_case ();
      printf ("\n");
    }

  return EXIT_SUCCESS;
}
