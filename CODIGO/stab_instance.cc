#include "stab_instance.H"
#include <iostream>

void StabInstance::loadFrom(FILE* fp) {
  char name[255];
  int m;
  fscanf(fp," NAME : %[^\n]",name);
  fscanf(fp," %*s %*[^\n]");
  fscanf(fp," %*s %*[^\n]");
  fscanf(fp," DIMENSION : %d",&n);
  fscanf(fp," EDGE_WEIGHT_TYPE : EUC_2D");
  fscanf(fp," NODE_COORD_SECTION");

  nome = std::string(name);
  for(int i=0;i<n;i++) {
    int ind;
    double x, y;
    fscanf(fp," %d %lf %lf",&ind,&x,&y);
    assert( i == ind - 1 );
    ponto.pb(std::make_pair(x,y));
  }

  m = (n*(n-1))/2;

  interM = std::vector<std::vector<bool> >(m);
  nintersections = std::vector<int>(m);
  fscanf(fp," %*[^\n]");
  for(int i=0,e=0;i<n;i++) {
    for(int j=i+1;j<n;j++,e++) {
      int pi, pj, k;
      ii pt;
      fscanf(fp," %d %d %d", &pi, &pj, &k);
      assert(pi == i+1 && pj == j+1);
      nintersections[e] = k;
      interM[e] = std::vector<bool>(m,false);
      pt = ii(i,j);
      for(int r=0;r<k;r++) {
	fscanf(fp," %d %d",&pi, &pj);
	interM[e][ijtoe(pi,pj,n)] = 1;
	//intersection[pt].insert(ii(pi-1,pj-1));
      }
    }
  }
  fscanf(fp," EOF");

  cost_graph = gr_creategraph(n, m);

  /* constrói o grafo */
  for (int i = 0; i < n; i++) {
    sprintf(name, "%d", i);
    gr_insertvertex(cost_graph, name);
  }

  for (int i = 0; i < n; i++)
    for (int j = i+1; j < n; j++) {
      sprintf(name, "%d-%d", i, j);
      gr_insertedge(cost_graph, name, i, j, 1.0);
    }
}

StabInstance::~StabInstance() {
  gr_closegraph(cost_graph);
}
