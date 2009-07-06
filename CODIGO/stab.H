#include "integer_program.H"
#include "stab_instance.H"

class Stab : public IntegerProgram {
  int n;

  StabInstance instance;

  bool intersect(ii a, ii b);
  int nvertices(int u, int removed, graphtype *G, std::vector<bool> &marked);

public:
  Stab(StabInstance& instance);
  bool isfeasible(double *x);
  void getParam(int& ncol,int& nrow,char** rowtype,double** rhs,
                double** obj,int** colbeg, int** rowidx,
                double** matval,double** lb,double** ub,
                int& nmip,char** miptype,int** mipcol,bool& relaxed);
  virtual double heurPrimal(std::vector<double>& in, std::vector<double>& out);
  virtual bool heurCuts(std::vector<double>& xsol, int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval);
  virtual bool exactCuts(std::vector<double>& xsol,int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval);
};
