/************************/
/*         Mosel        */
/*                      */
/* File  mmxprs.h       */
/************************/
/* (c) Copyright Fair Isaac Corporation 2008 */

#ifndef _MMXPRS_H
#define _MMXPRS_H

struct Xprsctx;

typedef struct Mmxprs_imci
	{
	 void (*loadcbks)(struct Vimactx *,struct Xprsctx *);
	 int (*syncstat)(struct Vimactx *,struct Xprsctx *);
	} *mmxprs_imci;
#endif
