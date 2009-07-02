/*

  Pequena biblioteca para manipulacao de grafos nao orientados.
  Algumas rotinas nao estao otimizadas.
  Para usar esta biblioteca, veja um resumo das operacoes que esta'
  no link http://www.ic.unicamp.br/~fkm/grafos/

*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <myutil.h>
#include <ugraph.h>
#include <ghc_tree.h>

/* pode ser GR_LINUX ou GR_WINDOWS (default eh GR_LINUX)*/
gr_os_type gr_sistema=GR_LINUX;
/* se quiser, mude para algum outro tipo (veja em ugraph.h)*/
gr_outputtype gr_output = GR_POSTSCRIPT_LINUX;


void gr_printedge(graphtype *g,int e,char *s);
#define vname(v) ((g->vertex[(v)].vertexname))
#define ename(e) ((g->edge[(e)].edgename))


int gr_number_vertices(graphtype *g)
{
  if (g==NULL) return(-1);
  return(g->nvertices);
}

int gr_number_edges(graphtype *g)
{
  if (g==NULL) return(-1);
  return(g->nedges);
}


void gr_pause(void)
{
  int c;
  printf("Pressione ENTER para continuar.\n");
  do {
    c=getchar();
  }while (c!=10);
}

void gr_bell(void)
{
  printf("%c",7);
  fflush(stdout);
}

void gr_erro(char *s)
{
  printf(s);
  gr_bell();
}


/* verifica se um numero maior que 1 e' primo */
int gr_isprime(int n)
{
  int raiz,i;
  if (n==2) return(1); /* 2 e' primo */
  if (!(n % 2)) return(0); /* todos os outros pares nao sao */
  raiz = (int) sqrt(n)+1;
  for (i=3;i<raiz;i+=2) 
    if (!(n % i)) return(0);
  return(1);
}

/* retorna o primeiro primo maior ou igual a n */
int gr_nextprime(int n)
{
  if (n<2) return(2);
  if (!(n % 2)) n++; /* deixa n impar */
  while (!gr_isprime(n)) n+=2;
  return(n);
}

void gr_freenotnull(void *p)
{
  if (p!=NULL) free(p);
}

/* Dado um texto e um padrao, devolve a posicao no
   texto onde o padrao ocorre. Devolve (-1) se nao achar. */
int gr_busca_padrao(char *texto,char *busca)
{
  int i,j,tam_texto,tam_busca;
  char *p1,*p2;
  boolean achou;
  tam_texto = strlen(texto);
  tam_busca = strlen(busca);
  for (i=0;i<tam_texto - tam_busca + 1;i++) {
      p1 = &texto[i];
      p2 = busca;
      for (j=0,achou = true;j<tam_busca && achou == true;j++)
	if (p1[j] != p2[j]) achou = false;
      if (achou) return(i);
  }
  return(-1);
}


void gr_copylabel(char *dest,char *source)
{
  strncpy(dest,source,MAXLABELNAME);
  dest[MAXLABELNAME-1] = '\0'; /* para ter certeza que nao teremos problema
				  de imprimir lixo */
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------- Funcoes associadas ao grafo  -----------------------------*/

boolean gr_setgraphname(graphtype *g,char *s)
{
  if (g==NULL) return(false);
  gr_copylabel(g->graphname,s);
  return(true);
}

boolean gr_getgraphname(graphtype *g,char *s)
{
  if (g==NULL) return(false);
  strcpy(s,g->graphname);
  return(true);
}



void gr_closegraph(graphtype *g)
{
  gr_freenotnull(g->vertex);
  gr_freenotnull(g->edge);
  gr_freenotnull(g->adj);
  gr_freenotnull(g->edgehash);
  gr_freenotnull(g->hashlist);
  gr_freenotnull(g->vertexnamehash);
  g->vertexnamehash=NULL;
  g->vertex = NULL;
  g->edge = NULL;
  g->adj = NULL;
  g->edgehash = NULL;
  g->hashlist = NULL;
  g->maxvertex = 0;
  g->maxedges = 0;
  g->nvertices = 0;
  g->nedges = 0;
  g->hashsize = 0;
  g->firstfreevertex = -1;  g->firstusedvertex = -1;
  g->firstfreeedge = -1;    g->firstusededge  = -1;
  g->firstfreeadj = -1;
  g->nextfreehashpos = -1;
  g->graphname[0]='\0';
}
  
/*----------------------------------------------------------------------------*/
/* retorna true se conseguiu inicializar o grafo. Note que o grafo podera' ter
   no maximo maxvertex vertices e no maximo maxedges arestas */
/* o motivo de entrar com as quantidades maxima de arestas e vertices e' para
   fazer toda a alocacao de memoria no inicio, mas nao fazer mais nenhuma outra
   alocacao de memoria. Por motivos de velocidade.  */

boolean gr_initgraph(graphtype *g,int maxvertex,int maxedges)
{
  int i;
  /* para a alocacao nao ser de 0 bytes */
  if (maxvertex<1) maxvertex = 1;
  if (maxedges<1) maxedges = 1;
  g->maxvertex = maxvertex;
  g->maxedges = maxedges;
  g->nvertices = 0;
  g->nedges = 0;
  g->sequencial = true;
  g->vertexnamehashsize = gr_nextprime(2*g->maxvertex); /* tam. da tab. hash. 
					       dos nomes dos vertices */
  
  g->hashsize = gr_nextprime(2*g->maxedges); /* tamanho da tabela de hashing e' primo */
  g->vertex = (vertextype *) malloc(maxvertex*(sizeof(vertextype)));
  g->edge  = (edgetype   *) malloc(maxedges*(sizeof(edgetype)));
  g->adj  = (adjacencytype   *) malloc(2*maxedges*(sizeof(adjacencytype)));
  g->edgehash = (int *) malloc(g->hashsize*sizeof(edgelisttype));
  g->hashlist = (edgelisttype *) malloc(g->maxedges * sizeof(edgelisttype));
  g->vertexnamehash = (int *) malloc(g->vertexnamehashsize * sizeof(int));
  /* se alguma alocacao falhou, desaloca tudo e retorna false */
  if ((!g->vertex) || (!g->edge) || (!g->adj) || (!g->edgehash) || 
      (!g->hashlist) || (!g->vertexnamehash)) {
    gr_closegraph(g);
    return(false);
  }
  /* constroi a lista ligada (simples) de vertices livres */
  g->firstusedvertex = -1; /* nao temos nenhum vertice sendo usado */
  g->firstfreevertex = 0; /* a primeira posicao */
  for (i=0;i<g->maxvertex;i++) {
    g->vertex[i].used = false;
    g->vertex[i].next = i+1;
  }
  g->vertex[g->maxvertex-1].next = -1; /* ultimo da lista */

  /* inicializa a lista de arestas */
  g->firstusededge = -1; /* nao temos nenhuma aresta usada */
  g->firstfreeedge = 0; /* posicao da primeira aresta livre */
  for (i=0;i<g->maxedges;i++) g->edge[i].next = i+1; 
  g->edge[g->maxedges-1].next = -1; /* ultimo da lista */

  /* inicializa as listas de adjacentes */
  g->firstfreeadj = 0;
  for (i=0;i<2*g->maxedges;i++) g->adj[i].next = i+1; 
  g->adj[2*g->maxedges-1].next = -1; /* ultimo da lista */

  /* inicializa hashing para as arestas */
  for (i=0;i<g->hashsize;i++) g->edgehash[i] = -1; /* nao tem nenhuma aresta na
						      tabela de hashing */
  g->nextfreehashpos = 0; /* primeira posicao livre da tabela de hashing */
  /* lista (simples) de posicoes livres da tabela de colisoes de hashing */
  for (i=0;i<g->maxedges;i++) g->hashlist[i].next = i+1;
  g->hashlist[g->maxedges-1].next = -1; /* ultimo da lista */


  /* inicializa a tabela de hashing dos nomes de vertices */
  for (i=0;i<g->vertexnamehashsize;i++)
    g->vertexnamehash[i] = -1;

  gr_copylabel(g->graphname,"");
  g->positions = false;
  return(true);
}

/*----------------------------------------------------------------------*/
/* Como a funcao anterior, initgraph, mas esta retorna o ponteiro do grafo,
   NULL se nao conseguiu */
graphtype *gr_creategraph(int maxvertex,int maxedges)
{
  graphtype *g;
  boolean s;
  g = (graphtype *) malloc(sizeof(graphtype));
  if (g==NULL) return(NULL);
  s = gr_initgraph(g,maxvertex,maxedges);
  if (!s) {
    free(g);
    return(NULL);
  } else {
    return(g);
  }
}


/* grava o grafo em um arquivo, no formato dot */
boolean gr_writegraph_neato(graphtype *g,char *filename)
{
  int i;
  FILE *fp;
  char dados[250],aux[250];
  
  if (g==NULL) return(false);
  fp = fopen(filename,"w");
  if (fp==NULL) return(false);
  fprintf(fp,"graph gr {\n     graph [fontsize=18, label=\"%s\"];\n",g->graphname);
  fprintf(fp,"node [shape=circle,style=bold,"
             "height=\"0.25\",width=\"0.25\"];\n");/* caracteristicas globais de vertice*/
  fprintf(fp,"edge [style=bold,labelfontsize=3];\n");/* caracteristica global de aresta*/
  if (g->positions) {
    fprintf(fp,"graph [lp=\"%d,%d\"];\n",g->lp1,g->lp2);
    fprintf(fp,"graph [bb=\"%d,%d,%d,%d\"];\n",g->bb1,g->bb2,g->bb3,g->bb4);
  }

  for (i=gr_getfirstvertex(g);i!=-1;i=gr_getnextvertex(g,i)) {
    if (g->vertex[i].color==NOCOLOR) 
      sprintf(dados,"color=black");
    else
      sprintf(dados,"color=%s",colorname(g->vertex[i].color));
    if (g->positions)
      sprintf(aux,",pos=\"%f,%f\"",(float) i*0.00001+g->vertex[i].pos_x,i*0.00001+g->vertex[i].pos_y);
    else 
      strcpy(aux,"");
    strcat(dados,aux);
    fprintf(fp,"   %s [%s];\n",g->vertex[i].vertexname,dados);
  }
  for (i=gr_getfirstedge(g);i!=-1;i=gr_getnextedge(g,i)) {
    if (g->edge[i].color==NOCOLOR) {/* se nao tem cor, nao imprime*/
      /* if (fabs(g->edge[i].weight)<0.00001)
	fprintf(fp,"   %s -- %s [style=dashed];\n",
		g->vertex[g->edge[i].u].vertexname, 
		g->vertex[g->edge[i].v].vertexname);
      else	  
	fprintf(fp,"   %s -- %s [label=%4.2lf];\n",
		g->vertex[g->edge[i].u].vertexname, 
		g->vertex[g->edge[i].v].vertexname,
		g->edge[i].weight);
      */ } else 
      if (fabs(g->edge[i].weight)<0.00001)
	fprintf(fp,"   %s -- %s [color=%s,style=dashed];\n",
		g->vertex[g->edge[i].u].vertexname, 
		g->vertex[g->edge[i].v].vertexname,
		colorname(g->edge[i].color));
      else
	fprintf(fp,"   %s -- %s [color=%s,label=%4.2lf];\n",
		g->vertex[g->edge[i].u].vertexname, 
		g->vertex[g->edge[i].v].vertexname,
		colorname(g->edge[i].color),
	      g->edge[i].weight);
  }
  fprintf(fp,"}\n");
  fclose(fp);
  return(true);
}

/* visualiza um grafo */
void gr_viewgraph(graphtype *g)
{
  if (g==NULL) return;
  if (g->nvertices<=0) {
    printf("Grafo sem vertices.\n");
    return;
  }
  if (!g->positions) gr_geraposicoes(g);
  /* grava o grafo no formato dot, em arq. tempor.*/
  gr_writegraph_neato(g,"tmp_view.dot"); 

  /* o programa neato pode ser obtido em 
     http://www.research.att.com/sw/tools/graphviz/download.html
     para windows ha' disponivel no link acima o arquivo neato.exe */

  switch (gr_output) {
    /* mude o visualizador (gsview/gv/gqview/...) de acordo com as suas necessidades */
  case GR_POSTSCRIPT_LINUX: 
    system("neato -Goverlap=false -Tps tmp_view.dot -o tmp_view.ps");
    system("gv tmp_view.ps");
    system("rm tmp_view.ps");
    system("rm tmp_view.dot");
    break;
  case GR_POSTSCRIPT_KDE: 
    system("neato -Goverlap=false -Tps tmp_view.dot -o tmp_view.ps");
    system("kghostview tmp_view.ps");
    system("rm tmp_view.ps");
    system("rm tmp_view.dot");
    break;
  case GR_POSTSCRIPT_WINDOWS: 
    system("neato.exe -Goverlap=false -Tps tmp_view.dot -o tmp_view.ps");
    system("gsview tmp_view.ps");
    system("del tmp_view.ps");
    system("del tmp_view.dot");
    /* gsview pode ser obtido em www.cs.wisc.edu/~ghost/gsview/  */
    break;
  case GR_GIF_LINUX:
    system("neato -Goverlap=false -Tgif tmp_view.dot -o tmp_view.gif");
    system("gqview tmp_view.gif"); /* gqview em geral ja' vem no linux */
    system("rm tmp_view.gif");
    system("rm tmp_view.dot");
    break;
  case GR_GIF_WINDOWS:
    system("neato.exe -Goverlap=false -Tgif tmp_view.dot -o tmp_view.gif");
    system("pbrush tmp_view.gif"); /* pbrush em geral ja' vem no windows */
    system("del tmp_view.gif");
    system("del tmp_view.dot");
    break;
  case GR_GIF_WINDOWS_MSPAINT:
    system("neato.exe -Goverlap=false -Tgif tmp_view.dot -o tmp_view.gif");
    system("mspaint.exe tmp_view.gif"); 
    system("del tmp_view.gif");
    system("del tmp_view.dot");
    break;
  case GR_GIF_WINDOWS_XP:
    system("neato.exe -Goverlap=false -Tgif tmp_view.dot -o tmp_view.gif");
    system("tmp_view.gif"); 
    system("del tmp_view.gif");
     system("del tmp_view.dot");
   break;
  case GR_JPG_LINUX:
    system("neato -Goverlap=false -Tjpg tmp_view.dot -o tmp_view.jpg");
    system("gqview tmp_view.jpg"); /* gqview em geral ja' vem no linux */
    system("rm tmp_view.jpg");
    system("rm tmp_view.dot");
    break;
  case GR_JPG_WINDOWS:
    system("neato.exe -Goverlap=false -Tjpg tmp_view.dot -o tmp_view.jpg");
    system("pbrush tmp_view.jpg"); /* pbrush em geral ja' vem no windows */
    system("del tmp_view.jpg");
    system("del tmp_view.dot");
   break;
  case GR_JPG_WINDOWS_MSPAINT:
    system("neato.exe -Goverlap=false -Tjpg tmp_view.dot -o tmp_view.jpg");
    system("mspaint.exe tmp_view.jpg");
    system("del tmp_view.jpg");
    system("del tmp_view.dot");
   break;
  case GR_JPG_WINDOWS_XP:
    system("neato.exe -Goverlap=false -Tjpg tmp_view.dot -o tmp_view.jpg");
    system("tmp_view.jpg");
    system("rm tmp_view.jpg");
    system("del tmp_view.dot");
   break;
  }
}

/* grava um grafo num arquivo texto formato vertices seguido de arestas.
   este formato e' o que a funcao readgraph le normalmente */
boolean gr_writegraph(graphtype *g,char *filename)
{
  int i;
  FILE *fp;
  if (g==NULL) return(false);
  fp = fopen(filename,"w");
  if (fp==NULL) return(false);
  fprintf(fp,"%3d %3d\n",g->nvertices,g->nedges);
  if (g->positions) {
    for (i=gr_getfirstvertex(g);i!=-1;i=gr_getnextvertex(g,i)) {
      fprintf(fp,"%s %lf %lf\n",g->vertex[i].vertexname,
	      g->vertex[i].pos_x,g->vertex[i].pos_y);
    }
  }else{
    for (i=gr_getfirstvertex(g);i!=-1;i=gr_getnextvertex(g,i)) {
      fprintf(fp,"%s\n",g->vertex[i].vertexname);
    }
  }
  for (i=gr_getfirstedge(g);i!=-1;i=gr_getnextedge(g,i)) 
      fprintf(fp,"%s  %s %20.6lf\n",g->vertex[g->edge[i].u].vertexname,
	      g->vertex[g->edge[i].v].vertexname,g->edge[i].weight);
  fclose(fp);
  return(true);
}

/* pega o proximo token. O branco e' o separador */
void gr_getnextname(char *str,FILE *fp)
{
  int i,c;
  i=0;
  c = fgetc(fp);
  while ((!feof(fp)) && (space(c))) c=fgetc(fp);
  while ((!feof(fp)) && (!space(c))) {
    if (i<MAXLABELNAME-1) {
      str[i] = c;
      i++;
    }
    c = fgetc(fp);
  }
  str[i] = '\0';
}


/* le um grafo a partir de um arquivo texto, formato arestas */
boolean gr_readedgegraph(graphtype *g,char *filename)
{
  FILE *fp;
  int i,v,u,n,m,j,k,l,nposicoes;
  double peso,x,y;
  char name[MAXLABELNAME],linha[1000];
  if (g==NULL) return(false);
  fp = fopen(filename,"r");
  if (fp==NULL) return(false);
  fgets(linha,1000,fp);
  sscanf(linha,"%d %d",&n,&m);
  sprintf(name,"Grafo com %d vertices e %d arestas",n,m); 
  
  if (!gr_initgraph(g,2*n,2*m)) return(false); /* consertar depois */
  gr_setgraphname(g,name);
  nposicoes = 0;
  for (i=0;i<n;i++) {
    fgets(linha,1000,fp);
    j = 0;
    k = strlen(linha);
    l = 0;
    while ((j<k) && (space(linha[j]))) j++;
    while ((j<k) && (!space(linha[j]))) {
      if (l<MAXLABELNAME-1) {
	name[l] = linha[j];
	l++;
      }
      j++;
    }
    name[l] = '\0';
    v = gr_insertvertex(g,name);
    if (v==-1) {
      printf("Arquivo %s inconsistente (vertice %s)\n",filename,name);
      fclose(fp);
      gr_closegraph(g);
      return(false);
    }
    if (sscanf(&linha[j],"%lf %lf",&x,&y)==2) {
      nposicoes++;
      g->vertex[v].pos_x = x;
      g->vertex[v].pos_y = y;
    }
  }
  if (nposicoes==g->nvertices) g->positions = true;
  else g->positions = false;
  for (i=0;i<m;i++) {
    gr_getnextname(name,fp);
    u = gr_getvertexindex(g,name);
    gr_getnextname(name,fp);
    v = gr_getvertexindex(g,name);
    fscanf(fp,"%lf",&peso);
    
    sprintf(name,"(%d , %d , %10.2lf)",u,v,peso);
    gr_insertedge(g,name,u,v,peso);
  }
  fclose(fp);
  return(true);
}

boolean gr_readmetricgraph(graphtype *g,char *filename)
{
  int n,m,i,j,v;
  double x,y,ix,iy,jx,jy;
  double peso;
  FILE *input;
  char name[100];

  input = fopen(filename,"r");
  if (input==NULL) {
    printf("Erro ao abrir arquivo de entrada\n");
    return(false);
  }
  
  fscanf(input,"%d",&n);
  if (n<=0) {
    printf("Invalid number (negative or null)\n");
    fclose(input);
    return(false);
  }
  m = (n*(n-1))/2;
  if (!gr_initgraph(g,n,m)) {
    printf("Nao foi possivel iniciar o grafo metrico do arquivo %s\n",
	   filename);
    fclose(input);
    return(false);
  }
  for (i=0;i<n;i++) {
    gr_getnextname(name,input);
    v = gr_insertvertex(g,name);
    if (v==-1) {
      printf("Erro nos dados do arquivo %s\n",filename);
      fclose(input);
      gr_closegraph(g);
      return(false);
    }
    fscanf(input,"%lf",&x);
    fscanf(input,"%lf",&y);
    g->vertex[v].pos_x = x;
    g->vertex[v].pos_y = y;
  }
  fclose(input);

  for (i=0;i<n;i++) {
    for (j=i+1;j<n;j++) {
      ix = g->vertex[i].pos_x;
      iy = g->vertex[i].pos_y;
      jx = g->vertex[j].pos_x;
      jy = g->vertex[j].pos_y;
      #ifndef METRICINTEGEREDGEWEIGHT
      peso = sqrt((double) ((ix-jx)*(ix-jx)+(iy-jy)*(iy-jy)));
      #endif
      #ifdef METRICINTEGEREDGEWEIGHT
      peso = (double) ((int) (sqrt((double) ((ix-jx)*(ix-jx)+(iy-jy)*(iy-jy)))+0.5));
      #endif
      if (gr_insertedge(g,"",i,j,peso)==-1) {
	printf("Erro na insercao de aresta (readmetricgraph)\n");
	gr_closegraph(g);
	return(false);
      }
    }
  }
  g->positions = true;
  return(true);
}

gr_filetype gr_ismetricfile(char *filename)
{
  FILE *fp;
  char linha[1000];
  int n,m,k;
  gr_filetype t;
  fp = fopen(filename,"r");
  if (fp==NULL) return(NULLFILE);
  fgets(linha,1000,fp);
  k = sscanf(linha,"%d %d",&n,&m);
  if (k==1){
    printf("Arquivo metrico\n");
    t = METRICFILE;
  }else if (k==2) {
    printf("Arquivo Aresta\n");
    t = EDGEFILE;
  }else t = NULLFILE;
  fclose(fp);
  return(t);
}

boolean gr_readgraph(graphtype *g,char *filename)
{
  switch (gr_ismetricfile(filename)) {
  case NULLFILE: printf("Error opening file %s\n\n",filename); return(false);
  case METRICFILE:
    if (!gr_readmetricgraph(g,filename)){
      printf("Erro na leitura do arquivo %s (grafo metrico)\n\n",filename);
      return(false);
    }
    break;
  case EDGEFILE:
    if (!gr_readedgegraph(g,filename)){
      printf("Erro na leitura do arquivo %s (grafo aresta)\n\n",filename);
      return(false);
    }
    break;
  }
  return(true);
}




/*-------------------------------------------------------------*/
void gr_printgraph(graphtype *g)
{
  int i,v,e,a,tam;
  /*printf("\n\n*******************************************************\n");
    printf("Graph name = %s\n",g->graphname);*/
  /*printf("Numero de vertices = %d (max %d)\n",g->nvertices,g->maxvertex);
    printf("Numero de arestas  = %d (max %d)\n",g->nedges,g->maxedges);*/
  printf("Numero de vertices = %d\n",g->nvertices);
  printf("Numero de arestas  = %d\n",g->nedges);
    
  /*printf("Impressao dos vertices usados:\n");*/
  printf("Impressao dos vertices:\n");
  v = gr_getfirstvertex(g);
  tam=0;
  while (v!=-1) {
    tam++;
    if (g->vertex[v].used)
      {
	/*printf("(v=%d,nom=%s,nex=%d,pre=%d,firstadj=%d,used=%d)\n",
	       v,g->vertex[v].vertexname,
	       g->vertex[v].next,g->vertex[v].prev,
	       g->vertex[v].firstadj,g->vertex[v].used);*/
	printf("nom=%s\n",g->vertex[v].vertexname);
      }
    else gr_erro("ERRO vertex used\n");
    v = gr_getnextvertex(g,v);
  }
  /*printf("Tivemos %d vertices usados\n",tam);
  printf("\nImpressao dos vertices livres:\n");
  tam=0;
  v = g->firstfreevertex;
  while (v!=-1) {
    tam++;
    if (!g->vertex[v].used)
      printf("(v=%d,nom=%s,nex=%d,pre=%d,firstadj=%d,used=%d)\n",
	     v,vname(v),
	     g->vertex[v].next,g->vertex[v].prev,
	     g->vertex[v].firstadj,g->vertex[v].used);
    else gr_erro("ERRO vertex free\n");
    v = g->vertex[v].next;
  }
  printf("Tivemos %d vertices livres\n",tam);*/
  printf("\n");
  /*printf("Impressao dos dados das arestas:\n");*/
  printf("Lista de arestas\n");
  e = gr_getfirstedge(g);
  tam = 0;
  while (e!=-1) {
    tam++;
    if (g->edge[e].used)
      {
	/*printf("(pos=%d, (%s,%s), nex=%d, pre=%d,"
	       "indadj_u=%d,indadj_v=%d, used=%d)\n",
	       e,vname(g->edge[e].u),vname(g->edge[e].v),
	       g->edge[e].next,g->edge[e].prev,
	       g->edge[e].indadj_u,g->edge[e].indadj_v,
	       g->edge[e].used);*/
	printf("(%s,%s) \tpeso/cap = %f\n", vname(g->edge[e].u),vname(g->edge[e].v), g->edge[e].weight);
      }
    else gr_erro("ERRO 1\n");
    e = gr_getnextedge(g,e);
  }
  /*printf("Tivemos %d arestas usadas\n",tam);*/
  
  /*printf("\nLista de arestas livres\n");
  tam = 0;
  e = g->firstfreeedge;
  while (e!=-1) {
    tam++;
    if (!g->edge[e].used)
      printf("(e=%d, nex=%d, pre=%d, used=%d)\n",
	     e,g->edge[e].next,g->edge[e].prev,g->edge[e].used);
    else gr_erro("ERRO 2\n");
    e = g->edge[e].next;
  }
  printf("Tivemos %d arestas livres\n",tam);*/
  
  /*printf("\nImpressao das listas de adjacencia dos vertices usados:\n");*/
  printf("\nImpressao das listas de adjacencia dos vertices:\n");
  v = gr_getfirstvertex(g);
  while (v!=-1) 
    {
      printf("Adjacentes de %s\n",g->vertex[v].vertexname);
      a = g->vertex[v].firstadj;
      while (a!=-1) 
	{
	  /*printf("(a=%d,e=%s,v_adj=%s,nex=%d,pre=%d) ",
	    a,ename(g->adj[a].e),vname(g->adj[a].v_adj),g->adj[a].next,g->adj[a].prev);*/
	  printf("%s, ", vname(g->adj[a].v_adj));
	  a = g->adj[a].next;
	}
      printf("\n");
      v = gr_getnextvertex(g,v);
    }
  /*printf("\n");
    printf("Lista de arestas pela tabela Hashing (tam = %d)\n",g->hashsize);
  for (i=0;i<g->hashsize;i++) {
    if (g->edgehash[i]!=-1) {
      printf("Pos %d: ",i);
      a = g->edgehash[i];
      while (a!=-1) {
	gr_printedge(g,g->hashlist[a].e,"aresta = ");
	a = g->hashlist[a].next;
      }
      printf("\n");
    }
    }*/
  /*printf("\n");
  printf("Lista de posicoes livres de hash (tam = %d)\n",g->hashsize);
  a = g->nextfreehashpos;
  tam=0;
  while (a!=-1) {
    tam++;
    printf("(pos=%d,next=%d)\n",a,g->hashlist[a].next);
    a = g->hashlist[a].next;
  }
  printf("\nTemos %d elementos livres na tabela hash\n",tam);*/
  /*printf("Tamanho da tabela hashing = %d\n",g->hashsize);
  printf("Tabela Hashing\n"); 
  for (i=0;i<g->hashsize;i++) { 
  printf("(i=%d,%d) ",i,g->edgehash[i]);
  } 
  printf("\n");
  printf("Listas ligadas da tabela hash\n"); 
  for (i=0;i<g->maxedges;i++) {
    printf("(i=%d,e=%d,u=%d,v=%d,next=%d) ",i, 
	   g->hashlist[i].e, 
	   g->hashlist[i].menor_v, 
	   g->hashlist[i].maior_v, 
	   g->hashlist[i].next); 
	   }*/
}


/* aplicar o algoritmo de Kamada e Kawai (por enquanto, aproveitando 
o neato) */
boolean gr_geraposicoes(graphtype *g)
{
  FILE *fp;
  char linha[1000];
  int v,i,t;
  float x,y;
  
  g->positions = false;
  if (g->nvertices<1) return(false);
  gr_writegraph_neato(g,"tmp_pos.dot"); /* grava grafo sem posicoes */
  /* o programa neato sem tipo de saida gera o mesmo arquivo contendo
     as posicoes calculadas */
  system("neato -Goverlap=false tmp_pos.dot -o tmp_pos2.dot");
  /* em seguida pegamos o arquivo com as posicoes e as extraimos */
  fp = fopen("tmp_pos2.dot","r"); 
  if (fp==NULL) {
    gr_erro("Erro (gr_geraposicoes) nao consegui abrir tmp_pos2.dot");
    return(false);
  }
  while (!feof(fp)) {
    if (fgets(linha,1000,fp)) {
      t = gr_busca_padrao(linha,"[lp=\"");
      if (t!=-1) {
	sscanf(linha+t+5,"%d,%d",&g->lp1,&g->lp2);
	break;
      }
    }
  }
  while (!feof(fp)) {
    if (fgets(linha,1000,fp)) {
      t = gr_busca_padrao(linha,"bb=\"");
      if (t!=-1) {
	sscanf(linha+t+5,"%d,%d,%d,%d",&g->bb1,&g->bb2,&g->bb3,&g->bb4);
	break;
      }
    }
  }
  for (i=0;i<g->nvertices;i++) {
    gr_getnextname(linha,fp);
    v = gr_getvertexindex(g,linha);
    if (v==-1) {
      gr_erro("Vertice %s nao encontrado no calculo de posicoes.\n");
      fclose(fp);
      return(false);
    }
    if (fgets(linha,1000,fp)) {
      t = gr_busca_padrao(linha,"pos=\"");
      if (t!=-1) {
	sscanf(linha+t+5,"%f,%f",&x,&y);
	g->vertex[v].pos_x = x;
	g->vertex[v].pos_y = y;
      } else {
	printf("Erro na leitura do arquivo de posicoes");
	fclose(fp);
	return(false);
      }
    }
  }
  fclose(fp);
  if (gr_sistema==GR_LINUX) {
    system("rm tmp_pos.dot");
    system("rm tmp_pos2.dot");
  }else{
    system("del tmp_pos.dot");
    system("del tmp_pos2.dot");
  } 
  g->positions = true;
  return(true);
}


  
/* retorna o primeiro vertice do grafo, -1 se o grafo nao te vertices */
int gr_getfirstvertex(graphtype *g)
{
  if (!g) return(-1);
  return(g->firstusedvertex);
}

/* retorna o proximo vertice seguinte a "v", -1 se o grafo nao mais vertices */
int gr_getnextvertex(graphtype *g,int v)
{
  if (!gr_existsvertex(g,v)) return(-1);  /* se nao existir mais vertices, retorna -1 */
  return(g->vertex[v].next);
}

/* retorna a primeira aresta do grafo, -1 se o grafo nao te arestas */
int gr_getfirstedge(graphtype *g)
{
  if (!g) return(-1);
  return(g->firstusededge);
}

/* retorna a aresta seguinte a "e", -1 se o grafo nao mais arestas */
int gr_getnextedge(graphtype *g,int e)
{
  if (!gr_existsedge(g,e)) return(-1);   /* se nao existir aresta, retorna -1 */
  return(g->edge[e].next);
}

/*----------------------------------------------------------------------------*/
/*----------------- Funcoes associadas a vertice -----------------------------*/
/*----------------------------------------------------------------------------*/

/* obtem o nome de um vertice. Retorna true se obteve sucesso, false c.c. */
boolean gr_getvertexname(graphtype *g,int v,char *name)
{
  if (!gr_existsvertex(g,v)) return(false);
  gr_copylabel(name,vname(v));
  return(true);
}
/* Retorna o ponteiro do nome do vertice. */
char *gr_vertexname(graphtype *g,int v)
{
  if (!gr_existsvertex(g,v)) return(NULL);
  return(g->vertex[v].vertexname);
}

/* esta funcao foi substituida pela de baixo (esta nao usa hashing) */
int gr_getvertexindex_old(graphtype *g,char *name)
{
  int i;
  char vertexname[MAXLABELNAME]; 
  gr_copylabel(vertexname,name); /* o motivo de ter copiado e' que o nome 
				 original pode nao ter final 0 */
  for (i=gr_getfirstvertex(g);i!=-1;i=gr_getnextvertex(g,i)) {
    if (g->vertex[i].used) {
      if (!strcmp(vertexname,g->vertex[i].vertexname)) return(i);
    } else gr_erro("Lista de vertices so' deveria ter vertices usados\n");
  }
  return(-1); /* o nome do vertice nao foi encontrado */
}

int gr_vertexnamehashpos(char *k)
{
  int i,hash = 0; /* uma funcao qualquer, nao sei se e' boa */
  i = MAXLABELNAME;
  while ((*k) && (i)) {
      hash = (hash << 5)-hash+(hash>>2) + *k++;
      i--;
    }
  if (hash<0) return(-hash);
  return(hash);
}

  
/* retorna o indice do vertice, dado o nome. -1 se nao encontrou */
int gr_getvertexindex(graphtype *g,char *name)
{
  int i,u;
  char vertexname[MAXLABELNAME]; 
  gr_copylabel(vertexname,name); /* o motivo de ter copiado e' que o nome 
				 original pode nao ter final 0 */
  i = (gr_vertexnamehashpos(vertexname) % g->vertexnamehashsize);
  u = g->vertexnamehash[i];
  while (u!=-1) {
    if (g->vertex[u].used) {
      if (!strcmp(vertexname,g->vertex[u].vertexname)) return(u);
    } else gr_erro("Lista de vertices so' deveria ter vertices usados\n");
    u = g->vertex[u].nextvertexname;
  }
  return(-1); /* o nome do vertice nao foi encontrado */
}

  


/* esta funcao retorna true se v pertence a g, false c.c. */
boolean gr_existsvertex(graphtype *g,int v)
{
  if (!g) return(false);
  if ((v<0) || (v>=g->maxvertex)) return(false);
  return(g->vertex[v].used);
}


/* insere um vertice. retorna um indice que e' o numero do vertice inserido
   (este indice e' um numero em [0,maxvertex-1] */
int gr_insertvertex(graphtype *g, char *name)
{
  int v,v2,i;
  if (g->firstfreevertex==-1) return(-1); /* nao tem espaco para mais um vertice */
  if (name==NULL) return(-1); /* devemos ter um nome para o vertice */
  if (!strcmp(name,"")) return(-1); /* nome nao pode ser vazio */
  if (gr_getvertexindex(g,name)!=-1) return(-1); /* nome ja' existe na tabela */
  v = g->firstfreevertex; /* pega o proximo vertice livre (ele existe) */
  g->nvertices++; 
  g->firstfreevertex = g->vertex[v].next; /* atualiza o proximo vertice livre */
  /* coloca o novo vertice no inicio da lista de usados */
  v2 = g->firstusedvertex;
  g->vertex[v].next = v2;
  g->vertex[v].prev = -1;
  if (v2!=-1) g->vertex[v2].prev = v;/*se existir segundo, este aponta para o primeiro*/
  g->firstusedvertex = v; /* atualiza o cabecalho dos vertices usados */

  gr_copylabel(g->vertex[v].vertexname,name); /* da' o nome do vetice */

  /* coloca o vertice no inicio da lista ligada dos nomes */
  i = gr_vertexnamehashpos(g->vertex[v].vertexname) % g->vertexnamehashsize;
  g->vertex[v].nextvertexname = g->vertexnamehash[i];
  g->vertexnamehash[i] = v;

  g->vertex[v].firstadj = -1; /* vertice novo nao tem arestas adjacentes */
  g->vertex[v].used = true; /* indicar que e' o indice v represetna um vertice usado */
  g->vertex[v].color = BLACK; /* o novo vertice nao tem cor */
  g->vertex[v].pos_x = -1; /* o novo vertice nao tem posicao definica */
  g->vertex[v].pos_y = -1;
  g->positions = false; /* quando tenho um vertice novo, nao tenho as 
			   posicoes calculadas para ele */
  return(v);
}

/* remove um vertice do grafo. retorna verdadeiro se o vertice foi removido,
 falso caso o vertice nao exista. Remove todas as arestas incidentes em v */
boolean gr_deletevertex(graphtype *g,int v)
{
  int e,pos,i,ant,u;
  if (!gr_existsvertex(g,v)) {
    printf("warning: delete of inexistent vertex %d\n",v); 
    return(false); 
  }
  g->sequencial = false;
  /* vamos tirar primeiro da tabela de hashing de nomes de vertices */
  i = gr_vertexnamehashpos(g->vertex[v].vertexname) % g->vertexnamehashsize;
  u = g->vertexnamehash[i]; /* encontra o primeiro vertice da colisao */
  if (u==v) { /* e' o primeiro da lista */
    g->vertexnamehash[i] = g->vertex[u].nextvertexname; /* facil tirar o primeiro*/
  } else {
    ant = u;
    u = g->vertex[u].nextvertexname;
    while (u!=v) {
      if (u==-1) {
	gr_erro("Nao encontrou o nome do vertice. "
	     "Obs.: Alteracao de nomes de vertices nao pode ser feita");
	break;
      }
      ant = u;
      u = g->vertex[u].nextvertexname;
    }
    if (u!=-1) {
      g->vertex[ant].nextvertexname = g->vertex[u].nextvertexname;
    }
  }

  /* agora vamos remover todas as arestas adjacentes a v */
  e = gr_getvertexfirstadj(g,v);
  while (e!=-1) {
    gr_deleteedge(g,e);
    e = gr_getvertexfirstadj(g,v);
  }
  /* agora podemos tirar o vertice mesmo */
  /* vamos tirar o vertice da lista de usados (duplamente ligada) */
  ant = g->vertex[v].prev;
  pos = g->vertex[v].next;
  if (ant==-1) g->firstusedvertex = pos; /* e' o primeiro vertice */
  else g->vertex[ant].next = pos;
  if (pos!=-1) g->vertex[pos].prev = ant;
  /* agora vamos por o vertice v na lista de livres (lista simples) */
  g->vertex[v].next = g->firstfreevertex;
  g->firstfreevertex = v;
  g->vertex[v].used = false; /* dizendo que o vertice nao esta' mais sendo usado */
  g->nvertices--;
  return(true);
}

/* determina a cor de um vertice, true se conseguiu */
boolean gr_setvertexcolor(graphtype *g,int v,colortype cor)
{
  if (!gr_existsvertex(g,v)) return(false);
  g->vertex[v].color = cor;
  return(true);
}


/* retorna o indice da aresta que e' o primeiro na lista de adjacencia, 
   -1 se nao tiver */
int gr_getvertexfirstadj(graphtype *g,int v)
{
  int a;
  if (!gr_existsvertex(g,v)) return(-1);
  a = g->vertex[v].firstadj;
  if (a==-1) return(-1); /* nao tem lista de adjacentes */
  return(g->adj[a].e); 
}


/* retorna o indice da proxima aresta, que e' adjacente a v, 
   e vem depois de e considerando a lista de adjacencia, -1 se nao tiver */
int gr_getvertexnextadj(graphtype *g,int v,int e)
{
  int a,p;
  if (!gr_existsvertex(g,v)) return(-1); /* se nao existir vertice, retorna -1 */
  if (!gr_existsedge(g,e)) return(-1);   /* se nao existir aresta, retorna -1 */
  if (v == g->edge[e].u) {
    a = g->edge[e].indadj_u; /* pega o elemento da lista de adjacencia de v */
    p = g->adj[a].next; /* vai para o proximo elemento da lista */
    if (p==-1) return(-1); /* a aresta e era a ultima da lista de adjacencia */
    else return(g->adj[p].e); /* existe proxima aresta. retorna o indice dela */
  } else if (v == g->edge[e].v) {
    a = g->edge[e].indadj_v; /* pega o elemento da lista de adjacencia de v */
    p = g->adj[a].next; /* vai para o proximo elemento da lista */
    if (p==-1) return(-1); /* a aresta e era a ultima da lista de adjacencia */
    else return(g->adj[p].e); /* existe proxima aresta. retorna o indice dela */
  } else return(-1); /* o vertice v nao esta' batendo com nenhum extremo de e */
}

void gr_listadjacency(graphtype *g,int v)
{
  int i;
  printf("\nAdjacentes do vertice %s\n",vname(v));
  i = gr_getvertexfirstadj(g,v);
  while (i!=-1) {
    gr_printedge(g,i,"aresta = ");
    i = gr_getvertexnextadj(g,v,i);
    printf("\n");
  }
} 



/* define as coordenadas x e y que o vertice v sera' desenhado, 
   true se conseguiu */
boolean gr_setvertexposition(graphtype *g,int v,double x,double y)
{
  if (!gr_existsvertex(g,v)) return(false);
  g->vertex[v].pos_x = x;
  g->vertex[v].pos_y = y;
  return(true);
}


/*----------------------------------------------------------------------------*/
/*----------------- Funcoes associadas a aresta  -----------------------------*/
/*----------------------------------------------------------------------------*/

/* os elementos da tabela de hashing (arestas), sao armazenadas discriminando o 
   vertice menor do maior */
void gr_menormaior(int *a,int *b,int u,int v)
{
  if (u<v) {*a = u; *b = v;} 
  else     {*a = v; *b = u;}
}

/* dado uma aresta na forma (u,v), esta funcao retorna um inteiro no intervalo
   [0,...,hashsize-1] */
int gr_hashposition(graphtype *g,int u,int v)
{
  int i,a,b;
  /* para que a busca de (u,v) sempre seja igual a busca de (v,u) */
  /* a conta abaixo e' uma qualquer. depois trocar por outra melhor */
  gr_menormaior(&a,&b,u,v);
  i = a*b*g->hashsize+a*g->hashsize+b*g->hashsize+a-b; 
  if (i<0) i = i*(-1); /* vai que um overflow faz este sinal ficar negativo...*/
  return(i % g->hashsize);
}

/* obtem o nome de uma aresta. Retorna true se obteve sucesso, false c.c. */
char *gr_edgename(graphtype *g,int e)
{
  if (!gr_existsedge(g,e)) return(false);
  return(g->edge[e].edgename);
}


/* se subg e' um subgrafo de g, e "e" e' uma aresta de "subg",
   esta funcao retorna a correspondente aresta em g. */
int gr_edge_graph_from_subgraph(graphtype *g,graphtype *subg,int e)
{
  int u,v,ge,gu,gv;
  if ((g==NULL)||(subg==NULL)) return(-1);
  if (!gr_existsedge(subg,e)) return(-1);
  u = subg->edge[e].u;
  v = subg->edge[e].v;
  gu = gr_getvertexindex(g,subg->vertex[u].vertexname);
  gv = gr_getvertexindex(g,subg->vertex[v].vertexname);
  if ((gu==-1)||(gv==-1)) return(-1);
  ge = gr_getedge(g,gu,gv);
  if (ge==-1) return(-1);
  return(ge);
}




/* retorna o indice da aresta correspondente a aresta (u,v). -1 se nao encontrou */
int gr_getedge(graphtype *g,int u, int v)
{
  int i,he,a,b;
  if (!gr_existsvertex(g,u)) return(-1);
  if (!gr_existsvertex(g,v)) return(-1);
  i = gr_hashposition(g,u,v);
  gr_menormaior(&a,&b,u,v);
  he = g->edgehash[i];
  while (he!=-1){
    if ((a == g->hashlist[he].menor_v) && (b == g->hashlist[he].maior_v)) 
      return(g->hashlist[he].e);
    he = g->hashlist[he].next;
  }
  return(-1); /* a aresta (u,v) nao pertence ao conjunto de arestas */
}


/* retorna true se existe a aresta e, false c.c. */
boolean gr_existsedge(graphtype *g,int e)
{
  if (!g) return(false);
  if ((e<0) || (e>=g->maxedges)) return(false);
  return(g->edge[e].used);
}


int gr_insertedge(graphtype *g,char *name,int u,int v,double weight)
{
  int e,e2,au,av,au2,av2,i,a,b,newpos;
  /* certificando que existem os extremos primeiro */
  if (!gr_existsvertex(g,u)) {printf("Vertice %d nao existe\n",u);return(-1);}
  if (!gr_existsvertex(g,v)) {printf("Vertice %d nao existe\n",v);return(-1);}
  e = gr_getedge(g,u,v);
  if (e!=-1) {printf("aresta %d ja' existe",u);gr_printedge(g,e,"aresta = ");return(-1);}
  /* a aresta ja' existe */
  if (g->firstfreeedge==-1) {printf("nao tem espaco para inserir aresta\n");return(-1);} /* nao tem espaco para mais arestas */

  /* agora sabemos que temos espaco e a aresta e' realmente nova */
  e = g->firstfreeedge; /* pega a proxima aresta livre */
  g->nedges++; 
  g->firstfreeedge = g->edge[e].next;/* atualiza a proxima aresta livre (lista simples)*/
  /* coloca a nova aresta no inicio da lista de usados */
  e2 = g->firstusededge;
  g->edge[e].next = e2; 
  g->edge[e].prev = -1;
  if (e2!=-1) g->edge[e2].prev = e;
  g->firstusededge = e; /* atualiza o cabecalho das arestas usados */

  /* agora vamos inserir a aresta e na lista de adjacencia de u */
  /* note que tenho certeza que ha' espaco, caso contrario teria tido problemas
   para inserir a aresta no passo anterior */

  /* remove um adj da lista livre e atualizo o cabecalho */
  au = g->firstfreeadj; 
  g->firstfreeadj = g->adj[au].next;
  /* insiro au na lista de usados */
  au2 = g->vertex[u].firstadj;
  g->adj[au].next = au2;
  g->adj[au].prev = -1;
  g->vertex[u].firstadj = au;
  if (au2!=-1) g->adj[au2].prev = au;
  g->adj[au].v_adj = v; /* informando que o adjacente a u e' o vertice v*/
  /* atualizo as informacoes com o vetor de arestas */
  g->adj[au].e = e;  
  g->edge[e].indadj_u = au;
  g->edge[e].u = u;

  /* agora vamos inserir a aresta e na lista de adjacencia de v */
  /* remove um adj da lista livre e atualizo o cabecalho */
  av = g->firstfreeadj; 
  g->firstfreeadj = g->adj[av].next;
  /* insiro av na lista de usados */
  av2 = g->vertex[v].firstadj;
  g->adj[av].next = av2;
  g->adj[av].prev = -1;
  g->vertex[v].firstadj = av;
  if (av2!=-1) g->adj[av2].prev = av;
  g->adj[av].v_adj = u; /* informando que o adjacente a v e' o vertice u */
  /* atualizo as informacoes com o vetor de arestas */
  g->adj[av].e = e;  
  g->edge[e].indadj_v = av;
  g->edge[e].v = v;

  g->edge[e].used = true; /* indicar que o indice e represetna uma aresta usada */
  g->edge[e].color = BLACK; /* a nova aresta tem cor preta */
  g->edge[e].weight = weight;
  gr_copylabel(g->edge[e].edgename,name);/* da' o nome para a aresta */

  /* agora vamos atualizar o hashing. Primeiro pegamos uma nova celula de hash */
  newpos = g->nextfreehashpos;
  g->nextfreehashpos = g->hashlist[newpos].next;
  /* pegamos onde deve entrar esta celula */
  i = gr_hashposition(g,u,v);
  /* inserimos no inicio da lista emcabecada pela posicao da funcao de hashing*/
  g->hashlist[newpos].next = g->edgehash[i];
  g->edgehash[i] = newpos;

  gr_menormaior(&a,&b,u,v);
  g->hashlist[newpos].e = e;
  g->hashlist[newpos].menor_v = a;
  g->hashlist[newpos].maior_v = b;

  return(e);
}


/* remove uma arestas do grafo. retorna verdadeiro se a aresta foi removida,
 falso caso a aresta nao exista. */
boolean gr_deleteedge(graphtype *g,int e)
{
  int a,b,h,i,ant,pos,v,u;
  if (!gr_existsedge(g,e)) return(false);
  gr_menormaior(&a,&b,g->edge[e].u,g->edge[e].v);
  
  g->sequencial = false;
  /* primeiro vamos tirar do hashing */
  ant = -1;
  h = gr_hashposition(g,a,b);
  i = g->edgehash[h];
  while (i!=-1) { 
    if ((a==g->hashlist[i].menor_v) && (b==g->hashlist[i].maior_v)) break; 
    ant = i; 
    i = g->hashlist[i].next; 
  }
  if (i==-1) gr_erro("Erro nos dados de gr_deleteedge\n");
  /* sabemos que a aresta existe (o while anterior deve ter parado pelo break) */
  if (ant==-1) { /* o elemento a remover e' o primeiro da lista */
    g->edgehash[h] = g->hashlist[i].next;
  } else {
    g->hashlist[ant].next = g->hashlist[i].next; /* existe um anterior */
  }
  g->hashlist[i].next = g->nextfreehashpos;
  g->nextfreehashpos = i;

  /* agora vamos remover do vetor de arestas */
  ant = g->edge[e].prev;
  pos = g->edge[e].next;
  if (ant==-1) { /* e' o primeiro da lista de arestas */
    g->firstusededge = pos;
    if (pos!=-1)  /* existe a proxima aresta */
      g->edge[pos].prev = -1;
  } else { /* nao e' o primeiro da lista */
    g->edge[ant].next = pos;
    if (pos!=-1)  /* existe a proxima aresta */
      g->edge[pos].prev = ant;
  }
  /* colocar e na lista de arestas livres */
  g->edge[e].next = g->firstfreeedge;
  g->firstfreeedge = e;
  g->edge[e].used = false;


  /* agora remover e da lista de adjacencia de u */
  a = g->edge[e].indadj_u;
  u = g->edge[e].u;
  ant = g->adj[a].prev;
  pos = g->adj[a].next;
  if (ant==-1) { /* e' o primeiro da lista de adjacencia */
    g->vertex[u].firstadj = pos;
    if (pos!=-1)  /* existe a proxima aresta da lista de adjacencia */
      g->adj[pos].prev = -1;
  } else { /* nao e' o primeiro da lista de adjacencia */
    g->adj[ant].next = pos;
    if (pos!=-1)  /* existe a proxima aresta da lista de adjacencia */
      g->adj[pos].prev = ant;
  }
  /* colocar elemento da lista de adj na lista de livres */
  g->adj[a].next = g->firstfreeadj;
  g->firstfreeadj = a;
  
  /* agora remover e da lista de adjacencia de v */
  a = g->edge[e].indadj_v;
  v = g->edge[e].v;
  ant = g->adj[a].prev;
  pos = g->adj[a].next;
  if (ant==-1) { /* e' o primeiro da lista de adjacencia */
    g->vertex[v].firstadj = pos;
    if (pos!=-1)  /* existe a proxima aresta da lista de adjacencia */
      g->adj[pos].prev = -1;
  } else { /* nao e' o primeiro da lista de adjacencia */
    g->adj[ant].next = pos;
    if (pos!=-1)  /* existe a proxima aresta da lista de adjacencia */
      g->adj[pos].prev = ant;
  }
  /* colocar elemento da lista de adj na lista de livres */
  g->adj[a].next = g->firstfreeadj;
  g->firstfreeadj = a;
  g->nedges--; /* decrementa o numero de arestas */
  return(true);
}

/* retorna um dos extremos da aresta e, -1 se nao existe aresta  */
int gr_getedgehead(graphtype *g,int e)
{
  if (!gr_existsedge(g,e)) return(-1);
  return(g->edge[e].u);
}

/* retorna outro extremo da aresta e, -1 se nao existe aresta  */
int gr_getedgetail(graphtype *g,int e)
{
  if (!gr_existsedge(g,e)) return(-1);
  return(g->edge[e].v);
}

/* retorna o peso da aresta e , -1 se nao existe aresta */
boolean gr_getedgeweight(graphtype *g,int e,double *peso)
{
  if (!gr_existsedge(g,e)) return(false);
  *peso = g->edge[e].weight;
  return(true);
}

/* retorna o peso da aresta e , -1 se nao existe aresta */
double gr_edgeweight(graphtype *g,int e)
{
  if (!gr_existsedge(g,e)) return(MINIMUMDOUBLE);
  return(g->edge[e].weight);
}

/* determina o peso de um vertice, -1 se nao existe aresta */
boolean gr_setedgeweight(graphtype *g,int e,double peso)
{
  if (!gr_existsedge(g,e)) return(false);
  g->edge[e].weight = peso;
  return(true);
}


/* retorna a cor da aresta e, -1 se nao existe aresta  */
boolean gr_getedgecolor(graphtype *g,int e, colortype *cor)
{
  if (!gr_existsedge(g,e)) return(false);
  *cor = g->edge[e].color;
  return(true);
}

/* determina a cor de uma aresta, -1 se nao existe aresta */
boolean gr_setedgecolor(graphtype *g,int e,colortype cor)
{
  if (!gr_existsedge(g,e)) return(false);
  g->edge[e].color = cor;
  return(true);
}


/* pinta todas as arestas de um grafo */
void gr_paintgraphedges(graphtype *g,colortype cor)
{
  int e;
  if (g==NULL) return;
  e = gr_getfirstedge(g);
  while (e!=-1) {
    gr_setedgecolor(g,e,cor);
    e = gr_getnextedge(g,e);
  }
}

/* pinta algumas arestas do grafo */
void gr_paintvectoredges(graphtype *g,int *edges,int m,colortype cor)
{
  int i;
  if (g==NULL) return;
  for (i=0;i<m;i++) gr_setedgecolor(g,edges[i],cor);
}

/* pinta todas as arestas de um grafo, considerando seu peso */
void gr_paintgraphscale(graphtype *g)
{
  int e;
  double peso;
  if (g==NULL) return;
  e = gr_getfirstedge(g);
  while (e!=-1) {
    gr_getedgeweight(g,e,&peso);
    if (peso>0.666) gr_setedgecolor(g,e,BLUE);
    else if (peso>0.333)  gr_setedgecolor(g,e,RED);
    else if (peso>0.000001) gr_setedgecolor(g,e,BLACK);
    else gr_setedgecolor(g,e,BLACK);
    e = gr_getnextedge(g,e);
  }
}
/* pinta todas as arestas de um grafo, considerando o peso correspondente
   em um vetor auxiliar*/
void gr_paintgraphvectorscale(graphtype *g,double *v)
{
  int e;
  double peso;
  if (g==NULL) return;
  e = gr_getfirstedge(g);
  while (e!=-1) {
    peso = v[e];
    if (peso>0.666) gr_setedgecolor(g,e,BLUE);
    else if (peso>0.333)  gr_setedgecolor(g,e,RED);
    else  if (peso>0.000001) gr_setedgecolor(g,e,BLACK);
    else gr_setedgecolor(g,e,NOCOLOR);
    e = gr_getnextedge(g,e);
  }
}

/* pinta todos os vertices de um grafo */
void gr_paintgraphvertex(graphtype *g,colortype cor)
{
  int e;
  if (g==NULL) return;
  e = gr_getfirstvertex(g);
  while (e!=-1) {
    gr_setvertexcolor(g,e,cor);
    e = gr_getnextvertex(g,e);
  }
}

void gr_paintgraph(graphtype *g,colortype cor)
{
  gr_paintgraphvertex(g,cor);
  gr_paintgraphedges(g,cor);
}


/* pinta as arestas de g que pertencem ao subgrafo subgraph */
void gr_paintsubgraph(graphtype *g,graphtype *sub) 
{
  int i,u,v,e;
  if ((g==NULL) || (sub==NULL)) return;
  gr_paintgraphedges(g,BLACK); /* tira as cores do grafo principal */
  /* percorre as arestas e pinta as arestas correspondentes */
  for (i=gr_getfirstedge(sub);i!=-1;i=gr_getnextedge(sub,i)) {
    u = gr_getvertexindex(g,sub->vertex[sub->edge[i].u].vertexname);
    v = gr_getvertexindex(g,sub->vertex[sub->edge[i].v].vertexname);
    e = gr_getedge(g,u,v);
    gr_setedgecolor(g,e,sub->edge[i].color);
  }
  /*  for (i=gr_getfirstvertex(sub);i!=-1;i=gr_getnextvertex(sub,i)) {
    u = gr_getvertexindex(g,sub->vertex[i].vertexname);
    g->vertex[u].color = sub->vertex[i].color;
    }*/
}


void gr_printedge(graphtype *g,int e,char *s)
{
  int a,b;
  if (!gr_existsedge(g,e)) {
    gr_erro("Erro (gr_printedge): grafo ou aresta nao existente");
    return;
  }
  gr_menormaior(&a,&b,g->edge[e].u,g->edge[e].v);
  printf("%s(%s,%s) ",s,vname(a),vname(b));
}


int gr_number_components(graphtype *g)
{
  unionfindtype uf;
  int ncomp,i,c1,c2;
  if (g==NULL) return(-1);
  init_uf(&uf,g->maxvertex);
  ncomp = g->nvertices;
  for (i=gr_getfirstedge(g);i!=-1;i=gr_getnextedge(g,i)) {
    c1 = find_uf(&uf,g->edge[i].u);
    c2 = find_uf(&uf,g->edge[i].v);
    if (c1!=c2) {
      ncomp--;
      union_uf(&uf,g->edge[i].u,g->edge[i].v);
    }
  }
  close_uf(&uf);
  return(ncomp);
}


void gr_quicksortrecursivo(graphtype *g,int *edge,int e,int d)
{
	int i,f;
	int aux;
	double x;
	if (e>d) return;
	if((e-d)!=0) {
		i = e;
		f = d;
		x = g->edge[edge[(e+d)/2]].weight;
		do {
			while (g->edge[edge[e]].weight < x) e++;
			while (g->edge[edge[d]].weight > x) d--;
			if (e<=d) {
				aux = edge[e];
                                edge[e] = edge[d];
				edge[d] = aux;
				e++;	d--;
			}
		}while (e<=d);
		gr_quicksortrecursivo(g,edge,i,e-1);
		gr_quicksortrecursivo(g,edge,e, f);
	}
}
void gr_quicksort(graphtype *g,int *edge,int tam)
{
	gr_quicksortrecursivo(g,edge,0,tam-1);
}


boolean gr_minimumspanningtree(graphtype *gr,graphtype *mst)
{
  unionfindtype uf;
  int *edge;
  int c1,c2,i,e,u,v,m;
  graphtype g;
  if (gr==NULL) {mst=NULL; return(false);}
  
  if (!gr_cleangraph(&g,gr)) return(false);

  if (g.nvertices>0) m=g.nvertices-1;
  else m = 0;

  if (!gr_initgraph(mst,g.nvertices,m)) return(false); 
  sprintf(mst->graphname,"Arvore geradora de G, com %d vertices",g.nvertices);
  for (i=gr_getfirstvertex(&g);i!=-1;i=gr_getnextvertex(&g,i)) {
    gr_insertvertex(mst,g.vertex[i].vertexname);
  }
  if (g.nedges>0) {
    edge = (int *) malloc(sizeof(int) * g.nedges);
    if (edge==NULL) {
      gr_closegraph(mst);
      return(false);
    }
    for (i=0,e=gr_getfirstedge(&g);e!=-1;e=gr_getnextedge(&g,e),i++) edge[i] = e;
    gr_quicksort(&g, edge, g.nedges);
    init_uf(&uf,g.nvertices);
    for (i=0;i<g.nedges;i++) {
      c1 = find_uf(&uf,g.edge[edge[i]].u);
      c2 = find_uf(&uf,g.edge[edge[i]].v);
      if (c1!=c2) {
	union_uf(&uf,g.edge[edge[i]].u,g.edge[edge[i]].v);
	u = gr_getvertexindex(mst,g.vertex[g.edge[edge[i]].u].vertexname);
	v = gr_getvertexindex(mst,g.vertex[g.edge[edge[i]].v].vertexname);
	gr_insertedge(mst,g.edge[edge[i]].edgename,u,v,g.edge[edge[i]].weight);
      }
    }
    free(edge);
    close_uf(&uf);
  }
  gr_closegraph(&g);
  return(true);
}

boolean gr_getminimumspanningtree(graphtype *g,int *mstedges, int *mstnedges)
{
  unionfindtype uf;
  int *edge;
  int c1,c2,i,e;
  if (g==NULL) return(false);
  *mstnedges = 0;
  if (g->nedges>0) {
    edge = (int *) malloc(sizeof(int) * g->nedges);
    if (edge==NULL) {
      return(false);
    }
    for (i=0,e=gr_getfirstedge(g);e!=-1;e=gr_getnextedge(g,e),i++) 
      edge[i] = e;
    gr_quicksort(g, edge, g->nedges);
    init_uf(&uf,g->nvertices);
    for (i=0;i<g->nedges;i++) {
      c1 = find_uf(&uf,g->edge[edge[i]].u);
      c2 = find_uf(&uf,g->edge[edge[i]].v);
      if (c1!=c2) {
	union_uf(&uf,g->edge[edge[i]].u,g->edge[edge[i]].v);
	mstedges[*mstnedges] = edge[i];
	*mstnedges = *mstnedges+1;
      }
    }
    free(edge);
    close_uf(&uf);
  }
  return(true);
}


/* supoe os vertices inicializados com cor branca */
void gr_depthfirstsearch(graphtype *g,int v,int pai,
		      int *pred,colortype *cor)
{
  int u,e;
  if (cor[v]==WHITE) {
    cor[v] = BLACK;
    pred[v] = pai;
    e = gr_getvertexfirstadj(g,v);
    while (e!=-1) {
      if (v==g->edge[e].v) u = g->edge[e].u;
      else u = g->edge[e].v;
      gr_depthfirstsearch(g,u,v,pred,cor);
      e = gr_getvertexnextadj(g,v,e);
    }
  }
}



/* esta rotina faz uma copia do grafo source para o grafo dest,
   mas faz com que os indices do grafo original sejam copiados com
   indices sequenciais. I.e., os vertices terao indices em 0,...,nvertices-1
 */
boolean gr_cleangraph(graphtype *dest,graphtype *source)
{
  int i,su,sv,du,dv,e,v;
  if (!gr_initgraph(dest,source->maxvertex+1,source->maxedges+1)){
    gr_erro("Erro (gr_cleangraph) nao consegui inicializar grafo");
    return(false);
  }
  gr_setgraphname(dest,source->graphname);
  for (i=gr_getfirstvertex(source);i!=-1;i=gr_getnextvertex(source,i)) {
    v = gr_insertvertex(dest,source->vertex[i].vertexname);
    if (v==-1) {
      gr_erro("Erro (gr_cleangraph) nao consegui inserir vertice");
      gr_closegraph(dest);
      return(false);
    }
  }
  /* isto tem de ser feito depois de inserir todos os vertices */
  /* um novo vertice faz com que todo o computo de posicoes se anule */
  if (source->positions) {
    dest->positions = source->positions;
    for (i=gr_getfirstvertex(source);i!=-1;i=gr_getnextvertex(source,i)) {
      v = gr_getvertexindex(dest,source->vertex[i].vertexname);
      dest->vertex[v].pos_x = source->vertex[i].pos_x;
      dest->vertex[v].pos_y = source->vertex[i].pos_y;
    }
  }
  for (i=gr_getfirstedge(source);i!=-1;i=gr_getnextedge(source,i)) {
    su = source->edge[i].u;
    sv = source->edge[i].v;
    du = gr_getvertexindex(dest,source->vertex[su].vertexname);
    dv = gr_getvertexindex(dest,source->vertex[sv].vertexname);
    e = gr_insertedge(dest,source->edge[i].edgename,du,dv,source->edge[i].weight);
    if (e==-1) {
      gr_erro("Erro (gr_cleangraph) nao consegui inserir aresta");
      gr_closegraph(dest);
      return(false);
    }
  }
  return(true);
}


graphtype *gr_getcleangraph(graphtype *g)
{
  graphtype *clean;
  boolean s;
  clean = (graphtype *) malloc(sizeof(graphtype));
  if (clean==NULL) return(NULL);
  s = gr_cleangraph(clean,g);
  if (!s) {
    free(clean);
    return(NULL);
  }
  return(clean);
}


boolean  gr_find_min_edge_ghc_tree(graphtype *tree,int s,int t,int *edgeindex)
{
  int i,*pred,u,e,v;
  colortype *cor;
  pred = (int *) malloc(sizeof(int)*tree->maxvertex);
  cor = (colortype *) malloc(sizeof(colortype)*tree->maxvertex);
  if (pred==NULL || cor==NULL) {
    gr_freenotnull(pred);
    gr_freenotnull(cor);
    return(false);
  }
  for (i=0;i<tree->maxvertex;i++) cor[i] = WHITE;
  gr_depthfirstsearch(tree,s,s,pred,cor);
  u = t;
  v = pred[t];
  *edgeindex = gr_getedge(tree,u,v);
  while (v!=s) {
    u = v;
    v = pred[u];
    e = gr_getedge(tree,u,v);
    if (tree->edge[e].weight < tree->edge[*edgeindex].weight)
      *edgeindex = e;
  }
  free(pred);
  free(cor);
  return(true);
}


/* Esta e' uma funcao que pode ser otimizada facilmente.
   Atualmente ela gera o corte minimo atraves da arvore de gomory cuts,
   (que gera n-1 chamadas de corte minimo). Exercicio: Aproveite o codigo
   de gomory cuts para que faca uma chamada de corte minimo 
   separando s e t*/

/*  ESTA ROTINA FOI SUBSTITUIDA PELA ROTINA QUE ESTA' EM mincut.c 
 por motivos de eficiencia */
boolean gr_min_st_cut2(graphtype *g,int s_g,int t_g,
		   graphtype *stcut,double *cutweight)
{
  graphtype ghctree,gcopy;
  unionfindtype uf;
  char st_graphname[100];
  int i,lbl,lbl_s,lbl_t,lbl_u,lbl_v,edgeindex,v,s,t,s_gh,t_gh,x,y,e;
  if ((!gr_existsvertex(g,s_g)) || (!gr_existsvertex(g,t_g))) {
    gr_erro("Erro (min_st_cut) vertices para corte invalidos.");
    return(false);
  }
  
  if (!gr_cleangraph(&gcopy,g)){
    gr_erro("Erro (min_st_cut) nao consegui fazer copia sequencial de g");
    return(false);
  }
  s = gr_getvertexindex(&gcopy,g->vertex[s_g].vertexname);
  t = gr_getvertexindex(&gcopy,g->vertex[t_g].vertexname);
  if ((s==-1) || (t==-1)) {
    printf("Erro (min_st_cut) vertices %s e %s invalidos no grafo.",
	     g->vertex[s_g].vertexname,g->vertex[t_g].vertexname);
     gr_closegraph(&gcopy);
    return(false);
  }
  
  gr_generate_ghc_tree(&gcopy,&ghctree);
  
  s_gh = gr_getvertexindex(&ghctree,gcopy.vertex[s].vertexname);
  t_gh = gr_getvertexindex(&ghctree,gcopy.vertex[t].vertexname);

  if (!gr_find_min_edge_ghc_tree(&ghctree,s_gh,t_gh,&edgeindex)){
    gr_closegraph(&ghctree);
    gr_closegraph(&gcopy);
    printf("nao encontrou aresta leve\n");
    return(false);
  }
  *cutweight = ghctree.edge[edgeindex].weight;

  sprintf(st_graphname,"Corte minimo entre vertices %s e %s de valor %10.4f",
	  gcopy.vertex[s].vertexname,gcopy.vertex[t].vertexname,*cutweight);
  init_uf(&uf,ghctree.nvertices);
  for (i=gr_getfirstedge(&ghctree);i!=-1;i=gr_getnextedge(&ghctree,i)) 
    if (i!=edgeindex) 
      union_uf(&uf,ghctree.edge[i].u,ghctree.edge[i].v);
  
  if (!gr_initgraph(stcut,gcopy.nvertices,gcopy.nedges)) {
    close_uf(&uf);
    gr_closegraph(&ghctree);
    printf("Erro na inicializacao do grafo stcut\n");
    return(false); 
  }
  gr_setgraphname(stcut,st_graphname);
  lbl_s = find_uf(&uf,s_gh);
  lbl_t = find_uf(&uf,t_gh);
  for (i=gr_getfirstvertex(&gcopy);i!=-1;i=gr_getnextvertex(&gcopy,i)) {
    lbl = find_uf(&uf,gr_getvertexindex(&ghctree,gcopy.vertex[i].vertexname));
    if (lbl==lbl_s) {
      v = gr_insertvertex(stcut,gcopy.vertex[i].vertexname);
      gr_setvertexcolor(stcut,v,RED);
    } else if (lbl==lbl_t) {
      v = gr_insertvertex(stcut,gcopy.vertex[i].vertexname);
      gr_setvertexcolor(stcut,v,BLUE);
    } else {
      gr_erro("Erro (min_st_cut) Grafo nao e' conexo.\n");
      close_uf(&uf);
      gr_closegraph(&ghctree);
      return(false);
    }
  }
  for (i=gr_getfirstedge(&gcopy);i!=-1;i=gr_getnextedge(&gcopy,i)) {
    lbl_u = find_uf(&uf,gr_getvertexindex(&ghctree,gcopy.vertex[gcopy.edge[i].u].vertexname));
    lbl_v = find_uf(&uf,gr_getvertexindex(&ghctree,gcopy.vertex[gcopy.edge[i].v].vertexname));
    if (lbl_u != lbl_v) {
      x = gr_getvertexindex(stcut,gcopy.vertex[gcopy.edge[i].u].vertexname);
      y = gr_getvertexindex(stcut,gcopy.vertex[gcopy.edge[i].v].vertexname);
      e = gr_insertedge(stcut,gcopy.edge[i].edgename,x,y,gcopy.edge[i].weight);
      stcut->edge[e].color = GREEN;
    }
  }
  close_uf(&uf);
  gr_closegraph(&ghctree);
  gr_closegraph(&gcopy);
  return(true);
}
/*---------------------------------------------------------------------*/
#define MAXPOINTPOSITION 6000 /* cada coordenada estara' 
				  no intervalo 0..MAXPOINTPOSITION*/
void getepscolor(char *epscolorname,colortype cor)
{
  switch(cor) {
  case BLACK: strcpy(epscolorname,"col0"); return;
  case BLUE: strcpy(epscolorname,"col1"); return;
  case GREEN: strcpy(epscolorname,"col2"); return;
  case RED: strcpy(epscolorname,"col4"); return;
  case WHITE: strcpy(epscolorname,"col7"); return;
  case NOCOLOR: strcpy(epscolorname,"col0"); return;
  }
  strcpy(epscolorname,"col0"); /* sem cor definida fica com preto */
}

    

boolean gr_viewmetricgraph(graphtype *g)
{
  FILE *fp;
  int gap,e,maxx, maxy, minx, miny,u,v,posx,posy,
    telax,posxu,posxv,posyu,posyv;
  char filename[100],epscolor[100];
  strcpy(filename,"tmp_metric.eps");
  fp = fopen(filename,"w");
  if (fp==NULL) {
    printf("Erro para abrir arquivo \"%s\"\n",filename);
    return(false);
  }
  if (g->nvertices<1){
    printf("Grafo sem vertices ou sem posicoes computadas\n");
    return(1);
  }
  if (!g->positions) gr_geraposicoes(g);
  maxx = -999999;
  maxy = -999999;
  minx =  999999;
  miny =  999999;
  for (v=gr_getfirstvertex(g);v!=-1;v=gr_getnextvertex(g,v)){
    if (g->vertex[v].pos_x > maxx) maxx = (int) g->vertex[v].pos_x;
    if (g->vertex[v].pos_x < minx) minx = (int) g->vertex[v].pos_x;
    if (g->vertex[v].pos_y > maxy) maxy = (int) g->vertex[v].pos_y;
    if (g->vertex[v].pos_y < miny) miny = (int) g->vertex[v].pos_y;
  }
  telax = 500;
  fprintf(fp,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(fp,"%%%%Title: x.eps\n");
  fprintf(fp,"%%%%Creator: fig2dev Version 3.2 Patchlevel 3c\n");
  fprintf(fp,"%%%%CreationDate: Thu Sep 12 13:02:34 2002\n");
  fprintf(fp,"%%%%For: fkm@hobbes.dcc.unicamp.br ()\n");
  fprintf(fp,"%%%%BoundingBox: 0 0 %d %d\n",telax,telax);
  fprintf(fp,"%%%%Magnification: 1.0000\n");
  fprintf(fp,"%%%%EndComments\n");
  fprintf(fp,"/$F2psDict 200 dict def\n");
  fprintf(fp,"$F2psDict begin\n");
  fprintf(fp,"$F2psDict /mtrx matrix put\n");
  fprintf(fp,"/col-1 {0 setgray} bind def\n");
  fprintf(fp,"/col0 {0.000 0.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col1 {0.000 0.000 1.000 srgb} bind def\n");
  fprintf(fp,"/col2 {0.000 1.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col3 {0.000 1.000 1.000 srgb} bind def\n");
  fprintf(fp,"/col4 {1.000 0.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col5 {1.000 0.000 1.000 srgb} bind def\n");
  fprintf(fp,"/col6 {1.000 1.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col7 {1.000 1.000 1.000 srgb} bind def\n");
  fprintf(fp,"/col8 {0.000 0.000 0.560 srgb} bind def\n");
  fprintf(fp,"/col9 {0.000 0.000 0.690 srgb} bind def\n");
  fprintf(fp,"/col10 {0.000 0.000 0.820 srgb} bind def\n");
  fprintf(fp,"/col11 {0.530 0.810 1.000 srgb} bind def\n");
  fprintf(fp,"/col12 {0.000 0.560 0.000 srgb} bind def\n");
  fprintf(fp,"/col13 {0.000 0.690 0.000 srgb} bind def\n");
  fprintf(fp,"/col14 {0.000 0.820 0.000 srgb} bind def\n");
  fprintf(fp,"/col15 {0.000 0.560 0.560 srgb} bind def\n");
  fprintf(fp,"/col16 {0.000 0.690 0.690 srgb} bind def\n");
  fprintf(fp,"/col17 {0.000 0.820 0.820 srgb} bind def\n");
  fprintf(fp,"/col18 {0.560 0.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col19 {0.690 0.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col20 {0.820 0.000 0.000 srgb} bind def\n");
  fprintf(fp,"/col21 {0.560 0.000 0.560 srgb} bind def\n");
  fprintf(fp,"/col22 {0.690 0.000 0.690 srgb} bind def\n");
  fprintf(fp,"/col23 {0.820 0.000 0.820 srgb} bind def\n");
  fprintf(fp,"/col24 {0.500 0.190 0.000 srgb} bind def\n");
  fprintf(fp,"/col25 {0.630 0.250 0.000 srgb} bind def\n");
  fprintf(fp,"/col26 {0.750 0.380 0.000 srgb} bind def\n");
  fprintf(fp,"/col27 {1.000 0.500 0.500 srgb} bind def\n");
  fprintf(fp,"/col28 {1.000 0.630 0.630 srgb} bind def\n");
  fprintf(fp,"/col29 {1.000 0.750 0.750 srgb} bind def\n");
  fprintf(fp,"/col30 {1.000 0.880 0.880 srgb} bind def\n");
  fprintf(fp,"/col31 {1.000 0.840 0.000 srgb} bind def\n");
  fprintf(fp,"\n");
  fprintf(fp,"end\n");
  fprintf(fp,"save\n");
  fprintf(fp,"newpath 0 %d moveto 0 0 lineto %d 0 lineto %d %d "
	  "lineto closepath clip newpath\n",telax,telax,telax,telax);
  fprintf(fp,"%d %d translate\n",-10,telax+10);
  fprintf(fp,"1 -1 scale\n");
  fprintf(fp,"\n");
  fprintf(fp,"/cp {closepath} bind def\n");
  fprintf(fp,"/ef {eofill} bind def\n");
  fprintf(fp,"/gr {grestore} bind def\n");
  fprintf(fp,"/gs {gsave} bind def\n");
  fprintf(fp,"/sa {save} bind def\n");
  fprintf(fp,"/rs {restore} bind def\n");
  fprintf(fp,"/l {lineto} bind def\n");
  fprintf(fp,"/m {moveto} bind def\n");
  fprintf(fp,"/rm {rmoveto} bind def\n");
  fprintf(fp,"/n {newpath} bind def\n");
  fprintf(fp,"/s {stroke} bind def\n");
  fprintf(fp,"/sh {show} bind def\n");
  fprintf(fp,"/slc {setlinecap} bind def\n");
  fprintf(fp,"/slj {setlinejoin} bind def\n");
  fprintf(fp,"/slw {setlinewidth} bind def\n");
  fprintf(fp,"/srgb {setrgbcolor} bind def\n");
  fprintf(fp,"/rot {rotate} bind def\n");
  fprintf(fp,"/sc {scale} bind def\n");
  fprintf(fp,"/sd {setdash} bind def\n");
  fprintf(fp,"/ff {findfont} bind def\n");
  fprintf(fp,"/sf {setfont} bind def\n");
  fprintf(fp,"/scf {scalefont} bind def\n");
  fprintf(fp,"/sw {stringwidth} bind def\n");
  fprintf(fp,"/tr {translate} bind def\n");
  fprintf(fp,"/tnt {dup dup currentrgbcolor\n");
  fprintf(fp,"  4 -2 roll dup 1 exch sub 3 -1 roll mul add\n");
  fprintf(fp,"  4 -2 roll dup 1 exch sub 3 -1 roll mul add\n");
  fprintf(fp,"  4 -2 roll dup 1 exch sub 3 -1 roll mul add srgb}\n");
  fprintf(fp,"  bind def\n");
  fprintf(fp,"/shd {dup dup currentrgbcolor 4 -2 roll mul 4 -2 roll mul\n");
  fprintf(fp,"  4 -2 roll mul srgb} bind def\n");
  fprintf(fp," /DrawEllipse {\n");
  fprintf(fp,"	/endangle exch def\n");
  fprintf(fp,"	/startangle exch def\n");
  fprintf(fp,"	/yrad exch def\n");
  fprintf(fp,"	/xrad exch def\n");
  fprintf(fp,"	/y exch def\n");
  fprintf(fp,"	/x exch def\n");
  fprintf(fp,"	/savematrix mtrx currentmatrix def\n");
  fprintf(fp,"	x y tr xrad yrad sc 0 0 1 startangle endangle arc\n");
  fprintf(fp,"	closepath\n");
  fprintf(fp,"	savematrix setmatrix\n");
  fprintf(fp,"	} def\n");
  fprintf(fp,"\n");
  fprintf(fp,"/$F2psBegin {$F2psDict begin "
	  "/$F2psEnteredState save def} def\n");
  fprintf(fp,"/$F2psEnd {$F2psEnteredState restore end} def\n");
  fprintf(fp,"\n");
  fprintf(fp,"$F2psBegin\n");
  fprintf(fp,"%%%%Page: 1 1\n");
  fprintf(fp,"10 setmiterlimit\n");
  fprintf(fp," %10.8lf %10.8lf sc\n",(double) MAXPOINTPOSITION/100000,(double) MAXPOINTPOSITION/100000);
  fprintf(fp,"%%\n");
  fprintf(fp,"%% Fig objects follow\n");
  fprintf(fp,"%%\n");
  fprintf(fp,"25.000 slw\n");
  gap = 300;
  for (e=gr_getfirstedge(g);e!=-1;e=gr_getnextedge(g,e)){
    if (g->edge[e].color==NOCOLOR) continue;
    u = g->edge[e].u;
    v = g->edge[e].v;
    posxu = (int) (MAXPOINTPOSITION*((double)(g->vertex[u].pos_x -minx))/
		  ((double) (maxx-minx)))+gap;
    posyu = MAXPOINTPOSITION - (int) (MAXPOINTPOSITION*((double)(g->vertex[u].pos_y -miny))/
		  ((double) (maxy-miny)))+gap;
    posxv = (int) (MAXPOINTPOSITION*((double)(g->vertex[v].pos_x -minx))/
		  ((double) (maxx-minx)))+gap;
    posyv = MAXPOINTPOSITION - (int) (MAXPOINTPOSITION*((double)(g->vertex[v].pos_y -miny))/
		  ((double) (maxy-miny)))+gap;
    getepscolor(epscolor,g->edge[e].color);
    fprintf(fp,"n %d %d m\n %d %d l gs %s s gr \n",posxu,posyu,posxv,posyv,epscolor);
  }
  for (v=gr_getfirstvertex(g);v!=-1;v=gr_getnextvertex(g,v)){
    posx = (int) (MAXPOINTPOSITION*((double)(g->vertex[v].pos_x -minx))/
		  ((double) (maxx-minx)))+gap;
    posy = MAXPOINTPOSITION - (int) (MAXPOINTPOSITION*((double)(g->vertex[v].pos_y -miny))/
		  ((double) (maxy-miny)))+gap;
    getepscolor(epscolor,g->vertex[v].color);
    /* os dois 30 e' o raio */
    fprintf(fp,"n %d %d 45 45 0 360 DrawEllipse gs %s 1.00 shd ef gr \n", 
	    posx,posy,epscolor);
  }
  fprintf(fp,"$F2psEnd\n");
  fprintf(fp,"rs\n");
  fclose(fp);
  switch (gr_output) {
    /* mude o visualizador (gsview/gv/gqview/...) de acordo com as suas necessidades */
  case GR_POSTSCRIPT_LINUX: 
    system("gv tmp_metric.eps");
    break;
  case GR_POSTSCRIPT_KDE: 
    system("kghostview tmp_metric.eps");
    break;
  case GR_POSTSCRIPT_WINDOWS: 
    system("gsview tmp_metric.eps");
    /* gsview pode ser obtido em www.cs.wisc.edu/~ghost/gsview/  */
    break;
  case GR_GIF_LINUX:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
    break;
  case GR_GIF_WINDOWS:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
    break;
  case GR_GIF_WINDOWS_MSPAINT:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
    break;
  case GR_GIF_WINDOWS_XP:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
    break;
  case GR_JPG_LINUX:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
    break;
  case GR_JPG_WINDOWS:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
   break;
  case GR_JPG_WINDOWS_MSPAINT:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
   break;
  case GR_JPG_WINDOWS_XP:
    printf("Nao foi possivel visualizar o grafo metrico.\n");
   break;
  }
  return(true);
}
