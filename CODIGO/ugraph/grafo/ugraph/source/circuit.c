
#include "ugraph.h"
#include "boolean.h"
#include "circuit.h"


/* so' chamo esta rotina para um vertice v que seja branco */
boolean gr_dfs_circuit(graphtype *g,int *nseq,int *seq,int *cor,
		       int *pilha,int npilha,int v,int pai_v,
		       boolean *edgesetmask)
{
  int e,u,i,j,k;
  cor[v] = GR_DFS_CIRCUIT_CINZA;
  pilha[npilha] = v;
  npilha++;
  for (e=gr_getvertexfirstadj(g,v);e!=-1;e=gr_getvertexnextadj(g,v,e)){
    /* se for uma aresta proibida, vai para a proxima */
    if (!edgesetmask[e]) continue;
    if (g->edge[e].v==v) u = g->edge[e].u;
    else u = g->edge[e].v;
    if (u==pai_v) continue;
    if (cor[u]==GR_DFS_CIRCUIT_CINZA) { /* encontrou um circuito */
      i=npilha-1;
      while (pilha[i]!=u) i--;
      /* localizei o inicio do circuito */
      for (j=i,k=0;j<npilha;j++,k++)
	seq[k] = pilha[j];
      *nseq = k;
      return(true);
    } else if (cor[u]==GR_DFS_CIRCUIT_BRANCO) {
      if (gr_dfs_circuit(g,nseq,seq,cor,pilha,npilha,u,v,edgesetmask))
	return(true);
      cor[u] = GR_DFS_CIRCUIT_PRETO;
    }
  }
  return(false);
}

/* retorna um circuito qualquer de g */
/* retorna true sse encontrou */
/* retorna em ncircuitedges a quantidade de arestas do circuito encontrado */
/* o grafo deve ser sequencial, i.e., todas os vertices devem ser de 0..n-1
   a sequencia dos vertices do circuito sao definidas por
   {seq[0],...seq[i],...,seq[(*nseq)-1],seq[0]}*/
boolean gr_get_circuit(graphtype *g,int *nseq,int *seq,boolean *edgesetmask)
{
  int *cor,branco,preto,cinza,*pilha,i,v;
  boolean r; 
  
  cor = (int *) malloc(sizeof(int)*g->nvertices);
  pilha = (int *) malloc(sizeof(int)*g->nvertices);
  if ((cor==NULL) || (pilha==NULL)) {
    printf("Problemas de alocacao de memoria em gr_get_circuit\n");
    exit(1);
  }
  branco = 0;
  cinza = 1;
  preto = 2;
  for (i=0;i<g->nvertices;i++) cor[i] = branco;
  r = false;
  while (!r) {
    for(i=0;i<g->nvertices;i++)
      if (cor[i]==branco) break;
    if (i==g->nvertices) break;
    else v = i;
    r = gr_dfs_circuit(g,nseq,seq,cor,pilha,0,v,-1,edgesetmask);
  }
  free(cor);
  free(pilha);
  return(r);
}

  
  
    


  
