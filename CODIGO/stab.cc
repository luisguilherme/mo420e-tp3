#include "integer_program.H"
#include "stab_instance.H"
#include "stab.H"



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
    y = (double) std::max((int)(y+EPSILON),instance.nintersections[edgep]);
  }
  out[m] = y;
  return(y);
}

inline bool Stab::intersect(ii a, ii b) {
  return ( present(instance.intersection[a],b) );
}

Stab::Stab(StabInstance& instance) {
  this->instance = instance;
  n = instance.n;

  // for(int i=0;i<n;i++)
  //   for(int j=0;j<n;j++) 
  // 	traverse(

}
