#include "stab_instance.H" 
#include "cutting.H"
#include "stab.H"
#include <cstdio>
#include <cstring>
#include <cstdlib>

/* mensagem de uso do programa */
void showUsage() {
  printf ("Uso: bnc <tipo-exec> <time-limit> <heur-primal> <separa-heur> <arq-input>\n");
  printf ("tipo-exec: b: branch-and-bound puro\n"
	  "           r: planos de corte puro\n"
          "           f: branch-and-bound completo\n"
	  "time-limit: tempo máximo de execução em segundos\n"
	  "heur-primal: 1 se usa heur. primal, 0 c.c\n"
	  "separa-heur: 1 se usa heur. separacao, 0 c.c\n"
	  "arq-input: caminho para o arquivo de entrada\n");
}

int main(int argc, char* argv[]) {
  int timelimit = MAX_CPU_TIME, bnc = 0;
  bool hp = false, sep = false;

  if (argc < 6) {
    showUsage();
    return 0;
  }

  hp = (argv[3][0] - '0' == 1);
  sep = (argv[4][0] - '0' == 1);
  timelimit = atoi(argv[2]);

  switch (argv[1][0]) {
  case 'b':
    bnc = 0;
    break;
  case 'r':
    bnc = 1;
    break;
  case 'f':
    bnc = 2;
    break;
  }

  StabInstance a;
  a.loadFrom(fopen(argv[5],"r"));

  FILE *sol, *est;
  char filename[128];
  sprintf(filename, "%s.sol", argv[5]);
  sol = fopen(filename, "w");
  sprintf(filename, "%s.est", argv[5]);
  est = fopen(filename, "w");

  Stab s(a);
  CuttingPlanes cp(s, hp, bnc, sep, timelimit, est, sol);
  std::vector<double> xstar((a.n*(a.n-1))/2, 0);
  cp.solve(xstar);

  /* arquivo com a solução */
  int m = (a.n*(a.n-1))/2;
  for (int i = 0; i < m-1; i++)
    if (xstar[i] > EPSILON)
      fprintf(sol, "%d %d\n", etoij(i, a.n).first, etoij(i, a.n).second);
  fprintf(sol, "%d\n", (int)xstar[m-1]);

  fclose(sol);
  fclose(est);
  
  return(0);
}
