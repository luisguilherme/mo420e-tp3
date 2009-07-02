#ifndef GHCTREEDEFINE
#define GHCTREEDEFINE
#include "ugraph.h"
#include "zib.h"
/* usado pelo odd min cut */

struct gh_adjac_nodes_t {
  struct Node *nd;
  struct gh_adjac_nodes_t *next;
};

typedef struct Node
{ long         id;
  /*node_type    type; */     /*usado pelo odd min cut */
  struct Edge  *first_edge;   /* in list of incident edges */
  struct Edge  *scan_ptr;     /* next edge to be scanned when node 
				 will be visited again */

  /* The entries bellow are for travelling through the Gomory Hu cut tree */

  /* Pointer to parent node in Gomory-Hu cut tree. Each node has
     exactly one parent, and possibly many children. */
  struct Node  *parent;

  double       mincap;    /* capacity of minimum cut between
			     node and parent in GH cut tree */

  /* Usado por insere_inequacao()
     Se diferente de zero, significa que a equação referente à aresta
     que liga este nó ao seu pai já foi inserida.
  */
  char inequacao_inserida;

  /* List of adjacent nodes in GH cut tree, including parent and all children. */
  struct gh_adjac_nodes_t  *gh_adjac_nodes; 

  /* Used for travelling by breadth first search */
  struct Node *path_predecessor; // when finding a path between two specific nodes
  char visited; // if this node has been already visited


  /* Subsequent entries for use by maxflow */
    long         dist;
    double       excess;
    struct Node  *bfs_link;     /* for one way BFS working queue */
    struct Node  *stack_link;   /* for stack of active nodes */
    BOOL         alive;    
    BOOL         unmarked;      /* while BFS in progress */

} node;


typedef struct Edge
  { node         *adjac; /* pointer to adjacent node */
    struct Edge  *next;  /* in incidence list of node 
			    from which edge is emanating */
    struct Edge  *back;  /* pointer to reverse edge */
    double       cap;
    double       rcap;   /* residual capacity */
  } edge;

typedef struct Graph
  { long    n_nodes;
    long    n_odd; /* usado pelo odd min cut */
    node    *nodes;
    long    n_edges;     /* number of edges with non-zero capacity */
    long    n_edges0;    /* number of all edges */
    edge    *edges;
  } graph;

void gr_generate_ghc_tree(graphtype *g,graphtype *tree);
#define  NILN (node *) 0
#define  NILE (edge *) 0
#endif
