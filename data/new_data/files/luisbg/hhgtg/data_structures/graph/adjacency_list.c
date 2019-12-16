#include <stdlib.h>
#include <stdio.h>

typedef enum
{
  UNDIRECTED, DIRECTED
} graph_direction_type;

typedef struct adj_node
{
  int vertex;                   // index to adjacency list array
  struct adj_node *next;        // pointer to the next node
} adj_node_t;

typedef struct adj_list
{
  int num_members;              // number of members in the list
  adj_node_t *head;             // first node in the linked list
} adj_list_t;

typedef struct graph
{
  graph_direction_type direction;       // type (directed or undirected)
  int num_vertices;             // number of vertices
  adj_list_t *list;             // adjacency list array
} graph_t;


/* Create a new node with the specified value */
adj_node_t *
create_node (int value)
{
  adj_node_t *new_node = (adj_node_t *) malloc (sizeof (adj_node_t));
  new_node->vertex = value;
  new_node->next = NULL;

  return new_node;
}

/* Create a new graph with gtype direction and n vertices */
graph_t *
create_graph (int num, graph_direction_type direction)
{
  int c;

  graph_t *graph = (graph_t *) malloc (sizeof (graph_t));
  graph->direction = direction;
  graph->num_vertices = num;

  /* Create an array of adjacency lists */
  graph->list = (adj_list_t *) malloc (sizeof (adj_list_t));
  for (c = 0; c < num; c++) {
    graph->list[c].head = NULL;
  }

  return graph;
}

/* Add an edge from src to dest in the adjacency list */
void
add_edge (graph_t * graph, int src, int dest)
{
  adj_node_t *new_node = create_node (dest);
  new_node->next = graph->list[src].head;       // prepend node to vertex adjacency list
  graph->list[src].head = new_node;
  graph->list[src].num_members++;

  if (graph->direction == UNDIRECTED) {
    /* Add a mirror edge from dest to src */
    new_node = create_node (src);
    new_node->next = graph->list[dest].head;
    graph->list[dest].head = new_node;
    graph->list[dest].num_members++;
  }
}

/* Function to print the adjacency list of graph*/
void
display_graph (graph_t graph)
{
  int c;
  for (c = 0; c < graph.num_vertices; c++) {
    adj_node_t *head = graph.list[c].head;

    printf ("%d: ", c);
    while (head) {
      printf ("%d ", head->vertex);
      head = head->next;
    }

    printf ("\n");
  }
}

/*Destroys the graph*/
void
destroy_graph (graph_t * graph)
{
  int c;

  if (graph) {
    if (graph->list) {
      /*Free up the nodes */
      for (c = 0; c < graph->num_vertices; c++) {
        adj_node_t *node = graph->list[c].head;
        while (node) {
          adj_node_t *tmp = node;
          node = node->next;
          free (tmp);
        }
      }
    }

    /*Free the graph */
    free (graph);
  }
}

int
main ()
{
  printf ("Creating an undirected graph: \n");
  graph_t *undir_graph = create_graph (5, UNDIRECTED);
  add_edge (undir_graph, 0, 1);
  add_edge (undir_graph, 0, 4);
  add_edge (undir_graph, 1, 2);
  add_edge (undir_graph, 1, 3);
  add_edge (undir_graph, 1, 4);
  add_edge (undir_graph, 2, 3);
  add_edge (undir_graph, 3, 4);

  display_graph (*undir_graph);

  printf ("\nCreating a directed graph: \n");
  graph_t *dir_graph = create_graph (5, DIRECTED);
  add_edge (dir_graph, 0, 1);
  add_edge (dir_graph, 0, 4);
  add_edge (dir_graph, 1, 2);
  add_edge (dir_graph, 1, 3);
  add_edge (dir_graph, 1, 4);
  add_edge (dir_graph, 2, 3);
  add_edge (dir_graph, 3, 4);

  display_graph (*dir_graph);

  destroy_graph (undir_graph);
  destroy_graph (dir_graph);
  undir_graph = NULL;
  dir_graph = NULL;

  return 0;
}
