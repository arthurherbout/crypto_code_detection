#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


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
  bool visited;                 // is the node represented by this list visited
  adj_node_t *head;             // first node in the linked list
} adj_list_t;

typedef struct graph
{
  graph_direction_type direction;       // type (directed or undirected)
  int num_vertices;             // number of vertices
  adj_list_t *list;             // adjacency list array
} graph_t;

// queue for breadth first search
typedef struct queue_node
{
  adj_node_t *node;
  struct queue_node *next;
} queue_node_t;

typedef struct queue
{
  queue_node_t *head;
  queue_node_t *last;
} queue_t;


adj_node_t * create_node (int value);
graph_t * create_graph (int num, graph_direction_type direction);
void add_edge (graph_t * graph, int src, int dest);
void display_graph (graph_t graph);
adj_node_t * depth_first_search (graph_t * graph, adj_node_t * curr, int v,
                                 int s);
adj_node_t * breadth_first_search (graph_t * graph, int s);
void unset_visited (graph_t * graph);
static void enqueue_node (queue_t * nq, adj_node_t * node);
static adj_node_t * dequeue_node (queue_t * nq);


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

  /* Create an array of adjacency lists and an array of visited vertices */
  graph->list = (adj_list_t *) malloc (sizeof (adj_list_t));
  //graph->visited = (int *) malloc (sizeof (int));
  for (c = 0; c < num; c++) {
    graph->list[c].head = NULL;
    graph->list[c].visited = false;
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

/* Search for node with number s by depth first */
adj_node_t *
depth_first_search (graph_t * graph, adj_node_t * curr, int v, int s)
{
  printf ("visited: %d\n", v);
  graph->list[v].visited = true;      // mark the node visited to avoid infinite loops

  while (curr) {    // search all adjacent nodes
    if (!graph->list[curr->vertex].visited) {
      if (curr->vertex == s)          // found it!
        return curr;

      depth_first_search (graph, graph->list[curr->vertex].head,
                          curr->vertex, s);
    }
    curr = curr->next;
  }

  return NULL;
}

/* Search for node with number s by breadth first */
adj_node_t *
breadth_first_search (graph_t * graph, int s)
{
  queue_t tmp_q;
  tmp_q.head = NULL;
  tmp_q.last = NULL;
  adj_node_t * curr = graph->list->head;;

  while (curr) {
    if (!graph->list[curr->vertex].visited)
      enqueue_node (&tmp_q, curr);

    curr = curr->next;
  }

  while (tmp_q.head) {
    curr = dequeue_node (&tmp_q);

    printf ("visited: %d\n", curr->vertex);
    graph->list[curr->vertex].visited = true;      // mark the node visited to avoid infinite loops

    if (curr->vertex == s)           // found it!
      return curr;

    curr = graph->list[curr->vertex].head;
    while (curr) {
      if (!graph->list[curr->vertex].visited)
        enqueue_node (&tmp_q, curr);

      curr = curr->next;
    }
  }

  return NULL;
}

/* Mark all nodes as not visited */
void
unset_visited (graph_t * graph)
{
  int c;
  for (c = 0; c < graph->num_vertices; c++) {
    graph->list[c].visited = false;
  }
}

/* Add a node to the queue */
static void
enqueue_node (queue_t * nq, adj_node_t * node)
{
  queue_node_t * new_node = (queue_node_t *) malloc (sizeof (queue_node_t));
  new_node->node = node;
  new_node->next = NULL;

  if (nq->head) {
    nq->last->next = new_node;
    nq->last = new_node;
  } else {
    nq->head = new_node;
    nq->last = new_node;
  }
}

/* Get head of the queue */
static adj_node_t *
dequeue_node (queue_t * nq)
{
  adj_node_t *node = NULL;

  if (nq->head) {
    node = nq->head->node;
    nq->head = nq->head->next;
  }

  return node;
}


int
main ()
{
  printf ("Creating a directed graph: \n");
  graph_t *graph = create_graph (10, DIRECTED);
  add_edge (graph, 0, 2);
  add_edge (graph, 0, 5);
  add_edge (graph, 0, 8);
  add_edge (graph, 1, 6);
  add_edge (graph, 1, 5);
  add_edge (graph, 2, 1);
  add_edge (graph, 2, 3);
  add_edge (graph, 2, 4);
  add_edge (graph, 5, 7);
  add_edge (graph, 5, 9);

  display_graph (*graph);

  printf ("\n");

  printf ("dfs search for %d\n", 6);
  depth_first_search (graph, graph->list->head, 0, 6);
  unset_visited (graph);

  printf ("\ndfs search for %d\n", 9);
  depth_first_search (graph, graph->list->head, 0, 2);
  unset_visited (graph);

  printf ("\nbfs search for %d\n", 1);
  breadth_first_search (graph, 1);

  return 0;
}
