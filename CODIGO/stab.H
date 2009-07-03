#include "integer_program.H"
#include "stab_instance.H"

class Stab : public IntegerProgram {
  int n;
  std::vector<std::vector<bool> > interM;
  StabInstance instance;
public:
  Stab(StabInstance& instance);
  void getParam(int& ncol,int& nrow,char** rowtype,double** rhs,
                double** obj,int** colbeg, int** rowidx,
                double** matval,double** lb,double** ub,
                int& nmip,char** miptype,int** mipcol,bool& relaxed);

};
