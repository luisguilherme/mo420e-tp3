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
  FILE *arq_est, *arq_sol;

  int *mrwind, *mgcols;
  char *qgtype;
  double *rhs, *obj, *dlb, *dub, melhor_limitante_dual;

public:
  CuttingPlanes(IntegerProgram &ip, bool hp, int bnc, bool sep, int tle, FILE *arq_est, FILE *arq_sol);
  void HeuristicaPrimal(int node);
  bool Corte(int node, double *x, int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval);
  int nvars() { return ip.getncols(); }
  bool solve(std::vector<double> &xstar);

};
