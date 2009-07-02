/*
        Projetista: Flavio Keidi Miyazawa
	
	Arquivo tempo.h
	
*/


#ifndef TEMPODEFINE
#define TEMPODEFINE
#include <sys/types.h>
#include <time.h>
long time70(void);  /* retorna o tempo em segundos desde 1/jan/1970*/
void printtime(long t); /* imprime o tempo em dias, horas, minutos, seg.*/

void sprinttime(char *s,long t); /* imprime o tempo na string s */
void shortprinttime(char *s,long t); /* imprime o tempo na string s */
#endif
