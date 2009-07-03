#include "integer_program.H"

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
  CuttingPlanes(IntegerProgram &ip, bool hp, bool bnc);
  bool solve();

};
