#include "stab_instance.H" 
#include "cutting.H"
#include "stab.H"
#include <cstdio>
#include <cstring>
#include <cstdlib>

/* mensagem de uso do programa */
void showUsage() {
	printf ("Uso: bnc <estrategia> <prof_max_para_corte> <instancia>\n");
        printf ("- estrategia: string de \"0\"\'s e \"1\"\'s de tamanho 2.\n");
	printf ("  - 1a. posição é \"1\" se a heurística primal é usada. \n");
	printf ("  - 2a. posição é \"1\" se as minhas \"cut inequalities\" forem separadas.\n");
	printf ("    Nota: estrategia=\"00\" equivale a um Branch-and-Bound puro\n");
	printf ("- prof_max_para_corte: maior altura de um nó para aplicar cortes\n");
	printf ("    (default=1000000) \n");
	printf ("- instancia: nome do arquivo contendo a instância \n");
}

int main(int argc, char* argv[]) {
  bool hp = false, bnc = false;

  if (argc != 4) {
    showUsage();
    return 0;
  }

  hp = (argv[1][0] - '0') == 1;
  bnc = (argv[1][1] - '0') == 1;

  StabInstance a;
  a.loadFrom(fopen(argv[3],"r"));

  Stab s(a);
  CuttingPlanes cp(s, hp, bnc, atoi(argv[2]));

  cp.solve();
  
  return(0);
}
