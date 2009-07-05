#include "stab_instance.H" 
#include "cutting.H"
#include "stab.H"

int main(int argc, char* argv[]) {
  StabInstance a;

  a.loadFrom(fopen(argv[1],"r"));

  Stab s(a);
  CuttingPlanes cp(s, true, false);

  cp.solve();
  
  return(0);
}
