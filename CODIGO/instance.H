#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include <cstdio>

class Instance {
public:
  //talvez caia fora:
  virtual double* getNewObj(double* dual, int* mindex) { return NULL; } 
  virtual void loadFrom(FILE* fp) { }
  virtual int dim(void) { return 0; }
};

#endif
