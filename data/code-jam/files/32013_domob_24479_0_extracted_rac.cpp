/* Solution for the Tennis Racquet problem. */

/* I'm going to calculate the total area in which the fly's center can be so
 * it isn't hit; this in proporion to the area of the racquet circle gives the
 * complement of the probability asked for.
 *
 * Increasing the chord-radius by the fly's radius and decreasing the inner
 * circle of the ring by the fly's radius gives us a new figure; the area not
 * covered is the one we look for. There are four symmetric quaters, so we have
 * to count the areas of the free cells in one quarter only.
 *
 * For the area of a square cut off partly by the circle, we integrate the
 * circle-line to get the area below it and subtract a rectangle.
 */

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>

typedef long double realT;

/**
 * Area of a circle segment, from wikipedia.
 */
realT circSeg (realT h, realT s)
{
  realT numer;
  numer = .5 * std::atan (2 * h / s) * std::pow (4 * h*h + s*s, 2);
  numer += h * s * (4 * h*h - s*s);

  return numer / (16 * h*h);
}

/**
 * Integrate the circle line with radius R.
 * @param x1, x2 The integral endpoints
 * @param R The circle's radius
 * @param y Shift the circle line down by y units
 */
realT integrate (realT x1, realT x2, realT R, realT y)
{
  static const unsigned N = 100000;

  assert (x1 <= x2);
  assert (x1 < R && x2 < R);
  if (x1 == x2)
    return 0;

  /*
  realT res(0);
  const realT dx((x2 - x1) / N);
  for (realT x(x1); x < x2; x += dx)
    res += std::sqrt (R*R - x*x) - y;

  return res * dx;
  */


  /* This one is the difference of two circle segments. */
  const realT seg1(circSeg (R - x1, 2 * std::sqrt (R*R - x1*x1)));
  const realT seg2(circSeg (R - x2, 2 * std::sqrt (R*R - x2*x2)));
  return (seg1 - seg2) / 2 - (x2 - x1) * y;
}

/**
 * Calculate the area of a square, possibly cutted of by the circle line.
 * @param x,y The lower left corner of the square.
 * @param g The square's original side length
 * @param R The radius of the circle
 * @return The clipped square's area
 */
realT getSquareArea (realT x, realT y, realT g, realT R)
{
  assert (x < R && y < R);
  assert (x*x + y*y < R*R);

  /* First, we find the x-coordinate of the right-most point in the square not
   * clipped (or exactly on the clipping edge). That is min(x+g, X) where X is
   * the intersection of the circle line and the bottom border:
   * border: y = c
   * circle: x^2 + y^2 = R^2
   * => x^2 = R^2 - c^2
   */

  const realT lowerInterX(std::sqrt (R*R - y*y));
  const realT rightMost(std::min (x + g, lowerInterX));

  /* Now, find the x-coordinate where the circle intersects the upper border.
   * This is needed for distincting the three possible cases.
   */

  realT upperInterX(std::sqrt (R*R - (y+g)*(y+g)));
  if (y + g >= R || upperInterX < x)
    upperInterX = x;
  if (upperInterX > x + g)
    upperInterX = x + g;
  assert (upperInterX >= x);

  /* Take the whole part of square as base for the result value. */

  realT res(0);
  if (upperInterX > x)
    res = (upperInterX - x) * g;

  /* Now integrate the circle line from upperInterX to rightMost. */
  
  assert (upperInterX <= rightMost);
  res += integrate (upperInterX, rightMost, R, y);

  //std::cout << x << " " << upperInterX << " " << rightMost << " " << x+g << std::endl;
  //std::cout << res << " " << g * g << " " << (g*g - res) << std::endl;
  assert (g*g - res > -1e-12);

  return res;
}

realT solveCase ()
{
  realT f, R, t, r, g;
  std::cin >> f >> R >> t >> r >> g;

  // Total area of the racquet where the fly's center could be
  const realT totalArea(M_PI * R * R);

  // Get the adapted inner radius and chord radius
  R -= t + f;
  r += f;
  g -= 2*f;

  // If the distance between chords is too small, just finish with prob. 1.
  if (g <= 0 || R <= 0)
    return 1;

  // Now sum up the uncovered area of one quarter
  realT uncovered(0);

  // Walk the possibly uncovered stipes in quarter I to the left
  for (realT x(r); x < R; x += g + 2*r)
    for (realT y(r); y*y < R*R - x*x; y += g + 2*r)
      uncovered += getSquareArea (x, y, g, R);
  uncovered *= 4;
  if (uncovered > M_PI * R * R)
    uncovered = M_PI * R * R;

  assert (uncovered >= 0);
  assert (uncovered <= M_PI * R * R);
  assert (totalArea >= uncovered);

  // Do final put-together
  return 1 - uncovered / totalArea;
}

int main ()
{
  unsigned n;
  std::cin >> n;

  for (unsigned i(1); i <= n; ++i)
    std::cout << "Case #" << i << ": " << solveCase () << std::endl;

  return EXIT_SUCCESS;
}
