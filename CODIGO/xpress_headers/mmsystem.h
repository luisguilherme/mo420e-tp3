/************************/
/*         Mosel        */
/*                      */
/* File  mmsystem.h     */
/************************/
/* (c) Copyright Fair Isaac Corporation 2008 */

#ifndef _MMSYSTEM_H
#define _MMSYSTEM_H

struct Sysctx;

typedef struct Mmsystem_imci
	{
	 int (*setenv)(struct Vimactx *ctx,struct Sysctx *sysctx,const char *name,const char *value);
	 const char *(*getenv)(struct Vimactx *ctx,struct Sysctx *sysctx,const char *name);
	 int (*system)(struct Vimactx *ctx,struct Sysctx *sysctx,const char *cmd);
	 int (*gettxtsize)(struct Vimactx *ctx,struct Sysctx *sysctx,void *txt);
	 char *(*gettxtbuf)(struct Vimactx *ctx,struct Sysctx *sysctx,void *txt);
	 char *(*txtresize)(struct Vimactx *ctx,struct Sysctx *sysctx,void *txt,int size);
	} *mmsystem_imci;
#endif
