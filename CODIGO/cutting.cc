/* ******************************************************************
   Arquivos  de exemplo  para  o desenvolvimento  de  um algoritmo  de
   branch-and-cut  usando  o XPRESS.   Este  branch-and-cut resolve  o
   problema  da mochila  0-1 e  usa  como cortes  as desigualdades  de
   cobertura simples (cover inequalities) da  forma x(c) < |C| -1 onde
   C é um conjunto de itens formando uma cobertura minimal.

   Autor: Cid Carvalho de Souza 
          Instituto de Computação - UNICAMP - Brazil

   Data: segundo semestre de 2003

   Arquivo: princ.c
   Descrição: arquivo com programas em C
 * *************************************************************** */

 
/* includes dos arquivos .h */
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include "integer_program.H"
#include "cutting.H"
#include "definitions.H"
#include "xprs.h"

/* variaveis globais */

/* valor de retorno das rotinas do XPRESS */
int xpress_ret;   
/* status retornado por algumas rotinas do XPRESS */
int xpress_status;   
/* armazena a melhor solucao inteira encontrada */
double *xstar;   
/* armazena o valor da solucao otima */
double zstar;  
/* armazena as solucoes do LP e do ILP  */
double *x;    
/* - diz se usara ou nao a heuristica primal */
bool HEURISTICA_PRIMAL;
/* - diz se usara ou nao cortes */
bool BRANCH_AND_CUT;
/* - nó onde encontrou a melhor solucao inteira */
int NODE_BEST_INTEGER_SOL;
/* estrutura do XPRESS contendo o problema */
XPRSprob prob;   
/* contador do total de cortes por nó da arvore B&B */
int totcuts;  
/* contador do total de nós explorados B&B */
int totnodes;  
/* contador do total de lacos de separacao de cortes por nó da arvore B&B */
int itersep;   
/* - profundidade maxima de um no da arvore de B&B aonde sera feita separacao */
int MAX_NODE_DEPTH_FOR_SEP;
/* - valor otimo da primeira relaxacao */
double objval_relax;
/* - valor da relaxacao linear no final do 1o nó */
double objval_node1;

/* declaracoes das callbacks */
int XPRS_CC Cortes(XPRSprob prob, void* classe);
void XPRS_CC SalvaMelhorSol(XPRSprob prob, void *my_object);

/* rotinas auxiliares */
void errormsg(const char *sSubName,int nLineNo,int nErrCode);
void ImprimeSol(double *x, int n);
void HeuristicaPrimal(int node);
void Mochila(double *c,int *w,int b,int n,int *x,double *val);

/* mensagem de uso do programa */
void showUsage() {
	printf ("Uso: bnc <estrategia> <prof_max_para_corte> < <instancia> \n");
        printf ("- estrategia: string de \"0\"\'s e \"1\"\'s de tamanho 2.\n");
	printf ("  - 1a. posição é \"1\" se a heurística primal é usada. \n");
	printf ("  - 2a. posição é \"1\" se as minhas \"cover inequalities\" forem separadas.\n");
	printf ("    Nota: estrategia=\"00\" equivale a um Branch-and-Bound puro\n");
	printf ("- prof_max_para_corte: maior altura de um nó para aplicar cortes\n");
	printf ("  (default=1000000) \n");
	printf ("- instancia: nome do arquivo contendo a instância \n");
}

CuttingPlanes::CuttingPlanes(IntegerProgram &ip, bool hp, bool bnc) : ip(ip) {
    bool relaxed;

    // FIXME: colocar em IntegerProgram
    char probname[] = "stab";

    /* inicializa valores de variaveis globais */
    HEURISTICA_PRIMAL = hp; BRANCH_AND_CUT = bnc;
    totcuts=0; totnodes = 0; itersep=0; zstar=XPRS_PLUSINFINITY; 
    MAX_NODE_DEPTH_FOR_SEP = 1000000;
    NODE_BEST_INTEGER_SOL = -1;

    ip.getParam(n, m, &qrtype, &rhs, &obj, &mstart, &mrwind,
		&dmatval, &dlb, &dub, n, &qgtype, &mgcols, relaxed);

    /* incializacão do XPRESS */
    xpress_ret=XPRSinit("");
    if (xpress_ret) errormsg("Main: Erro de inicializacao do XPRESS.\n",__LINE__,xpress_ret);

    /* "cria" o problema  */
    xpress_ret=XPRScreateprob(&prob);
    if (xpress_ret) errormsg("Main: Erro na initializacao do problema",__LINE__,xpress_ret);

    /* ======================================================================== */
    /* Atribui valores a vários parametros de controle do XPRESS                */
    /* ======================================================================== */

    /* limita o tempo de execucao */
    xpress_ret=XPRSsetintcontrol(prob,XPRS_MAXTIME,MAX_CPU_TIME);
    if (xpress_ret) errormsg("Main: Erro ao tentar setar o XPRS_MAXTIME.\n",__LINE__,xpress_ret);

    /* aloca  espaço extra de  linhas para inserção de  cortes. CUIDADO:
       Isto tem que ser feito ANTES (!) de carregar o problema ! */
    xpress_ret=XPRSsetintcontrol(prob,XPRS_EXTRAROWS,MAX_NUM_CORTES+1);
    if (xpress_ret) 
      errormsg("Main: Erro ao tentar setar o XPRS_EXTRAROWS.\n",__LINE__,xpress_ret);
  
    /*  aloca espaço  extra  de  elementos não  nulos  para inserção  de
	cortes. CUIDADO: Isto  tem que ser feito ANTES  (!) de carregar o
	problema ! */
    xpress_ret=XPRSsetintcontrol(prob,XPRS_EXTRAELEMS,n*n);
    if (xpress_ret) 
      errormsg("Main: Erro ao tentar setar o XPRS_EXTRAELEMS.",__LINE__,xpress_ret);
    
    xpress_ret=XPRSloadglobal(prob, probname, n, m,  qrtype, rhs, NULL, obj, mstart, NULL, 
			      mrwind, dmatval, dlb, dub, n, 0, qgtype, mgcols, NULL, NULL, NULL,  NULL, NULL);
    if (xpress_ret) errormsg("Main: Erro na carga do modelo.",__LINE__,xpress_ret);

    /* libera memoria dos vetores usado na carga do LP */
    free(qrtype);   free(rhs);    free(obj);      free(mstart);   free(mrwind);   free(dmatval); 
    free(dlb);      free(dub);    free(qgtype);   free(mgcols); 
    
    /* salva um arquivo ".lp" com o LP original */
    xpress_ret=XPRSwriteprob(prob,"LP","l");
    if (xpress_ret) 
      errormsg("Main: Erro na chamada da rotina XPRSwriteprob.\n",__LINE__,xpress_ret);
    
    /* Desabilita o PRESOLVE: o problema da mochila é muito fácil para o XPRESS */
    xpress_ret=XPRSsetintcontrol(prob,XPRS_PRESOLVE,0);
    if (xpress_ret) errormsg("Main: Erro ao desabilitar o presolve.",__LINE__,xpress_ret);
}
bool CuttingPlanes::solve() {
  /* impressão para conferência */
  if (HEURISTICA_PRIMAL) printf("*** Heurística Primal será usada\n");

  if (BRANCH_AND_CUT) { 

    printf("*** Algoritmo de branch-and-cut.\n");
    /*  aloca  espaco  para  as  estruturas que  serao  usadas  para
     * armazenar  os cortes  encontrados na separacao.   Para evitar
     * perda de tempo, estas  estruturas sao alocadas uma unica vez.
     * Para  isso, o  tamanho das mesmas  deve ser o  maior possivel
     *  para  comportar os  dados  obtidos  por  *qualquer* uma  das
     * rotinas  de separacao. No  caso a rotina de  separação poderá
     *  gerar até um  "cover ineuqality"  por vez.  Ler no  manual a
     * descrição da rotina XPRSaddcuts */
    qrtype=(char *)malloc(sizeof(char)); 
    mtype=(int *) malloc(sizeof(int));
    drhs=(double *)malloc(sizeof(double));
    mstart=(int *)malloc((n+1)*sizeof(int));
    mcols=(int *)malloc(n*sizeof(int));   /* cada corte tera no maximo n nao-zeros */
    dmatval=(double *)malloc(n*sizeof(double));

    /* callback indicando que sera feita separacao de cortes em cada
       nó da arvore de B&B */
    xpress_ret=XPRSsetcbcutmgr(prob,Cortes,this);

    if (xpress_ret) 
      errormsg("Main: Erro na chamada da rotina XPRSsetcbcutmgr.\n",__LINE__,xpress_ret);
    /*=====================================================================================*/
    /* RELEASE NOTE: The controls CPKEEPALLCUTS, CPMAXCUTS and CPMAXELEMS have been removed*/    
    /* Diz ao XPRESS que quer manter cortes no pool 
       xpress_ret=XPRSsetintcontrol(prob,XPRS_CPKEEPALLCUTS,0); 
       if (xpress_ret)   
       errormsg("Main: Erro ao tentar setar o XPRS_CPKEEPALLCUTS.\n",__LINE__,xpress_ret); */ 
    /*=====================================================================================*/
  } 
  else { 

    printf("*** Algoritmo de branch-and-bound puro.\n");

    qrtype=NULL;   mtype=NULL;   drhs=NULL;   mstart=NULL;   
    mcols=NULL;    dmatval=NULL;

    if (HEURISTICA_PRIMAL) {
      /*  callback indicando que sera feita separacao de cortes em
	  cada nó da  arvore de B&B.  Mas, neste  caso, a rotina
	  nao  faz   corte,  limitando-se  apenas   a  executar  a
	  heuristica primal. */
      xpress_ret=XPRSsetcbcutmgr(prob,Cortes,this);
      if (xpress_ret) 
	errormsg("Main: rotina XPRSsetcbcutmgr.\n",__LINE__,xpress_ret);
    }
  }
  /* Desabilita a separacao de cortes do XPRESS. */
  xpress_ret=XPRSsetintcontrol(prob,XPRS_CUTSTRATEGY,0);
  if (xpress_ret) 
    errormsg("Main: Erro ao tentar setar o XPRS_CUTSTRATEGY.\n",__LINE__,xpress_ret);
  
  /* callback para salvar a melhor solucao inteira encontrada */
  xpress_ret=XPRSsetcbintsol(prob, SalvaMelhorSol, NULL);
  if (xpress_ret) 
    errormsg("Main: Erro na chamada da rotina XPRSsetcbintsol.\n",__LINE__,xpress_ret);
  
  /* aloca  espaco  para o  vetor  "x"  que  contera as  solucoes  das
   * relaxacoes e  de "xstar" que armazenara a  melhor solucao inteira
   * encontrada. */
  x=(double *)malloc(n*sizeof(double));
  xstar=(double *)malloc(n*sizeof(double));
  
  /* resolve o problema */
  xpress_ret=XPRSminim(prob,"g");
  if (xpress_ret) errormsg("Main: Erro na chamada da rotina XPRSminim.\n",__LINE__,xpress_ret);
  
  /* imprime a solucao otima ou a melhor solucao encontrada (se achou) e o seu valor */
  xpress_ret=XPRSgetintattrib(prob,XPRS_MIPSTATUS,&xpress_status);
  if (xpress_ret) 
    errormsg("Main: Erro na chamada da rotina XPRSgetintatrib.\n",__LINE__,xpress_ret);
  
  if ((xpress_status==XPRS_MIP_OPTIMAL)  || 
      (xpress_status==XPRS_MIP_SOLUTION) ||
      (zstar < XPRS_PLUSINFINITY)){
    
    XPRSgetintattrib(prob,XPRS_MIPSOLNODE,&NODE_BEST_INTEGER_SOL);
    printf("\n");
    printf("- Valor da solucao otima =%12.6f \n",(double)(zstar));
    printf("- Variaveis otimas: (nó=%d)\n",NODE_BEST_INTEGER_SOL);
    
    if (zstar == XPRS_PLUSINFINITY) {
      xpress_ret=XPRSgetsol(prob,xstar,NULL,NULL,NULL);
      if (xpress_ret) 
	errormsg("Main: Erro na chamada da rotina XPRSgetsol\n",__LINE__,xpress_ret);
    }
    ImprimeSol(xstar, n);
    
  } 
  else  printf("Main: programa terminou sem achar solucao inteira !\n");

  /* impressao de estatisticas */
  printf("********************\n");
  printf("Estatisticas finais:\n");
  printf("********************\n");
  printf(".total de cortes inseridos ........ = %d\n",totcuts);
  printf(".valor da FO da primeira relaxação. = %.6f\n",objval_relax);
  printf(".valor da FO no nó raiz ........... = %.6f\n",objval_node1);
  
  xpress_ret=XPRSgetintattrib(prob,XPRS_NODES,&totnodes);
  if (xpress_ret) 
    errormsg("Main: Erro na chamada da rotina XPRSgetintatrib.\n",__LINE__,xpress_ret);
  
  printf(".total de nós explorados .......... = %d\n",totnodes);
  printf(".nó da melhor solucao inteira ..... = %d\n",NODE_BEST_INTEGER_SOL);
  printf(".valor da melhor solucao inteira .. = %d\n",(int)(zstar+0.5));  
  /* somar 0.5 evita erros de arredondamento */
  
  /* verifica o valor do melhor_limitante_dual */
  xpress_ret=XPRSgetdblattrib(prob,XPRS_BESTBOUND,&melhor_limitante_dual);
  if (xpress_ret) 
    errormsg("Main: Erro na chamada de XPRSgetdblattrib.\n",__LINE__,xpress_ret);
  
  if (melhor_limitante_dual > zstar+EPSILON)
    melhor_limitante_dual=zstar; 
  printf(".melhor limitante dual ............ = %.6f\n",melhor_limitante_dual);
  
  /* libera a memoria usada pelo problema */

  xpress_ret=XPRSdestroyprob(prob);

  /* libera toda memoria usada no programa */
  free(qrtype);
  free(mtype);
  free(drhs);
  free(mstart);
  free(mcols);
  free(dmatval);
  free(x);
  free(xstar);

  xpress_ret=XPRSfree();
  if (xpress_ret)
    errormsg("Main: Erro na liberacao de memoria final.\n",__LINE__,xpress_ret);
  printf("========================================\n");

  return true;
}

/* =====================================================================
 * Rotina (callback) para salvar a  melhor solucao.  Roda para todo nó
 * onde acha solucao inteira.
 * =====================================================================
 */
void XPRS_CC SalvaMelhorSol(XPRSprob prob, void *my_object)
{
   int i, cols, peso_aux=0, node;
   double objval;
   bool viavel;

   /* pega o numero do nó corrente */
   xpress_ret=XPRSgetintattrib(prob,XPRS_NODES,&node);
   if (xpress_ret) 
     errormsg("SalvaMelhorSol: rotina XPRSgetintattrib.\n",__LINE__,xpress_ret);

   xpress_ret=XPRSgetintattrib(prob,XPRS_COLS,&cols);
   if (xpress_ret) 
       errormsg("SalvaMelhorSol: rotina XPRSgetintattrib\n",__LINE__,xpress_ret);

   xpress_ret=XPRSgetdblattrib(prob,XPRS_LPOBJVAL,&objval);
   if (xpress_ret) 
       errormsg("SalvaMelhorSol: rotina XPRSgetdblattrib\n",__LINE__,xpress_ret);

   xpress_ret=XPRSgetsol(prob,x,NULL,NULL,NULL);
   if (xpress_ret) 
     errormsg("SalvaMelhorSol: Erro na chamada da rotina XPRSgetsol\n",__LINE__,xpress_ret);

   /* testa se a solução é viável
      FIXME: porque? essa callback só não é chamada quando a solução é viável?*/
   // for(i=0;i<cols;i++) peso_aux += x[i]*w[i];
   // viavel=(peso_aux <= W + EPSILON);

   /*
   printf("\n..Encontrada uma solução inteira (nó=%d): valor=%f, peso=%d,",
	  node,objval,peso_aux);
   if (viavel) printf(" viavel\n"); else printf(" inviavel\n");
   for(i=0;i<cols;i++) 
     if (x[i]>EPSILON) printf(" x[%3d] = %12.6f (w=%6d, c=%12.6f)\n",i,x[i],w[i],c[i]);
   */
   
   viavel = true;

   /* se a solucao tiver custo melhor que a melhor solucao disponivel entao salva */
   if ((objval < zstar-EPSILON) && viavel) {

     printf(".. atualizando melhor solução ...\n");
     for(i=0;i<cols;i++) xstar[i]=x[i];
     zstar=objval;
     
     /* informa xpress sobre novo incumbent */
     xpress_ret=XPRSsetdblcontrol(prob, XPRS_MIPABSCUTOFF, zstar + 1.0 - EPSILON);
     if (xpress_ret) 
       errormsg("SalvaMelhorSol: XPRSsetdblcontrol.\n",__LINE__,xpress_ret);
     
     NODE_BEST_INTEGER_SOL = node; 

     /* Impressão para saída */
     printf("..Melhor solução inteira encontrada no nó %d, peso %d e custo %12.6f\n",
	    NODE_BEST_INTEGER_SOL, peso_aux, zstar);
     printf("..Solução encontrada: \n");

     ImprimeSol(x, cols);
   }
   return;
}

/**********************************************************************************\
 * Rotina para separacao de Cover inequalities. Roda em todo nó.
 * Autor: Cid Carvalho de Souza 
 * Data: segundo semestre de 2003
\**********************************************************************************/
int XPRS_CC Cortes(XPRSprob prob, void* classe)
{
  int encontrou, i, irhs, k, node, node_depth, solfile, ret;
  int nLPStatus, nIntInf;

  /*  variaveis para a separacao das cover inequalities */
  int *peso, capacidade, *sol;
  double *custo, val, lpobjval, ajuste_val;


  /* se for B&B puro e não usar Heurística Primal, não faz nada */
  if ( (!BRANCH_AND_CUT) && (!HEURISTICA_PRIMAL)) return 0;

  /* Recupera a  status do LP e o  número de inviabilidades inteiras
   * Procura  cortes e executa heurística primal  (quando tiver sido
   *  solicitado) apenas  se o  LP  for ótimo  e a  solução não  for
   * inteira. */
  XPRSgetintattrib(prob,XPRS_LPSTATUS,&nLPStatus);
  XPRSgetintattrib(prob,XPRS_MIPINFEAS,&nIntInf);
  if (!(nLPStatus == 1 && nIntInf>0)) return 0;
    
  /* Muda  o parâmetro  SOLUTIONFILE para pegar  a solução do  LP da
     memória. LEIA O MANUAL PARA ENTENDER este trecho */
  XPRSgetintcontrol(prob,XPRS_SOLUTIONFILE,&solfile);
  XPRSsetintcontrol(prob,XPRS_SOLUTIONFILE,0);
  /* Pega a solução do LP. */
  XPRSgetsol(prob,x,NULL,NULL,NULL);
  /* Restaura de volta o valor do SOLUTIONFILE */
  XPRSsetintcontrol(prob,XPRS_SOLUTIONFILE,solfile);

  /* verifica o número do nó em que se encontra */
  XPRSgetintattrib(prob,XPRS_NODES,&node);

  /* Imprime cabeçalho do nó */
  printf("\n=========\n");
  printf("Nó %d\n",node);
  printf("\n=========\n");
  printf("Laço de separação: %d\n",itersep);
    
  /* executa a heurística primal se for o caso */
  if (HEURISTICA_PRIMAL) ((CuttingPlanes *) classe)->HeuristicaPrimal(node);
    
  /* pega o valor otimo do LP ... */
  XPRSgetdblattrib(prob, XPRS_LPOBJVAL,&lpobjval);

  /* Imprime dados sobre o nó */
  printf(".Valor ótimo do LP: %12.6f\n",lpobjval);
  printf(".Solução ótima do LP:\n");
  ImprimeSol(x,((CuttingPlanes *) classe)->nvars());
  printf(".Rotina de separação\n");
    
  /* guarda o valor da função objetivo no primeiro nó */
  if (node==1) objval_node1=lpobjval;

  /* guarda o valor da função objetivo da primeira relaxação */
  if ((node==1) && (!itersep)) objval_relax=lpobjval;
    
  /* sai fora se for branch and bound puro */
  if (!BRANCH_AND_CUT) return 0;

  // /*  sai  fora se  a  profundidade do  nó  corrente  for maior  que
  //  * MAX_NODE_DEPTH_FOR_SEP. */
  // xpress_ret=XPRSgetintattrib(prob,XPRS_NODEDEPTH,&node_depth);
  // if (xpress_ret) 
  //    errormsg("Cortes: erro na chamada da rotina XPRSgetintattrib.\n",__LINE__,xpress_ret);
  // if (node_depth > MAX_NODE_DEPTH_FOR_SEP) return 0;

  // /* variável indicando se achou desigualdade violada ou não */
  // encontrou=0; 
    
  // /* carga dos parametros para a rotina de separacao da Cover Ineq */
    
  // /* ATENCAO:  A rotina Mochila nao  usa a posicao  zero dos vetores
  //    custo, peso  e sol,  portanto para carregar  o problema  e para
  //    pegar a solucao eh preciso acertar os indices */

  // peso=(int *)malloc(sizeof(int)*(n+1)); /* +1 !! */
  // assert(peso);
  // capacidade=0;

  // for(i=0;i<n;i++){
  //   peso[i+1]=w[i]; /* +1 !!! */
  //   capacidade=capacidade+peso[i+1]; /* +1 !! */
  // }
  // capacidade=capacidade-1-W;
    
  // /* calcula custos para o problema de separação (mochila) */
  // ajuste_val=0.0; /* ajuste para o custo do pbm da separação */
  // custo=(double *)malloc(sizeof(double)*(n+1));  /* +1 !!! */
  // assert(custo);
  // for(i=0;i<n;i++) {
  //   custo[i+1]=1.0-x[i]; 
  //   ajuste_val += custo[i+1];
  // }
    
  // /* aloca espaco para o vetor solucao da rotina Mochila */
  // sol=(int *)malloc(sizeof(int)*(n+1)); /* +1 !!! */
  // assert(sol);

  // /* resolve a mochila usando Programação Dinâmica */
  // Mochila(custo,peso,capacidade,n,sol,&val);

  // /* calculo do RHS da desigualdade de cobertura */
  // irhs=0;
  // for(i=1;i<=n;i++)
  //   if (sol[i]==0) irhs++;
    
  // /* verifica se a desigualdade estah violada */
  // if (ajuste_val - val < 1.0-EPSILON) {
  //   encontrou=1;
      
  //   /* prepara a insercao do corte */
  //   mtype[0]=1;
  //   qrtype[0]='L';
  //   drhs[0]=(double)irhs - 1.0;
  //   mstart[0]=0; mstart[1]=irhs;
  //   k=0;
  //   for(i=1;i<=n;i++)
  // 	if (!sol[i]) {
  // 	  mcols[k]=i-1;   dmatval[k]=1.0;   k++;
  // 	}
  //   assert(k==irhs);

  //   /* Impressão do corte */
  //   printf("..corte encontrado: (viol=%12.6f)\n\n   ",1.0-ajuste_val+val);
  //   for(i=0;i<irhs;i++){
  // 	printf("x[%d] ",mcols[i]);
  // 	if (i==irhs-1) printf("<= %d\n\n",irhs-1);
  // 	else printf("+ ");
  //   }

  //   /* adiciona o corte usando rotina XPRSaddcuts */
  //   xpress_ret=XPRSaddcuts(prob, 1, mtype, qrtype, drhs, mstart, mcols, dmatval);
  //   totcuts++;
  //   if (xpress_ret) 
  // 	errormsg("Cortes: erro na chamada da rotina XPRSgetintattrib.\n",__LINE__,xpress_ret);
      
  // }
  // else printf("..corte não encontrado\n");
    
  // assert(peso && sol && custo);
  // free(peso);      free(sol);         free(custo); 

  // printf("..Fim da rotina de cortes\n");

  // /* salva um arquivo MPS com o LP original */
  // xpress_ret=XPRSwriteprob(prob,"LPcuts","l");
  // if (xpress_ret) 
  //   errormsg("Cortes: rotina XPRSwriteprob.\n",__LINE__,xpress_ret);


  // if ((encontrou) && (itersep < MAX_ITER_SEP)) 
  //   { itersep++; ret=1; /* continua buscando cortes neste nó */ }
  // else
  //   { itersep=0; ret=0; /* vai parar de buscar cortes neste nó */}

  // return ret;
  return 0;
}


/**********************************************************************************\
 *  Rotina  que encontra  uma solução  heurística para  mochila binaria
 *  dada a solução e uma relaxação linear.
 *
 *  Autor: Cid Carvalho de Souza
 *  Data: 10/2003
 *
 *  Entrada: a solução fracionária corrente é $x$ e o nó corrente sendo
 *  explorado é o nó "node".
 *
 *  Idéia da heurística: ordenar itens em ordem decrescente dos valores
 *  de $x$. Em seguida ir construindo a solução heurística $xh$ fixando
 *  as variáveis em 1 ao varrer o vetor na ordem decrescente enquanto a
 *  capacidade da mochila permitir.
 *
\**********************************************************************************/

typedef struct{
  double valor;
  int indice;
} RegAux;

/* rotina auxiliar  de comparacao para o "qsort".  CUIDADO: Feito para
 * ordenar em ordem *DECRESCENTE* ! */
int ComparaRegAux(const void *a, const void *b) {
    int ret;
    if (((RegAux *)a)->valor > ((RegAux *)b)->valor) ret=-1;
    else {
	if (((RegAux *)a)->valor < ((RegAux *)b)->valor) ret=1;
	else ret=0;
    }
    return ret; 
}

void CuttingPlanes::HeuristicaPrimal(int node){

  /* Chama heurística primal do filho */

  /* calcula custo e  compara com a melhor solucao  corrente. Se for
   * melhor,  informa o  XPRESS sobre  o  novo incumbent  e salva  a
   * solucao. */
  int n = ip.getncols();
  
  std::vector<double> xsol(n), xheur;
  double zheur;
  for(int i=0;i<n;i++) xsol[i] = x[i];
  zheur = ip.heurPrimal(xsol,xheur);
  
  /* Impressão da solução heurística encontrada */
  printf("..Solução Primal encontrada:\n");
  printf("%12.6f\n",zheur);

  /* verifica se atualizará o incumbent */
  if (zheur < zstar) {

    printf("..Heurística Primal melhorou a solução\n");

    for(int i=0;i<n;i++) {
      xstar[i]=xheur[i];
    }
    
    /* atualiza numero do nó onde encontrou a melhor solucao */
    NODE_BEST_INTEGER_SOL=node;
    
    /* informa xpress sobre novo incumbent */
#warning FIXME: Soma 1.0 mesmo?
    xpress_ret=XPRSsetdblcontrol(prob,XPRS_MIPABSCUTOFF,zheur+1.0-EPSILON); 
    if (xpress_ret) 
      errormsg("Heuristica Primal: Erro \n",__LINE__,xpress_ret);
  }
  else printf("..Heurística Primal não melhorou a solução\n");
  
}

void ImprimeSol(double *a, int n){
  int i;
  for(i=0;i<n;i++)
    if (a[i] > EPSILON)
      printf("        x[%3d]=%12.6f\n",i, a[i]);
}

/**********************************************************************************\
 * Rotina copiada de programas exemplo do XPRES ... :(
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 * Name:         errormsg 
 * Purpose:      Display error information about failed subroutines.
 * Arguments:    const char *sSubName   Subroutine name             
 *               int nLineNo            Line number                 
 *               int nErrCode           Error code                  
 * Return Value: None                                               
\**********************************************************************************/
void errormsg(const char *sSubName,int nLineNo,int nErrCode)
{
   int nErrNo;             /* Optimizer error number */

   /* Print error message */
   printf("The subroutine %s has failed on line %d\n",sSubName,nLineNo);

   /* Append the error code, if it exists */
   if (nErrCode!=-1) printf("with error code %d\n",nErrCode);

   /* Append Optimizer error number, if available */
   if (nErrCode==32) {
      XPRSgetintattrib(prob,XPRS_ERRORCODE,&nErrNo);
      printf("The Optimizer error number is: %d\n",nErrNo);
   }

   /* Free memory, close files and exit */
   XPRSdestroyprob(prob);
   XPRSfree();
   exit(nErrCode);
}

