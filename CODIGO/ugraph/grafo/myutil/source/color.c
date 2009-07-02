/* note que a sequencia de cores casa com a sequencia definida em color.h*/
#include "color.h"
char *_colorname[257]={"BLACK","WHITE","RED","GREEN","BLUE","NOCOLOR"};
char *colorname(colortype cor)
{
  return(_colorname[cor]);
}

