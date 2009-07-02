#include <stdio.h>
#include <stdlib.h>
#include "oddgraph.h"


static oddnode **active;
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


BOOL oddinit_maxflow (long n)
{
  active = (oddnode **) malloc ((n+1L) * sizeof (oddnode *));
    /* holds stacks of active nodes arranged by distances */ 
  if (active == (oddnode **) 0)
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


void oddfini_maxflow ()
{
  free (active);
  free (number);
} /* fini_maxflow */


void oddglobal_relabel (oddgraph *gr, oddnode *tptr)
{ 
  /* breadth first search to get exact distance labels
     from sink with reordering of stack of active nodes */

  oddnode *front, *rear, *nptr, **ptr;
  oddedge *eptr;
  long n, level, count, i;

  n = gr->n_nodes;
  for (nptr = &(gr->nodes[n-1L]); nptr >= gr->nodes; nptr--)
    { nptr->unmarked = TRUE;
      nptr->stack_link = ODDNILN;
      nptr->scan_ptr = nptr->first_edge;
    }
  tptr->unmarked = FALSE;
     /* initialize stack of active nodes */
  for (ptr = &(active[n]); ptr >= active; ptr--)
    *ptr = ODDNILN;
  for (i = 0L; i <= n; i++)
    number[i] = 0L;
  max_dist = 0L;
  count = 1L;     /* number of alive nodes */
  front = tptr;
  rear = front;

 bfs_next:
  level = rear->dist + 1L;
  eptr = rear->first_edge;
  while (eptr != ODDNILE)
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


double oddmaxflow (oddgraph *gr, oddnode *s_ptr, oddnode *t_ptr)
{
  /* Determines maximum flow and minimum cut between nodes
     s (= *s_ptr) and t (= *t_ptr) in an undirected graph  

     References:
     ----------
     A. Goldberg/ E. Tarjan: "A New Approach to the
     Maximum Flow Problem", in Proc. 18th ACM Symp. 
     on Theory of Computing, 1986.
  */
  oddnode *aptr, *nptr, *q_front, *q_rear;
  /*oddnode **ptr;*/
  oddedge *eptr/*, *my_scan_ptr*/;
  long n, m, level, i, n_discharge;
  double incre;
  long /*count, dist,*/ dmin/*, *upper, *lower*/;
  double cap;
  /*char any;*/
   
  /* node ids range from 1 to n, node array indices  
     range from 0 to n-1                             */

  n = gr->n_nodes;
  for (nptr = &(gr->nodes[n-1L]); nptr >= gr->nodes; nptr--)
    {  nptr->scan_ptr = nptr->first_edge;
       if (nptr->scan_ptr == ODDNILE)
	{ fprintf (stderr, "isolated node in input graph (maxflow.c)\n");
	  return (FALSE);
        }
       nptr->excess = 0.0L;
       nptr->stack_link = ODDNILN;
       nptr->alive = TRUE;
       nptr->unmarked = TRUE;
    }
  m = gr->n_edges;
  for (eptr = &(gr->edges[2L*m-1L]); eptr >= gr->edges; eptr--)
     eptr->rcap = eptr->cap;
      
  for (i = n; i >= 0L; i--)
    { number[i] = 0L;
      active[i] = ODDNILN;
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
  while (eptr != ODDNILE)
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
  while (eptr != ODDNILE)
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
  bound = n - 1L; 
  n_discharge = 0L;

  /* main loop */

  do { /* get maximum distance active node */
       aptr = active[max_dist];
       while (aptr != ODDNILN)
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
             if (eptr->next == ODDNILE) 
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
			active[nptr->dist] = ODDNILN; 
		        nptr->alive = FALSE; 
			nptr->dist = n;
			--bound;
		      }
                   --number[aptr->dist];
		   active[aptr->dist] = ODDNILN;
		   aptr->alive = FALSE;
		   aptr->dist = n;
		   --bound;
		   goto node_ready;
                 }
	        else
		 { /* determine new label value */
                   dmin = n;
		   aptr->scan_ptr = ODDNILE;
		   eptr = aptr->first_edge;
		   while (eptr != ODDNILE)
		      { if (eptr->adjac->dist < dmin
			    && eptr->rcap > EPS)
                         { dmin = eptr->adjac->dist;
			   if (aptr->scan_ptr == ODDNILE)
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
	      oddglobal_relabel (gr, t_ptr);
            }
	   aptr = active[max_dist];
         } /* aptr != ODDNILN */ 
       --max_dist;
     } 
  while (max_dist > 0L);  

  return (t_ptr->excess - 1.0L);
}
