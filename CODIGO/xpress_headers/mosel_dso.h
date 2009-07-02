/************************/
/*         Mosel        */
/*                      */
/* File  mosel_dso.h    */
/************************/
/* (c) Copyright Fair Isaac Corporation 2008 */

struct Vimactx;
union Alltypes;

						/* Interface version */
#ifdef USE_NI_16
#define MM_NIVERS	4006
#else
#define MM_NIVERS	5003
#endif
						/* Make module version */
#define MM_MKVER(M,m,r) ((r)+(m)*1000+(M)*1000000)
							/* Services codes */
#define MM_SRV_PARAM	0		/* Encode a parameter */
				/* int findparm(char *,int *) */
#define MM_SRV_RESET	1		/* Reset a DSo for a run */
				/* void *reset(ctx, void *,int) */
#define MM_SRV_CHKVER	2		/* Check version numbers */
				/* int chkvers(int) */
#define MM_SRV_UNLOAD	3		/* Before unloading the DSO */
				/* void unload(void) */
#define MM_SRV_PARLST	4		/* Enumerate the parameter names */
		/* void *nextpar(void *,const char **,const char **,int *) */
#define MM_SRV_IMCI	5	/* Inter-Module Communication Interface */
				/* void * */
#define MM_SRV_DEPLST	6		/* Module dependency list */
				/* const char ** */
#define MM_SRV_CERTIF	7		/* Module certification */
				/* void certif(void *,void *) */
#define MM_SRV_IODRVS	8		/* table of IO drivers */
				/* mm_iodrvtab * */
#define MM_SRV_ONEXIT	9		/* End of execution of the model */
				/* void onexit(mm_context,void *,int) */
					

#define MM_FCT_GETPAR	0		/* Read a library parameter */
				/* {int,text,real} getpar(int) */
#define MM_FCT_SETPAR	1		/* Set a library parameter */
				/* void setpar(int,{int,text,real}) */

					/* Type properties */
#define MM_DTYP_PNCTX	1	/* 'tostring' does not use ctx */
#define MM_DTYP_RFCNT	2	/* create/delete supports reference count */

				/* Control codes for IO drivers */
#define MM_IOCTRL_FCTS	-2
#define MM_IOCTRL_DSO	-1
#define MM_IOCTRL_NAME	0
#define MM_IOCTRL_OPEN	1
#define MM_IOCTRL_CLOSE	2
#define MM_IOCTRL_READ	3
#define MM_IOCTRL_WRITE	4
#define MM_IOCTRL_IFROM	5
#define MM_IOCTRL_ITO	6
#define MM_IOCTRL_INFO	7
#define MM_IOCTRL_ERROR	8
#define MM_IOCTRL_RM	9
#define MM_IOCTRL_MV	10
#define MM_IOCTRL_DUP	11

			/* Type decoding for records in IO drivers */
#define MM_INIT_SHT_FLD		12
#define MM_INIT_SHT_SKIP	(MM_INIT_SHT_FLD+10)
#define MM_INIT_FLD(o)		(((o)>>MM_INIT_SHT_FLD)&1023)
#define MM_INIT_SKIP(o)		((o)>>MM_INIT_SHT_SKIP)
#define MM_INIT_MSK_SKIP	(1023u<<MM_INIT_SHT_SKIP)

typedef struct Dsoconst
	{
	 char *name;
	 int type;
	 union
	 {
	  char *s;
	  int i;
	  double *d;
	 } val;
	} mm_dsoconst;

typedef struct Dsovimfct
	{
	 char *name;
	 int code;
	 int type;
	 int nbpar;
	 char *typpar;
	 int (*vimfct)(struct Vimactx *,void *);
	} mm_dsofct;

typedef struct Dsotype
	{
	 char *name;
	 int code;
	 int props;
#ifdef USE_NI_16
	 void *(*create)(struct Vimactx *,void *);
	 void (*fdelete)(struct Vimactx *,void *,void *);
	 int (*tostring)(struct Vimactx *,void *,void *,char *,int);
	 int (*fromstring)(struct Vimactx *,void *,void *,const char *);
#else
	 void *(*create)(struct Vimactx *,void *,void *,int);
	 void (*fdelete)(struct Vimactx *,void *,void *,int);
	 int (*tostring)(struct Vimactx *,void *,void *,char *,int,int);
	 int (*fromstring)(struct Vimactx *,void *,void *,const char *,int);
	 int (*copy)(struct Vimactx *,void *,void *,void *,int);
	 void *reserved;
#endif
	} mm_dsotyp;

typedef struct
	{
	 int code;
	 void *fct;
	} mm_dsoserv;

typedef struct Dsointer
	{
	 int nbconst;
	 mm_dsoconst *tabconst;
	 int nbvmfct;
	 mm_dsofct *tabvmfct;
	 int nbtypes;
	 mm_dsotyp *tabtypes;
	 int nbserv;
	 mm_dsoserv *tabserv;
	} mm_dsointer;

typedef struct
	{
	 int code;
	 void *fct;
	} mm_iofcttab;

typedef struct
	{
	 const char *name;
	 mm_iofcttab *fcttab;
	} mm_iodrvtab;

				/* Functions for IO drivers */
typedef void *(*mm_iodrv_open)(struct Vimactx *,int *mode,const char *filename);
typedef int (*mm_iodrv_close)(struct Vimactx *,void *fd,int mode);
typedef long (*mm_iodrv_sync)(struct Vimactx *,void *fd,void *buffer,unsigned long size);
typedef int (*mm_iodrv_err)(struct Vimactx *,void *fd,char *msg,unsigned long size);
typedef int (*mm_iodrv_initfrom)(struct Vimactx *,void *fd,int nbrec,const char **labels,int *types,union Alltypes **adrs,int *nbread);
typedef int (*mm_iodrv_initto)(struct Vimactx *,void *fd,int nbrec,const char **labels,int *types,union Alltypes **adrs);
typedef int (*mm_iodrv_rm)(struct Vimactx *,const char *);
typedef int (*mm_iodrv_mv)(struct Vimactx *,const char *,const char *);


typedef struct Iodrvlst* mm_iodrv;
typedef struct Mosel_ni* mm_nifct;
typedef struct Mosel_nix* mm_nixfct;

