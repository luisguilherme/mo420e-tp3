/*

O codigo deste arquivo corresponde a geracao da arvore de
cortes de Gormory-Hu. Este arquivo foi adaptado de um disponibilizado
no site do ZIB: 
MATHPROG: A Collection of Codes for Solving Various 
          Mathematical Programming Problems 
          http://elib.zib.de/pub/Packages/mathprog/index.html
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ugraph.h"
#include "ghc_tree.h"


static node **active;
static long *number;
static long max_dist, bound;
static BOOL co_check;

  /* This maxflow version is for undirected graphs and
     computes maximum flow only, i.e. the resulting flow
     is not computed for all edges, the graph structure
     is expected to be initialized already, the function
     "init_maxflow" must be called first, then "maxflow"
     may be called any number of times, the function 
     "fini_maxflow" should be called after the final 
     maxflow call.                                      */


BOOL init_maxflow (long n)
{
  active = (node **) malloc ((n+1L) * sizeof (node *));
    /* holds stacks of active nodes arranged by distances */ 
  if (active == (node **) 0)
   { printf ("Unable to allocate memory\n");
     return (FALSE);
   }
  number = (long *) malloc ((n+1L) * sizeof (long));
    /* counts occurences of node distances in set 
       of alive nodes, i.e. nodes not contained in
       the set of nodes disconnected from the sink */ 
  if (number == (long *) 0)
   { printf ("Unable to allocate memory\n");
     return (FALSE);
   }
  co_check = TRUE;
  return (TRUE);

} /* init_maxflow */


void fini_maxflow ()
{
  free (active);
  free (number);

} /* fini_maxflow */


void global_relabel (graph *gr, node *tptr)
{ 
  /* breadth first search to get exact distance labels
     from sink with reordering of stack of active nodes */

  node *front, *rear, *nptr, **ptr;
  edge *eptr;
  long n, level, count, i;

  n = gr->n_nodes;
  for (nptr = &(gr->nodes[n-1L]); nptr >= gr->nodes; nptr--)
    { nptr->unmarked = TRUE;
      nptr->stack_link = NILN;
      nptr->scan_ptr = nptr->first_edge;
    }
  tptr->unmarked = FALSE;
     /* initialize stack of active nodes */
  for (ptr = &(active[n]); ptr >= active; ptr--)
    *ptr = NILN;
  for (i = 0L; i <= n; i++)
    number[i] = 0L;
  max_dist = 0L;
  count = 1L;     /* number of alive nodes */
  front = tptr;
  rear = front;

 bfs_next:
  level = rear->dist + 1L;
  eptr = rear->first_edge;
  while (eptr != NILE)
     { nptr = eptr->adjac;
       if (nptr->alive && nptr->unmarked
	               && eptr->back->rcap > EPS) 
	{ nptr->unmarked = FALSE;
	  nptr->dist = level;
	  ++count;
	  ++number[level];
	  if (nptr->excess > EPS)
           { nptr->stack_link = active[level];
	     active[level] = nptr;
	     max_dist = level;
           }
	  front->bfs_link = nptr;
	  front = nptr;
        }
       eptr = eptr->next;
     }
  if (front == rear)
    goto bfs_ready;
       
  rear = rear->bfs_link;
  goto bfs_next;

 bfs_ready: 

  if (count < bound)
   { /* identify nodes that are marked alive but have
	not been reached by BFS and mark them as dead  */
     for (nptr = &(gr->nodes[n-1L]); nptr >= gr->nodes; nptr--)
       if (nptr->unmarked && nptr->alive)
        { nptr->dist = n;
          nptr->alive = FALSE;
        }
     bound = count;
   }

} /* global_relabel */


double maxflow (graph *gr, node *s_ptr, node *t_ptr)
{
  /* Determines maximum flow and minimum cut between nodes
     s (= *s_ptr) and t (= *t_ptr) in an undirected graph  

     References:
     ----------
     A. Goldberg/ E. Tarjan: "A New Approach to the
     Maximum Flow Problem", in Proc. 18th ACM Symp. 
     on Theory of Computing, 1986.
  */
  node *aptr, *nptr, *q_front, *q_rear;
  /*node **ptr;*/
  edge *eptr;
  long n, m, m0, level, i, n_discharge;
  double incre;
  long /* count, dist,*/ dmin /*, *upper, *lower  */;
  double cap;
  /* char any; */
   
  /* node ids range from 1 to n, node array indices  
     range from 0 to n-1                             */

  n = gr->n_nodes;
  for (nptr = &(gr->nodes[n-1L]); nptr >= gr->nodes; nptr--)
    {  nptr->scan_ptr = nptr->first_edge;
       if (nptr->scan_ptr == NILE)
	{ fprintf (stderr, "isolated node in input graph (ghc_tree.c)\n");
	  return (FALSE);
        }
       nptr->excess = 0.0L;
       nptr->stack_link = NILN;
       nptr->alive = TRUE;
       nptr->unmarked = TRUE;
    }
  m = gr->n_edges;
  m0 = gr->n_edges0;
  for (eptr = &(gr->edges[m-1L]); eptr >= gr->edges; eptr--)
     eptr->rcap = eptr->cap;
  for (eptr = &(gr->edges[m0+m-1L]); eptr >= &(gr->edges[m0]); eptr--)
     eptr->rcap = eptr->cap;
      
  for (i = n; i >= 0L; i--)
    { number[i] = 0L;
      active[i] = NILN;
    }
  t_ptr->dist = 0L;

    /* breadth first search to get exact distances 
       from sink and for test of graph connectivity */

  t_ptr->unmarked = FALSE; 
  q_front = t_ptr;
  q_rear = q_front;
 bfs_next:
  level = q_rear->dist + 1L;
  eptr = q_rear->first_edge;
  while (eptr != NILE)
     { if (eptr->adjac->unmarked && eptr->back->rcap > EPS)
        { nptr = eptr->adjac;
	  nptr->unmarked = FALSE;
	  nptr->dist = level;
	  ++number[level];
          q_front->bfs_link = nptr;
	  q_front = nptr;
        }
       eptr = eptr->next;
     }
  if (q_rear == q_front)
    goto bfs_ready;

  q_rear = q_rear->bfs_link;
  goto bfs_next;

 bfs_ready:
  if (co_check)
   { co_check = FALSE;
     for (nptr = &(gr->nodes[n-1]); nptr >= gr->nodes; --nptr)
       if (nptr->unmarked)
        { fprintf (stderr,"Input graph not connected\n");
          return (-1.0L);
        }
   }


  s_ptr->dist = n; /* number[0] and number[n] not required */
  t_ptr->dist = 0L;
  t_ptr->excess = 1.0L;  /* to be subtracted again */


  /* initial preflow push from source node */

  max_dist = 0L;  /* = max_dist of active nodes */
  eptr = s_ptr->first_edge;
  while (eptr != NILE)
     { nptr = eptr->adjac;
       cap = eptr->rcap;
       nptr->excess += cap;
       s_ptr->excess -= cap;
       eptr->back->rcap += cap;
       eptr->rcap = 0.0L;

       if (nptr != t_ptr && nptr->excess <= cap + EPS) 
        { /* push node nptr onto stack for nptr->dist,
	     but only once in case of double edges     */
	  nptr->stack_link = active[nptr->dist];
	  active[nptr->dist] = nptr;
          if (nptr->dist > max_dist)
	    max_dist = nptr->dist;
        }
       eptr = eptr->next;
     }

  s_ptr->alive = FALSE;
  bound = n;
  n_discharge = 0L;

  /* main loop */


  do { /* get maximum distance active node */

       aptr = active[max_dist];
       while (aptr != NILN)
	 { active[max_dist] = aptr->stack_link;
	   eptr = aptr->scan_ptr;

           edge_scan:  /* for current active node  */

	     nptr = eptr->adjac;

             if (nptr->dist == aptr->dist - 1L &&
		 eptr->rcap > EPS) 
              { incre = aptr->excess;

	        if (incre <= eptr->rcap)
	         { /* perform a non saturating push */
	           eptr->rcap -= incre;
	           eptr->back->rcap += incre;
		   aptr->excess = 0.0L;
                   nptr->excess += incre;
		   if (nptr->excess <= incre + EPS)
		    { /* push nptr onto active stack */
		      nptr->stack_link = active[nptr->dist];
		      active[nptr->dist] = nptr;
                    }
                   aptr->scan_ptr = eptr;
		   goto node_ready;
                 }
                else
	         { /* perform a saturating push */
		   incre = eptr->rcap;
		   eptr->back->rcap += incre; 
		   aptr->excess -= incre;
		   nptr->excess += incre;
		   eptr->rcap = 0.0L;
		   if (nptr->excess <= incre + EPS)
		    { /* push nptr onto active stack */
                      nptr->stack_link = active[nptr->dist];
		      active[nptr->dist] = nptr;
                    }
		   if (aptr->excess <= EPS)
		    { aptr->scan_ptr = eptr;
		      goto node_ready;
                    }
                 }
              }
             if (eptr->next == NILE) 
              { /* all incident arcs scanned, but node still
		   has positive excess, check if for all nptr       
		   nptr->dist != aptr->dist                  */

                if (number[aptr->dist] == 1L)
		 { /* put all nodes v with dist[v] >= dist[a] 
		      into the set of "dead" nodes since they
		      are disconnected from the sink          */
                     
		   for (nptr = &(gr->nodes[n-1L]);
			nptr >= gr->nodes; nptr--)
                     if (nptr->alive &&
			 nptr->dist > aptr->dist)
		      { --number[nptr->dist];
			active[nptr->dist] = NILN; 
		        nptr->alive = FALSE; 
			nptr->dist = n;
			--bound;
		      }
                   --number[aptr->dist];
		   active[aptr->dist] = NILN;
		   aptr->alive = FALSE;
		   aptr->dist = n;
		   --bound;
		   goto node_ready;
                 }
	        else
		 { /* determine new label value */
                   dmin = n;
		   aptr->scan_ptr = NILE;
		   eptr = aptr->first_edge;
		   while (eptr != NILE)
		      { if (eptr->adjac->dist < dmin
			    && eptr->rcap > EPS)
                         { dmin = eptr->adjac->dist;
			   if (aptr->scan_ptr == NILE)
			     aptr->scan_ptr = eptr;
                         }
                        eptr = eptr->next;
                      }
		   if (++dmin < bound)
		    { /* ordinary relabel operation */
		      --number[aptr->dist];
		      aptr->dist = dmin;
		      ++number[dmin];
		      max_dist = dmin;
                      eptr = aptr->scan_ptr;
		      goto edge_scan;
                    }
                   else
		    { aptr->alive = FALSE;
		      --number[aptr->dist];
		      aptr->dist = n;
		      --bound;
		      goto node_ready;
                    }
                 }
              }
             else
	      { eptr = eptr->next;
                goto edge_scan;
              }

          node_ready: 
	   ++n_discharge;
	   if (n_discharge == n)
	    { n_discharge = 0L;
	      global_relabel (gr, t_ptr);
            }
	   aptr = active[max_dist];
         } /* aptr != NILN */ 
       --max_dist;
     } 
  while (max_dist > 0L);  
  return (t_ptr->excess - 1.0L);
}

BOOL ghc_tree (graph *gr)
{
  /* Determines Gomory/Hu cut tree for input graph with
     capacitated edges, the tree structures is represented
     by parent pointers which are part of the node structure,
     the capacity of a tree edge is stored at the child node,
     the root of the cut tree is the first node in the list
     of graph nodes (&gr->nodes[0]). The implementation is
     described in [1].
     
     References:
     ----------
     1) D. Gusfield: "Very Simple Algorithms and Programs for
        All Pairs Network Flow Analysis", Computer Science Di-
        vision, University of California, Davis, 1987.
     
     2) R.E. Gomory and T.C. Hu: "Multi-Terminal Network Flows",
	SIAM J. Applied Math. 9 (1961), 551-570.

   */

  node *nptr, *nptr1, *nptrn, *sptr, *tptr;
  /* edge *eptr; */
  long n, m;
  double maxfl;

  
  n = gr->n_nodes;
  m = gr->n_edges;

  if (! init_maxflow (n))
    return (FALSE);

  nptr1 = gr->nodes;
  nptrn = &(gr->nodes[n-1L]);
  for (nptr = nptrn; nptr >= nptr1; nptr--)
    nptr->parent = nptr1;

  for (sptr = &(gr->nodes[1L]); sptr <= nptrn; sptr++)
    { tptr = sptr->parent;
      maxfl = maxflow (gr, sptr, tptr);
      if (maxfl < 0L)
	return (FALSE);
      
      sptr->mincap = maxfl;
      for (nptr = &(gr->nodes[1L]); nptr <= nptrn; nptr++)
	if (nptr != sptr &&  
	    ! nptr->alive && nptr->parent == tptr)
          nptr->parent = sptr;
      if (! tptr->parent->alive)
       { sptr->parent = tptr->parent;
	 tptr->parent = sptr;
	 sptr->mincap = tptr->mincap;
	 tptr->mincap = maxfl;
       }
    }

  fini_maxflow ();
  return TRUE;
} /* ghc_tree */


/* begin of ghc_tree_main */
#define NO_MEM  fprintf(stderr,"Unable to allocate memory\n")



BOOL alloc_graph (long n, long m, graph **gr)
{
  if ((*gr = (graph *) malloc (sizeof (graph))) == (graph *) 0)
    return (FALSE);
  if (((*gr)->nodes = (node *) malloc (n * sizeof (node))) 
      == (node *) 0)
   { free (*gr);
     return (FALSE);
   }
  if (((*gr)->edges = (edge *) malloc (2L * m * sizeof (edge)))
      == (edge *) 0)
   { free (*gr);
     free ((*gr)->nodes);
     return (FALSE);
   }
  return (TRUE);
}

void dealloc_graph (graph *gr)
{
  free (gr->nodes);
  free (gr->edges);
  free (gr);
}


/* assegure que passou o cleangraph antes de chamar esta rotina 
   (ou assegure que o grafo nao contem "buracos") */ 
graph *get_graph (graphtype *g)
{
  long n, m, m0, i, j, nod1, nod2;
  double cap;
  node *nptr, *nptr1, *nptr2;
  edge *eptr1, *eptr2;
  graph *gr;
  

  n = g->nvertices;
  m = g->nedges;
  m0 = 0;

  if (! alloc_graph (n, m, &gr))
   { NO_MEM;
     return ((graph *) 0);
   }

  gr->n_nodes = n;
  gr->n_edges0 = m;

  for (i = n, nptr = &(gr->nodes[n-1]); i > 0L; --nptr, --i) {
    nptr->id = i;
    nptr->first_edge = NILE;
    nptr->gh_adjac_nodes = NULL;
  }

  eptr1 = &(gr->edges[0L]);
  eptr2 = &(gr->edges[m]);
  for (j = gr_getfirstedge(g); j!=-1; j=gr_getnextedge(g,j)) {

    nod1 = g->edge[j].u+1;
    nod2 = g->edge[j].v+1;
    cap = g->edge[j].weight;

    if (nod1 < 1 || nod1 > n)
      { fprintf (stderr,
		 "invalid node1 id %ld in input graph\n", nod1);
      dealloc_graph (gr);
      return ((graph *) 0);
      }
    if (nod2 < 1 || nod2 > n)
      { fprintf (stderr, 
		 "invalid node2 id %ld in input graph\n", nod2);
      dealloc_graph (gr);
      return ((graph *) 0);
      }
    if (nod1 == nod2)
      { fprintf (stderr,
		 "warning: loop on node %ld in input graph\n", nod1);
      /*dealloc_graph (gr);
	return ((graph *) 0);*/
      m0++; 
      continue;
      }



    if (cap < EPS) cap = DELTA; 

    --nod1;
    --nod2;
    nptr1 = &(gr->nodes[nod1]);
    nptr2 = &(gr->nodes[nod2]);
    eptr1->adjac = nptr2;
    eptr2->adjac = nptr1;
    eptr1->cap = cap;
    eptr2->cap = cap;
    eptr1->back = eptr2;
    eptr2->back = eptr1;
    if (nptr1->first_edge == NILE)
      { nptr1->first_edge = eptr1;
      eptr1->next = NILE;
      }
    else
      { eptr1->next = nptr1->first_edge;
      nptr1->first_edge = eptr1;
      }
    if (nptr2->first_edge == NILE)
      { nptr2->first_edge = eptr2;
      eptr2->next = NILE;
      }
    else
      { eptr2->next = nptr2->first_edge;
      nptr2->first_edge = eptr2;
      }
    ++eptr1;
    ++eptr2;
  }
  gr->n_edges = m - m0;
  return gr;
}


void auxgenerate_ghc_tree (graph *gr,graphtype *g,graphtype *tree)
{
  node *nptr;
  int i,u,v;
  char edgename[100];
  gr_initgraph(tree,g->nvertices,g->nvertices-1);
  gr_setgraphname(tree,"Gomory Cut Tree");
  for (i=gr_getfirstvertex(g);i!=-1;i=gr_getnextvertex(g,i)) {
    gr_insertvertex(tree,g->vertex[i].vertexname);
  }
  for (nptr = &(gr->nodes[gr->n_nodes-1L]);
       nptr >= &(gr->nodes[1L]); nptr--){
    u = gr_getvertexindex(tree,g->vertex[(int) nptr->id-1].vertexname);
    v = gr_getvertexindex(tree,g->vertex[(int) nptr->parent->id-1].vertexname);
    sprintf(edgename,"(%s,%s,%lf)",tree->vertex[u].vertexname,
	    tree->vertex[v].vertexname,nptr->mincap);
    gr_insertedge(tree,edgename,u,v,(double) nptr->mincap);
  }
}  

void gr_generate_ghc_tree(graphtype *g,graphtype *tree){
  graph *gr;
  graphtype copia;
  if (g->sequencial) {
    gr = get_graph(g);
    if (gr == (graph *) 0)
      exit (1);
    ghc_tree (gr);
    auxgenerate_ghc_tree (gr,g,tree);
    dealloc_graph (gr);
  } else {
    if (!gr_cleangraph(&copia,g)) {
      printf("Problemas de alocacao de memoria (generate_ghc_tree)\n");
      return;
    }
    gr = get_graph(&copia);
    if (gr == (graph *) 0)
      exit (1);
    ghc_tree (gr);
    auxgenerate_ghc_tree (gr,&copia,tree);
    gr_closegraph(&copia);
    dealloc_graph (gr);
  }
}

