#include "stab_instance.H" 

int main(int argc, char* argv[]) {
  StabInstance a;
  a.loadFrom(fopen(argv[1],"r"));
  return(0);
}
