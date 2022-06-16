#include "floodit.h"

/* ------------------------------------------------------------------------------ */

int main ()
{
  graph_t *G;

  if (!(G = read_map("map", stdin)))
  {
    perror("unable to read input");
    exit(1);
  }

  merge_initial_graph(G);
  find_solution(G);
}
