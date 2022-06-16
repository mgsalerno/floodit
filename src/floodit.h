#ifndef _FLOODIT_H
#define _FLOODIT_H

/* ------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "queue-graph/src/graph.h"
#include "queue-graph/src/queue.h"

/* ------------------------------------------------------------------------------ */

#define idx(cols,i,j) ((cols) * (i) + (j))

/* ------------------------------------------------------------------------------ */

typedef struct node_t node_t ;

/* ------------------------------------------------------------------------------ */

struct node_t
{
  node_t *prev, *next ;
  node_t *parent ;
  graph_t *current_map ;
  int color ;
  double g ;
  double h ;
} ;

/* ------------------------------------------------------------------------------ */

graph_t *read_map (char *name, FILE *input) ;

int merge_initial_graph (graph_t *map) ;

int transfer_neighbours (graph_t *map, vertex_t *receiver, int color) ;

int color_propagation (graph_t *map, vertex_t *root, int color) ;

int find_solution (graph_t *map) ;

void queue_append_priority (node_t **queue, node_t *elem) ;

graph_t *clone_graph (graph_t *old) ;

node_t *search_children_color (node_t *node, int color) ;

int a_star (graph_t *map, node_t *open_nodes) ;

/* ------------------------------------------------------------------------------ */

#endif
