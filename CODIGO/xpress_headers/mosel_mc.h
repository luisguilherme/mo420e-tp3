/************************/
/*         Mosel        */
/*                      */
/* File  mosel_mc.h     */
/************************/
/* (c) Copyright Fair Isaac Corporation 2008 */

#ifndef _MOSEL_MC_H
#define _MOSEL_MC_H
#include "mosel_rt.h"

#ifdef __cplusplus
extern "C" {
#endif

int MM_RTC XPRMcompmod(const char *options,const char *srcfile,const char *destfile,const char *userc);
int MM_RTC XPRMexecmod(const char *options,const char *srcfile,const char *parlist, int *returned, mm_model *rtmod);

#ifdef __cplusplus
}
#endif
#endif
