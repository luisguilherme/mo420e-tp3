#include "logfile.h"

main()
{
  logfile_type lf;
  logfile_init(&lf,"logfile.log");
  logfile_print(&lf,"Testando %d",123);
  printf("Espere um tempo e entre com um numero: ");
  getchar();
  logfile_print(&lf,"Testando %d",456);
  logfile_print(&lf,"Testando %d",457);
  logfile_print(&lf,"Testando %d",458);
  logfile_print(&lf,"Testando %d",459);
  logfile_close(&lf);
}

  
