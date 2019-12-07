/* Find the minimum number of steps between point A and B */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>


typedef struct node
{
  int x, y;
  int steps;                    // Number of steps we have taken to reach this point
  int x_route[256];
  int y_route[256];
} node;

typedef struct queue_node
{
  node *data;
  struct queue_node *next;
} queue_node;

typedef struct queue
{
  queue_node *head;
  queue_node *last;
} queue;


node *min_steps (int w, int h, bool grid[w][h], int start_x, int start_y,
    int end_x, int end_y);
void show_steps (int w, int h, bool map[w][h], node * steps);
static void init_queue (queue * q);
static void enqueue (queue * q, int x, int y, int steps, node * prev);
static node *dequeue (queue * q);
static bool empty_queue (queue q);


/* discover and return the shortest path between two nodes */
node *
min_steps (int w, int h, bool grid[w][h], int start_x, int start_y,
    int end_x, int end_y)
{
  int c, r;
  int x_delta, y_delta;
  bool visited[w][h];
  node *top = NULL;
  queue q;

  // mark all nodes as unvisited
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      visited[c][r] = false;
    }
  }

  init_queue (&q);

  // enqueue starting position with 0 steps
  enqueue (&q, start_x, start_y, 0, NULL);

  // breadth first travel until find destination or out of paths
  while (!empty_queue (q)) {
    top = dequeue (&q);

    if (top->x == end_x && top->y == end_y)
      return top;

    // Check if out of bounds
    if (top->x < 0 || top->x >= w)
      continue;
    if (top->y < 0 || top->y >= h)
      continue;

    // Check if node blocked
    if (!grid[top->x][top->y])
      continue;

    // printf (": %d, %d\n", top->x, top->y);
    // Check if already visited
    if (visited[top->x][top->y])
      continue;
    visited[top->x][top->y] = true;

    // Generate all of the transitions between nodes
    // Generate the following deltas: (-1,-1), (-1,0), (-1,1), (0,-1), (0,0),
    // (0,1), (1,-1), (1,0), (1,1)
    for (x_delta = -1; x_delta <= 1; x_delta++) {
      for (y_delta = -1; y_delta <= 1; y_delta++) {
        // Add the new node into the queue
        enqueue (&q, top->x + x_delta, top->y + y_delta, top->steps + 1, top);
      }
    }

    free (top);
  }

  return NULL;
}

/* display the map with the path between nodes marked */
void
show_steps (int w, int h, bool map[w][h], node * steps)
{
  int c, r, s;
  int route_map[w][h];

  // Generate map with obstacles
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      if (map[c][r])
        route_map[c][r] = 0;
      else
        route_map[c][r] = 1;
    }
  }

  // Mark nodes which path used
  for (s = 0; s < steps->steps; s++) {
    route_map[steps->x_route[s]][steps->y_route[s]] = 2;
  }

  // Print map
  printf ("\n");
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      if (route_map[c][r] == 0)
        printf (". ");
      else if (route_map[c][r] == 1)
        printf ("x ");
      else
        printf ("O ");
    }
    printf ("\n");
  }
}

/* initialize queue */
static void
init_queue (queue * q)
{
  q->head = NULL;
  q->last = NULL;
}

/* add node to the tail of the queue */
static void
enqueue (queue * q, int x, int y, int steps, node * prev)
{
  int i;

  queue_node *new_node = (queue_node *) malloc (sizeof (queue_node));
  node *pos = (node *) malloc (sizeof (node));
  pos->x = x;
  pos->y = y;
  pos->steps = steps;
  new_node->data = pos;
  new_node->next = NULL;

  if (prev) {
    for (i = 0; i < steps; i++) {
      pos->x_route[i] = prev->x_route[i];
      pos->y_route[i] = prev->y_route[i];
    }
    pos->x_route[steps] = x;
    pos->y_route[steps] = y;
  }

  if (q->head) {
    q->last->next = new_node;
    q->last = new_node;
  } else {
    q->head = new_node;
    q->last = new_node;
  }
}

/* get head of the queue */
static node *
dequeue (queue * q)
{
  node *dq = NULL;

  if (q->head) {
    dq = q->head->data;
    q->head = q->head->next;
  }

  return dq;
}

/* check if queue is empty */
static bool
empty_queue (queue q)
{
  return q.head == NULL;
}

/* initialize random map with obstacles */
void
init_map (int w, int h, bool map[w][h])
{
  int c, r;

  /* use time as seed for pseud-randomness */
  srand (time (NULL));

  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      if (rand () % 10 > 2) {
        map[c][r] = true;
        printf (". ");
      } else {
        map[c][r] = false;
        printf ("X ");
      }
    }
    printf ("\n");
  }
  printf ("\n");

  return;
}


int
main ()
{
  int w = 30;
  int h = 20;
  int start_x = 0, start_y = 0;
  int end_x = 29, end_y = 19;
  bool map[w][h];
  node *steps;

  init_map (w, h, map);

  steps = min_steps (w, h, map, start_x, start_y, end_x, end_y);
  if (steps) {
    printf ("min number of steps between %d,%d and %d,%d: %d\n",
        start_x, start_y, end_x, end_y, steps->steps);

    show_steps (w, h, map, steps);
  } else
    printf ("%d,%d can't be reached from %d,%d\n", end_x, end_y,
        start_x, start_y);

  return 0;
}
