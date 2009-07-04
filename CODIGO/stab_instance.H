#ifndef __STAB_INSTANCE_H__
#define __STAB_INSTANCE_H__

#include <cstdio>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <set>
#include "instance.H"
#include "ugraph.hpp"
#include "definitions.H"

// extern gr_os_type gr_sistema;
// extern gr_outputtype gr_output;

typedef std::pair<double,double> point;
typedef std::pair<int,int> ii;
typedef std::pair<ii,ii> inter;

class StabInstance : public Instance {
public:
  
  int n;
  std::string nome;
  std::vector<point> ponto;
  std::map<ii,std::set<ii> > intersection;
  std::vector<int> nintersections;
  /* talvez saia daqui e vá para o stab.H */
  graphtype *cost_graph;

  virtual void loadFrom(FILE* fp);
  
  //virtual double* getNewObj(double* dual, int* mindex) { return NULL; }
  virtual int dim(void) { return 0; }
  ~StabInstance();
};

#endif
