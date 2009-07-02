#ifndef LOGFILE_DEFINE
#define LOGFILE_DEFINE
#include <stdio.h>
#include <stdarg.h>
#include "boolean.h"
typedef struct {
  FILE *fp; /* arquivo de log */
  int lognumberlines;
  long tempo_inicial;
} logfile_type;


boolean logfile_init(logfile_type *lf,char *filename);
void logfile_print(logfile_type *lf,char *format,...);
void logfile_flush(logfile_type *lf);
void logfile_close(logfile_type *lf);
#endif
