/* Adjacency Matrix by storing a bi-dimensional array of where each row is a
 * Vertex and each Column is an edge */

#include <stdio.h>
#include <stdlib.h>

#define V 7


typedef enum
{
  DISCONNECTED, CONNECTED
} connection;

typedef struct graph
{
  int num_vertices;             // number of vertices
  connection **adj;                   // adjacency matrix
} graph_t;


/* Create a new graph with given number of vertices */
graph_t
create_graph (int vertices)
{
  int i;

  graph_t *graph = (graph_t *) malloc (sizeof (graph_t));;
  graph->num_vertices = vertices;

  //int graph[verties][vertices];
  graph->adj = malloc (vertices * sizeof (connection *));
  for (i = 0; i < vertices; i++)
    graph->adj[i] = calloc (vertices, sizeof (connection));

  // All vertixes are connected with themselves
  for (i = 0; i < vertices; i++)
    graph->adj[i][i] = 1;

  return *graph;
}

/* Add an edge from src to dest in the adjacency matrix */
void
add_edge (graph_t graph, int src, int dest)
{
  graph.adj[src][dest] = CONNECTED;
}

/* Remove an edge from src to dest in the adjacency matrix */
void
remove_edge (graph_t graph, int src, int dest)
{
  graph.adj[src][dest] = DISCONNECTED;
}

/* Print the adjacency list of the graph */
void
display_graph (graph_t graph)
{
  int v, e;
  for (v = 0; v < graph.num_vertices; v++) {
    printf ("%d: ", v);
    for (e = 0; e < graph.num_vertices; e++)
      if (graph.adj[v][e])
        printf ("%d ", e);

    printf ("\n");
  }
}

/* Destroys the graph */
void
destroy_graph (graph_t graph)
{
  int i;
  for (i = 0; i < graph.num_vertices; i++)
    free (graph.adj[i]);
  free (graph.adj);
}


int
main ()
{
  printf ("Creating a directed graph: \n");
  graph_t graph = create_graph (V);

  add_edge (graph, 0, 1);
  add_edge (graph, 0, 4);
  add_edge (graph, 1, 2);
  add_edge (graph, 1, 3);
  add_edge (graph, 1, 4);
  add_edge (graph, 2, 3);
  add_edge (graph, 3, 4);
  add_edge (graph, 4, 0);
  add_edge (graph, 5, 0);
  add_edge (graph, 6, 0);

  display_graph (graph);

  printf ("Removing edges from vertix 1:\n");
  remove_edge (graph, 1, 2);
  remove_edge (graph, 1, 3);
  remove_edge (graph, 1, 4);

  display_graph (graph);
  destroy_graph (graph);

  return 0;
}
