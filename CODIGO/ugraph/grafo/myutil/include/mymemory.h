#ifndef MEMORYDEFINE
#define MEMORYDEFINE
void *myinitmemory(void);
void *myclosememory(void);
void *mymallocmemory(int size,char *);
void myfreememory(void *p);
void myfreenotnull(void *p);
/* realoca a memoria de um vetor, aumentando o numero de itens que pode aumentar  */
void myresizevector(void **pp,
		    int item_size, /* tam. em bytes de cada elem. do vetor*/
		    int original_number_elements,
		    int new_number_elements);
#endif

