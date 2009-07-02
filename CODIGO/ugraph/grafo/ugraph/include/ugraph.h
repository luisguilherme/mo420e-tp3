#ifndef UGRAFOSDEFINE
#define UGRAFOSDEFINE
/*       Arquivo: ugraph.h

	 Ultima atualizacao: 06/2002 

	 Projetista: Flavio Keidi Miyazawa
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unionfin.h>
#include <boolean.h>
#include <color.h>


//#define METRICINTEGEREDGEWEIGHT

#define space(c)	((c==' ') || (c==9) || (c==10) || (c==13))
#define MINIMUMDOUBLE (-10E30)
typedef enum {GR_WINDOWS, GR_LINUX} gr_os_type;

typedef enum {GR_POSTSCRIPT_LINUX,GR_POSTSCRIPT_WINDOWS,
	      GR_POSTSCRIPT_KDE,
	      GR_GIF_LINUX,GR_GIF_WINDOWS,
	      GR_GIF_WINDOWS_MSPAINT,GR_GIF_WINDOWS_XP,
	      GR_JPG_LINUX,GR_JPG_WINDOWS,
	      GR_JPG_WINDOWS_MSPAINT,GR_JPG_WINDOWS_XP} gr_outputtype;


/*-------------------------------------------------------------------------*/
/*                     ESTRUTURAS PARA GRAFOS                              */
/*-------------------------------------------------------------------------*/

#define MAXLABELNAME   100 /* Tamanho maximo do nome de grafo/aresta/vertice */
typedef struct
{
  int u,v;
  boolean used;  /* tem valor true se aresta esta' sendo usado, false c.c. */
  
  char edgename[MAXLABELNAME];
  colortype color;
  double weight;
  int next,prev; /* vamos considerar listas duplamente ligadas 
		    -1 indica que nao tem proximo/anterior */
  int indadj_u, /* indice desta aresta no vetor de adjacentes de u */
      indadj_v; /* indice desta aresta no vetor de adjacentes de v */ 
} edgetype;

typedef struct 
{
  boolean used;  /* tem valor true se o vertice esta' sendo usado, false c.c. */
  char vertexname[MAXLABELNAME];
  int firstadj; /* numero da posicao na lista de adj. do prox. vertice
		  -1 se nao tem adjacentes*/
  colortype color;
  double pos_x,pos_y;
  int next,prev; /* next e prev sao usado na lista de livres e usados.
		    a lista de livres so' usa next. A lista de usados e' uma
		    lista duplamente ligada e usa next e prev. -1 quando nao tem
		    o proximo */

  int nextvertexname; /* campo usado para fazer as listas ligadas do hashing
			 de nomes de vertices -1 se nao tem proximo */
} vertextype;

typedef struct 
{
  int e; /* indice da aresta */
  int v_adj; /* vertice que e' adjacente */
  int next;  /* usado como parte de lista de adj livres e de adj usados.
		se for de lista dos livres, a lista e' simples e basta next
		se for de usados, e' lista duplamente ligada e tambem se usa prev. 
		Se nao tem proximo/anterior, tem valor -1 */
  int prev;     
} adjacencytype;



/* elemento do vetor de hashing. uma lista ligada simples */
typedef struct 
{
  int e; /* indice da aresta */
  int menor_v,maior_v; /* extremos da aresta (menor_v <= maiorv) */
  int next;  /* posicao do proximo item na lista de
		   -1  se for o ultimo */
} edgelisttype;


/* muitas das estruturas definidas para este tipo foram consideradas para que 
 gaste tempo O(maxvertex+maxedges) mas para que insercoes, remocoes, consultas
 de estruturas do grafo gastem tempo medio O(1) */
typedef struct
{
  int maxvertex,maxedges; /*numero maximo de vertices e arestas permitido*/
  int nvertices,nedges; /* numero de vertices e arestas existentes */

  int firstfreevertex; /* indice do primeiro vertice livre, -1 se nao tem */
  int firstusedvertex; /* indice do primeiro vertice usado, -1 se nao tem */
  vertextype *vertex;  /* e' um vetor que tem contem os vertices usados (marcados como
			  used e uma lista de vertices livres, 
			  encabecada por firstfreevertex */

  int firstfreeedge;   /* indice da primeira aresta livre, -1 se nao tem */
  int firstusededge;   /* indice da primeira aresta usada, -1 se nao tem */
  edgetype *edge;   /* e' um vetor que tem duas listas de arestas duplamente ligadas 
		     uma para as arestas usadas e outra para as arestas livres */

  int firstfreeadj;   /* indice da primeira aresta livre, -1 se nao tem */
  adjacencytype *adj; /* como cada aresta entra na lista de adj. de dois vertices,
		       este vetor tem tamanho 2*maxedges.
		      Este vetor vai representar varias listas, para cada
		      vertice vai ter uma lista duplamente ligada */

  int vertexnamehashsize; /* tamanho da tabela de hashing para encontrar 
			     nomes de vertices = a nextprime(2*maxvertex)*/
  int *vertexnamehash;  /* vetor de inteiros. Cada posicao aponta para 
			   o numero do vertice. A lista ligada de colisoes
			   e' feita diretamente nos vertices pelo campo 
			   nextvertexname. Se posicao for -1, celula vazia*/

  int hashsize;  /* igual a nextprime(2*maxedges), para deixar a tabela bem esparsa 
		  e com diminuir bastante as chances de colisoes */
  int *edgehash; /* vetor de indices (para posicoes de hashlist) de tamanho
		    hashsize */
  int nextfreehashpos; 
  edgelisttype *hashlist; /* vetor de tamanho maxedges, para armazenar as colisoes 
			     inicialmente, hashlist e' toda uma lista encabecada por
			     nextfreehashpos */
  boolean positions; /* verdadeiro sse as posicoes estao calculadas */
  int lp1,lp2,bb1,bb2,bb3,bb4;
  
  /* nome do grafo */
  char graphname[MAXLABELNAME];

  boolean sequencial; /* eh true se todos os vertices e arestas estao
			 sequenciais (0,1,2,3,....). Se o grafo nunca teve
			 uma remocao de aresta ou vertice, isto e' verdade.*/
} graphtype;
/*----------------------------------------------------------------------------*/
/*----------------- Funcoes associadas ao grafo  -----------------------------*/
/*----------------------------------------------------------------------------*/
/* retorna true se conseguiu inicializar o grafo. Note que o grafo podera' ter
   no maximo maxvertex vertices e no maximo maxedges arestas */
/* o motivo de entrar com as quantidades maxima de arestas e vertices e' para
   fazer toda a alocacao de memoria no inicio, mas nao fazer mais nenhuma outra
   alocacao de memoria. Por motivos de velocidade.  */
boolean gr_initgraph(graphtype *g,int maxvertex,int maxedges);
graphtype *gr_creategraph(int maxvertex,int maxedges);

void gr_closegraph(graphtype *g);

boolean gr_geraposicoes(graphtype *g);

/* visualiza um grafo */
void gr_viewgraph(graphtype *g);
/* visualiza um grafo com as posicoes computadas 
   (OBS.: ESTE COMANDO USA O PROGRAMA GV PARA VISUALIZAR ARQUIVO EPS) */
boolean gr_viewmetricgraph(graphtype *g);

/* grava um grafo num arquivo texto */
boolean gr_writegraph(graphtype *g,char *filename);
boolean gr_writegraph_neato(graphtype *g,char *filename);
int gr_number_components(graphtype *g);
int gr_number_vertices(graphtype *g);
int gr_number_edges(graphtype *g);

/* le um grafo a partir de um arquivo texto */
/* em formato qualquer */
boolean gr_readgraph(graphtype *g,char *filename);
/* formato vertice + arestas */
boolean gr_readedgegraph(graphtype *g,char *filename);
/* formato vertice (cada um com posicao (x,y)) */
boolean gr_readmetricgraph(graphtype *g,char *filename);

boolean gr_cleangraph(graphtype *dest,graphtype *source);

typedef enum {NULLFILE, METRICFILE, EDGEFILE} gr_filetype;
gr_filetype gr_ismetricfile(char *filename);

boolean gr_setgraphname(graphtype *g,char *s);
boolean gr_getgraphname(graphtype *g,char *s);

/*----------------------------------------------------------------------------*/
/*----------------- Funcoes associadas a vertice -----------------------------*/
/*----------------------------------------------------------------------------*/

/* retorna o indice do vertice, dado o nome. -1 se nao encontrou */
/* e' uma funcao lenta, usada na leitura de um grafo a partir de arquivo */
/* tempo O(|V|) */
int gr_getvertexindex(graphtype *g,char *name);

/* esta funcao retorna true se v pertence a g, false c.c. */
boolean gr_existsvertex(graphtype *g,int v);

/* insere um vertice. retorna um indice que e' o numero do vertice inserido
   (este indice e' um numero em [0,maxvertex-1] */
int gr_insertvertex(graphtype *g, char *name);
/* remove um vertice do grafo. retorna verdadeiro se o vertice foi removido,
 falso caso o vertice nao exista. Remove todas as arestas incidentes em v */
boolean gr_deletevertex(graphtype *g,int v);

/* determina a cor de um vertice, true se conseguiu */
boolean gr_setvertexcolor(graphtype *g,int v,colortype cor);

/* retorna o indice da aresta que e' o primeiro na lista de adjacencia, 
   -1 se nao tiver */
int gr_getvertexfirstadj(graphtype *g,int v);

/* retorna o indice da proxima aresta, que e' adjacente a v, 
   e vem depois de e considerando a lista de adjacencia, -1 se nao tiver */
int gr_getvertexnextadj(graphtype *g,int v,int e);

/* define as coordenadas x e y que o vertice v sera' desenhado, 
   true se conseguiu  */
boolean gr_setvertexposition(graphtype *g,int v,double x,double y);

void gr_listadjacency(graphtype *g,int v);
/* obtem o nome de um vertice. Retorna true se obteve sucesso, false c.c. */
boolean gr_getvertexname(graphtype *g,int v,char *name);
char *gr_vertexname(graphtype *g,int v);

int gr_getfirstvertex(graphtype *g);
int gr_getnextvertex(graphtype *g,int v);

/*----------------------------------------------------------------------------*/
/*----------------- Funcoes associadas a aresta  -----------------------------*/
/*----------------------------------------------------------------------------*/

/* se subg e' um subgrafo de g, e "e" e' uma aresta de "e",
   esta funcao retorna a correspondente aresta em g. */
int gr_edge_graph_from_subgraph(graphtype *g,graphtype *subg,int e);

/* retorna o indice da aresta correspondente a aresta (u,v) */
int gr_getedge(graphtype *g,int u, int v);

/* obtem o nome de uma aresta. Retorna true se obteve sucesso, false c.c. */
boolean gr_getedgename(graphtype *g,int e,char *name);
char *gr_edgename(graphtype *g,int e);

/* retorna true se existe a aresta e, false c.c. */
boolean gr_existsedge(graphtype *g,int e);

int gr_insertedge(graphtype *g,char *name,int u,int v,double weight);
/* remove uma arestas do grafo. retorna verdadeiro se a aresta foi removida,
 falso caso a aresta nao exista. */
boolean gr_deleteedge(graphtype *g,int e);

/* retorna um dos extremos da aresta e, -1 se nao existe aresta  */
int gr_getedgehead(graphtype *g,int e);

/* retorna outro extremo da aresta e, -1 se nao existe aresta  */
int gr_getedgetail(graphtype *g,int e);

/* retorna em 'peso' o peso da aresta e, false se nao existe aresta */
boolean gr_getedgeweight(graphtype *g,int e,double *peso);

/* retorna o peso da aresta e, MINIMUMDOUBLE se nao conseguiu */
double gr_edgeweight(graphtype *g,int e);

/* determina o peso de um vertice, true se conseguiu */
boolean gr_setedgeweight(graphtype *g,int e,double peso);


boolean gr_getedgecolor(graphtype *g,int e,colortype *cor);

/* determina a cor de uma aresta */
boolean gr_setedgecolor(graphtype *g,int e,colortype cor);

void gr_getnextname(char *str,FILE *fp);

int gr_getfirstedge(graphtype *g);
int gr_getnextedge(graphtype *g,int e);
int gr_getedge(graphtype *g,int u, int v);

void gr_paintgraphedges(graphtype *g,colortype cor);
void gr_paintvectoredges(graphtype *g,int *edges,int m,colortype cor);
void gr_paintgraphvertex(graphtype *g,colortype cor);
void gr_paintgraph(graphtype *g,colortype cor);
void gr_paintgraphscale(graphtype *g);
void gr_paintgraphvectorscale(graphtype *g,double *v);
void gr_paintsubgraph(graphtype *g,graphtype *sub);
boolean gr_minimumspanningtree(graphtype *gr,graphtype *mst);
boolean gr_getminimumspanningtree(graphtype *g,int *mstedges, int *mstnedges);

boolean gr_min_st_cut(graphtype *g,int s_g,int t_g, graphtype *stcut,double *cutweight);
boolean gr_min_st_cut2(graphtype *g,int s_g,int t_g, graphtype *stcut,double *cutweight);
boolean gr_get_min_st_cut(graphtype *g,int s_g,int t_g,
			  int *cutedges,int *ncutedges,double *cutweight);
void gr_generate_ghc_tree(graphtype *g,graphtype *tree);
boolean  gr_find_min_edge_ghc_tree(graphtype *tree,int s,int t,int *edgeindex);
void gr_erro(char *s);
void gr_copylabel(char *dest,char *source);
void gr_pause(void);
void gr_freenotnull(void *p);
/*#define gr_freenotnull(p) if (p!=NULL) free(p);*/
#include "ghc_tree.h"
#endif
/*----------------------------------------------------------------------------*/
