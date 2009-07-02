#ifndef COLORDEFINE
#define COLORDEFINE
/*       Arquivo: color.h

	 Projetista: Flavio Keidi Miyazawa
*/
typedef enum          { BLACK , WHITE , RED , GREEN , BLUE , NOCOLOR } colortype;
char *colorname(colortype cor);
#endif
