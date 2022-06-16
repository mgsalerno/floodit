#ifndef _GRAPH_H
#define _GRAPH_H

/* ------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "queue.h"

/* ------------------------------------------------------------------------------ */

typedef struct graph_t graph_t ;
typedef struct vertex_t vertex_t ;
typedef struct edge_t edge_t ;

/* ------------------------------------------------------------------------------
 * structure: graph
 * ------------------------------------------------------------------------------
 * vertices: graph vertices
 * name: graph name
 * size: graph size (number of vertices)
 * ------------------------------------------------------------------------------ */

struct graph_t
{
  vertex_t *vertices ;
  char *name ;
  int size ;
} ;

/* ------------------------------------------------------------------------------
 * structure: vertex
 * ------------------------------------------------------------------------------
 * prev: pointer to the previous vertex
 * next: pointer to the next vertex
 * edges: pointer to a list of edges that connect to the vertex
 * value: vertex value (generic)
 * degree: current vertex degree
 * id: vertex id (must be unique)
 * ------------------------------------------------------------------------------ */

struct vertex_t
{
  vertex_t *prev, *next ;
  edge_t *edges ;
  int value ;
  int degree ;
  int id ;
} ;

/* ------------------------------------------------------------------------------
 * structure: edge
 * ------------------------------------------------------------------------------
 * prev: pointer to the previous edge
 * next: pointer to the next edge
 * vertex: vertex connected to this edge
 * ------------------------------------------------------------------------------ */

struct edge_t
{
  edge_t *prev, *next ;
  vertex_t *vertex ;
} ;

/* ------------------------------------------------------------------------------
 * function: create_graph
 * ------------------------------------------------------------------------------
 * creates a graph with a given name
 *
 * name: name of the graph to be created
 *
 * returns: pointer to the created graph
 * ------------------------------------------------------------------------------ */

graph_t *create_graph (char *name) ;

/* ------------------------------------------------------------------------------
 * function: add_vertex
 * ------------------------------------------------------------------------------
 * inserts a vertex in the graph
 *
 * g: graph in which the vertex will be inserted
 * value: vertex value
 * id: vertex id
 *
 * returns: pointer to the added vertex
 * ------------------------------------------------------------------------------ */

vertex_t *add_vertex (graph_t *g, int value, int id) ;

/* ------------------------------------------------------------------------------
 * function: remove_vertex
 * ------------------------------------------------------------------------------
 * removes a vertex from the graph
 *
 * g: graph from which the vertex will be removed
 * v: vertex to be removed
 * is_directed: indicates if the graph is directed (1) or not (0)
 *
 * returns: pointer to the removed vertex
 * ------------------------------------------------------------------------------ */

vertex_t *remove_vertex (graph_t *g, vertex_t *v, int is_directed) ;

/* ------------------------------------------------------------------------------
 * function: add_edge
 * ------------------------------------------------------------------------------
 * inserts an edge from v1 to v2. If the graph is undirected, this function
 * must be called for both ends to create a bidirectional edge.
 * Example: you need to add from A->B and from B->A.
 *
 * v1: vertex that will receive v2 in its neighbourhood
 * v2: vertex that will be added to the neighbourhood of v1
 *
 * returns: 0 if an error has ocurred or 1 if no errors
 * ------------------------------------------------------------------------------ */

int add_edge (vertex_t *v1, vertex_t *v2) ;

/* ------------------------------------------------------------------------------
 * function: remove_edge
 * ------------------------------------------------------------------------------
 * removes the edge from v1 to v2. If the graph is undirected, this function
 * must be called for both ends to remove the edge for each node.
 * Example: you need to remove from A->B and from B->A.
 *
 * v1: vertex that will have v2 removed from its neighbourhood
 * v2: vertex that will be removed from the neighbourhood of v1
 *
 * returns: pointer to the removed edge
 * ------------------------------------------------------------------------------ */

edge_t *remove_edge (vertex_t *v1, vertex_t *v2) ;

/* ------------------------------------------------------------------------------
 * function: search_edge
 * ------------------------------------------------------------------------------
 * search for a specific edge in the vertex
 *
 * e: edge to search
 * v: vertex to which the edge connects
 *
 * returns: pointer to the edge found or NULL if not found
 * ------------------------------------------------------------------------------ */

edge_t *search_edge (edge_t *e, vertex_t *v) ;

/* ------------------------------------------------------------------------------
 * function: print_graph
 * ------------------------------------------------------------------------------
 * prints the graph with formatted output
 *
 * g: graph to be printed
 * is_directed: indicates if the graph is directed (1) or not (0)
 *
 * The print_graph function behaves differently if the graph is directed
 * or undirected.
 * ------------------------------------------------------------------------------ */

void print_graph (graph_t *g, int is_directed) ;

/* ------------------------------------------------------------------------------
 * function: edge_count
 * ------------------------------------------------------------------------------
 * counts the number of edges in a graph
 *
 * g: graph to have the edges counted
 * is_directed: indicates if the graph is directed (1) or not (0)
 *
 * The edge_count function behaves differently if the graph is directed
 * or undirected.
 *
 * returns: the number of edges in that graph
 * ------------------------------------------------------------------------------ */

int edge_count (graph_t *g, int is_directed) ;

/* ------------------------------------------------------------------------------
 * function: get_vertex_by_id
 * ------------------------------------------------------------------------------
 * searches for a vertex with a specific id in a graph
 *
 * g: graph in which the vertex will be searched
 * id: vertex id to be found
 *
 * returns: pointer to the vertex found or NULL if not found
 * ------------------------------------------------------------------------------ */

vertex_t *get_vertex_by_id (graph_t *g, int id) ;

/* ------------------------------------------------------------------------------
 * function: search_neighbourhood
 * ------------------------------------------------------------------------------
 * verifies if if v1 has v2 as a neighbour.
 *
 * v1: vertex which the neighbourhood will be searched
 * v2: vertex to search in the neighbourhood of v1
 *
 * returns: 1 if there is neighbourhood or 0 if there is not
 * ------------------------------------------------------------------------------ */

int search_neighbourhood (vertex_t *v1, vertex_t *v2) ;

/* ------------------------------------------------------------------------------
 * function: search_vertex_in_array
 * ------------------------------------------------------------------------------
 * searches for a vertex in an array of vertices
 *
 * v: vertex to be found
 * array: array of vertices
 * array_size: size of the array
 *
 * returns: 1 if the vertex is found or 0 if it is not found
 * ------------------------------------------------------------------------------ */

int search_vertex_in_array (vertex_t *v, vertex_t **array, int array_size) ;

/* ------------------------------------------------------------------------------
 * function: read_graph
 * ------------------------------------------------------------------------------
 * reads a graph from an input
 *
 * name: name of the graph
 * input: input from which the graph will be read
 * is_directed: indicates if the graph is directed (1) or not (0)
 *
 * The read_graph function behaves differently if the graph is directed
 * or undirected.
 *
 * returns: pointer to the read graph
 * ------------------------------------------------------------------------------ */

graph_t *read_graph (char *name, FILE *input, int is_directed) ;

/* ------------------------------------------------------------------------------
 * function: write_graph
 * ------------------------------------------------------------------------------
 * writes a graph in the same format used in read_graph to an output
 *
 * g: graph that will be written
 * output: output in which the graph will be written
 * is_directed: indicates if the graph is directed (1) or not (0)
 *
 * The write_graph function behaves differently if the graph is directed
 * or undirected.
 *
 * returns: pointer to the written graph or NULL if an error has ocurred
 * ------------------------------------------------------------------------------ */

graph_t *write_graph (graph_t *g, FILE *output, int is_directed) ;

/* ------------------------------------------------------------------------------
 * function: destroy_graph
 * ------------------------------------------------------------------------------
 * deallocate all the memory used in a graph
 *
 * g: graph to have the memory deallocated
 *
 * returns: 0 if an error has ocurred or 1 if no errors
 * ------------------------------------------------------------------------------ */

int destroy_graph (graph_t *g) ;

/* ------------------------------------------------------------------------------ */

#endif
