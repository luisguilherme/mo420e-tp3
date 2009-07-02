/************************/
/*         Mosel        */
/*                      */
/* File  mosel_ni.h     */
/************************/
/* (c) Copyright Fair Isaac Corporation 2008 */

#ifndef _MOSEL_NI_H
#define _MOSEL_NI_H

#include <limits.h>		/* To identify 64bit OS */
#include "mosel_rt.h"
#include "mosel_dso.h"
#include "mosel_nifct.h"

#if defined(MOSEL_NIX) || defined(XPRM_NIX)
#include "mosel_sl.h"
#include "mosel_nixfct.h"
typedef struct Mipsolver mm_mipsolver;
typedef struct Matrix mm_matrix;
#endif

#if defined(_WIN32) || defined(_WIN64)
#define DSO_INIT __declspec(dllexport) int
#elif !defined(__GNUC__) || ( __GNUC__ < 4 )
#define DSO_INIT int
#else
#define DSO_INIT __attribute__ ((visibility("default"))) int
#endif

struct Vimactx			/** Vima Context (extract) **/
	{
	 mm_alltypes *stack;
	 mm_alltypes *base_stack,*top_stack;
	 struct Model *model;
	 void *other_things;
	};

/******************************** Useful macros ********************/
#define MM_TOP_ST(ctx) ((ctx)->stack)
#define MM_FREE_ST(ctx) ((ctx)->top_stack-(ctx)->stack)
#define MM_POP_INT(ctx) (((ctx)->stack--)->integer)
#define MM_POP_REAL(ctx) (((ctx)->stack--)->real)
#define MM_POP_STRING(ctx) (((ctx)->stack--)->string)
#define MM_POP_REF(ctx) (((ctx)->stack--)->ref)
#define MM_PUSH_INT(ctx,i) (++((ctx)->stack))->integer=i
#define MM_PUSH_REAL(ctx,r) (++((ctx)->stack))->real=r
#define MM_PUSH_STRING(ctx,s) (++((ctx)->stack))->string=s
#define MM_PUSH_REF(ctx,r) (++((ctx)->stack))->ref=r

#define MM_GETMOD(ctx) ((ctx)->model)

#ifdef _WIN64
#define MM_INT2PTR(i) ((__int64)((unsigned int)(i))|(((unsigned __int64)((unsigned int)(i)))<<32))
#else
#if INT_MAX == LONG_MAX
#define MM_INT2PTR(i) (i)
#else
#define MM_INT2PTR(i) ((long)((unsigned int)(i))|(((unsigned long)((unsigned int)(i)))<<32))
#endif
#endif

#define MM_CST_INT(n,i) {n,MM_TYP_INT,{(char *)MM_INT2PTR(i)}}
#define MM_CST_BOOL(n,b) {n,MM_TYP_BOOL,{(char *)MM_INT2PTR(b)}}
#define MM_CST_REAL(n,d) {n,MM_TYP_REAL,{(char *)(&(d))}}
#define MM_CST_STRING(n,s) {n,MM_TYP_STRING,{s}}

#define RT_OK MM_RT_OK
#define RT_INSTR MM_RT_INSTR
#define RT_MATHERR MM_RT_MATHERR
#define RT_UNKN_PF MM_RT_UNKN_PF
#define RT_UNKN_SYS MM_RT_UNKN_SYS
#define RT_PROB MM_RT_PROB
#define RT_ERROR MM_RT_ERROR
#define RT_IOERR MM_RT_IOERR
#define RT_STOP MM_RT_STOP

			/* Constraint types */
#define MM_CTYPE_UNCONS	0
#define MM_CTYPE_GEQ	1
#define MM_CTYPE_LEQ	2
#define MM_CTYPE_EQ	3
#define MM_CTYPE_RNG	4
#define MM_CTYPE_SOS1	5
#define MM_CTYPE_SOS2	6
#define MM_CTYPE_CONT	7
#define MM_CTYPE_INT	8
#define MM_CTYPE_BIN	9
#define MM_CTYPE_PINT	10
#define MM_CTYPE_SEC	11
#define MM_CTYPE_SINT	12
#define MM_CTYPE_FREE	13

			/* Constraint status */
#define MM_CSTAT_EMPTY	(1<<21)
#define MM_CSTAT_HIDN	(1<<23)
#define MM_CSTAT_TEMP	(1<<25)
#define MM_CSTAT_READY	(1<<26)

#define MM_SHT_CTL	16
#define MM_MSK_CTL	((1|2|4|8)<<MM_SHT_CTL)
#define MM_CHKCSTAT(c,s) ((c)&(s))
#define MM_GETCTYPE(c)	(((c)&MM_MSK_CTL)>>MM_SHT_CTL)

typedef struct Vimactx* mm_context;
typedef struct Fdesc* mm_fdesc;

#endif
