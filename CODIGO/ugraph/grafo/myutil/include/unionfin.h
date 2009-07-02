/*

   Bibliotecas de funcoes C de Flavio Keidi Miyazawa

   unionfin.h

   Ultima atualizacao 23/04/95

*/
 
#ifndef UNIONFINDEFINE
#define UNIONFINDEFINE
#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"


typedef struct {
  int size, *father, *cardinality;
} unionfindtype;

/* prototipos */

/* Inicializa um variavel dotipo unionfind. usa de alocacao dinamica para  */
/* alocar espaco para os vetores de pais e cardinalidade.                  */

boolean init_uf(unionfindtype *uf,int n);

/* apenas reinicializa uma variavel de union find, usando a estrutura ja'  */
/* inicializada. Portando nao usa alocacao dinamica pois ela ja' foi feita */
/* pelo init_uf */

boolean reinit_uf(unionfindtype *uf);

/* devolve o nome do conjunto a que o (elemento) pertence */
int find_uf(unionfindtype *uf, int element);

/* une dois conjuntos e devolve o nome do conjunto resultante. */
int union_uf(unionfindtype *uf, int A, int B);

/* fecha uma variavel do tipo unionfind e coloca a disposicao do sittema a  */
/* memoria alocada para definir o tipo */
void close_uf (unionfindtype *uf);

/* imprime uma variavel de union find */

void print_uf (unionfindtype *uf);

#endif
