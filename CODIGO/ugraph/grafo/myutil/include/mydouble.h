/*
        Projetista: Flavio Keidi Miyazawa
	
	Arquivo myutil.h
	
	Definicao de tipos booleanos
	Data da ultima atualizacao: 01/11/91


        declara um novo tipo booleano . */

#ifndef MY_DOUBLE_DEFINE
#define MY_DOUBLE_DEFINE
#define MY_EPS 0.01
#define MY_INFINITY 10E30
#include "boolean.h"
boolean isdoublevalue(double x,double y);
double  double_max(double x,double y);
double  double_min(double x,double y);
boolean double_isfrac(double x);
boolean double_isinteger(double x);
double  double_diff_to_ceil(double x);
boolean double_isequal(double x,double y);
boolean double_islessequal(double x,double y);
boolean double_isgreaterequal(double x,double y);
boolean double_isnonnull(double x);
boolean double_isnull(double x);
#endif
