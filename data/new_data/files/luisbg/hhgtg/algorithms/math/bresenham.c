/* Bresenham's line algorithm and midpoint circle algorithm
 *
 * https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */

#include <stdio.h>
#include <stdlib.h>


void
create_canvas (int h, int w, char canvas[h][w])
{
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      canvas[y][x] = '.';
    }
  }
}

void
print_canvas (int w, int h, char canvas[h][w])
{
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      printf ("%c", canvas[y][x]);
    }
    printf ("\n");
  }
}

void
bresenham_line (int x0, int y0, int x1, int y1, int w, int h, char canvas[h][w])
{
  int diff_x = abs (x1 - x0);
  int diff_y = abs (y1 - y0);
  int direction_x, direction_y, err, e2;

  if (x0 < x1)
    direction_x = 1;
  else
    direction_x = -1;

  if (y0 < y1)
    direction_y = 1;
  else
    direction_y = -1;

  if (diff_x > diff_y)
    err = diff_x / 2;
  else
    err = -diff_y / 2;

  canvas[y0][x0] = '#';
  while (x0 != x1 && y0 != y1) {
    e2 = err;
    if (e2 > -diff_x) {
      err -= diff_y;
      x0 += direction_x;
    }
    if (e2 < diff_y) {
      err += diff_x;
      y0 += direction_y;
    }

    canvas[y0][x0] = '#';
  }
}

void
put_pixel (int x, int y, int h, int w, char canvas[h][w])
{
  if ((x < w && y < h) && (x >= 0 && y >= 0))
    canvas[y][x] = '#';
}

void
bresenham_circle (int x0, int y0, int radius, int h, int w, char canvas[h][w])
{
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    // mirror in each octant of the circle
    put_pixel (x0 + x, y0 + y, h, w, canvas);   // 4 o'clock
    put_pixel (x0 + y, y0 + x, h, w, canvas);   // 5 o'clock
    put_pixel (x0 - y, y0 + x, h, w, canvas);   // 7 o'clock
    put_pixel (x0 - x, y0 + y, h, w, canvas);   // 8 o'clock
    put_pixel (x0 - x, y0 - y, h, w, canvas);   // 10 o'clock
    put_pixel (x0 - y, y0 - x, h, w, canvas);   // 11 o'clock
    put_pixel (x0 + y, y0 - x, h, w, canvas);   // 1 o'clock
    put_pixel (x0 + x, y0 - y, h, w, canvas);   // 2 o'clock

    if (err <= 0) {
      y += 1;
      err += (2 * y) + 1;
    }

    if (err > 0) {
      x -= 1;
      err -= (2 * x) + 1;
    }
  }
}


int
main ()
{
  int w, h;
  int x1, y1, x3, y3;
  int cx, cy, r;

  scanf ("%d %d", &w, &h);
  scanf ("%d %d %d %d", &x1, &y1, &x3, &y3);
  scanf ("%d %d %d", &cx, &cy, &r);

  char canvas[h][w];
  create_canvas (h, w, canvas);

  bresenham_line (x1, y1, x3, y3, h, w, canvas);
  bresenham_circle (cx, cy, r, h, w, canvas);
  print_canvas (w, h, canvas);

  return 0;
}
