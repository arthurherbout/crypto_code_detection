/* Bucket fill the section with the same color of a certain pixel */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COLUMNS 30
#define ROWS 20


typedef struct node
{
  int x;
  int y;
  struct node *next;
} node;

typedef struct stack
{
  node *top;
} stack;


int paint_region (int col, int row, int bitmap[col][row], int x, int y,
    int color);
static int do_fill_map (int col, int row, int bitmap[col][row],
    bool fill[col][row], int x, int y, int color);
static node *pop (stack * s);
static void push (stack * s, int x, int y);
static bool empty (stack s);


/* paint the region connected to the given pixel to the given color */
int
paint_region (int col, int row, int bitmap[col][row], int x, int y, int color)
{
  int c, r;
  int pixels;
  int old_color = bitmap[x][y];
  bool fill[col][row];

  // we first get the region to paint
  pixels = do_fill_map (col, row, bitmap, fill, x, y, old_color);

  // then we paint it
  for (r = 0; r < row; r++) {
    for (c = 0; c < col; c++) {
      if (fill[c][r])
        bitmap[c][r] = color;
    }
  }

  return pixels;
}

/* find connected pixels to the given one */
static int
do_fill_map (int col, int row, int bitmap[col][row], bool fill[col][row], int x,
    int y, int color)
{
  int c, r;
  int result = 0;
  bool visited[col][row];

  // mark all pixels as unfilled and unvisited
  for (r = 0; r < row; r++) {
    for (c = 0; c < col; c++) {
      fill[c][r] = false;
      visited[c][r] = false;
    }
  }

  // declare our stack of pixels, and push our starting pixel onto the stack
  stack s;
  s.top = NULL;
  push (&s, x, y);

  // travel breadth first to all connected pixels
  while (!empty (s)) {
    node *top = pop (&s);

    // check to ensure that we are within the bounds of the grid
    if (top->x < 0 || top->x >= col)
      continue;
    if (top->y < 0 || top->y >= row)
      continue;

    // check that we haven't already visited this position
    if (visited[top->x][top->y])
      continue;

    visited[top->x][top->y] = true;     // Save this position as visited
    if (bitmap[top->x][top->y] == color) {
      // this pixel has the same color and is connected, add it to results
      fill[top->x][top->y] = true;
      result++;

      // visit every node adjacent to this node.
      push (&s, top->x + 1, top->y);
      push (&s, top->x - 1, top->y);
      push (&s, top->x, top->y + 1);
      push (&s, top->x, top->y - 1);
    }

    free (top);
  }

  return result;
}

// get the node at the top of the stack
static node *
pop (stack * s)
{
  node *ret;

  if (s->top) {
    ret = s->top;
    s->top = s->top->next;
  }

  return ret;
}

// put a node at the top of the stack
static void
push (stack * s, int x, int y)
{
  node *new = (node *) malloc (sizeof (node));
  new->x = x;
  new->y = y;

  new->next = s->top;
  s->top = new;
}

// check if stack is empty
static bool
empty (stack s)
{
  if (s.top)
    return false;
  else
    return true;
}

// display the bitmap
void
print_bitmap (int col, int row, int bitmap[col][row])
{
  int c, r;;

  for (r = 0; r < row; r++) {
    for (c = 0; c < col; c++) {
      if (bitmap[c][r] == 0)
        printf (". ");
      else if (bitmap[c][r] == 1)
        printf ("* ");
      else
        printf ("O ");
    }
    printf ("\n");
  }
  printf ("\n");
}

// initialize bitmap to a random image
void
init_bitmap (int col, int row, int bitmap[col][row])
{
  int c, r;;

  // initialize bitmap
  for (r = 0; r < row; r++) {
    for (c = 0; c < col; c++) {
      bitmap[c][r] = rand () % 2;
    }
  }
}


int
main ()
{
  int col = COLUMNS;
  int row = ROWS;

  int pixels;
  int bitmap[col][row];
  int x = 16, y = 12, color = 4;        // pixel to bucket fill and color

  init_bitmap (col, row, bitmap);
  print_bitmap (col, row, bitmap);

  // paint region connected to pixel
  pixels = paint_region (col, row, bitmap, x, y, color);
  printf ("painted %d pixels: \n\n", pixels);
  print_bitmap (col, row, bitmap);

  return 0;
}
