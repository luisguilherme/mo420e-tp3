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
  std::vector<std::pair<double,int> >  pp;
  for(int e=0;e<m;e++) pp.pb(std::make_pair(in[e],e));
  
  sort(pp.rbegin(),pp.rend());
  for(int ed=0;ed<m;ed++) {
    int e = pp[ed].second;
    ii ij = etoij(e,n);
    int i = ij.first; int j = ij.second;
    if (paired[i] || paired[j]) continue;
    paired[i] = true;
    paired[j] = true;
    out[e] = 1.0;
  }

  double y = 0;

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
  /* Cria corte simples que viola restrição (3):
     Para cada aresta de valor maior que 0.5 e menor que 1.0:
       Procura S composto por ij e mais um k cujo ciclo tenha tamanho maior que 1
       Cria corte que impede S.
  */

  int dim = sz(xsol);
  int nelem = 0;
  std::vector<std::vector<double> > cuts;
  
  ncuts = 0;

  for(int i=0,e=0;i<n;i++) {
    for(int j=i+1;j<n;j++,e++) {
      if (xsol[e] > 0.5 + EPSILON  && xsol[e] < 1 - EPSILON) {
	for(int k=0;k<n;k++) {
	  if (k == i || k == j) continue;
	  int e1 = ijtoe(std::min(i,k),std::max(i,k),n);
	  int e2 = ijtoe(std::min(j,k),std::max(j,k),n);
	  if (xsol[e] + xsol[e1] + xsol[e2] > 1+EPSILON) {
	    cuts.pb(std::vector<double>(dim,0));
	    cuts[ncuts][e] = 1; cuts[ncuts][e1] = 1; cuts[ncuts][e2] = 1;
	    ncuts++;
	  }
	} // end k
      } //end i
    } // end j
  }

  nelem = 3*ncuts;
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
    (*qrtype)[cut] = 'L';
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
     de cada componente de $T - e$, se alguma delas é ímpar, encontrou um
     corte desejado.
   */
  int e, ncomp1, nelem;
  std::vector<bool> marked(gr_number_vertices(graph), false);
  std::vector<std::vector<double> > cuts;

  ncuts = nelem = 0;
  for (e = gr_getfirstedge(tree); gr_existsedge(tree, e);  e = gr_getnextedge(tree, e)) {
    if (gr_edgeweight(tree, e) >= 1.0 - EPSILON)
      continue;

    for (int i = 0; i < sz(marked); i++)
      marked[i] = false;
    ncomp1 = nvertices(gr_getedgehead(tree, e), e, tree, marked);

    if (ncomp1 % 2) {
      cuts.push_back(std::vector<double>(n, 0.0));
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
	      cuts[ncuts][f] = 1.0;
	      nelem++;
	    }
	  }
	}
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
