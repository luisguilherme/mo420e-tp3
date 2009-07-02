
/*

Este arquivo foi adaptado do codigo do CONCORDE.

David Applegate, Robert Bixby, Vasek Chvatal, and William Cook 

*/



/****************************************************************************/
/*                                                                          */
/*  This file is part of CONCORDE                                           */
/*                                                                          */
/*  (c) Copyright 1995--1999 by David Applegate, Robert Bixby,              */
/*  Vasek Chvatal, and William Cook                                         */
/*                                                                          */
/*  Permission is granted for academic research use.  For other uses,       */
/*  contact the authors for licensing options.                              */
/*                                                                          */
/*  Use at your own risk.  We make no guarantees about the                  */
/*  correctness or usefulness of this code.                                 */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*         MINIMUM S-T CUTS IN DIRECTED AND UNDIRECTED GRAPHS               */
/*                                                                          */
/*  Written by:  Applegate, Bixby, Chvatal, and Cook                        */
/*  Date: July 25, 1996                                                     */
/*                                                                          */
/*  An implementation of the Push-Relabel Flow Algorithm described in       */
/*   A. Goldberg and R. Tarjan, "A new approach to the maximum-flow         */
/*   problem",  Journal of the ACM 35 (1988) 921-940.                       */
/*                                                                          */
/*    EXPORTED FUNCTIONS:                                                   */
/*                                                                          */
/*  int CCcut_mincut_st (int ncount, int ecount, int *elist, double *ecap,  */
/*      int s, int t, double *value, int **cut, int *cutcount)              */
/*    COMPUTES the min st-cut in a directed or undirected graph.            */
/*      -ncount is the number of nodes in the graph.                        */
/*      -ecount is the number of directed (undirected) edges.               */
/*      -elist gives the edges in node node format (interpreted as          */
/*           tail head when compiled for directed graphs).                  */
/*      -ecap gives the capacities of the edges.                            */
/*      -s is the name of the source node.                                  */
/*      -t is the name of the sink node.                                    */
/*      -value returns the capacity of the minimum cut.                     */
/*      -cut (if not NULL) returns a list of nodes in a a minimum cut (it   */
/*       returns the side that contains t); it will be allocated to an      */
/*       array of the appropriate size.                                     */
/*      -cutcount returns the number of nodes in the listed cut, if cut     */
/*       is not NULL (if cut is NULL, then cutcount can be NULL).           */
/*                                                                          */
/*    NOTES:                                                                */
/*      Returns 0 if it worked and 1 otherwise (for example, when one       */
/*      of the mallocs failed). The nodes in the graph should be named      */
/*      0 through #nodes - 1.                                               */
/*                                                                          */
/*      Define UNDIRECTED_GRAPH to compile the code for undirected          */
/*      graphs. (This appears to be the way to go for tsp instances.)       */
/*                                                                          */
/*      Two node selection rules are implemented: queue and highest         */
/*      label. One of QUEUE_PRF and HIGHEST_LABEL_PRF must be defined       */
/*      (but not both).                                                     */
/*                                                                          */
/*      The code can carry out global relabelings via a backwards           */
/*      breadth-first-search from the sink. The frequency of the            */
/*      relabelings is controlled by the defined constant                   */
/*      GLOBAL_RELABEL_FREQUENCY. A relabling will occur after each         */
/*      #nodes * GLOBAL_RELABEL_FREQUENCY nodes have been processed.        */
/*      A resonable choice for the constant is 1.                           */
/*                                                                          */
/*      Defining USE_GAP turns on the gap heuristic of Derigs and           */
/*      Meyer for determing nodes that can be labeled to ncount.            */
/*      This can be used with either the queue or highest label             */
/*      variants.                                                           */
/*                                                                          */
/*      To use this code for maxflows, allow nodes with labels up to        */
/*      2*ncount to become active, or implement an algorithm to             */
/*      decompose the preflow to create a flow.                             */
/*                                                                          */
/****************************************************************************/

#include "machdefs.h" 
#include "util.h"
#include "cut.h"
#include "ugraph.h"

#define UNDIRECTED_GRAPH

#define HIGHEST_LABEL_PRF
#undef  QUEUE_PRF

#define USE_GAP
#define GLOBAL_RELABEL_FREQUENCY 1.0

#define MINCUT_INFINITY (1<<30)
#define PRF_EPSILON 0.000000001
#define GOING_IN  0
#define GOING_OUT 1

#ifdef QUEUE_PRF
#define ADD_TO_ACTIVE(n) {                                                \
    if (!(n)->active) {                                                   \
        (n)->qnext = (concordenode *) NULL;                                       \
        if (qtail) {                                                      \
            qtail->qnext = (n);                                           \
        } else {                                                          \
            qhead = (n);                                                  \
        }                                                                 \
        qtail = (n);                                                      \
        (n)->active = 1;                                                  \
    }                                                                     \
}
#endif

#ifdef HIGHEST_LABEL_PRF
#define ADD_TO_ACTIVE(n) {                                                \
    if (!(n)->active) {                                                   \
        (n)->highnext = high[(n)->flowlabel];                             \
        high[(n)->flowlabel] = (n);                                       \
        if (G->highest < (n)->flowlabel)                                  \
            G->highest = (n)->flowlabel;                                  \
        (n)->active = 1;                                                  \
    }                                                                     \
}
#endif

#ifdef UNDIRECTED_GRAPH
#define RELABEL_BODY(n)                                                   \
    for (rele = (n)->out; rele; rele = rele->outnext) {                   \
        if (rele->cap - rele->flow > PRF_EPSILON &&                       \
                (relt = rele->ends[1]->flowlabel) < relm)                 \
            relm = relt;                                                  \
    }                                                                     \
    for (rele = (n)->in; rele; rele = rele->innext) {                     \
        if (rele->cap + rele->flow > PRF_EPSILON &&                       \
                 (relt = rele->ends[0]->flowlabel) < relm)                \
            relm = relt;                                                  \
    }                                                                     \
    (n)->flowlabel = ++relm;
#else
#define RELABEL_BODY(n)                                                   \
    for (rele = (n)->out; rele; rele = rele->outnext) {                   \
        if (rele->cap - rele->flow > PRF_EPSILON &&                       \
                (relt = rele->ends[1]->flowlabel) < relm)                 \
            relm = relt;                                                  \
    }                                                                     \
    for (rele = (n)->in; rele; rele = rele->innext) {                     \
        if (rele->flow > PRF_EPSILON &&                                   \
                 (relt = rele->ends[0]->flowlabel) < relm)                \
            relm = relt;                                                  \
    }                                                                     \
    (n)->flowlabel = ++relm;
#endif

#ifdef USE_GAP
#define RELABEL(n) {                                                      \
    int relm = MINCUT_INFINITY;                                           \
    concordeedge *rele;                                                   \
    int relt, relold = (n)->flowlabel;                                    \
                                                                          \
    RELABEL_BODY(n)                                                       \
                                                                          \
    if ((n)->levelprev) {                                                 \
        (n)->levelprev->levelnext = (n)->levelnext;                       \
    } else {                                                              \
        level[relold] = (n)->levelnext;                                   \
    }                                                                     \
    if ((n)->levelnext)                                                   \
        (n)->levelnext->levelprev = (n)->levelprev;                       \
                                                                          \
    if (relm < ncount) {                                                  \
        if (level[relm]) {                                                \
            level[relm]->levelprev = (n);                                 \
            (n)->levelnext = level[relm];                                 \
            (n)->levelprev = (concordenode *) NULL;                       \
            level[relm] = (n);                                            \
        } else {                                                          \
            (n)->levelprev = (concordenode *) NULL;                       \
            (n)->levelnext = (concordenode *) NULL;                       \
            level[relm] = (n);                                            \
        }                                                                 \
        if (!level[relold]) {                                             \
            relold++;                                                     \
            while (level[relold]) {                                       \
                concordenode *relno;                                      \
                for (relno = level[relold]; relno;                        \
                                            relno = relno->levelnext) {   \
                    relno->flowlabel = ncount;                            \
                }                                                         \
                level[relold] = (concordenode *) NULL;                    \
                relold++;                                                 \
            }                                                             \
        }                                                                 \
    }                                                                     \
}
#else
#define RELABEL(n) {                                                      \
    int relm = MINCUT_INFINITY;                                           \
    concordeedge *rele;                                                   \
    int relt;                                                             \
                                                                          \
    RELABEL_BODY(n)                                                       \
}
#endif /* USE_GAP */

typedef struct concordeedge {
    struct concordenode     *ends[2];
    struct concordeedge     *innext;
    struct concordeedge     *outnext;
    double          cap;
    double          flow;
} concordeedge;

typedef struct concordenode {
    struct  concordenode    *qnext;
    struct  concordenode    *tnext;
#ifdef USE_GAP
    struct  concordenode    *levelnext;
    struct  concordenode    *levelprev;
#endif
#ifdef HIGHEST_LABEL_PRF
    struct  concordenode    *highnext;
#endif
    struct  concordeedge    *in;
    struct  concordeedge    *out;
    struct  concordeedge    *incurrent;
    struct  concordeedge    *outcurrent;
    double           excess;
    int              magiclabel;
    int              flowlabel;
    char             inout;
    char             active;
} concordenode;

typedef struct concordegraph {
    struct  concordenode    *nodelist;
    struct  concordeedge    *edgelist;
#ifdef USE_GAP
    struct  concordenode   **level;
#endif
#ifdef HIGHEST_LABEL_PRF
    struct  concordenode   **high;
    int              highest;
#endif
    int              nnodes;
    int              nedges;
    int              magicnum;
} concordegraph;



static void
    setlabels (concordegraph *G, concordenode *s, concordenode *t),
    backwards_bfs (concordenode *s, int K, concordegraph *G),
    init_graph (concordegraph *G),
    free_graph (concordegraph *G);
static int
    grab_the_cut (concordegraph *G, concordenode *n, int *cut, int *cutcount);
static int
    buildgraph (concordegraph *G, graphtype *inputgraph);
static double
    flow (concordegraph *G, concordenode *s, concordenode *t);
int CCcut_mincut_st (graphtype *inputgraph,
    int s, int t, double *value, int *cut, int *cutcount);


boolean gr_min_st_cut(graphtype *g,int s_g,int t_g,
		   graphtype *stcut,double *cutweight)
{
  graphtype gcopy;
  int i,v,s,t,x,y,e,*cut,cutcount;
  if ((!gr_existsvertex(g,s_g)) || (!gr_existsvertex(g,t_g))) {
    gr_erro("Erro (min_st_cut) vertices para corte invalidos.");
    return(false);
  }
  cut = (int *) malloc(g->nvertices*sizeof(int));
  if (cut==NULL) {
    printf("Falta de memoria em min_st_cut\n");
    return(false);
  }
  if (!gr_cleangraph(&gcopy,g)){
    gr_erro("Erro (min_st_cut) nao consegui fazer copia sequencial de g");
    free(cut);
    return(false);
  }
  s = gr_getvertexindex(&gcopy,g->vertex[s_g].vertexname);
  t = gr_getvertexindex(&gcopy,g->vertex[t_g].vertexname);
  if ((s==-1) || (t==-1)) {
    printf("Erro (min_st_cut) vertices %s e %s invalidos no grafo.",
	     g->vertex[s_g].vertexname,g->vertex[t_g].vertexname);
    gr_closegraph(&gcopy);
    free(cut);
    return(false);
  }
  if (CCcut_mincut_st (&gcopy,s,t,cutweight,cut,&cutcount)) {
    printf("Erro na execucao do mincut_st\n");
    gr_closegraph(&gcopy);
    free(cut);
    return(false);
  }
  if (!gr_initgraph(stcut,gcopy.nvertices,gcopy.nedges)){
    printf("Erro na execucao do initgraph em mincut_st\n");
    gr_closegraph(&gcopy);
    free(cut);
    return(false);
  }
  for (v=gr_getfirstvertex(&gcopy);v!=-1;v=gr_getnextvertex(&gcopy,v)){
    x = gr_insertvertex(stcut,gcopy.vertex[v].vertexname);
    gr_setvertexcolor(&gcopy,v,RED);
    gr_setvertexcolor(stcut,x,RED);
    gr_setvertexposition(stcut,x,gcopy.vertex[v].pos_x,gcopy.vertex[v].pos_y);
  }
  for (i=0;i<cutcount;i++) {
    gr_setvertexcolor(&gcopy,cut[i],BLUE);
    x = gr_getvertexindex(stcut,gcopy.vertex[cut[i]].vertexname);
    gr_setvertexcolor(stcut,x,BLUE);
  }
  for (e=gr_getfirstedge(&gcopy);e!=-1;e=gr_getnextedge(&gcopy,e)){
    if (gcopy.vertex[gcopy.edge[e].u].color!=gcopy.vertex[gcopy.edge[e].v].color){
      x = gr_getvertexindex(stcut,gcopy.vertex[gcopy.edge[e].u].vertexname);
      y = gr_getvertexindex(stcut,gcopy.vertex[gcopy.edge[e].v].vertexname);
      i = gr_insertedge(stcut,"",x,y,gcopy.edge[e].weight);
      stcut->edge[i].color = GREEN;
    }
  }
  gr_closegraph(&gcopy);
  free(cut);
  return(true);
}


boolean gr_get_min_st_cut(graphtype *g,int s_g,int t_g,
		   int *cutedges,int *ncutedges,double *cutweight)
{
  graphtype *gcopy;
  int i,v,s,t,x,y,e,f,*cut,cutcount;
  if ((!gr_existsvertex(g,s_g)) || (!gr_existsvertex(g,t_g))) {
    gr_erro("Erro (min_st_cut) vertices para corte invalidos.");
    return(false);
  }
  
  /* vai armazenar os vertices de um dos lados do corte */
  cut = (int *) malloc(g->nvertices*sizeof(int));
  if (cut==NULL) {
    printf("Falta de memoria em min_st_cut\n");
    return(false);
  }
  if (g->sequencial) {
    gcopy = g;
    s = s_g;
    t = t_g;
  } else {
    gcopy = (graphtype *) malloc(sizeof(graphtype));
    if (gcopy==NULL) {
      free(cut);
      return(false);
    }
    if (!gr_cleangraph(gcopy,g)){
      gr_erro("Erro (min_st_cut) nao consegui fazer copia sequencial de g");
      free(cut);
      free(gcopy);
      return(false);
    }
    s = gr_getvertexindex(gcopy,g->vertex[s_g].vertexname);
    t = gr_getvertexindex(gcopy,g->vertex[t_g].vertexname);
  }
  if (CCcut_mincut_st (gcopy,s,t,cutweight,cut,&cutcount)) {
    printf("Erro na execucao do mincut_st\n");
    free(cut);
    if (!g->sequencial) {
      gr_closegraph(gcopy);
      free(gcopy);
    }
    return(false);
  }
  for (v=gr_getfirstvertex(gcopy);v!=-1;v=gr_getnextvertex(gcopy,v)){
    gr_setvertexcolor(gcopy,v,RED);
  }
  for (i=0;i<cutcount;i++) {
    gr_setvertexcolor(gcopy,cut[i],BLUE);
  }
  *ncutedges = 0;
  *cutweight = 0;
  for (e=gr_getfirstedge(gcopy);e!=-1;e=gr_getnextedge(gcopy,e)){
    if (gcopy->vertex[gcopy->edge[e].u].color !=
	gcopy->vertex[gcopy->edge[e].v].color){
      x = gr_getvertexindex(g,gcopy->vertex[gcopy->edge[e].u].vertexname);
      y = gr_getvertexindex(g,gcopy->vertex[gcopy->edge[e].v].vertexname);
      f = gr_getedge(g,x,y);
      cutedges[*ncutedges] = f;
      *ncutedges = *ncutedges + 1;
      *cutweight = *cutweight + g->edge[f].weight;
    }
  }
  if (!g->sequencial) {
    gr_closegraph(gcopy);
    free(gcopy);
  }
  free(cut);
  return(true);
}





int CCcut_mincut_st (graphtype *inputgraph,int s, int t, double *value, int *cut, int *cutcount)
{
    int rval = 0;
    concordegraph G;

    init_graph (&G);

    rval = buildgraph (&G, inputgraph);
    if (rval) {
        fprintf (stderr, "Buildgraph failed\n"); goto CLEANUP;
    }
    *value = flow (&G, G.nodelist + s, G.nodelist + t);
    if (cut) {
        rval = grab_the_cut (&G, G.nodelist + t, cut, cutcount);
        if (rval) {
            fprintf (stderr, "grab_the_cut failed\n"); goto CLEANUP;
        }
    }


CLEANUP:

    free_graph (&G);
    return rval;
}

static double flow (concordegraph *G, concordenode *s, concordenode *t)
{
#ifdef QUEUE_PRF
    concordenode *qhead = (concordenode *) NULL;
    concordenode *qtail = (concordenode *) NULL;
#endif
    concordenode *n;
    concordeedge *e;
    int count, round;
    int i;
    int ncount = G->nnodes;
    concordeedge *edgelist = G->edgelist;
    concordenode *nodelist = G->nodelist;
#ifdef USE_GAP
    concordenode **level = G->level;
#endif
#ifdef HIGHEST_LABEL_PRF
    concordenode **high = G->high;
#endif

/*
    printf ("Find cut separating %d and %d ...\n", s - nodelist, t - nodelist);
    fflush (stdout);
*/

    for (i = 0; i < ncount; i++) {
        nodelist[i].excess = 0.0;
        nodelist[i].active = 0;
#ifdef HIGHEST_LABEL_PRF
        high[i] = (concordenode *) NULL;
#endif
    }
#ifdef HIGHEST_LABEL_PRF
    G->highest = 0;
#endif

    for (i = G->nedges - 1; i >= 0; i--)
        edgelist[i].flow = 0.0;

    t->active = 1;              /* a lie, which keeps s and t off the */
    s->active = 1;              /* active int                         */

    for (e = s->out; e; e = e->outnext) {
        if (e->cap > 0.0) {
            e->flow = e->cap;
            e->ends[1]->excess += e->cap;
#ifdef QUEUE_PRF
            ADD_TO_ACTIVE(e->ends[1]);
#endif
#ifdef HIGHEST_LABEL_PRF
            e->ends[1]->active = 1;
#endif
        }
    }
#ifdef UNDIRECTED_GRAPH
    for (e = s->in; e; e = e->innext) {
        if (e->cap > 0.0) {
            e->flow = -e->cap;
            e->ends[0]->excess += e->cap;
#ifdef QUEUE_PRF
            ADD_TO_ACTIVE(e->ends[0]);
#endif
#ifdef HIGHEST_LABEL_PRF
            e->ends[0]->active = 1;
#endif
        }
    }
#endif


    setlabels (G, s, t);
    count = 0;
    round = (int) (GLOBAL_RELABEL_FREQUENCY * ncount);

#ifdef QUEUE_PRF
    while (qhead) {
        n = qhead;
        qhead = qhead->qnext;
        if (!qhead)
            qtail = (concordenode *) NULL;
        n->active = 0;
        if (n->flowlabel >= ncount)
            continue;
#endif

#ifdef HIGHEST_LABEL_PRF
    while (G->highest) {
        n = high[G->highest];
        n->active = 0;
        high[G->highest] = high[G->highest]->highnext;
        if (!high[G->highest]) {
            G->highest--;
            while (G->highest && (high[G->highest] == (concordenode *) NULL))
                G->highest--;
        }
#endif

        if (count == round) {
            setlabels (G, s, t);
            if (n->flowlabel >= ncount)
                continue;
            count = 0;
        } else
            count++;

        if (n->inout == GOING_IN)
            goto DO_ME_IN;

        if (n->outcurrent) {
            while (n->excess > 0.0) {
                e = n->outcurrent;
                { /* PUSH OUT */
                    double rf = e->cap - e->flow;
                    concordenode *n1 = e->ends[1];
                    if (n->flowlabel == n1->flowlabel + 1 && rf > 0.0) {
                        if (n->excess <= rf) {
                            e->flow += n->excess;
                            n1->excess += n->excess;
                            n->excess = 0.0;
                            ADD_TO_ACTIVE(n1);
                        } else {
                            e->flow += rf;
                            n1->excess += rf;
                            n->excess -= rf;
                            ADD_TO_ACTIVE(n1);
                            n->outcurrent = e->outnext;
                            if (!n->outcurrent) {
                                n->outcurrent = n->out;
                                n->inout = GOING_IN;
                                goto DO_ME_IN;
                            }
                        }
                    } else {
                        n->outcurrent = e->outnext;
                        if (!n->outcurrent) {
                            n->outcurrent = n->out;
                            n->inout = GOING_IN;
                            goto DO_ME_IN;
                        }
                    }
                }
            }
        }
DO_ME_IN:
        if (n->incurrent) {
            while (n->excess > 0.0) {
                e = n->incurrent;
                { /* PUSH IN */
#ifdef UNDIRECTED_GRAPH
                    double rf = e->cap + e->flow;
#else
                    double rf = e->flow;
#endif
                    concordenode *n1 = e->ends[0];
                    if (n->flowlabel == n1->flowlabel + 1 && rf > 0.0) {
                        if (n->excess <= rf) {
                            e->flow -= n->excess;
                            n1->excess += n->excess;
                            n->excess = 0.0;
                            ADD_TO_ACTIVE(n1);
                        } else {
                            e->flow -= rf;
                            n1->excess += rf;
                            n->excess -= rf;
                            ADD_TO_ACTIVE(n1);
                            n->incurrent = e->innext;
                            if (!n->incurrent) {
                                n->incurrent = n->in;
                                n->inout = GOING_OUT;
                                RELABEL(n);
                                break;
                            }
                        }
                    } else {
                        n->incurrent = e->innext;
                        if (!n->incurrent) {
                            n->incurrent = n->in;
                            n->inout = GOING_OUT;
                            RELABEL(n);
                            break;
                        }
                    }
                }
            }
        } else {
            /* n->in is NULL */
            n->inout = GOING_OUT;
            RELABEL(n);
        }
        if (n->excess > 0.0 && n->flowlabel < ncount) {
            ADD_TO_ACTIVE(n);
        }
    }

    return t->excess;
}

static void setlabels (concordegraph *G, concordenode *s, concordenode *t)
{
    concordenode *n;
    int ncount = G->nnodes;
    int num = ++(G->magicnum);
    int i;
    /* static int duke = 0; */

    t->flowlabel = 0;
    backwards_bfs (t, num, G);
    if (s->magiclabel == num) {
        printf ("Help - s should not get a label\n");
        s->flowlabel = ncount;
    }

    for (i = G->nnodes, n = G->nodelist; i;  i--, n++) {
        n->outcurrent = n->out;
        n->incurrent = n->in;
        n->inout = GOING_OUT;
        if (n->magiclabel != num) {
            n->flowlabel = ncount;
        }
    }
}

static void backwards_bfs (concordenode *s, int K, concordegraph *G)
{
    concordenode *This, *next, *tail;
    concordeedge *e;
    int  dist;
#ifdef USE_GAP
    concordenode dummy;
    concordenode **level = G->level;
#endif
#ifdef HIGHEST_LABEL_PRF
    concordenode **high = G->high;
#endif

    s->magiclabel = K;
    next = s;
    s->tnext = (concordenode *) NULL;
    dist = s->flowlabel;

#ifdef USE_GAP
    {
        int i;
        for (i = 0; level[i]; i++)
            level[i] = (concordenode *) NULL;
        level[dist] = s;
        s->levelnext = (concordenode *) NULL;
    }
#endif
#ifdef HIGHEST_LABEL_PRF
    {
        int i;
        for (i = 0; i <= G->highest; i++)
            high[i] = (concordenode *) NULL;
        G->highest = 0;
    }
#endif

    do {
#ifdef USE_GAP
        level[dist]->levelprev = (concordenode *) NULL;
        level[dist + 1] = &dummy;
        dummy.levelprev = (concordenode *) NULL;
#endif
        dist++;
        for (This = next, next = (concordenode *) NULL; This; This = This->tnext) {
            for (e = This->out; e; e = e->outnext) {
                tail = e->ends[1];
#ifdef UNDIRECTED_GRAPH
                if (tail->magiclabel != K && e->cap + e->flow > PRF_EPSILON) {
#else
                if (tail->magiclabel != K && e->flow > PRF_EPSILON) {
#endif
                    tail->flowlabel = dist;
                    tail->tnext = next;
                    next = tail;
                    tail->magiclabel = K;
#ifdef USE_GAP
                    tail->levelnext = level[dist];
                    level[dist]->levelprev = tail;
                    level[dist] = tail;
#endif
#ifdef HIGHEST_LABEL_PRF
                    if (tail->active) {
                        tail->highnext = high[dist];
                        high[dist] = tail;
                    }
#endif
                }
            }
            for (e = This->in; e; e = e->innext) {
                tail = e->ends[0];
                if (tail->magiclabel != K && e->cap - e->flow > PRF_EPSILON) {
                    tail->flowlabel = dist;
                    tail->tnext = next;
                    next = tail;
                    tail->magiclabel = K;
#ifdef USE_GAP
                    tail->levelnext = level[dist];
                    level[dist]->levelprev = tail;
                    level[dist] = tail;
#endif
#ifdef HIGHEST_LABEL_PRF
                    if (tail->active) {
                        tail->highnext = high[dist];
                        high[dist] = tail;
                    }
#endif
                }
            }
        }
#ifdef USE_GAP
        if (dummy.levelprev) {
            dummy.levelprev->levelnext = (concordenode *) NULL;
            level[dist]->levelprev = (concordenode *) NULL;
        } else {
            level[dist] = (concordenode *) NULL;
        }
#endif
#ifdef HIGHEST_LABEL_PRF
        if (high[dist])
            G->highest = dist;
#endif
    } while (next);
}

static int grab_the_cut (concordegraph *G, concordenode *n, int *cut, int *cutcount)
{
    int rval = 0;
    concordeedge *e;
    concordenode *q, *top;
    int count = 0;
    int i, num;
    concordenode *nodelist = G->nodelist;
    int *tcut = (int *) NULL;

    *cutcount = 0;

    tcut = CC_SAFE_MALLOC (G->nnodes, int);
    if (!tcut) {
        fprintf (stderr, "out of memory in grab_the_cut\n");
        rval = 1; goto CLEANUP;
    }

    G->magicnum++;
    num = G->magicnum;
    tcut[count++] = (int) (n - nodelist);
    q = n;
    q->magiclabel = num;
    q->tnext = (concordenode *) NULL;

    while (q) {
        top = q;
        q = q->tnext;
        for (e = top->out; e; e = e->outnext) {
#ifdef UNDIRECTED_GRAPH
            if (e->cap + e->flow > 0.0 && e->ends[1]->magiclabel != num) {
#else
            if (e->flow > 0.0 && e->ends[1]->magiclabel != num) {
#endif
                tcut[count++] = (int) (e->ends[1] - nodelist);
                e->ends[1]->magiclabel = num;
                e->ends[1]->tnext = q;
                q = e->ends[1];
            }
        }
        for (e = top->in; e; e = e->innext) {
            if (e->cap - e->flow > 0.0 && e->ends[0]->magiclabel != num) {
                tcut[count++] = (int) (e->ends[0] - nodelist);
                e->ends[0]->magiclabel = num;
                e->ends[0]->tnext = q;
                q = e->ends[0];
            }
        }
    }

    for (i = 0; i < count; i++) {
        cut[i] = tcut[i];
    }
    *cutcount = count;

CLEANUP:

    CC_IFFREE (tcut, int);
    return rval;
}

static int buildgraph (concordegraph *G, graphtype *inputgraph)
  /*int ncount, int ecount, int *elist, double *ecap*/
{
    int i;
    concordeedge *edgelist;
    concordenode *nodelist;

    G->nodelist = (concordenode *) NULL;
    G->edgelist = (concordeedge *) NULL;
#ifdef USE_GAP
    G->level = (concordenode **) NULL;
#endif
#ifdef HIGHEST_LABEL_PRF
    G->high = (concordenode **) NULL;
#endif

    G->magicnum = 0;
    G->nnodes = inputgraph->nvertices;
    G->nedges = inputgraph->nedges;
    G->nodelist = CC_SAFE_MALLOC (inputgraph->nvertices, concordenode);
    G->edgelist = CC_SAFE_MALLOC (inputgraph->nedges, concordeedge);
    if (!G->nodelist || !G->edgelist) {
        fprintf (stderr, "Out of memory in buildgraph\n");
        CC_IFFREE (G->nodelist, concordenode);
        CC_IFFREE (G->edgelist, concordeedge);
        return 1;
    }
#ifdef USE_GAP
    G->level = CC_SAFE_MALLOC (inputgraph->nvertices + 1, concordenode *);
    if (!G->level) {
        fprintf (stderr, "Out of memory in buildgraph\n");
        CC_IFFREE (G->nodelist, concordenode);
        CC_IFFREE (G->edgelist, concordeedge);
        return 1;
    }
    for (i = 0; i < inputgraph->nvertices; i++)
        G->level[i] = (concordenode *) NULL;
    G->level[inputgraph->nvertices] = (concordenode *) NULL;  /* A guard dog for a while loop */
#endif

#ifdef HIGHEST_LABEL_PRF
    G->high = CC_SAFE_MALLOC (inputgraph->nvertices, concordenode *);
    if (!G->high) {
        fprintf (stderr, "Out of memory in buildgraph\n");
        CC_IFFREE (G->nodelist, concordenode);
        CC_IFFREE (G->edgelist, concordeedge);
        return 1;
    }
#endif

    nodelist = G->nodelist;
    edgelist = G->edgelist;

    for (i = 0; i < inputgraph->nvertices; i++) {
        nodelist[i].in = (concordeedge *) NULL;
        nodelist[i].out = (concordeedge *) NULL;
        nodelist[i].magiclabel = 0;
    }

    for (i = 0; i < inputgraph->nedges; i++) {
      int tail = inputgraph->edge[i].u;
      int head = inputgraph->edge[i].v;
      if (tail < 0 || tail >= inputgraph->nvertices ||
	  head < 0 || head >= inputgraph->nvertices) {
	fprintf (stderr, "Edge list in wrong format: Edge %d = [%d, %d]\n",
		 i, tail, head);
	return 1;
      }
      edgelist[i].ends[0] = nodelist + tail;
      edgelist[i].ends[1] = nodelist + head;
      edgelist[i].cap = inputgraph->edge[i].weight;
      edgelist[i].outnext = nodelist[tail].out;
      nodelist[tail].out = &(edgelist[i]);
      edgelist[i].innext = nodelist[head].in;
      nodelist[head].in = &(edgelist[i]);
    }
    return 0;
}

static void init_graph (concordegraph *G)
{
    if (G) {
        G->nodelist = (concordenode *) NULL;
        G->edgelist = (concordeedge *) NULL;
#ifdef USE_GAP
        G->level = (concordenode **) NULL;
#endif
#ifdef HIGHEST_LABEL_PRF
        G->high = (concordenode **) NULL;
#endif
    }
}

static void free_graph (concordegraph *G)
{
    CC_IFFREE (G->nodelist, concordenode);
    CC_IFFREE (G->edgelist, concordeedge);
#ifdef USE_GAP
    CC_IFFREE (G->level, concordenode *);
#endif
#ifdef HIGHEST_LABEL_PRF
    CC_IFFREE (G->high, concordenode *);
#endif
}
