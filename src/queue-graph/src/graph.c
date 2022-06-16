#include "graph.h"

/* ------------------------------------------------------------------------------ */

graph_t *create_graph (char *name)
{
  graph_t *g = (graph_t *) malloc(sizeof(graph_t));

  g->name = (char *) calloc(strlen(name), sizeof(char));
  strcpy(g->name, name);
  g->vertices = NULL;
  g->size = 0;

  return g;
}

/* ------------------------------------------------------------------------------ */

vertex_t *add_vertex (graph_t *g, int value, int id)
{
  if (!g)
    return NULL;

  vertex_t *new_vertex = (vertex_t *) malloc(sizeof(vertex_t));

  g->size++;

  new_vertex->next = new_vertex->prev = NULL;
  new_vertex->edges = NULL;
  new_vertex->degree = 0;
  new_vertex->value = value;
  new_vertex->id = id;

  queue_append((queue_t **) &(g->vertices), (queue_t *) new_vertex);
  return new_vertex;
}

/* ------------------------------------------------------------------------------ */

vertex_t *remove_vertex (graph_t *g, vertex_t *v, int is_directed)
{
  if (!g || !v)
    return NULL;

  g->size--;

  while(v->edges) // removes all edges from the vertex
  {
    if (is_directed)
    {
      free(remove_edge(v->edges->vertex, v));
      free(queue_remove((queue_t **) &(v->edges), (queue_t *) v->edges));
    }
    else
    {
      free(remove_edge(v->edges->vertex, v));
      free(remove_edge(v, v->edges->vertex));
    }
  }

  return (vertex_t *) queue_remove((queue_t **) &(g->vertices), (queue_t *) v);
}

/* ------------------------------------------------------------------------------ */

int add_edge (vertex_t *v1, vertex_t *v2)
{
  if (!v1 || !v2)
    return 0;

  edge_t *new_edge = (edge_t *) malloc(sizeof(edge_t));

  new_edge->vertex = v2;
  new_edge->next = new_edge->prev = NULL;
  // inserts v2 in v1
  queue_append((queue_t **) &(v1->edges), (queue_t *) new_edge);
  v1->degree++;

  return 1;
}

/* ------------------------------------------------------------------------------ */

edge_t *remove_edge (vertex_t *v1, vertex_t *v2)
{
  if (!v1 || !v2)
    return NULL;

  edge_t *aux_edge;

  if ((aux_edge = search_edge(v1->edges, v2)))
  {
    v1->degree--;
    // removes v2 from v1
    return (edge_t *) queue_remove((queue_t **) &(v1->edges), (queue_t *) aux_edge);
  }

  return NULL;
}

/* ------------------------------------------------------------------------------ */

edge_t *search_edge (edge_t *e, vertex_t *v)
{
  if (!e || !v)
    return NULL;

  edge_t *edge_it = e;

  do
    if (edge_it->vertex == v) // if the edge is found
      return edge_it;
  while ((edge_it = edge_it->next) != e);

  return NULL;
}

/* ------------------------------------------------------------------------------ */

void print_graph (graph_t *g, int is_directed)
{
  if (!g)
    return;

  vertex_t *vertex_it = g->vertices;
  edge_t *edge_it;

  printf("graph: %s | nodes: %d | edges: %d\n", g->name, g->size, edge_count(g, is_directed));

  for (int i = 0; i < g->size; ++i)
  {
    printf("|(%d)[%2d]|", vertex_it->value, vertex_it->id);

    if ((edge_it = vertex_it->edges)) // if there are edges
      do
        printf("-> (%d)[%2d]", edge_it->vertex->value, edge_it->vertex->id);
      while ((edge_it = edge_it->next) != vertex_it->edges);

    vertex_it = vertex_it->next;
    printf("\n");
  }
}

/* ------------------------------------------------------------------------------ */

int edge_count (graph_t *g, int is_directed)
{
  vertex_t *vertex_it = g->vertices;
  int sum = 0;

  do
    sum += vertex_it->degree; // sum all the vertices degrees
  while ((vertex_it = vertex_it->next) != g->vertices);

  return is_directed ? sum : (sum/2);
}

/* ------------------------------------------------------------------------------ */

vertex_t *get_vertex_by_id (graph_t *g, int id)
{
  if (!g)
    return NULL;

  vertex_t *vertex_it = g->vertices;

  if (!g->vertices)
    return NULL;

  do
    if (vertex_it->id == id) // if the vertex has that id
      return vertex_it;
  while ((vertex_it = vertex_it->next) != g->vertices);

  return NULL;
}

/* ------------------------------------------------------------------------------ */

int search_neighbourhood (vertex_t *v1, vertex_t *v2)
{
  if (!v1 || !v2)
    return 0;

  edge_t *edge_it;

  if ((edge_it = v1->edges))
    do
      if (edge_it->vertex == v2) // checks if v1 has v2 as a neighbour
        return 1;
    while ((edge_it = edge_it->next) != v1->edges);

  return 0;
}

/* ------------------------------------------------------------------------------ */

int search_vertex_in_array (vertex_t *v, vertex_t **array, int array_size)
{
  if (!v || !array)
    return 0;

  for (int i = 0; i < array_size; ++i)
    if (array[i] == v) // if the vertex is found
      return 1;

  return 0;
}

/* ------------------------------------------------------------------------------ */

graph_t *read_graph (char *name, FILE *input, int is_directed)
{
  int rd, nodes[2];
  char *str = (char *) malloc(256);

  graph_t *g = create_graph(name);
  vertex_t *v1, *v2;

  while (fgets(str, 256, input))
  {
    if (str[0] != '\n')
    {
      rd = sscanf(str, "%d %d", &nodes[0], &nodes[1]);

      switch (rd)
      {
        case 1:
          if (!get_vertex_by_id(g, nodes[0]))
            add_vertex(g, nodes[0], nodes[0]);

          break;

        case 2:
          if (!(v1 = get_vertex_by_id(g, nodes[0])))
            v1 = add_vertex(g, nodes[0], nodes[0]);

          if (!(v2 = get_vertex_by_id(g, nodes[1])))
            v2 = add_vertex(g, nodes[1], nodes[1]);

          if (!search_neighbourhood(v1, v2))
            add_edge(v1, v2);

          if (!is_directed && !search_neighbourhood(v2, v1))
            add_edge(v2, v1);

          break;

        default:
          fprintf(stderr, "Error: Unable to read input\n");
          exit(1);
      }
    }
  }

  free(str);
  return g;
}

/* ------------------------------------------------------------------------------ */

graph_t *write_graph (graph_t *g, FILE *output, int is_directed)
{
  if (!g || !output)
    return NULL;

  edge_t *edge_it;
  vertex_t *vertex_it;
  vertex_t **visited = (vertex_t **) malloc(g->size * sizeof(vertex_t *));

  int i = 0, array_size = 0;

  if ((vertex_it = g->vertices))
    do
    {
      if ((edge_it = vertex_it->edges)) // if it has edges
        do
          // if the vertex was not written
          if (is_directed || !search_vertex_in_array(edge_it->vertex, visited, array_size))
            fprintf(output, "%d %d\n", vertex_it->id, edge_it->vertex->id);
        while ((edge_it = edge_it->next) != vertex_it->edges);
      else
        fprintf(output, "%d\n", vertex_it->id);

      visited[i++] = vertex_it;
      array_size++;
    }
    while ((vertex_it = vertex_it->next) != g->vertices);

  free(visited);
  return g;
}

/* ------------------------------------------------------------------------------ */

int destroy_graph (graph_t *g)
{
  if (!g)
    return 0;

  while (g->vertices) // while there is vertices to be removed
  {
    while (g->vertices->edges) // while there is edges to be removed
      free(queue_remove((queue_t **) &(g->vertices->edges), (queue_t *) g->vertices->edges));
    free(queue_remove((queue_t **) &(g->vertices), (queue_t *) g->vertices));
  }

  free(g->name);
  free(g);
  return 1;
}
