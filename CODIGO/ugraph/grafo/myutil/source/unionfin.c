/*

   Bibliotecas de funcoes C de Flavio Keidi Miyazawa

   unionfin.c

   Ultima atualizacao 23/04/95

*/


#include "unionfin.h"

/*    Rotina Union Find

      Contem funcoes para executar unioes e pesquisas de nomes sobre conjuntos,
      eficientemente.

*/



/*
      Inicializacao de uma variavel do tipo Union find.
      Aloca espaco para armazenar os elementso a pesquisar.
      Aloca espaco para armazenar os nomes dos conjuntos de cada elemento.
*/
boolean init_uf(unionfindtype *uf, int n)
{
  int i;
  /* alocacao de memoria necessaria */
  uf->size = n;
  uf->father = (int *) malloc(sizeof(int) * n);
  if (!uf->father) return (false);
  
  uf->cardinality = (int *) malloc(sizeof(int) * n);
  if (!uf->cardinality) {
    free(uf->father);
    return(false);
  }

  /* atribuicao inicial para que o nome dos conjuntos onde esta' cada */
  /* elemento seja unico para cada elemento */
  
  for (i=0;i<n;i++) {
    uf->father[i] = i;
    uf->cardinality[i]= 1;
  }
  return(true);
}


/* reinicializa o campo onde esta os valores dos nomes dos conjuntos onde */
/* esta' cada elemento de forma que o nome de seu conjunto seja unico. */
boolean reinit_uf(unionfindtype *uf)
{
  int i;
  if ((!uf->father) || (!uf->cardinality)) return (false);
  for (i=0;i<uf->size;i++) {
    uf->father[i] = i;
    uf->cardinality[i] = 1;
  }
  return(true);
}


/* faz uma busca de um elemento na variavel union find */
/* aproveita o caminho percorrido para aproximar os elementos que estao no */
/* mesmo caminho com o nome do conjunto */
int find_uf(unionfindtype *uf,int element)
{
  int block,fatherelement;

  if ((element>=0) &&(element < uf->size)) {
    block = element;
    
    /* primeiro encontre o nome do elemento. */
    while (uf->father[block] != block) block = uf->father[block];
    

    /* percorra o caminho ate' o nome do conjunto e va atualizando */
    /* o nome do conjunto de todos os elementos do caminho percorrido. */
    while (uf->father[element] != element) {
      fatherelement = uf->father[element];
      uf->father[element] = block;
      element = fatherelement;
    }
    return(block);
  }else {
    printf("Uso indevido da funcao \"find_uf\"\n");
    return(0);
  }
}


/* Faz a uniao de dois conjuntos.  */
/* pega o nome do primeiro conjunto e o nome do segunto , e "pendura" */
/* o menor conjunto no maior conjunto */
int union_uf(unionfindtype *uf,int A, int B)
{
  int BlocoMenor, BlocoMaior, BlocoA, BlocoB;
  if ((A>=0) && (A<uf->size) && (B>=0) && (B<uf->size)) {
    BlocoA = find_uf(uf,A);
    BlocoB = find_uf(uf,B);
    if (uf->cardinality[BlocoA] > uf->cardinality[BlocoB]) {
      BlocoMenor = BlocoB;
      BlocoMaior = BlocoA;
    } else {
      BlocoMenor = BlocoA;
      BlocoMaior = BlocoB;
    }
    if (BlocoMenor!=BlocoMaior) {
      uf->father[BlocoMenor] = BlocoMaior;
      uf->cardinality[BlocoMaior] += uf->cardinality[BlocoMenor];
    }
    return(BlocoMaior);
  } else {
    printf("uso indevido da funcao \"union_uf\"\n");
    return(0);
  }
}

/* Fecha uma variavel do tipo unionfind e coloca a disposicao do sistema a 
   memoria usada para armazenar o nome dos conjuntos e a cardinalidade de
   cada conjunto. */
void close_uf(unionfindtype *uf)
{
  if ((uf->father) && (uf->cardinality)) {
    free(uf->father);
    free(uf->cardinality);
  }else{
    printf("A funcao \"close_uf\" detetou erro de inicializacao "
	   "de seu parametro.\n");
  }
}


/* Funcao para imprimir uma variavel de unionfind */
void print_uf(unionfindtype *uf)
{
  int i;
  printf("Elem. ");
  for (i=0;i<uf->size;i++) printf(" %2d",i);
  printf("\nConj. ");
  for (i=0;i<uf->size;i++) printf(" %2d",find_uf(uf,i));
  printf("\n\n");
}
