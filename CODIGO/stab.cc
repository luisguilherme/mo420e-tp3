#include "integer_program.H"
#include "stab_instance.H"
#include "stab.H"
#include "ugraph.hpp"


void Stab::getParam(int& ncol,int& nrow,char** rowtype,double** rhs,
		    double** obj,int** colbeg, int** rowidx,
		    double** matval,double** lb,double** ub,
		    int& nmip,char** miptype,int** mipcol,bool& relaxed) {

  relaxed = false;
  nrow = n + (n*(n-1))/2;
  ncol = (n*(n-1))/2 + 1;
  nmip = ncol;

  *rowtype = (char*) calloc(nrow,sizeof(char));
  *rhs = (double*) calloc(nrow,sizeof(double));

  *obj = (double*) calloc(ncol,sizeof(double));
  *lb = (double*) calloc(ncol,sizeof(double));
  *ub = (double*) calloc(ncol,sizeof(double));
  *colbeg = (int*) calloc(ncol+1,sizeof(int));
  *miptype = (char*) calloc(ncol,sizeof(char));
  *mipcol = (int*) calloc(ncol,sizeof(int));

  /* Talvez seja necessário usar menos memória :( */
  *rowidx = (int*) calloc(nrow*ncol,sizeof(int));
  *matval = (double*) calloc(nrow*ncol,sizeof(double));

  int pos = 0;
  for(int i=0,e=0;i<n;i++) {
    for(int j=i+1;j<n;j++,e++) {
      /* as primeiras n linhas são referentes às arestas.
	 São 1 se estiverem em uma coluna correspondente a um dos vértices
      */
      (*lb)[e] = 0;
      (*ub)[e] = 1;
      (*obj)[e] = 0;
      (*colbeg)[e] = pos;
      (*miptype)[e] = 'B';
      (*mipcol)[e] = e;

      (*matval)[pos] = 1;
      (*rowidx)[pos++] = i;
      (*matval)[pos] = 1;
      (*rowidx)[pos++] = j;

      /* depois precalcular em interM se não for memória absurda */
      for(int ni=0,ri=n;ni<n;ni++)
	for(int nj=ni+1;nj<n;nj++,ri++)
	  if ( intersect(ii(ni,nj),ii(i,j)) ) {
	    (*matval)[pos] = 1;
	    (*rowidx)[pos++] = ri;
	  }
    }
  }

  /* variável y */
  (*colbeg)[ncol-1] = pos;
  (*miptype)[ncol-1] = 'I';
  (*mipcol)[ncol-1] = ncol - 1;
  (*ub)[ncol-1] = ncol;
  (*lb)[ncol-1] = 0;
  (*obj)[ncol-1] = 1;

  for(int r=0;r<n;r++) {
    (*rowtype)[r] = 'E';
    (*rhs)[r] = 1;
  }

  for(int r=n;r<nrow;r++) {
    (*rowtype)[r] = 'L';
    (*rhs)[r] = 0;

    (*matval)[pos] = -1;
    (*rowidx)[pos++] = r;
  }

  (*colbeg)[ncol] = pos;
}

bool Stab::isfeasible(double *x) {
  return false;
}

double Stab::heurPrimal(std::vector<double>& in, std::vector<double>& out) {
  int m = (n*(n-1))/2;
  out = std::vector<double>(m+1,0);
  std::vector<bool> paired(n,false);
  double y = 0;

  //for all vertices, pair it with the highest value
  for(int i=0,e=0;i<n;i++) {
    int maxp = -1;
    int pairp, edgep;
    if (paired[i]) continue; //evita computação inútil
    for(int j=i+1;j<n;j++,e++) {
      if (!paired[j] && maxp < in[e]) {
	maxp = in[e];
	pairp = j; edgep = e;
      }
    }
    paired[i] = paired[pairp] = true;
    out[edgep] = 1;

  }

  for(int i=0;i<n;i++) {
    for(int j=i+1;j<n;j++) {
      int prey = 0;
      for(int e=0;e<m;e++)
	if (out[e] > 1 - EPSILON && intersect(ii(i,j),etoij(e,n)) )
	  prey++;

      y = std::max((int) (y+EPSILON),prey);
    }
  }
  out[m] = y;
  return(y);
}

bool Stab::heurCuts(std::vector<double>& xsol, int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval) {
  /* Cria corte simples que viola restrição (6):
     Para cada aresta de valor maior que 0.5 e menor que 1.0:
       Cria um S composto de i e j.
       Cria corte somando o x de todas as arestas saindo de i e de j diferentes de ij sendo maior que 1.
  */

  int dim = sz(xsol);
  int nelem = 0;
  std::vector<std::vector<double> > cuts;

  ncuts = 0;

  for(int i=0,e=0;i<n;i++) {
    for(int j=i+1;j<n;j++,e++) {
      if (xsol[e] > 0.5 + EPSILON  && xsol[e] < 1 - EPSILON) {
	cuts.pb(std::vector<double>(dim,0));
	ncuts++;
	for(int ni=0,ne=0;ni<n;ni++)
	  for(int nj=ni+1;nj<n;nj++,ne++)
	    /* Se é uma aresta saindo de i ou saindo de j, mas não é i -- j,
	       ela faz parte do corte */
	    if ((ni != i || nj != j) &&
		(ni == i || ni == j || nj == i || nj == j)) {
	      cuts[ncuts-1][ne] = 1.0;
	      nelem++;
	    } // end if

      } //end i
    } // end j
  }

  if (ncuts == 0)
    return false;

  *mtype = (int*) calloc(ncuts,sizeof(int));
  *qrtype = (char*) calloc(ncuts,sizeof(char));
  *drhs = (double*) calloc(ncuts,sizeof(double));

  *mstart = (int*) calloc(ncuts+1,sizeof(int));

  *dmatval = (double*) calloc(nelem,sizeof(double));
  *mcols = (int*) calloc(nelem,sizeof(int));

  int nc = 0;
  for(int cut=0;cut<ncuts;cut++) {
    (*mstart)[cut] = nc;

    (*mtype)[cut] = 2; //Corte heurístico
    (*qrtype)[cut] = 'G';
    (*drhs)[cut] = 1;

    for(int col=0;col<dim;col++) {
      if (cuts[cut][col] > 1 - EPSILON) {
	(*dmatval)[nc] = 1;
	(*mcols)[nc++] = col;
      }
    }
  }
  (*mstart)[ncuts] = nc;

  return true;
}

bool Stab::exactCuts(std::vector<double>& xsol,int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval) {
  int n = sz(xsol);
  graphtype *graph, *tree;

  graph = instance.cost_graph;
  tree = gr_creategraph(gr_number_vertices(graph),
			gr_number_vertices(graph)-1);

  for (int i = 0; i < n-1; i++)
    gr_setedgeweight(graph, i, xsol[i]);
  gr_generate_ghc_tree(graph, tree);

  /* Para cada aresta da GH-T com peso < 1, conta o número de vértices
     de cada componente de $T - e$, se algum delas é ímpar, encontrou um
     corte.
   */
  int e, ncomp1, ncomp2, nelem;
  std::vector<bool> marked(gr_number_vertices(graph), false);
  std::vector<std::vector<double> > cuts;

  ncuts = nelem = 0;
  for (e = gr_getfirstedge(tree); gr_existsedge(tree, e);  e = gr_getnextedge(tree, e)) {
    if (gr_edgeweight(tree, e) >= 1.0 - EPSILON)
      continue;

    for (int i = 0; i < sz(marked); i++) marked[i] = false;
    ncomp1 = nvertices(gr_getedgehead(tree, e), e, tree, marked);
    ncomp2 = sz(marked) - ncomp1;

    //printf(" numero de vertices no corte(%d): %d %d\n", e, ncomp1, ncomp2);
    // printf(" custo do corte %lf\n", gr_edgeweight(tree, e));
    // graphtype *scut = gr_creategraph(gr_number_vertices(graph),
    // 				     gr_number_edges(graph));
    // double cut_weight;
    // gr_min_st_cut(graph,
    // 		  gr_getvertexindex(graph,
    // 				    gr_vertexname(tree,
    // 					       gr_getedgehead(tree, e))),
    // 		  gr_getvertexindex(graph,
    // 				    gr_vertexname(tree,
    // 						  gr_getedgetail(tree, e))),
    // 		  scut, &cut_weight);
    // printf(" custo do corte manual %lf\n", cut_weight);

    if (ncomp1 % 2) {
      cuts.push_back(std::vector<double>(n, 0.0));
      double peso = 0.0;
      for (int i = 0; i < sz(marked); i++)
	if (marked[i]) {
	  int u = gr_getvertexindex(graph,
				    gr_vertexname(tree, i));

	  for (int f = gr_getvertexfirstadj(graph, u);
	       gr_existsedge(graph, f); f = gr_getvertexnextadj(graph, u, f)) {

	    if((gr_getedgehead(graph, f) == u &&
		!marked[gr_getvertexindex(tree,
					  gr_vertexname(graph,
							gr_getedgetail(graph, f)))]) ||
	       (gr_getedgetail(graph, f) == u &&
		!marked[gr_getvertexindex(tree,
					  gr_vertexname(graph,
							gr_getedgehead(graph, f)))])) {

	      //printf("aresta do corte: %d\n", f);
	      cuts[ncuts][f] = 1.0;
	      peso += gr_edgeweight(graph, f);
	      nelem++;
	    }
	  }
	  //printf("  %d está em S\n", i);
	}
      //printf("peso no original: %lf\n", peso);
      ncuts++;
    }

    if (ncomp2 % 2) {
      cuts.push_back(std::vector<double>(n, 0.0));
      double peso = 0.0;
      for (int i = 0; i < sz(marked); i++)
	if (!marked[i]) {
	  int u = gr_getvertexindex(graph,
				    gr_vertexname(tree, i));

	  for (int f = gr_getvertexfirstadj(graph, u);
	       gr_existsedge(graph, f); f = gr_getvertexnextadj(graph, u, f)) {

	    if((gr_getedgehead(graph, f) == u &&
		marked[gr_getvertexindex(tree,
					  gr_vertexname(graph,
							gr_getedgetail(graph, f)))]) ||
	       (gr_getedgetail(graph, f) == u &&
		marked[gr_getvertexindex(tree,
					  gr_vertexname(graph,
							gr_getedgehead(graph, f)))])) {

	      //printf("aresta do corte: %d\n", f);
	      cuts[ncuts][f] = 1.0;
	      peso += gr_edgeweight(graph, f);
	      nelem++;
	    }
	  }
	  //printf("  %d está em S\n", i);
	}
      //printf("peso no original: %lf\n", peso);
      ncuts++;
    }
  }

  if (ncuts == 0)
    return false;

  /* cria estrutura do Xpress para cortes */
  *mtype = (int*) calloc(ncuts,sizeof(int));
  *qrtype = (char*) calloc(ncuts,sizeof(char));
  *drhs = (double*) calloc(ncuts,sizeof(double));

  *mstart = (int*) calloc(ncuts+1,sizeof(int));

  *dmatval = (double*) calloc(nelem,sizeof(double));
  *mcols = (int*) calloc(nelem,sizeof(int));

  int nc = 0;
  for(int cut=0;cut<ncuts;cut++) {
    (*mstart)[cut] = nc;

    (*mtype)[cut] = 3; // corte exato
    (*qrtype)[cut] = 'G';
    (*drhs)[cut] = 1;

    for(int col=0;col<n;col++) {
      if (cuts[cut][col] >= 1.0 - EPSILON) {
	(*dmatval)[nc] = 1.0;
	(*mcols)[nc++] = col;
      }
    }
  }
  (*mstart)[ncuts] = nc;

  return true;
}

int Stab::nvertices(int u, int removed, graphtype *G, std::vector<bool> &marked) {
  if (marked[u])
    return 0;

  int nv = 0;
  marked[u] = true;
  for (int e = gr_getvertexfirstadj(G, u);
       gr_existsedge(G, e); e = gr_getvertexnextadj(G, u, e)) {
    if (removed == e) continue;
    nv += nvertices(gr_getedgehead(G, e), e, G, marked) +
      nvertices(gr_getedgetail(G, e), e, G, marked);
  }

  return nv + 1;
}

inline bool Stab::intersect(ii a, ii b) {
  //return ( present(instance.intersection[a],b) );
  return instance.interM[ijtoe(a.first,a.second,instance.n)][ijtoe(b.first,b.second,instance.n)];
}

Stab::Stab(StabInstance& instance) {
  this->instance = instance;
  n = instance.n;
  nrows = n + (n*(n-1))/2;
  ncols = (n*(n-1))/2 + 1;

  // for(int i=0;i<n;i++)
  //   for(int j=0;j<n;j++)
  // 	traverse(

}
