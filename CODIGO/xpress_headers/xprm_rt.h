/************************/
/*         Mosel        */
/*                      */
/* File  xprm_rt.h      */
/************************/
/* (c) Copyright Fair Isaac Corporation 2008 */

#ifndef _MOSEL_RT_H
#include "mosel_rt.h"

#define XPRM_RTC MM_RTC
#define XPRM_VERSION MM_VERSION
#define XPRM_TYP_NOT MM_TYP_NOT
#define XPRM_TYP_INT MM_TYP_INT
#define XPRM_TYP_REAL MM_TYP_REAL
#define XPRM_TYP_STRING MM_TYP_STRING
#define XPRM_TYP_BOOL MM_TYP_BOOL
#define XPRM_TYP_MPVAR MM_TYP_MPVAR
#define XPRM_TYP_LINCTR MM_TYP_LINCTR
#define XPRM_TYP_EXTN MM_TYP_EXTN
#define XPRM_STR_CONST MM_STR_CONST
#define XPRM_STR_REF MM_STR_REF
#define XPRM_STR_ARR MM_STR_ARR
#define XPRM_STR_SET MM_STR_SET
#define XPRM_STR_LIST MM_STR_LIST
#define XPRM_STR_PROC MM_STR_PROC
#define XPRM_STR_MEM MM_STR_MEM
#define XPRM_STR_UTYP MM_STR_UTYP
#define XPRM_STR_NTYP MM_STR_NTYP
#define XPRM_STR_REC MM_STR_REC
#define XPRM_GRP_FIX MM_GRP_FIX
#define XPRM_GRP_DYN MM_GRP_DYN
#define XPRM_GRP_RNG MM_GRP_RNG
#define XPRM_GRP_GEN MM_GRP_GEN
#define XPRM_ARR_DENSE MM_ARR_DENSE
#define XPRM_ARR_SPARSE MM_ARR_SPARSE
#define XPRM_CPAR_READ MM_CPAR_READ
#define XPRM_CPAR_WRITE MM_CPAR_WRITE
#define XPRM_MTP_CREAT MM_MTP_CREAT
#define XPRM_MTP_DELET MM_MTP_DELET
#define XPRM_MTP_TOSTR MM_MTP_TOSTR
#define XPRM_MTP_FRSTR MM_MTP_FRSTR
#define XPRM_MTP_PRTBL MM_MTP_PRTBL
#define XPRM_MTP_RFCNT MM_MTP_RFCNT
#define XPRM_MTP_COPY MM_MTP_COPY
#define XPRM_MSK_TYP MM_MSK_TYP
#define XPRM_MSK_STR MM_MSK_STR
#define XPRM_MSK_GRP MM_MSK_GRP
#define XPRM_MSK_FIN MM_MSK_FIN
#define XPRM_TYP MM_TYP
#define XPRM_STR MM_STR
#define XPRM_GRP MM_GRP
#define XPRM_IS_PUBLIC MM_IS_PUBLIC
#define XPRM_PBSOL MM_PBSOL
#define XPRM_PBOPT MM_PBOPT
#define XPRM_PBUNF MM_PBUNF
#define XPRM_PBINF MM_PBINF
#define XPRM_PBUNB MM_PBUNB
#define XPRM_PBOTH MM_PBOTH
#define XPRM_PBRES MM_PBRES
#define XPRM_PBCHG MM_PBCHG
#define XPRM_PBOBJ MM_PBOBJ
#define XPRM_RT_OK MM_RT_OK
#define XPRM_RT_INSTR MM_RT_INSTR
#define XPRM_RT_MATHERR MM_RT_MATHERR
#define XPRM_RT_UNKN_PF MM_RT_UNKN_PF
#define XPRM_RT_UNKN_SYS MM_RT_UNKN_SYS
#define XPRM_RT_PROB MM_RT_PROB
#define XPRM_RT_ERROR MM_RT_ERROR
#define XPRM_RT_EXIT MM_RT_EXIT
#define XPRM_RT_IOERR MM_RT_IOERR
#define XPRM_RT_BREAK MM_RT_BREAK
#define XPRM_RT_NIFCT MM_RT_NIFCT
#define XPRM_RT_NULL MM_RT_NULL
#define XPRM_RT_STOP MM_RT_STOP
#define XPRM_DBG_STOP MM_DBG_STOP
#define XPRM_DBG_NEXT MM_DBG_NEXT
#define XPRM_DBG_STEP MM_DBG_STEP
#define XPRM_DBG_CONT MM_DBG_CONT
#define XPRM_TRUE MM_TRUE
#define XPRM_FALSE MM_FALSE
#define XPRM_F_TEXT MM_F_TEXT
#define XPRM_F_BINARY MM_F_BINARY
#define XPRM_F_READ MM_F_READ
#define XPRM_F_INPUT MM_F_INPUT
#define XPRM_F_WRITE MM_F_WRITE
#define XPRM_F_OUTPUT MM_F_OUTPUT
#define XPRM_F_APPEND MM_F_APPEND
#define XPRM_F_ERROR MM_F_ERROR
#define XPRM_F_LINBUF MM_F_LINBUF
#define XPRM_F_INIT MM_F_INIT
#define XPRM_F_SILENT MM_F_SILENT
#define XPRM_F_ONCE MM_F_ONCE
#define XPRM_PROP_NAME MM_PROP_NAME
#define XPRM_PROP_ID MM_PROP_ID
#define XPRM_PROP_VERSION MM_PROP_VERSION
#define XPRM_PROP_SYSCOM MM_PROP_SYSCOM
#define XPRM_PROP_USRCOM MM_PROP_USRCOM
#define XPRM_PROP_SIZE MM_PROP_SIZE
#define XPRM_PROP_NBREF MM_PROP_NBREF
#define XPRM_PROP_DATE MM_PROP_DATE
#define XPRM_TPROP_NAME MM_TPROP_NAME
#define XPRM_TPROP_FEAT MM_TPROP_FEAT
#define XPRM_TPROP_EXP MM_TPROP_EXP
#define XPRM_KEEPOBJ MM_KEEPOBJ

typedef struct Model* XPRMmodel;
typedef struct Dsolist* XPRMdsolib;
typedef int XPRMinteger;
typedef double XPRMreal;
typedef int XPRMboolean;
typedef const char* XPRMstring;
typedef struct Lpvar* XPRMmpvar;
typedef struct Lpctr* XPRMlinctr;
typedef struct Array* XPRMarray;
typedef union SetR* XPRMset;
typedef struct List* XPRMlist;
typedef void* XPRMref;
typedef struct Htentry* XPRMproc;
typedef struct Memblk XPRMmemblk;
typedef union Alltypes XPRMalltypes;

#define XPRM_TIME_UTC MM_TIME_UTC
#define XPRM_TIME_LOCAL MM_TIME_LOCAL

#define XPRMfree XPRMfinish           /* for, say, "legacy" code !! */
#endif
