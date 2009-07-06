#include "integer_program.H"
#include "xprs.h"


class CuttingPlanes {
  IntegerProgram &ip;

  /* - variaveis  para  inclusao de  cortes:  qrtype,  mstart e  dmatval
   *   usadas para carga inicial do LP tambem. */
  int n, m;
  int *mcols, *mtype, *mstart;
  char *qrtype;
  double *drhs, *dmatval; 

  int *mrwind, *mgcols;
  char *qgtype;
  double *rhs, *obj, *dlb, *dub, melhor_limitante_dual;

public:
  CuttingPlanes(IntegerProgram &ip, bool hp, bool bnc, int mndfs);
  void HeuristicaPrimal(int node);
  bool CorteExato(int node, double *x, int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval);
  int nvars() { return ip.getncols(); }
  bool solve();

};
