#include "graph.h"

/* ------------------------------------------------------------------------------ */

int main (int argc, char **argv)
{
  int opt;
  FILE *fp = NULL;

  while ((opt = getopt(argc, argv, "f:h")) != -1)
    switch (opt)
    {
      case 'f':
        if(!(fp = fopen(optarg, "r")))
        {
          fprintf(stderr, "Error: unable to open file\n");
          exit(1);
        }
        break;
      case 'h':
      default:
        fprintf(stdout, "Usage: %s -f <file> [-h]\n", argv[0]);
        exit(0);
    }

  if (!fp)
  {
    fprintf(stdout, "Usage: %s -f <file> [-h]\n", argv[0]);
    exit(1);
  }

  // undirected graph
  graph_t *g1 = read_graph("und_graph", fp, 0);
  printf("-------------------------\n");
  printf("undirected graph example\n");
  printf("-------------------------\n");
  printf("\nformatted output:\n\n");
  print_graph(g1, 0);
  printf("\nnormal output:\n\n");
  write_graph(g1, stdout, 0);
  destroy_graph(g1);

  rewind(fp);

  // directed graph
  graph_t *g2 = read_graph("d_graph", fp, 1);
  printf("\n-------------------------\n");
  printf("directed graph example\n");
  printf("-------------------------\n");
  printf("\nformatted output:\n\n");
  print_graph(g2, 1);
  printf("\nnormal output:\n\n");
  write_graph(g2, stdout, 1);
  destroy_graph(g2);


  fclose(fp);
  return 0;
}
