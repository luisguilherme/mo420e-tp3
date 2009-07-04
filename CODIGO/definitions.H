#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define MAX_CPU_TIME 1800
#define MAX_NUM_CORTES 100   /* limite maximo de cortes */
#define MAX_ITER_SEP 10   /* numero maximo de iteracoes de corte por nodo */

#define EPSILON 1e-6
#define INF 1e14

#define pb push_back
#define sz(a) (int) (a).size()
#define NONZERO(a) ((a) > EPSILON || (a) < -EPSILON)
#define present(c,x) ((c).find(x) != (c).end()) 
#define traverse(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++) 

#include <algorithm>

#endif
