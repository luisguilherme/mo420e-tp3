/*
        Projetista: Flavio Keidi Miyazawa
	
	Arquivo boolean.h
	
	Definicao de tipos booleanos
	Data da ultima atualizacao: 01/11/91


        declara um novo tipo booleano . */

#ifndef BOOLEANDEFINE
#define BOOLEANDEFINE
#define false        0
#define true         1
#define bol_ambiguo  2
#define bol_error    2
typedef char boolean;  /* este tipo so' tem true ou false */
typedef char triboolean;  /* este tipo pode ter true, false, bol_ambiguo, bol_error */
#endif
