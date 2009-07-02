#include "stdio.h"
#include "myutil.h"

/* gcc tempo.c tempo-ex.c -o x */

int main()
{
  long t,x,y;
    t = time70();
    printf("Espere alguns segundos e entre com numero: ");
    scanf("%ld",&y);
    x = time70();
    printf("Voce esperou ");
    printtime(x-t);printf("\n");
    return(1);
}
