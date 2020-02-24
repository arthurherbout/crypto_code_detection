// Dijkstra's shortest path algorithm
// With adjacency matrix representation of the graph

#include <stdio.h>


#define NUM_VERTICES 9
#define INFINITY 65535

// find the vertex with minimum distance from unvisited vertices
static int
min_distance (int dist[], int visited[])
{
  // Initialize min value
  int min = INFINITY, min_index;

  for (int v = 0; v < NUM_VERTICES; v++)
    if (visited[v] == 0 && dist[v] <= min) {
      min = dist[v];
      min_index = v;
    }

  return min_index;
}

static void
print_solution (int dist[], int n)
{
  printf ("Vertex \t\t Distance from initial node\n");
  for (int i = 0; i < NUM_VERTICES; i++)
    printf ("%d \t\t %d\n", i, dist[i]);
}

static void
show_graph (int graph[NUM_VERTICES][NUM_VERTICES])
{
  int c, d;

  for (c = 0; c < NUM_VERTICES; c++) {
    for (d = 0; d < NUM_VERTICES; d++) {
      printf ("%3d ", graph[c][d]);
    }
    printf ("\n");
  }

  printf ("\n");
}

// Dijkstra's shortest path algorithm
void
dijkstra (int graph[NUM_VERTICES][NUM_VERTICES], int src)
{
  int dist[NUM_VERTICES];
  int visited[NUM_VERTICES];

  // Assign to every node a tentative distance value
  // Mark all nodes other than initial as unvisited.
  for (int i = 0; i < NUM_VERTICES; i++)
    dist[i] = INFINITY, visited[i] = 0;

  // Set the initial node as current.
  dist[src] = 0;

  // Find shortest path for all vertices
  for (int count = 0; count < (NUM_VERTICES - 1); count++) {
    // Pick the minimum distance vertex from the set of vertices not
    // yet processed
    int u = min_distance (dist, visited);

    // Mark the picked vertex as processed
    visited[u] = 1;

    // Update dist value of the adjacent vertices of the picked vertex
    for (int v = 0; v < NUM_VERTICES; v++)
      // Update dist[v] only if it is not visited, there is an edge from
      // u to v, and total weight of path from src to v through u is
      // smaller than current value of dist[v]
      if (!visited[v] && graph[u][v] && (dist[u] + graph[u][v]) < dist[v]) {
        dist[v] = dist[u] + graph[u][v];
      }
  }

  print_solution (dist, NUM_VERTICES);
}


int
main ()
{
  int graph[NUM_VERTICES][NUM_VERTICES] = { {0, 4, 0, 0, 0, 0, 0, 8, 0},
  {4, 0, 8, 0, 0, 0, 0, 11, 0},
  {0, 8, 0, 7, 0, 4, 0, 0, 2},
  {0, 0, 7, 0, 9, 14, 0, 0, 0},
  {0, 0, 0, 9, 0, 10, 0, 0, 0},
  {0, 0, 4, 0, 10, 0, 2, 0, 0},
  {0, 0, 0, 14, 0, 2, 0, 1, 6},
  {8, 11, 0, 0, 0, 0, 1, 0, 7},
  {0, 0, 2, 0, 0, 0, 6, 7, 0}
  };

  show_graph (graph);
  dijkstra (graph, 0);

  return 0;
}
