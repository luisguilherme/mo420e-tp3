#include "mydate.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

char *NomeDeMes[13]={"","janeiro","fevereiro","marco","abril","maio","junho",
		     "julho","agosto","setembro","outubro","novembro","dezembro"};
char *NomeDeMesShort[13]={"","jan","fev","mar","abr","mai","jun",
		     "jul","ago","set","out","nov","dez"};

void datahoje(int *ano,int *mes,int *dia)
{
  time_t t;
  struct tm *loctime;
  t = time(NULL);
  loctime = localtime(&t);
  *dia = loctime->tm_mday;
  *mes = loctime->tm_mon+1;
  *ano = loctime->tm_year;
  *ano += 1900;
}
void horahoje(int *hora,int *minuto,int *segundo)
{
  time_t t;
  struct tm *loctime;
  t = time(NULL);
  loctime = localtime(&t);
  *segundo = loctime->tm_sec;
  *minuto = loctime->tm_min;
  *hora = loctime->tm_hour;
}
void peganomemes(char *nomemes,int mes)
{
  if ((mes<1) || (mes>12)) mes = 0;
  strcpy(nomemes,NomeDeMes[mes]);
}
void peganomemesshort(char *nomemes,int mes)
{
  if ((mes<1) || (mes>12)) mes = 0;
  strcpy(nomemes,NomeDeMesShort[mes]);
}

