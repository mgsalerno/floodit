#include "floodit.h"

/* ------------------------------------------------------------------------------ */

graph_t *read_map (char *name, FILE *input)
{
  int height, width, colors;

  assert(fscanf(input, "%d %d %d", &height, &width, &colors));

  if (height <= 0 || width <= 0 || colors <= 0)
    return NULL;

  graph_t *map = create_graph(name);
  vertex_t **matrix_map = (vertex_t **) malloc(height * width * sizeof(vertex_t *));
  int color;

  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      assert(fscanf(input, "%d", &color));
      matrix_map[idx(width, i, j)] = add_vertex(map, color, idx(width, i, j));
    }
  }

  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      vertex_t *current = matrix_map[idx(width, i, j)];
      vertex_t *neighbour = NULL;

      if (i > 0) // has neighbour above
      {
        neighbour = matrix_map[idx(width, i - 1, j)];
        if (!search_neighbourhood(current, neighbour))
          add_edge(current, neighbour);
        if (!search_neighbourhood(neighbour, current))
          add_edge(neighbour, current);
      }

      if (i < height - 1) // has neighbour below
      {
        neighbour = matrix_map[idx(width, i + 1, j)];
        if (!search_neighbourhood(current, neighbour))
          add_edge(current, neighbour);
        if (!search_neighbourhood(neighbour, current))
          add_edge(neighbour, current);
      }

      if (j > 0) // has neighbour to the left
      {
        neighbour = matrix_map[idx(width, i, j - 1)];
        if (!search_neighbourhood(current, neighbour))
          add_edge(current, neighbour);
        if (!search_neighbourhood(neighbour, current))
          add_edge(neighbour, current);
      }

      if (j < width - 1) // has neighbour to the right
      {
        neighbour = matrix_map[idx(width, i, j + 1)];
        if (!search_neighbourhood(current, neighbour))
          add_edge(current, neighbour);
        if (!search_neighbourhood(neighbour, current))
          add_edge(neighbour, current);
      }
    }
  }

  free(matrix_map);
  return map;
}

/* ------------------------------------------------------------------------------ */

int merge_initial_graph (graph_t *map)
{
  if (!map)
    return 0;

  vertex_t *vertex_iterator = map->vertices;
  unsigned int size = map->size;

  // run through the graph and merge initial colors
  for (unsigned int i = 0; i < size; ++i)
  {
    transfer_neighbours(map, vertex_iterator, vertex_iterator->value);
    vertex_iterator = vertex_iterator->next;
  }

  return 1;
}

/* ------------------------------------------------------------------------------ */

int transfer_neighbours (graph_t *map, vertex_t *receiver, int color)
{
  if (!map || !receiver || color < 0)
    return 0;

  edge_t *iterator_edges = receiver->edges, *aux;
  unsigned int i = 0, size = receiver->degree;

  while (i < size)
  {
    if ((iterator_edges->vertex->value == color) && (aux = iterator_edges->vertex->edges))
    {
      do
      {
        if ((receiver->id != aux->vertex->id) && !search_neighbourhood(receiver, aux->vertex))
        {
          add_edge(receiver, aux->vertex);
          add_edge(aux->vertex, receiver);
        }
      }
      while ((aux = aux->next) != iterator_edges->vertex->edges);

      free(remove_vertex(map, iterator_edges->vertex, 0));

      i = 0;
      size = receiver->degree;
      iterator_edges = receiver->edges;
    }
    else
    {
      i++;
      iterator_edges = iterator_edges->next;
    }
  }

  return 1;
}

/* ------------------------------------------------------------------------------ */

int color_propagation (graph_t *map, vertex_t *root, int color)
{
  if (!map || !root || color < 0)
    return 0;

  root->value = color;
  transfer_neighbours(map, root, color);
  return 1;
}

/* ------------------------------------------------------------------------------ */

int a_star (graph_t *map, node_t *open_nodes)
{
  if (!map)
    return 0;

  node_t *node, *node_child;
  edge_t *node_edges;

  while (open_nodes)
  {
    // update head map
    node = open_nodes;
    node->current_map = clone_graph(node->current_map);
    color_propagation(node->current_map, node->current_map->vertices, node->color);

    node_edges = node->current_map->vertices->edges;

    // create all possible children
    if (node_edges)
    {
      do
      {
        node_child = search_children_color(node, node_edges->vertex->value);

        if (!node_child)
        {
          node_child = (node_t *) malloc(sizeof(node_t));
          node_child->next = node_child->prev = NULL;
          node_child->parent = node;
          node_child->color = node_edges->vertex->value;
          node_child->current_map = node_child->parent->current_map;

          // heuristic ------------------------------

          int i = 0, degree = 0;
          edge_t *e_it = node_child->current_map->vertices->edges;

          do
          {
            if (e_it->vertex->value == node_child->color)
            {
              i++;
              degree += e_it->vertex->degree - 1;
            }
          }
          while ((e_it = e_it->next) != node_child->current_map->vertices->edges);

          node_child->g = node->g + 2.0;
          node_child->h = (node_child->current_map->size - i) * 1.0;
          node_child->h -= degree * 0.95;

          if (node_child->h <= 0)
            node_child->h = 0.0;

          node_child->h += (node_child->current_map->size - i) * 1.0;

          // ----------------------------------------

          queue_append_priority(&(open_nodes), node_child);
        }
      }
      while ((node_edges = node_edges->next) != node->current_map->vertices->edges);
    }
    else
    {
      int size = 0;
      node_t *it = node;

      do
      {
        it = it->parent;
        size++;
      }
      while(it->parent);

      // print parents until root
      int *solution_array = malloc(size * sizeof(int));
      printf("%d\n", size);

      for (int i = 0; i < size; ++i)
      {
        solution_array[i] = node->color;
        node = node->parent;
      }

      for (int i = size - 1; i >= 0; --i)
        printf("%d ", solution_array[i]);
      printf("\n");

      free(solution_array);
      break;
    }

    queue_remove((queue_t **) &(open_nodes), (queue_t *) node);
  }

  return 1;
}

/* ------------------------------------------------------------------------------ */

node_t *search_children_color (node_t *node, int color)
{
  node_t *iterator = node;

  if (!iterator)
    return NULL;

  do
    if (iterator->color == color && iterator->parent == node)
      return iterator;
  while ((iterator = iterator->next) != node);

  return NULL;
}

/* ------------------------------------------------------------------------------ */

int find_solution (graph_t *map)
{
  // create search tree
  node_t *root = malloc(sizeof(node_t));
  root->parent = NULL;
  root->next = root->prev = root;
  root->g = 0.0;
  root->h = map->size;
  root->color = map->vertices->value;
  root->current_map = map;

  node_t *open_nodes = root;
  return a_star(map, open_nodes);
}

/* ------------------------------------------------------------------------------ */

void queue_append_priority (node_t **queue, node_t *elem)
{
  if (!queue || !elem)
  {
    fprintf(stderr, "Error: queue_append_priority\n");
    return ;
  }

  if (*queue)
  {
    node_t *iterator = (*queue);
    double elem_f = elem->g + elem->h, iterator_f;

    if(elem->h <= 1)
    {
      iterator_f = iterator->g + iterator->h;
      iterator = iterator->next;
    }
    else
    {
      do
      {
        iterator_f = iterator->g + iterator->h;
        iterator = iterator->next;
      }
      while ((iterator_f < elem_f) && (iterator != (*queue)));
    }

    elem->next = iterator;
    elem->prev = iterator->prev;
    iterator->prev->next = elem;
    iterator->prev = elem;

    if (((*queue)->g + (*queue)->h) > elem_f)
      (*queue) = elem;
  }
  else
  {
    (*queue) = elem;
    (*queue)->next = (*queue)->prev = elem;
  }

  return ;
}

/* ------------------------------------------------------------------------------ */

graph_t *clone_graph (graph_t *old)
{
  graph_t *new = create_graph("new");

  vertex_t *vertex_iterator = old->vertices;
  unsigned int size = old->size;

  vertex_t **v_old = (vertex_t **) malloc(sizeof(vertex_t *) * size);
  vertex_t **v_new = (vertex_t **) malloc(sizeof(vertex_t *) * size);
  int i = 0;

  do
  {
    v_new[i] = add_vertex(new, vertex_iterator->value, vertex_iterator->id);
    v_old[i] = vertex_iterator;
    i++;
  }
  while ((vertex_iterator = vertex_iterator->next) != old->vertices);

  edge_t *e;
  vertex_t *neighbour = NULL;

  for (int j = 0; j < i; ++j)
  {
    e = v_old[j]->edges;
    do
    {
      for (int k = 0; k < i; ++k)
      {
        if (v_new[k]->id == e->vertex->id)
        {
          neighbour = v_new[k];
          k = i;
        }
      }

      edge_t *aux_edge;
      aux_edge = (edge_t *) malloc(sizeof(edge_t));
      aux_edge->vertex = neighbour;
      aux_edge->next = aux_edge->prev = NULL;
      queue_append((queue_t **) &(v_new[j]->edges), (queue_t *) aux_edge);
      v_new[j]->degree++;
    }
    while ((e = e->next) != v_old[j]->edges);
  }

  free(v_old);
  free(v_new);
  return new;
}
