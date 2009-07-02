#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#define MAXMEMORYPOS 100000
#define MAXMEMORYNAME 100
#define FASTMEMORYDEF


#ifndef FASTMEMORYDEF
struct {
  void *end;
  enum {FREE,OCCUPIED} status;
  char name[MAXMEMORYNAME];
  int size;
} vetormemoria[MAXMEMORYPOS];
int pos=0;
#endif

void myinitmemory(void)
{
  #ifndef FASTMEMORYDEF
  int i;
  for (i=0;i<MAXMEMORYPOS;i++) {
    vetormemoria[i].end = NULL;
    vetormemoria[i].status = FREE;
  }
  #endif
}

void myclosememory(void)
{
  #ifndef FASTMEMORYDEF
  int i;
  for (i=0;i<MAXMEMORYPOS;i++) {
    if (vetormemoria[i].status == OCCUPIED)
      printf("Existencia de memoria nao desalocada. Size = %d, Name = %s\n",vetormemoria[i].size,vetormemoria[i].name);
  }
  #endif
}


void *mymallocmemory(int size,char *name)
{
  #ifdef FASTMEMORYDEF
  return(malloc(size));
  #endif
  #ifndef FASTMEMORYDEF
  int i;
  void *p;
  p = malloc(size);
  if (p==NULL) return(NULL);
  for (i=0;i<pos;i++) {
    if (vetormemoria[i].end==p) break;
  }
  if (i<pos) {
    if (vetormemoria[i].status==FREE) {
        vetormemoria[i].status = OCCUPIED;
        vetormemoria[i].size = size;
	strncpy(vetormemoria[i].name,name,MAXMEMORYNAME);
	return(p);
    } else {
      printf("Mesma posicao de memoria alocada duas vezes. Erro no malloc\n");
      return(NULL);
    }
  }
  if (pos<MAXMEMORYPOS) {
    vetormemoria[pos].end = p;
    vetormemoria[pos].status = OCCUPIED;
    vetormemoria[i].size = size;
    strncpy(vetormemoria[pos].name,name,MAXMEMORYNAME);
    pos++;
    return(p);
  }else printf("Falta de memoria em memory.c\n");
  return(NULL);
  #endif
}


void myfreememory(void *p)
{
  #ifdef FASTMEMORYDEF
  free(p);
  return;
  #endif
  #ifndef FASTMEMORYDEF
  int i;
  if (p==NULL) return;
  for (i=0;i<pos;i++) {
    if (vetormemoria[i].end==p) break;
  }
  if (i<pos) {
    if (vetormemoria[i].status==OCCUPIED) {
      free(p);
      vetormemoria[i].status=FREE;
      return;
    }
    printf("Memoria sendo desalocada duas vezes\n");
    gr_pause();
    return;
  } 
  printf("Nao localizou endereco alocado\n");
  gr_pause();
  #endif
}

void myfreenotnull(void *p)
{
  if (p) myfreememory(p);
}

    
/* realoca a memoria apontada por *pp, podendo aumentar o tamanho do vetor */
void myresizevector(void **pp, 
		    int item_size, /* tam. em bytes de cada elem. do vetor*/
		    int original_number_elements,
		    int new_number_elements)
{
  void *p;
  /* esta rotina so' aumenta a memoria, nunca diminui */
  if (new_number_elements <= original_number_elements) return;
  p = *pp;
  *pp = malloc(item_size * new_number_elements);
  if (*pp==NULL) {
    printf("Nao foi possivel aumentar a memoria do vetor\n"
	   "de %d itens para %d itens (cada item de %d bytes)\n",
	   original_number_elements,new_number_elements,
	   item_size);
    exit(1);
  }
  memcpy(*pp,p,item_size*original_number_elements);
  free(p);
}

 
