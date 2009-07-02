#ifndef ODDGRAPHDEFINE
#define ODDGRAPHDEFINE
#include "zib.h"

typedef struct OddNode
  { long         id;
    int          type;
    struct OddEdge  *first_edge;
		 /* in list of incident edges */
    struct OddEdge  *scan_ptr;
		 /* next edge to be scanned when node 
		    will be visited again */
      /* subsequent entries for use by maxflow */
    long         dist;
    double       excess;
    struct OddNode  *bfs_link;     /* for one way BFS working queue */
    struct OddNode  *stack_link;   /* for stack of active nodes */
    BOOL         alive;    
    BOOL         unmarked;      /* while BFS in progress */
  } oddnode;

typedef struct OddEdge
  { oddnode         *adjac; /* pointer to adjacent node */
    struct OddEdge  *next;  /* in incidence list of node 
			    from which edge is emanating */
    struct OddEdge  *back;  /* pointer to reverse edge */
    double       cap;
    double       rcap;   /* residual capacity */
  } oddedge;

typedef struct OddGraph
  { long    n_nodes;
    long    n_odd;
    oddnode    *nodes;
    long    n_edges;
    oddedge    *edges;
  } oddgraph;

#define  ODDNILN (oddnode *) 0
#define  ODDNILE (oddedge *) 0

#endif
