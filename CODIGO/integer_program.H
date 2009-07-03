#ifndef __INTEGER_PROGRAM_H__
#define __INTEGER_PROGRAM_H__

#include "instance.H"
#include <vector>

class IntegerProgram { 
  Instance instance;
protected:
  int ncols, nrows;
public:
  IntegerProgram() {}
  virtual Instance& getInstance() { return instance; }
  virtual int getncols() { return ncols; }
  virtual int getnrows() { return nrows; }
  // virtual std::vector<double>& getcost() { return cost; }
  virtual void getParam(int&,int&,char**,double**,double**,int**,int**,
			double**,double**,double**,int&,char**,int**,bool&) { }
  virtual void addcut() {} //depois fazer assinatura
  
  virtual double heurPrimal(std::vector<double>& in, std::vector<double>& out) { return 0.0; }
};

#endif
