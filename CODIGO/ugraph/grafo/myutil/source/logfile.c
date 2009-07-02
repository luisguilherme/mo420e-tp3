#include "logfile.h"
#include "boolean.h"
#include "tempo.h"
#include "mydate.h"
#include <string.h>
boolean logfile_init(logfile_type *lf,char *filename)
{
  if (lf==NULL) {
    printf("Null pointer to logfile parameter.\n");
    return(false);
  }
  lf->fp = fopen(filename,"a");
  if (lf->fp==NULL){
    printf("Unable to open %s file in logfile_init\n",filename);
    return(false);
  }
  lf->lognumberlines = 0;
  lf->tempo_inicial = time70();
  return(true);
}

void logfile_close(logfile_type *lf)
{
  if (lf==NULL) {
    printf("Unable to close logfile\n");
    return;
  }
  fclose(lf->fp);
}

void logfile_print(logfile_type *lf,char *format,...)
{
  va_list argument;
  char data[100],horario[100],nomemes[20];
  int i,n,dia,mes,ano,hora,minuto,segundo;
  datahoje(&ano,&mes,&dia);
  horahoje(&hora,&minuto,&segundo);
  peganomemesshort(nomemes,mes);
  lf->lognumberlines++;
  sprintf(data,"%2d.%s.%2d",dia,nomemes,ano%100);
  sprintf(horario,"%2d:%2d:%2d",hora,minuto,segundo);
  n = strlen(data);
  for (i=0;i<n;i++) if (data[i]==' ') data[i]='0';
  n = strlen(horario);
  for (i=0;i<n;i++) if (horario[i]==' ') horario[i]='0';
  //shortprinttime(tempo,time70()-lf->tempo_inicial);
  va_start(argument,format);
  //fprintf(lf->fp,"%s %d",linha,tempo);
  fprintf(lf->fp,"%s_%s>  ",data,horario);
  vfprintf(lf->fp,format,argument);
  fprintf(lf->fp,"\n");
  va_end(argument);
  fflush(lf->fp);
}

void logfile_flush(logfile_type *lf)
{
  fflush(lf->fp);
}

