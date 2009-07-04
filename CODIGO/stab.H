#include "integer_program.H"
#include "stab_instance.H"


class Stab : public IntegerProgram {
  int n;
  std::vector<std::vector<bool> > interM;
  StabInstance instance;
  bool intersect(ii a, ii b);
public:
  
  Stab(StabInstance& instance);
  void getParam(int& ncol,int& nrow,char** rowtype,double** rhs,
                double** obj,int** colbeg, int** rowidx,
                double** matval,double** lb,double** ub,
                int& nmip,char** miptype,int** mipcol,bool& relaxed);
  virtual double heurPrimal(std::vector<double>& in, std::vector<double>& out);
  virtual bool heurCuts(std::vector<double>& xsol, int& ncuts, int** mtype, char** qrtype, double** drhs, int** mstart, int** mcols, double** dmatval);
};
