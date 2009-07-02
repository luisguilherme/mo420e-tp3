/*************************************/
/* BCL :  Xpress Builder Component   */
/* ===    Library                    */
/*                                   */
/* File xprb.h                       */
/* ```````````                       */
/* -- C header file --               */
/*                                   */
/* (c)2008 Fair Isaac Corporation    */
/*    Creation: 1999, rev. June 2008 */
/*************************************/

#ifndef _XPRB
#define _XPRB
#if defined(_WIN32) || defined(_WIN64)
#define XB_CC __stdcall
#ifdef _FILE_DEFINED
#define _STDIO_H
#endif
#else
#define XB_CC
#if defined(_STDIO_INCLUDED) || defined (_H_STDIO)
#define _STDIO_H
#endif
#endif

#define XB_INFINITY 1.0e+20

/* BCL version */
#define XB_VERSION "4.0.3"

/* Error types */
#define XB_ERR 1
#define XB_WAR 0

#define XB_FGETS (char*(*)(char*, int, void*))fgets

/* Variable types */
#define XB_PL  0
#define XB_BV  1
#define XB_UI  2
#define XB_PI  3
#define XB_SC  4
#define XB_SI  5

/* Constraint types */
#define XB_N   0
#define XB_L   1
#define XB_G   2
#define XB_E   3
#define XB_R   4

/* Sense of the objective function */
#define XB_MAXIM 1
#define XB_MINIM 0

/* SOS types */
#define XB_S1    0
#define XB_S2    1

/* Directive types */
#define XB_PR    1
#define XB_UP    2
#define XB_DN    3
#define XB_PU    4
#define XB_PD    5

/* Range types */
#define XB_UPACT 0
#define XB_LOACT 1
#define XB_UUP   2
#define XB_UDN   3
#define XB_UCOST 4
#define XB_LCOST 5

/* Dictionary types */
#define XB_VAR  1
#define XB_ARR  2
#define XB_CTR  3
#define XB_SOS  4
#define XB_IDX  5

/* Dictionaries */
#define XB_DICT_NAMES 0
#define XB_DICT_IDX   1

/* File formats */
#define XB_LP   1
#define XB_MPS  2

/* Synchronization */
#define XB_XO_SOL  1
#define XB_XO_PROB 2

/* BCL problem status */
#define XB_GEN  1       /* Matrix has been generated */
#define XB_DIR  2       /* Directive added */
#define XB_MOD  4       /* Row/Col modification */
#define XB_SOL  8       /* Solution available */

/* LP status */
#define XB_LP_OPTIMAL        1
#define XB_LP_INFEAS         2
#define XB_LP_CUTOFF         3
#define XB_LP_UNFINISHED     4
#define XB_LP_UNBOUNDED      5
#define XB_LP_CUTOFF_IN_DUAL 6
#define XB_LP_UNSOLVED       7

/* MIP status */
#define XB_MIP_NOT_LOADED     0
#define XB_MIP_LP_NOT_OPTIMAL 1
#define XB_MIP_LP_OPTIMAL     2
#define XB_MIP_NO_SOL_FOUND   3
#define XB_MIP_SOLUTION       4
#define XB_MIP_INFEAS         5
#define XB_MIP_OPTIMAL        6

/* Objects of the modeling library */
typedef struct Xbprob xbprob;
typedef struct Xbvar xbvar;
typedef struct Xbvar *xbarrvar;
typedef struct Xbctr xbctr;
typedef struct Xbsos xbsos;
typedef struct Xbcut xbcut;
typedef struct Xbexpr xbexpr;
typedef struct Xbidxset xbidxset;
typedef struct Xbbasis xbbasis;

#ifdef __cplusplus
extern "C" {
#endif

/**************************** Function prototypes ****************************/
/**** Initialization/termination ****/
int XB_CC xbslinit(void);
int XB_CC xbslfinish(void);
const char * XB_CC xbgetversion(void);
int XB_CC xbgettime(void);

/**** Error handling ****/
int XB_CC xbseterrctrl(int flag);
int XB_CC xbdefcberr_prob(struct Xbprob *prob,void (XB_CC *usererror)(struct Xbprob *,void *,int, int, const char *),void *vp);


/**** Problem definition ****/
struct Xbprob * XB_CC xbnewprob(const char *name);
int XB_CC xbchgprob(struct Xbprob *prob);
struct xo_prob_struct * XB_CC xbgetsprob_prob(struct Xbprob *prob);
struct Xbprob * XB_CC xbgetprob(void);

/**** Names ****/
const char * XB_CC xbnewname(const char *format,...);

/**** Output and printing ****/
int XB_CC xbdefcbmsg_prob(struct Xbprob *prob,void (XB_CC *userprint)(struct Xbprob *,void *,const char *),void *vp);
int XB_CC xbsetmsglevel_prob(struct Xbprob *prob, int level);  /* default: 2 */
int XB_CC xbsetrealfmt_prob(struct Xbprob *prob,const char *realfmt); /* default: "%g" */
int XB_CC xbsetdecsign(char sign);	/* default: '.' */
int XB_CC xbprintf(const char *format, ...);
int XB_CC xbprintf_prob(struct Xbprob *prob,const char *format, ...);

/**** Files ****/
int XB_CC xbreadlinecb(char *(*fgs)(char *,int,void *),void *f, int maxsize, const char *format,...);
int XB_CC xbreadarrlinecb(char *(*fgs)(char *,int,void *),void *f, int maxlen, const char *format, void *t, int size);
#ifdef _STDIO_H
int XB_CC xbreadline(FILE *f, int maxsize, const char *format,...);

int XB_CC xbreadarrline(FILE *f, int maxlen, const char *format, void *t, int size);
#endif

/**** Problem definition ****/
int XB_CC xbdelprob_prob(struct Xbprob *prob);
int XB_CC xbresetprob_prob(struct Xbprob *prob);

/**** Variable definition ****/
struct Xbvar * XB_CC xbnewvar_prob(struct Xbprob *prob, int type, const char *name, double lob, double upb);
int XB_CC xbsetvartype(struct Xbvar *var, int type);

/**** Arrays of variables ****/
struct Xbvar ** XB_CC xbnewarrvar_prob(struct Xbprob *prob, int nbvar, int type, const char *name, double lob, double upb);
struct Xbvar ** XB_CC xbstartarrvar_prob(struct Xbprob *prob, int nbvar, const char *name);
int XB_CC xbsetarrvarel(struct Xbvar **av, int n, struct Xbvar *var);
int XB_CC xbapparrvarel(struct Xbvar **av, struct Xbvar *var);
int XB_CC xbendarrvar(struct Xbvar **av);
int XB_CC xbdelarrvar(struct Xbvar **av);

/**** Constraint definition ****/
struct Xbctr * XB_CC xbnewctr_prob(struct Xbprob *prob, const char *name, int qrtype);   
int XB_CC xbdelctr(struct Xbctr *lct);
int XB_CC xbaddterm(struct Xbctr *lct, struct Xbvar *var, double coeff);
int XB_CC xbdelterm(struct Xbctr *lct, struct Xbvar *var);
int XB_CC xbsetterm(struct Xbctr *lct, struct Xbvar *var, double coeff);
int XB_CC xbaddqterm(struct Xbctr *lct, struct Xbvar *var1, struct Xbvar *var2, double coeff);
int XB_CC xbdelqterm(struct Xbctr *lct, struct Xbvar *var1, struct Xbvar *var2);
int XB_CC xbsetqterm(struct Xbctr *lct, struct Xbvar *var1, struct Xbvar *var2,double coeff);
int XB_CC xbaddarrterm(struct Xbctr *lct, struct Xbvar **av, double *cof);
struct Xbctr * XB_CC xbnewsum_prob(struct Xbprob *prob,const char *name, struct Xbvar **av, int qrtype, double rhs);
struct Xbctr * XB_CC xbnewsumc_prob(struct Xbprob *prob,const char *name, struct Xbvar **av, double c, int qrtype, double rhs);
struct Xbctr * XB_CC xbnewarrsum_prob(struct Xbprob *prob,const char *name, struct Xbvar **av, double *cof, int qrtype, double rhs);
struct Xbctr * XB_CC xbnewprec_prob(struct Xbprob *prob,const char *name, struct Xbvar *v1, double dur,struct Xbvar *v2);
int XB_CC xbsetctrtype(struct Xbctr *lct, int qrtype);
int XB_CC xbsetrange(struct Xbctr *lct, double low, double up);
int XB_CC xbsetmodcut(struct Xbctr *lct, int mcstat);

/**** SOS definition ****/
struct Xbsos * XB_CC xbnewsos_prob(struct Xbprob *prob, const char *name, int type);
int XB_CC xbdelsos(struct Xbsos *sos);
int XB_CC xbaddsosel(struct Xbsos *sos, struct Xbvar *var, double ref);
int XB_CC xbdelsosel(struct Xbsos *sos, struct Xbvar *var);
int XB_CC xbaddsosarrel(struct Xbsos *sos, struct Xbvar **av, double *cof);
struct Xbsos * XB_CC xbnewsosrc_prob(struct Xbprob *prob, const char *name, int type, struct Xbvar **av, struct Xbctr *lct);
struct Xbsos * XB_CC xbnewsosw_prob(struct Xbprob *prob, const char *name, int type, struct Xbvar **av, double *cof);

/**** Objective function ****/
int XB_CC xbsetsense_prob(struct Xbprob *prob, int dir);   /* default: minimise */
int XB_CC xbgetsense_prob(struct Xbprob *prob); 
int XB_CC xbsetobj_prob(struct Xbprob *prob, struct Xbctr *lct);

/**** Variable bounds ****/
int XB_CC xbsetub(struct Xbvar *var, double c);
int XB_CC xbsetlb(struct Xbvar *var, double c);
int XB_CC xbfixvar(struct Xbvar *var, double c);
int XB_CC xbsetlim(struct Xbvar *var, double c);

/**** Names ****/
void * XB_CC xbgetbyname_prob(struct Xbprob *prob, const char *name, int type);
int XB_CC xbsetdictsize_prob(struct Xbprob *prob,int dico,int size);

/**** Directives ****/
int XB_CC xbsetvardir(struct Xbvar *var, int type, double cost);
int XB_CC xbsetsosdir(struct Xbsos *ls, int type, double val);
int XB_CC xbcleardir_prob(struct Xbprob *prob); 

/**** Index sets ****/
struct Xbidxset * XB_CC xbnewidxset_prob(struct Xbprob *prob, const char *name, int maxsize);
int XB_CC xbaddidxel(struct Xbidxset *idx, const char *name);
int XB_CC xbgetidxel(struct Xbidxset *idx, const char *name);
const char * XB_CC xbgetidxelname(struct Xbidxset *idx, int i);     
const char * XB_CC xbgetidxsetname(struct Xbidxset *idx);
int XB_CC xbgetidxsetsize(struct Xbidxset *idx);

/**** Output and printing ****/
int XB_CC xbprintprob_prob(struct Xbprob *prob);
int XB_CC xbprintobj_prob(struct Xbprob *prob);
int XB_CC xbexportprob_prob(struct Xbprob *prob, int format, const char *filename);
int XB_CC xbwritedir_prob(struct Xbprob *prob, const char *filename);
int XB_CC xbloadmat_prob(struct Xbprob *prob);

int XB_CC xbgetvarstat(struct Xbvar *v);
int XB_CC xbprintvar(struct Xbvar *var);
int XB_CC xbprintarrvar(struct Xbvar **av);
int XB_CC xbprintctr(struct Xbctr *lct);
int XB_CC xbprintsos(struct Xbsos *ls);
int XB_CC xbprintidxset(struct Xbidxset *idx);  

/**** Information on variables and constraints ****/
const char * XB_CC xbgetprobname_prob(struct Xbprob *prob);
const char * XB_CC xbgetvarname(struct Xbvar *var);
const char * XB_CC xbgetctrname(struct Xbctr *lct);
const char * XB_CC xbgetarrvarname(struct Xbvar **av);
const char * XB_CC xbgetsosname(struct Xbsos *sos);
int XB_CC xbgetcolnum(struct Xbvar *var);
int XB_CC xbgetrownum(struct Xbctr *lct);
int XB_CC xbgetarrvarsize(struct Xbvar **av);
int XB_CC xbgetvartype(struct Xbvar *var);
int XB_CC xbgetctrtype(struct Xbctr *lct);
int XB_CC xbgetsostype(struct Xbsos *sos);
double XB_CC xbgetrhs(struct Xbctr *lct);
int XB_CC xbgetmodcut(struct Xbctr *lct);
int XB_CC xbgetbounds(struct Xbvar *var, double *lbd, double *ubd);
int XB_CC xbgetlim(struct Xbvar *var, double *lim);      
int XB_CC xbgetrange(struct Xbctr *lct, double *low, double *up); 
struct Xbbasis* XB_CC xbsavebasis_prob(struct Xbprob *prob);
int XB_CC xbloadbasis(struct Xbbasis *basis);
void XB_CC xbdelbasis(struct Xbbasis *basis);

/**** Problem solving and solution information ****/
int XB_CC xbsolve_prob(struct Xbprob *prob, const char *alg);
int XB_CC xbminim_prob(struct Xbprob *prob, const char *alg);
int XB_CC xbmaxim_prob(struct Xbprob *prob, const char *alg);
int XB_CC xbgetprobstat_prob(struct Xbprob *prob);
int XB_CC xbgetlpstat_prob(struct Xbprob *prob);
int XB_CC xbgetmipstat_prob(struct Xbprob *prob);
int XB_CC xbsync_prob(struct Xbprob *prob, int synctype);
double XB_CC xbgetobjval_prob(struct Xbprob *prob);
double XB_CC xbgetsol(struct Xbvar *var);
double XB_CC xbgetrcost(struct Xbvar *var);
double XB_CC xbgetslack(struct Xbctr *lct);
double XB_CC xbgetact(struct Xbctr *lct);
double XB_CC xbgetdual(struct Xbctr *lct);
double XB_CC xbgetctrrng(struct Xbctr *lct,int what);
double XB_CC xbgetvarrng(struct Xbvar *var,int what);
int XB_CC xbgetnumiis_prob(struct Xbprob *prob);
int XB_CC xbgetiis_prob(struct Xbprob *prob, struct Xbvar ***arrvar, 
             int *numvar, struct Xbctr ***arrctr, int *numctr, int numiis);
int XB_CC xbsetcolorder_prob(struct Xbprob *prob,int colorder);

/****Interface to other software****/
void * XB_CC xbgetvarlink(struct Xbvar *var);
int XB_CC xbsetvarlink(struct Xbvar *var,void *val);
int XB_CC xbdefcbdelvar_prob(struct Xbprob *prob,void (XB_CC *userdelinter)(struct Xbprob *,void *,struct Xbvar *,void *),void *vp);

/****Defining and adding cuts****/
struct Xbcut * XB_CC xbnewcut_prob(struct Xbprob *prob, int qrtype, int mtype);
int XB_CC xbdelcut(struct Xbcut *lct);
int XB_CC xbaddcutterm(struct Xbcut *lct, struct Xbvar *var, double coeff);
int XB_CC xbdelcutterm(struct Xbcut *lct, struct Xbvar *var);
int XB_CC xbsetcutterm(struct Xbcut *lct, struct Xbvar *var, double coeff);
int XB_CC xbaddcutarrterm(struct Xbcut *cut, struct Xbvar **tv,double *tc);
int XB_CC xbaddcuts_prob(struct Xbprob *prob, struct Xbcut **cta, int num);
struct Xbcut * XB_CC xbnewcutsum_prob(struct Xbprob *prob, struct Xbvar **tv, int qrtype, double rhs, int mtype);
struct Xbcut * XB_CC xbnewcutsumc_prob(struct Xbprob *prob, struct Xbvar **tv, double c, int qrtype, double rhs, int mtype);
struct Xbcut * XB_CC xbnewcutarrsum_prob(struct Xbprob *prob, struct Xbvar **tv, double *tc, int qrtype, double rhs, int mtype);
struct Xbcut * XB_CC xbnewcutprec_prob(struct Xbprob *prob, struct Xbvar *v1, double dur, struct Xbvar *v2, int mtype);
int XB_CC xbprintcut(struct Xbcut *lct);
int XB_CC xbsetcuttype(struct Xbcut *lct, int qrtype);
int XB_CC xbsetcutid(struct Xbcut *lct, int val);
int XB_CC xbgetcuttype(struct Xbcut *lct);
double XB_CC xbgetcutrhs(struct Xbcut *lct);
int XB_CC xbgetcutid(struct Xbcut *lct);
int XB_CC xbsetcutmode_prob(struct Xbprob *prob, int mode);

/**** Term lists management ****/
struct Xbexpr *XB_CC xbnewexpr(double ,struct Xbvar *,struct Xbvar *);
void XB_CC xbdelexpr(struct Xbexpr *);
void XB_CC xbclearexpr(struct Xbexpr *);
int XB_CC xbassignexpr(struct Xbexpr *,struct Xbexpr *);
int XB_CC xbgetstatexpr(struct Xbexpr *);
double XB_CC xbevalexpr(struct Xbexpr *);
int XB_CC xbdupexpr(struct Xbexpr**,struct Xbexpr*);
int XB_CC xbaddexpr_term(struct Xbexpr *,struct Xbvar *,struct Xbvar *, double);
int XB_CC xbsetexpr_term(struct Xbexpr *,struct Xbvar *,struct Xbvar *, double);
int XB_CC xbaddexpr(struct Xbexpr*,struct Xbexpr*);
void XB_CC xbchsexpr(struct Xbexpr*);
void XB_CC xbmulcexpr(struct Xbexpr *,double);
int XB_CC xbmulexpr(struct Xbexpr*,struct Xbexpr*);
int XB_CC xbsetobjexpr(struct Xbprob *, struct Xbexpr*);
int XB_CC xbprintexpr(struct Xbprob *,struct Xbexpr *,int,double);
int XB_CC xbappctr(struct Xbctr *lct, struct Xbexpr *tls);
int XB_CC xbsetctr(struct Xbctr *lct, struct Xbexpr *tls, int type);
int XB_CC xbappsos(struct Xbsos *ls, struct Xbexpr *tls);
int XB_CC xbsetsos(struct Xbsos *ls, struct Xbexpr *tls);
int XB_CC xbappcut(struct Xbcut *lct,struct Xbexpr *tls);
int XB_CC xbsetcut(struct Xbcut *lct,struct Xbexpr *tls, int type);

char * XB_CC xbexpr2str(struct Xbexpr *expr,int type);
void XB_CC xbfree2str(char *buf);
#ifdef __cplusplus
};
#endif

#define xbdefcberr(fct,p) xbdefcberr_prob(xbgetprob(),fct,p)
#define xbdefcbmsg(fct,p) xbdefcbmsg_prob(xbgetprob(),fct,p)
#define xbdefcbdelvar(fct,p) xbdefcbdelvar_prob(xbgetprob(),fct,p)

/****XPRB****/
#ifndef BCL_CPP
typedef struct Xbprob *XPRBprob;
typedef struct Xbvar *XPRBvar;
typedef struct Xbvar **XPRBarrvar;
typedef struct Xbctr *XPRBctr;
typedef struct Xbsos *XPRBsos;
typedef struct Xbcut *XPRBcut;
typedef struct Xbidxset *XPRBidxset;
typedef struct Xbexpr *XPRBexpr;
typedef struct Xbbasis *XPRBbasis;
#endif

#define XPRB_CC XB_CC

#define XPRB_INFINITY XB_INFINITY
#define XPRB_VERSION XB_VERSION

#define XPRB_ERR XB_ERR
#define XPRB_WAR XB_WAR

#define XPRB_FGETS XB_FGETS

#define XPRB_PL XB_PL
#define XPRB_BV XB_BV
#define XPRB_UI XB_UI
#define XPRB_PI XB_PI
#define XPRB_SC XB_SC
#define XPRB_SI XB_SI

#define XPRB_N XB_N
#define XPRB_L XB_L
#define XPRB_G XB_G
#define XPRB_E XB_E
#define XPRB_R XB_R

#define XPRB_MAXIM XB_MAXIM
#define XPRB_MINIM XB_MINIM

#define XPRB_S1 XB_S1
#define XPRB_S2 XB_S2

#define XPRB_UPACT XB_UPACT 
#define XPRB_LOACT XB_LOACT 
#define XPRB_UUP   XB_UUP   
#define XPRB_UDN   XB_UDN   
#define XPRB_UCOST XB_UCOST 
#define XPRB_LCOST XB_LCOST 

#define XPRB_PR XB_PR
#define XPRB_UP XB_UP
#define XPRB_DN XB_DN
#define XPRB_PU XB_PU
#define XPRB_PD XB_PD

#define XPRB_VAR XB_VAR
#define XPRB_ARR XB_ARR
#define XPRB_CTR XB_CTR
#define XPRB_SOS XB_SOS
#define XPRB_IDX XB_IDX

#define XPRB_DICT_NAMES XB_DICT_NAMES
#define XPRB_DICT_IDX   XB_DICT_IDX

#define XPRB_LP  XB_LP
#define XPRB_MPS XB_MPS

#define XPRB_XPRS_SOL  XB_XO_SOL
#define XPRB_XPRS_PROB XB_XO_PROB

#define XPRB_GEN XB_GEN
#define XPRB_DIR XB_DIR
#define XPRB_MOD XB_MOD
#define XPRB_SOL XB_SOL

#define XPRB_LP_OPTIMAL XB_LP_OPTIMAL
#define XPRB_LP_INFEAS XB_LP_INFEAS
#define XPRB_LP_CUTOFF XB_LP_CUTOFF
#define XPRB_LP_UNFINISHED XB_LP_UNFINISHED
#define XPRB_LP_UNBOUNDED XB_LP_UNBOUNDED
#define XPRB_LP_CUTOFF_IN_DUAL XB_LP_CUTOFF_IN_DUAL
#define XPRB_LP_UNSOLVED XB_LP_UNSOLVED

#define XPRB_MIP_NOT_LOADED XB_MIP_NOT_LOADED
#define XPRB_MIP_LP_NOT_OPTIMAL XB_MIP_LP_NOT_OPTIMAL
#define XPRB_MIP_LP_OPTIMAL XB_MIP_LP_OPTIMAL
#define XPRB_MIP_NO_SOL_FOUND XB_MIP_NO_SOL_FOUND
#define XPRB_MIP_SOLUTION XB_MIP_SOLUTION
#define XPRB_MIP_INFEAS XB_MIP_INFEAS
#define XPRB_MIP_OPTIMAL XB_MIP_OPTIMAL

/****2.2.x****/

/**** Initialization/termination ****/
#define XPRBinit xbslinit
#define XPRBfinish xbslfinish
#define XPRBfree xbslfinish
#define XPRBgetversion xbgetversion
#define XPRBgettime xbgettime

/**** Error handling ****/
#define XPRBseterrctrl xbseterrctrl
#define XPRBdefcberr xbdefcberr_prob

/**** Problem definition ****/
#define XPRBnewprob xbnewprob
#define XPRBchgprob xbchgprob
#define XPRBgetXPRSprob xbgetsprob_prob
#define XPRBgetprob xbgetprob

/**** Names ****/
#define XPRBnewname xbnewname

/**** Output and printing ****/
#define XPRBdefcbmsg xbdefcbmsg_prob
#define XPRBsetmsglevel xbsetmsglevel_prob
#define XPRBsetrealfmt xbsetrealfmt_prob
#define XPRBsetdecsign xbsetdecsign
#define XPRBprintf xbprintf_prob

/**** Files ****/
#define XPRBreadlinecb xbreadlinecb
#define XPRBreadarrlinecb xbreadarrlinecb
#define XPRBreadline xbreadline
#define XPRBreadarrline xbreadarrline

/**** Problem definition ****/
#define XPRBdelprob xbdelprob_prob
#define XPRBresetprob xbresetprob_prob

/**** Variable definition ****/
#define XPRBnewvar xbnewvar_prob
#define XPRBsetvartype xbsetvartype

/**** Arrays of variables ****/
#define XPRBnewarrvar xbnewarrvar_prob
#define XPRBstartarrvar xbstartarrvar_prob
#define XPRBsetarrvarel xbsetarrvarel
#define XPRBapparrvarel xbapparrvarel
#define XPRBendarrvar xbendarrvar
#define XPRBdelarrvar xbdelarrvar

/**** Constraint definition ****/
#define XPRBnewctr xbnewctr_prob
#define XPRBdelctr xbdelctr
#define XPRBaddterm xbaddterm
#define XPRBdelterm xbdelterm
#define XPRBsetterm xbsetterm
#define XPRBaddqterm xbaddqterm
#define XPRBdelqterm xbdelqterm
#define XPRBsetqterm xbsetqterm
#define XPRBaddarrterm xbaddarrterm
#define XPRBnewsum xbnewsum_prob
#define XPRBnewsumc xbnewsumc_prob
#define XPRBnewarrsum xbnewarrsum_prob
#define XPRBnewprec xbnewprec_prob
#define XPRBsetctrtype xbsetctrtype
#define XPRBsetrange xbsetrange
#define XPRBsetmodcut xbsetmodcut

/**** SOS definition ****/
#define XPRBnewsos xbnewsos_prob
#define XPRBdelsos xbdelsos
#define XPRBaddsosel xbaddsosel
#define XPRBdelsosel xbdelsosel
#define XPRBaddsosarrel xbaddsosarrel
#define XPRBnewsosrc xbnewsosrc_prob
#define XPRBnewsosw xbnewsosw_prob

/**** Objective function ****/
#define XPRBsetsense xbsetsense_prob
#define XPRBgetsense xbgetsense_prob
#define XPRBsetobj xbsetobj_prob

/**** Variable bounds ****/
#define XPRBsetub xbsetub
#define XPRBsetlb xbsetlb
#define XPRBfixvar xbfixvar
#define XPRBsetlim xbsetlim

/**** Names ****/
#define XPRBgetbyname xbgetbyname_prob
#define XPRBsetdictionarysize xbsetdictsize_prob

/**** Directives ****/
#define XPRBsetvardir xbsetvardir
#define XPRBsetsosdir xbsetsosdir
#define XPRBcleardir xbcleardir_prob

/**** Index sets ****/
#define XPRBnewidxset xbnewidxset_prob
#define XPRBaddidxel xbaddidxel
#define XPRBgetidxel xbgetidxel
#define XPRBgetidxelname xbgetidxelname
#define XPRBgetidxsetname xbgetidxsetname
#define XPRBgetidxsetsize xbgetidxsetsize

/**** Output and printing ****/
#define XPRBprintprob xbprintprob_prob
#define XPRBprintobj xbprintobj_prob
#define XPRBexportprob xbexportprob_prob
#define XPRBwritedir xbwritedir_prob
#define XPRBloadmat xbloadmat_prob

#define XPRBprintvar xbprintvar
#define XPRBprintarrvar xbprintarrvar
#define XPRBprintctr xbprintctr
#define XPRBprintsos xbprintsos
#define XPRBprintidxset xbprintidxset

/**** Information on variables and constraints ****/
#define XPRBgetprobname xbgetprobname_prob
#define XPRBgetvarname xbgetvarname
#define XPRBgetctrname xbgetctrname
#define XPRBgetarrvarname xbgetarrvarname
#define XPRBgetsosname xbgetsosname
#define XPRBgetcolnum xbgetcolnum
#define XPRBgetrownum xbgetrownum
#define XPRBgetarrvarsize xbgetarrvarsize
#define XPRBgetvartype xbgetvartype
#define XPRBgetctrtype xbgetctrtype
#define XPRBgetsostype xbgetsostype
#define XPRBgetrhs xbgetrhs
#define XPRBgetmodcut xbgetmodcut
#define XPRBgetbounds xbgetbounds
#define XPRBgetlim xbgetlim      
#define XPRBgetctrrng  xbgetctrrng 
#define XPRBgetvarrng  xbgetvarrng 
#define XPRBgetnumiis xbgetnumiis_prob
#define XPRBgetiis xbgetiis_prob
#define XPRBgetrange  xbgetrange 
#define XPRBsavebasis xbsavebasis_prob
#define XPRBloadbasis xbloadbasis
#define XPRBdelbasis xbdelbasis

/**** Problem solving and solution information ****/
#define XPRBsolve xbsolve_prob
#define XPRBminim xbminim_prob
#define XPRBmaxim xbmaxim_prob
#define XPRBgetprobstat xbgetprobstat_prob
#define XPRBgetlpstat xbgetlpstat_prob
#define XPRBgetmipstat xbgetmipstat_prob
#define XPRBsync xbsync_prob
#define XPRBgetobjval xbgetobjval_prob
#define XPRBgetsol xbgetsol
#define XPRBgetrcost xbgetrcost
#define XPRBgetslack xbgetslack
#define XPRBgetact xbgetact
#define XPRBgetdual xbgetdual
#define XPRBsetcolorder xbsetcolorder_prob

/****Interface to other software****/
#define XPRBgetvarlink xbgetvarlink
#define XPRBsetvarlink xbsetvarlink
#define XPRBdefcbdelvar xbdefcbdelvar_prob

/****Defining and adding cuts****/
#define XPRBnewcut xbnewcut_prob
#define XPRBdelcut xbdelcut
#define XPRBaddcutterm xbaddcutterm
#define XPRBdelcutterm xbdelcutterm
#define XPRBsetcutterm xbsetcutterm
#define XPRBaddcutarrterm xbaddcutarrterm
#define XPRBaddcuts xbaddcuts_prob
#define XPRBnewcutsum xbnewcutsum_prob
#define XPRBnewcutsumc xbnewcutsumc_prob
#define XPRBnewcutarrsum xbnewcutarrsum_prob
#define XPRBnewcutprec xbnewcutprec_prob
#define XPRBprintcut xbprintcut
#define XPRBsetcuttype xbsetcuttype
#define XPRBsetcutid xbsetcutid
#define XPRBgetcuttype xbgetcuttype
#define XPRBgetcutrhs xbgetcutrhs
#define XPRBgetcutid xbgetcutid
#define XPRBsetcutmode xbsetcutmode_prob

#endif
