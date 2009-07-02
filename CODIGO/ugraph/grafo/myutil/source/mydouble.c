#include "mydouble.h"
#include <math.h>
boolean isdoublevalue(double x,double y)
{
  if ((x>y-MY_EPS) && (x<y+MY_EPS)) return(true);
  return(false);
}

double double_max(double x,double y)
{
  if (x>y) return(x);
  return(y);
}

double double_min(double x,double y)
{
  if (x<y) return(x);
  return(y);
}

boolean double_isfrac(double x)
{
  double f;
  f = ceil(x)-x;
  if (f<MY_EPS) return(false);
  if (f>1.0-MY_EPS) return(false);
  return(true);
}
boolean double_isinteger(double x)
{
  return(!double_isfrac(x));
}

double double_diff_to_ceil(double x)
{
  if (double_isfrac(x)) return(ceil(x)-x);
  return(0.0);
}


boolean double_isequal(double x,double y)
{
  if (x==y) return(true);
  if ((x>y-MY_EPS) && (x<y+MY_EPS)) return(true);
  return(false);
}

boolean double_isnull(double x)
{
  if ((x<MY_EPS) && (x>-MY_EPS)) return(true);
  return(false);
}

boolean double_isnonnull(double x)
{
  if (x>MY_EPS) return(true);
  if (x<-MY_EPS) return(true);
  return(false);
}


boolean double_islessequal(double x,double y)
{
  if (x < y+MY_EPS) return(true);
  return(false);
}
boolean double_isgreaterequal(double x,double y)
{
  if (x+MY_EPS > y) return(true);
  return(false);
}
