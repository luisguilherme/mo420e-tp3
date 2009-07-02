/***************************************************************************\
 *                                                                         *
 * File:         xslp.h                                                    *
 * Copyright:    2008 Fair Isaac Corporation                               *
 *                                                                         *
 * Purpose:      provides the declarations necessary for using Xpress-SLP  *
 *                                                                         *
 * Description:  Release version 1.37  Feb 2008                            *
 *                                                                         *
\***************************************************************************/ 
#ifndef XSLP_H
#define XSLP_H

/***************************************************************************\
 * calling conventions for Windows                                         *
\***************************************************************************/
#ifdef _WIN32
#define XPRS_CC __stdcall
#else
#define XPRS_CC
#endif

#ifndef XSLP_CC
#define XSLP_CC XPRS_CC
#endif 

typedef struct tagXSLPproblem *XSLPprob;
typedef int (XPRS_CC *XSLPcbFunc)(XSLPprob,...);

/****************************************************************************\
 * Xpress-SLP constants                                                     *
\****************************************************************************/ 
 
/****************************************************************************\
 * Integer problem attributes                                               *
\****************************************************************************/ 

#ifndef XSLP_CONSTANTS
#define XSLP_CONSTANTS
#define XSLP_INTATTRIBSTART           12000
#define XSLP_ITER                     12001
#define XSLP_STATUS                   12002
#define XSLP_UNCONVERGED              12003
#define XSLP_SBXCONVERGED             12004
#define XSLP_CVS                      12005
#define XSLP_XVS                      12006
#define XSLP_UFS                      12007
#define XSLP_IFS                      12008
#define XSLP_PENALTYDELTAROW          12009
#define XSLP_PENALTYDELTACOLUMN       12010
#define XSLP_PENALTYERRORROW          12011
#define XSLP_PENALTYERRORCOLUMN       12012
#define XSLP_EQUALSCOLUMN             12013
#define XSLP_VARIABLES                12014
#define XSLP_IMPLICITVARIABLES        12015
#define XSLP_COEFFICIENTS             12016
#define XSLP_PENALTYDELTAS            12017
#define XSLP_PENALTYERRORS            12018
#define XSLP_PLUSPENALTYERRORS        12019
#define XSLP_MINUSPENALTYERRORS       12020
#define XSLP_UCCONSTRAINEDCOUNT       12021
#define XSLP_MIPNODES                 12022
#define XSLP_MIPITER                  12023
#define XSLP_VERSION                  12024
#define XSLP_MINORVERSION             12025
#define XSLP_NONLINEARCONSTRAINTS     12026
#define XSLP_IPROBLEM                 12027
#define XSLP_TOLSETS                  12028
#define XSLP_VALIDATIONCOUNT          12029
#define XSLP_INTERNALFUNCCALLS        12030
#define XSLP_USERFUNCCALLS            12031
#define XSLP_NLOBJROW                 12032
#define XSLP_NLOBJCOL                 12033
#define XSLP_UFINSTANCES              12034
#define XSLP_ECFCOUNT                 12035
#define XSLP_CHECKSUM                 12036
#define XSLP_USEDERIVATIVES           12037
#define XSLP_PRESOLVEPASSES           12038
#define XSLP_INTERRUPT                12039
#define XSLP_USEDFORMULAHASH          12040
#define XSLP_DELTAS                   12041
#define XSLP_ITERSOL                  12043
#define XSLP_PVNODES                  12044
#define XSLP_PVPARENT                 12045
#define XSLP_ZEROESRESET              12046
#define XSLP_ZEROESTOTAL              12047
#define XSLP_ZEROESRETAINED           12048
#define XSLP_THREADSTARTTIME          12049
#define XSLP_THREADOPTWINNER          12050
#define XSLP_PRESOLVEFIXEDSLPVAR      12051
#define XSLP_PRESOLVEFIXEDDR          12052
#define XSLP_PRESOLVEFIXEDCOEF        12053
#define XSLP_PRESOLVEFIXEDZCOL        12054
#define XSLP_PRESOLVEFIXEDNZCOL       12055
#define XSLP_PRESOLVEDELETEDDELTA     12056
#define XSLP_PRESOLVETIGHTENED        12057
#define XSLP_INTATTRIBEND             12058
/****************************************************************************\
 * Double control variables                                                 *
\****************************************************************************/ 
#define XSLP_DBLCONTROLSTART          12100
#define XSLP_CASCADETOL_A             12101
#define XSLP_CASCADETOL_R             12102
#define XSLP_DAMP                     12103
#define XSLP_DAMPEXPAND               12104
#define XSLP_DAMPSHRINK               12105
#define XSLP_DELTA_A                  12106
#define XSLP_DELTA_R                  12107
#define XSLP_DELTA_Z                  12108
#define XSLP_DELTACOST                12109
#define XSLP_DELTAMAXCOST             12110
#define XSLP_DELTAMULT                12111
#define XSLP_DJTOL                    12112
#define XSLP_ERRORCOST                12113
#define XSLP_ERRORMAXCOST             12114
#define XSLP_ERRORMULT                12115
#define XSLP_ERRORTOL_A               12116
#define XSLP_ERRORTOL_R               12117
#define XSLP_EXPAND                   12118
#define XSLP_INFINITY                 12119
#define XSLP_MAXWEIGHT                12120
#define XSLP_MINWEIGHT                12121
#define XSLP_SHRINK                   12122
#define XSLP_ZERO                     12123

/****************************************************************************\
 * Convergence tolerances                                                   *
\****************************************************************************/ 
#define XSLP_TOLCONTROLSTART          12124
#define XSLP_CTOL                     12124
#define XSLP_ATOL_A                   12125
#define XSLP_ATOL_R                   12126
#define XSLP_MTOL_A                   12127
#define XSLP_MTOL_R                   12128
#define XSLP_ITOL_A                   12129
#define XSLP_ITOL_R                   12130
#define XSLP_STOL_A                   12131
#define XSLP_STOL_R                   12132
#define XSLP_MVTOL                    12133
#define XSLP_XTOL_A                   12134
#define XSLP_XTOL_R                   12135
#define XSLP_TOLCONTROLEND            12136
/****************************************************************************\
 * End of convergence tolerances                                            *
\****************************************************************************/ 

#define XSLP_DEFAULTSTEPBOUND         12136
#define XSLP_DAMPMAX                  12137
#define XSLP_DAMPMIN                  12138
#define XSLP_DELTACOSTFACTOR          12139
#define XSLP_ERRORCOSTFACTOR          12140
#define XSLP_ERRORTOL_P               12141
#define XSLP_CASCADETOL_PA            12142
#define XSLP_CASCADETOL_PR            12143
#define XSLP_CASCADETOL_Z             12144
#define XSLP_DEFAULTIV                12145
#define XSLP_OBJSENSE                 12146
#define XSLP_OPTTIME                  12147
#define XSLP_UPDATESCALE              12148
#define XSLP_DELTASCALE               12149
#define XSLP_OTOL_A                   12150
#define XSLP_OTOL_R                   12151
#define XSLP_DELTA_X                  12152
#define XSLP_ERRORCOSTS               12153
#define XSLP_EQTOL_A                  12154
#define XSLP_EQTOL_R                  12155
#define XSLP_DELTA_ZR                 12156
#define XSLP_GRANULARITY              12157
#define XSLP_MIPCUTOFF_A              12158
#define XSLP_MIPCUTOFF_R              12159
#define XSLP_MIPOTOL_A                12160
#define XSLP_MIPOTOL_R                12161
#define XSLP_MINZEROBOUND             12162
#define XSLP_SBFROMMAX                12163
#define XSLP_MEMORYFACTOR             12164
#define XSLP_VALIDATIONTOL_A          12165
#define XSLP_VALIDATIONTOL_R          12166
#define XSLP_VALIDATIONINDEX_A        12167
#define XSLP_VALIDATIONINDEX_R        12168
#define XSLP_ESCALATION               12169
#define XSLP_OBJTOPENALTYCOST         12170
#define XSLP_PDRATIO                  12171
#define XSLP_DPRATIO                  12172
#define XSLP_UNBSTEPFACTOR            12176
#define XSLP_VTOL_A                   12177
#define XSLP_VTOL_R                   12178
#define XSLP_OBJVAL                   12179
#define XSLP_ETOL_A                   12180
#define XSLP_ETOL_R                   12181
#define XSLP_EVTOL_A                  12182
#define XSLP_EVTOL_R                  12183
#define XSLP_GTOL_A                   12184
#define XSLP_GDELTA                   12185
#define XSLP_NLSMALL                  12186
#define XSLP_NLSMALLSLOPE             12187
#define XSLP_NLSMALLPSLOPE            12188
#define XSLP_ECFTOL_A                 12189
#define XSLP_ECFTOL_R                 12190
#define XSLP_WTOL_A                   12191
#define XSLP_WTOL_R                   12192
#define XSLP_PRESOLVEZERO             12193
#define XSLP_PVERRORTOL_A             12194
#define XSLP_PVERRORTOL_R             12195
#define XSLP_USERFUNCTIONTHREADWAIT   12196
#define XSLP_USERFUNCTIONTHREADINCREMENT 12197
#define XSLP_MIPERRORTOL_A            12198
#define XSLP_MIPERRORTOL_R            12199
#define XSLP_DBLCONTROLEND            12200
		
/****************************************************************************\
 * Integer control variables                                                *
\****************************************************************************/ 
#define XSLP_INTCONTROLSTART          12300
#define XSLP_ALGORITHM                12301
#define XSLP_AUGMENTATION             12302
#define XSLP_BARLIMIT                 12303
#define XSLP_CASCADE                  12304
#define XSLP_CASCADEFLIMIT            12305
#define XSLP_CASCADENLIMIT            12306
#define XSLP_CONTROL                  12307
#define XSLP_DAMPSTART                12308
#define XSLP_DCLIMIT                  12309
#define XSLP_DEBUG                    12310
#define XSLP_DELTAZLIMIT              12311
#define XSLP_FUNCEVAL                 12312
#define XSLP_INFEASCHECK              12313
#define XSLP_INFEASLIMIT              12314
#define XSLP_ITERLIMIT                12315
#define XSLP_LOG                      12316
#define XSLP_SAMECOUNT                12317
#define XSLP_PROFILER                 12318
#define XSLP_SAMEDAMP                 12319
#define XSLP_SBSTART                  12320
#define XSLP_XCOUNT                   12321
#define XSLP_XLIMIT                   12322
#define XSLP_MEMORYSTART              12323
#define XSLP_EXTRACVS                 12323
#define XSLP_EXTRAUFS                 12324
#define XSLP_EXTRAXVS                 12325
#define XSLP_EXTRAXVITEMS             12326
#define XSLP_MEMORYEND                12327
#define XSLP_CONTINUE                 12328
#define XSLP_DELAYUPDATEROWS          12329
#define XSLP_AUTOSAVE                 12330
#define XSLP_SCRAMBLENAMES            12331
#define XSLP_PRINTFAILED              12332
#define XSLP_OCOUNT                   12333
#define XSLP_EVALUATE                 12334
#define XSLP_EXCELVISIBLE             12335
#define XSLP_MIPALGORITHM             12336
#define XSLP_MIPRELAXSTEPBOUNDS       12337
#define XSLP_MIPFIXSTEPBOUNDS         12338
#define XSLP_MIPITERLIMIT             12339
#define XSLP_MIPCUTOFFLIMIT           12340
#define XSLP_MIPOCOUNT                12341
#define XSLP_MIPINFEASLIMIT           12342
#define XSLP_MIPDEFAULTALGORITHM      12343
#define XSLP_PRESOLVE                 12344
#define XSLP_TIMEPRINT                12345
#define XSLP_SLPLOG                   12346
#define XSLP_MIPLOG                   12347
#define XSLP_DELTAOFFSET              12348
#define XSLP_UPDATEOFFSET             12349
#define XSLP_ERROROFFSET              12350
#define XSLP_SBROWOFFSET              12351
#define XSLP_PDSTART                  12352
#define XSLP_NLINFO                   12353
#define XSLP_STOPOUTOFRANGE           12354
#define XSLP_RESERVED                 12355
#define XSLP_VCOUNT                   12356
#define XSLP_VLIMIT                   12357
#define XSLP_ABORT                    12358
#define XSLP_SAVEALL                  12359
#define XSLP_NLALGORITHM              12360
#define XSLP_WARMSTART                12361
#define XSLP_NLITERPRINT              12362
#define XSLP_NLITERFREQ               12363
#define XSLP_NLITERLIMIT              12364
#define XSLP_NLSRC                    12365
#define XSLP_MAXTIME                  12366
#define XSLP_SCALE                    12367
#define XSLP_SCALECOUNT               12368
#define XSLP_ECFCHECK                 12369
#define XSLP_MIPCUTOFFCOUNT           12370
#define XSLP_FORMULAHASH              12371
#define XSLP_UNQUIETMIP               12372
#define XSLP_DERIVATIVES              12373
#define XSLP_WCOUNT                   12374
#define XSLP_PRESOLVEPASSLIMIT        12375
#define XSLP_UNFINISHEDLIMIT          12376
#define XSLP_PVCONTROL                12377
#define XSLP_ZEROCRITERION            12378
#define XSLP_ZEROCRITERIONSTART       12379
#define XSLP_ZEROCRITERIONCOUNT       12380
#define XSLP_USERFUNCTIONTHREADS      12381
#define XSLP_USERFUNCTIONTHREADLOG    12382
#define XSLP_USERFUNCTIONCBMUTEX      12383
#define XSLP_PENALTYINFOSTART         12384
#define XSLP_DCLOG                    12385
#define XSLP_DECOMPOSE                12386
#define XSLP_THREADOPT                12387
#define XSLP_THREADOPTLOG             12388
#define XSLP_THREADBARLIMIT           12389
#define XSLP_DECOMPOSEPASSLIMIT       12390
#define XSLP_INTCONTROLEND            12391
		
/****************************************************************************\
 * Character string control variables                                       *
\****************************************************************************/ 
#define XSLP_STRCONTROLSTART          12400
#define XSLP_CVNAME                   12401
#define XSLP_DELTAFORMAT              12402
#define XSLP_IVNAME                   12403
#define XSLP_MAXFORMAT                12404
#define XSLP_MINFORMAT                12405
#define XSLP_MINUSDELTAFORMAT         12406
#define XSLP_MINUSERRORFORMAT         12407
#define XSLP_PLUSDELTAFORMAT          12408
#define XSLP_PLUSERRORFORMAT          12409
#define XSLP_SBNAME                   12410
#define XSLP_TOLNAME                  12411
#define XSLP_UPDATEFORMAT             12412
#define XSLP_PENALTYROWFORMAT         12413
#define XSLP_PENALTYCOLFORMAT         12414
#define XSLP_LOWERBOUNDFORMAT         12415
#define XSLP_UPPERBOUNDFORMAT         12416
#define XSLP_SBLOROWFORMAT            12417
#define XSLP_SBUPROWFORMAT            12418
#define XSLP_NLOBJFORMAT              12419
#define XSLP_CASCADEREPORT            12420
#define XSLP_CONVERGEREPORT           12421
#define XSLP_THREADOPTCHOICE          12422
#define XSLP_ALGORITHMCHOICE          12423
#define XSLP_STRCONTROLEND            12424
		
/****************************************************************************\
 * Character string attributes                                              *
\****************************************************************************/ 
#define XSLP_STRATTRIBSTART           12500
#define XSLP_UNIQUEPREFIX             12501
#define XSLP_VERSIONDATE              12502
#define XSLP_THREADOPTALGORITHMS      12503
#define XSLP_STRATTRIBEND             12504

/****************************************************************************\
 * Pointer attributes                                                       *
\****************************************************************************/ 
#define XSLP_POINTERATTRIBSTART       12600
#define XSLP_XPRSPROBLEM              12601
#define XSLP_XSLPPROBLEM              12602
#define XSLP_XPRMLIBCTX               12603
#define XSLP_XPRMMOSELIDCALL          12604
#define XSLP_XPRMFUNCTIONCALL         12605
#define XSLP_XPRMFUNCTIONCALLA        12606
#define XSLP_GLOBALFUNCOBJECT         12607
#define XSLP_USERFUNCOBJECT           12608
#define XSLP_INSTANCEFUNCOBJECT       12609
#define XSLP_XPRMFUNCTIONCALLX        12610
#define XSLP_MIPPROBLEM               12611
#define XSLP_XPRMMOSELIDREF           12612
#define XSLP_POINTERATTRIBEND         12613

/****************************************************************************\
 * Double attributes		                                                *
\****************************************************************************/ 
#define XSLP_DBLATTRIBSTART           12700
#define XSLP_CURRENTDELTACOST         12701
#define XSLP_CURRENTERRORCOST         12702
#define XSLP_XTIME                    12703
#define XSLP_PENALTYERRORTOTAL        12704
#define XSLP_PENALTYERRORVALUE        12705
#define XSLP_PENALTYDELTATOTAL        12706
#define XSLP_PENALTYDELTAVALUE        12707
#define XSLP_VSOLINDEX                12708
#define XSLP_CLOCKSTART               12709
#define XSLP_DBLATTRIBEND             12710

/****************************************************************************\
 * Memory size controls                                               *
\****************************************************************************/ 
#define XSLP_MEMCONTROLSTART          12800
#define XSLP_MEM_STRING               12801
#define XSLP_MEM_CVAR                 12802
#define XSLP_MEM_XV                   12803
#define XSLP_MEM_XVITEM               12804
#define XSLP_MEM_UF                   12805
#define XSLP_MEM_VAR                  12806
#define XSLP_MEM_COEF                 12807
#define XSLP_MEM_FORMULA              12808
#define XSLP_MEM_STACK                12809
#define XSLP_MEM_COL                  12810
#define XSLP_MEM_ROW                  12811
#define XSLP_MEM_XROW                 12812
#define XSLP_MEM_TOL                  12813
#define XSLP_MEM_FORMULAVALUE         12814
#define XSLP_MEM_CALCSTACK            12815
#define XSLP_MEM_ITERLOG              12816
#define XSLP_MEM_XF                   12817
#define XSLP_MEM_XFVALUE              12818
#define XSLP_MEM_XFNAMES              12819
#define XSLP_MEM_EXCELDOUBLE          12820
#define XSLP_MEM_RETURNARRAY          12821
#define XSLP_MEM_VALSTACK             12822
#define XSLP_MEM_FORMULAHASH          12823
#define XSLP_MEM_GETNEXT              12824
#define XSLP_MEM_LU                   12825
#define XPRM_MEM_GEXP                 12826
#define XPRM_MEM_STACK                12827
#define XPRM_MEM_GENCTR               12828
#define XPRM_MEM_XTRA                 12829
#define XPRM_MEM_XSTACK               12830
#define XPRM_MEM_DATA                 12831
#define XPRM_MEM_XV                   12832
#define XPRM_MEM_ROW                  12833
#define XPRM_MEM_COL                  12834
#define XPRM_MEM_GXSTACK              12835
#define XPRM_MEM_QSTACK               12836
#define XSLP_MEM_DERIVATIVES          12837
#define XSLP_MEMCONTROLEND            12838
/****************************************************************************\
 * End of list                                                              *
\****************************************************************************/ 

#define XSLP_OBJTOL_A XSLP_XTOL_A
#define XSLP_OBJTOL_R XSLP_XTOL_R

/****************************************************************************\
 * Token types                                                              *
\****************************************************************************/ 
#define XSLP_EOF                          0
#define XSLP_FIRSTOPERAND                 1
#define XSLP_CON                          1
#define XSLP_CV                           2
#define XSLP_XV                           3
#define XSLP_VAR                          4
#define XSLP_STRING                       5
#define XSLP_RESOLVEDTOCONSTANT           6
#define XSLP_FORMULA                      7
#define XSLP_CONSTRAINT                   8
#define XSLP_ROW                          9
#define XSLP_COL                         10
#define XSLP_LASTOPERAND                 10
#define XSLP_FUN                         11
#define XSLP_IFUN                        12
#define XSLP_MPVAR                       13
#define XSLP_GEXP                        14
#define XSLP_GENCTR                      15
#define XSLP_XVITEM                      16
#define XSLP_MPXV                        17
#define XSLP_LINCTR                      18
#define XSLP_LINEXP                      19
#define XSLP_VARREF                      20
#define XSLP_LB                          21
#define XSLP_RB                          22
#define XSLP_DEL                         24
#define XSLP_UNKFUN                      25
#define XSLP_MOSELREF                    26
#define XSLP_MOSELSTR                    27
#define XSLP_MOSELFRM                    28
#define XSLP_OP                          31
#define XSLP_RESOLVED                    91
#define XSLP_DELETED                     92
#define XSLP_UNKNOWN                     99
#define XSLP_UFUNKNOWN                   98
#define XSLP_INFOTOKEN                   81
#define XSLP_COEFTOKEN                   82
#define XSLP_XVTOKEN                     83
#define XSLP_UFTOKEN                     84
#define XSLP_DCTOKEN                     85
#define XSLP_UFARGTYPE                   86
#define XSLP_UFEXETYPE                   87
#define XSLP_ECTOKEN                     88
#define XSLP_IVFTOKEN                    89
#define XSLP_UFLTHREAD                   90
#define XSLP_XVVARTYPE                   71
#define XSLP_XVVARINDEX                  72
#define XSLP_XVINTINDEX                  73
#define XSLP_XVDELTA1                    74
#define XSLP_XVDELTA2                    75
#define XSLP_XVINTADDRESS                76

#define XSLP_UMINUS                       1
#define XSLP_EXPONENT                     2
#define XSLP_MULTIPLY                     3
#define XSLP_DIVIDE                       4
#define XSLP_PLUS                         5
#define XSLP_MINUS                        6
#define XSLP_LE                           7
#define XSLP_EQ                           8
#define XSLP_GE                           9
#define XSLP_FUNCTION                    10
#define XSLP_NULL                        32

#define XSLP_COMMA                        1
#define XSLP_COLON                        2
#define XSLP_EQUALS                      11

/****************************************************************************\
 * Problem status settings                                                          *
\****************************************************************************/
#define XSLP_MADECONSTRUCT             0x01
#define XSLP_MADECASCADE               0x02
#define XSLP_SYNCHRONISED              0x04
#define XSLP_NODELTAZ                  0x08
#define XSLP_BASECALC                  0x10
#define XSLP_DELTACALC                 0x20
#define XSLP_ONEOFFCALC                0x40
#define XSLP_GLOBALINIT                0x80
#define XSLP_GLOBALOPT                0x100
#define XSLP_ENDGLOBALOPT             0x200
#define XSLP_GLOBALTOCONVERGE         0x400
#define XSLP_FIXEDERRORS              0x800
#define XSLP_PRESOLVED               0x1000
#define XSLP_FUNCTIONERROR           0x2000
#define XSLP_GLOBALSTART             0x4000
#define XSLP_BOUNDSRESTORED          0x8000
#define XSLP_SWITCHEDTOPRIMAL       0x10000
#define XSLP_ERRORCOSTSET           0x20000
#define XSLP_ENDMIPWITHINSLP        0x40000
#define XSLP_NODE0                  0x80000
#define XSLP_PVRUN                 0x100000
#define XSLP_REMAKE                0x200000

/****************************************************************************\
 * XSLP_STATUS settings                                                     *
\****************************************************************************/
#define XSLP_STATUS_CONVERGEDOBJUCC    0x01
#define XSLP_STATUS_CONVERGEDOBJSBX    0x02
#define XSLP_STATUS_LPINFEASIBLE       0x04
#define XSLP_STATUS_LPUNFINISHED       0x08
#define XSLP_STATUS_MAXSLPITERATIONS   0x10
#define XSLP_STATUS_INTEGERINFEASIBLE  0x20
#define XSLP_STATUS_RESIDUALPENALTIES  0x40
#define XSLP_STATUS_CONVERGEDOBJOBJ    0x80
#define XSLP_STATUS_CONVERGEDOBJSLOPE 0x100
#define XSLP_STATUS_MAXTIME           0x200
#define XSLP_STATUS_USER              0x400

/****************************************************************************\
 * Misc settings                                                            *
\****************************************************************************/
#define XSLP_RECALC                    0x08
#define XSLP_TOLCALC                   0x10
#define XSLP_ALLCALCS                  0x20
#define XSLP_2DERIVATIVE               0x40
#define XSLP_1DERIVATIVE               0x80
#define XSLP_ALLDERIVATIVES           0x100
#define XSLP_CDECL                    0x100
#define XSLP_STDCALL                  0x000
#define XSLP_INSTANCEFUNCTION         0x200
#define XSLP_NETCALL                  0x400
#define XSLP_DEDUCECONSTDERIVS        0x800
#define XSLP_SOMECONSTDERIVS         0x1000
#define XSLP_MULTIVALUED          0x1000000
#define XSLP_MOSELARRAYARGS       0x2000000
#define XSLP_ROWARG               0x4000000
#define XSLP_COLARG               0x8000000
#define XSLP_NODERIVATIVES       0x10000000
#define XSLP_THREADSAFE          0x20000000
#define XSLP_LOGICALTHREAD       0x40000000

#define XSLP_PRESOLVEFIXZERO           0x02
#define XSLP_PRESOLVEFIXALL            0x04
#define XSLP_PRESOLVESETBOUNDS         0x08
#define XSLP_PRESOLVEINTBOUNDS         0x10
#define XSLP_NOPRESOLVECOEFFICIENTS   0x100
#define XSLP_NOPRESOLVEDELTAS         0x200

#define XSLP_FUNCINFOSIZE                12
#define XSLPINFO_CALLERFLAG               1
#define XSLPINFO_NINPUT                   2
#define XSLPINFO_NOUTPUT                  3
#define XSLPINFO_NDELTA                   4
#define XSLPINFO_NINSTRING                5
#define XSLPINFO_NOUTSTRING               6
#define XSLPINFO_FUNCNUM                  7
#define XSLPINFO_INSTANCE                 8
#define XSLPINFO_THREAD                   9
#define XSLPINFO_LOGICALTHREAD           10

#define XSLP_GETUFNAME                   31
#define XSLP_GETUFPARAM1                 32
#define XSLP_GETUFPARAM2                 33
#define XSLP_GETUFPARAM3                 34
#define XSLP_GETUFARGTYPE                35
#define XSLP_GETUFEXETYPE                36
#define XSLP_GETUFLTHREAD                37

#define XSLP_SETUFNAME                   41
#define XSLP_SETUFPARAM1                 42
#define XSLP_SETUFPARAM2                 43
#define XSLP_SETUFPARAM3                 44
#define XSLP_SETUFARGTYPE                45
#define XSLP_SETUFEXETYPE                46
#define XSLP_SETUFLTHREAD                47

#define XSLP_GETPVNODES                 101 
#define XSLP_GETPVBESTNODE              102
#define XSLP_GETPVBESTVALUE             103
#define XSLP_GETPVSENSE                 104

#define XSLP_GETROWNUMPENALTYERRORS     201
#define XSLP_GETROWMAXPENALTYERROR      202
#define XSLP_GETROWTOTALPENALTYERROR    203
#define XSLP_GETROWAVERAGEPENALTYERROR  204
#define XSLP_GETROWCURRENTPENALTYERROR  205
#define XSLP_GETROWCURRENTPENALTYFACTOR 206
#define XSLP_SETROWPENALTYFACTOR        207
#define XSLP_GETROWPENALTYCOLUMN1       208
#define XSLP_GETROWPENALTYCOLUMN2       209

#define XSLP_CASCADE_COEF_VAR          0x02
#define XSLP_CASCADE_ALL_COEF_VAR      0x04
#define XSLP_CASCADE_STRUCT_VAR	       0x08
#define XSLP_CASCADE_ALL_STRUCT_VAR    0x10

#define XSLP_PARMTYP_INT                001
#define XSLP_PARMTYP_DBL                002
#define XSLP_PARMTYP_STR                004
#define XSLP_PARMTYP_REF               0100
#define XSLP_PARMTYP_MEM               0201

#define XSLP_PARMTYP_READ               010
#define XSLP_PARMTYP_WRITE              020
#define XSLP_PARMTYP_CONTROL            040

/****************************************************************************\
 * AddNames values                                                          *
\****************************************************************************/ 
#define XSLP_CVNAMES                      3
#define XSLP_XVNAMES                      4
#define XSLP_USERFUNCNAMES                5
#define XSLP_INTERNALFUNCNAMES            6
#define XSLP_USERFUNCNAMESNOCASE          7
#define XSLP_INTERNALFUNCNAMESNOCASE      8

#endif

/****************************************************************************\
 * function declarations                                                    *
\****************************************************************************/ 
#ifdef __cplusplus
extern "C" {
#endif
int XSLP_CC XSLPcreateprob(XSLPprob *Prob, XPRSprob *XprsProb);
int XSLP_CC XSLPdestroyprob(XSLPprob Prob);
int XSLP_CC XSLPfree(void); 
int XSLP_CC XSLPgetbanner(char *Banner); 
int XSLP_CC XSLPgetversion(int *version, int *minorversion);
int XSLPgetlibstring(int MsgId, char *Message);
int XSLP_CC XSLPgetdblcontrol(XSLPprob Prob, int Param, double *dValue);
int XSLP_CC XSLPgetintattrib(XSLPprob Prob, int Param, int *iValue);
int XSLP_CC XSLPgetdblattrib(XSLPprob prob, int param, double *value);
int XSLP_CC XSLPgetstrattrib(XSLPprob Prob, int Param, char *cValue);
int XSLP_CC XSLPgetintcontrol(XSLPprob Prob, int Param, int *iValue);
int XSLP_CC XSLPgetstrcontrol(XSLPprob Prob, int Param, char *cValue);
int XSLP_CC XSLPgetptrattrib(XSLPprob prob, int param, void **value);
int XSLP_CC XSLPinit(void); 
int XSLP_CC XSLPsetdblcontrol(XSLPprob Prob, int Param, double dValue);
int XSLP_CC XSLPsetdefaults(XSLPprob Prob);
int XSLP_CC XSLPsetintcontrol(XSLPprob Prob, int Param, int iValue);
int XSLP_CC XSLPsetstrcontrol(XSLPprob Prob, int Param, const char *cValue);
int XSLP_CC XSLPgetlasterror(XSLPprob Prob, int *Code, char *Buffer);
int XSLP_CC XSLPsetstring(XSLPprob Prob, int *Param, const char *cValue);
int XSLP_CC XSLPgetstring(XSLPprob Prob, int Param, char *cValue);
int XSLP_CC XSLPsetcbmessage(XSLPprob prob, void (XPRS_CC *UserFunc)(XSLPprob,void*,char*,int,int),void *Object);
int XSLP_CC XSLPsetcbiterend(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbiterstart(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbitervar(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int),void *Object);
int XSLP_CC XSLPsetcbcascadestart(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbcascadevar(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int),void *Object);
int XSLP_CC XSLPsetcbcascadeend(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbslpend(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbslpstart(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbprenode(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbslpnode(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbintsol(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcboptnode(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbpvprenode(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbpvoptsol(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbpvintsol(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbpvpostnode(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbconstruct(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);
int XSLP_CC XSLPsetcbformula(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,double,double*),void *Object);
int XSLP_CC XSLPsetcbthreadstart(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int),void *Object);
int XSLP_CC XSLPsetcbthreadend(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int),void *Object);
int XSLP_CC XSLPsetcblogicalthreadstart(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int,int),void *Object);
int XSLP_CC XSLPsetcblogicalthreadend(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*,int,int),void *Object);

int XSLP_CC XSLPsetcbcascadevarF(XSLPprob prob,int (*UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbitervarF(XSLPprob prob,int (*UserFunc)(XSLPprob,void*,int*),void *Object);
int XSLP_CC XSLPsetcbmessageF(XSLPprob prob, void (*UserFunc)(XSLPprob,void*,char*,int*,int*),void *Object);
int XSLP_CC XSLPsetcbdestroy(XSLPprob prob,int (XPRS_CC *UserFunc)(XSLPprob,void*),void *Object);

int XSLP_CC XSLPsetcbxxx(XSLPprob Prob, int (XPRS_CC *UserFunc)(XSLPprob,...),void *Object,int XSLPcbnum);
int XSLP_CC XSLPpreparseformula(XSLPprob Prob, const char *Formula, int *nToken, int *Type, double *Value, char *StringTable, int *SizeTable);
int XSLP_CC XSLPaddcvars(XSLPprob Prob, int nSLPCVar, const char *cValue);
int XSLP_CC XSLPchgcvar(XSLPprob Prob, int nSLPCVar, const char *cValue);
int XSLP_CC XSLPgetcvar(XSLPprob Prob, int nSLPCVar, char *cValue);
int XSLP_CC XSLPaddnames(XSLPprob Prob, int type, const char *cNames, int First, int Last);
int XSLP_CC XSLPgetnames(XSLPprob Prob, int type, char *cNames, int First, int Last);
int XSLP_CC XSLPloadcvars(XSLPprob Prob, int nSLPCVar, const char *cValue);
int XSLP_CC XSLPloaduserfuncs(XSLPprob Prob, int nSLPUserFunc, const int *Type, const double *Value);
int XSLP_CC XSLPadduserfuncs(XSLPprob Prob, int nSLPUserFunc, const int *Type, const double *Value);
int XSLP_CC XSLPgetuserfunc(XSLPprob Prob, int nSLPUserFunc, char *xName, int *ArgType, int *ExeType, char *Param1, char *Param2, char *Param3);
int XSLP_CC XSLPchguserfunc(XSLPprob Prob, int nSLPUserFunc, const char *xName, const int *ArgType, const int *ExeType, const char *Param1, const char *Param2, const char *Param3);
int XSLP_CC XSLPloadxvs(XSLPprob Prob, int nSLPXV, const int *XVStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPaddxvs(XSLPprob Prob, int nSLPXV, const int *XVStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPgetxv(XSLPprob Prob, int nXSLPXV, int *nXVitems);
int XSLP_CC XSLPchgxv(XSLPprob Prob, int nXSLPXV, const int *nXVitems);
int XSLP_CC XSLPgetxvitem(XSLPprob Prob, int nSLPXV, int nXVitem, int Parsed,
		int *VarType, int *VarIndex, int *IntIndex, double *Delta1, double *Delta2, int *Status,
		int *Type, double *Value);
int XSLP_CC XSLPchgxvitem(XSLPprob Prob, int nSLPXV, int nXVitem, int Parsed,
		const int *VarType, const int *VarIndex, const int *IntIndex, const double *Delta1, const double *Delta2, const int *Status,
		const int *Type, const double *Value);
int XSLP_CC XSLPloadcoefs(XSLPprob Prob, int nSLPCoef, const int *RowIndex, const int *ColIndex,
								 const double *Factor, const int *FormulaStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPaddcoefs(XSLPprob Prob, int nSLPCoef, const int *RowIndex, const int *ColIndex,
								 const double *Factor, const int *FormulaStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPloadvars(XSLPprob Prob, int nSLPVar, const int *ColIndex, const int *VarType, const int *DetRow, const int *SeqNum, const int *TolIndex,
							const double *InitValue, const double *StepBound);
int XSLP_CC XSLPaddvars(XSLPprob Prob, int nSLPVar, const int *ColIndex, const int *VarType, const int *DetRow, const int *SeqNum, const int *TolIndex,
					   const double *InitValue, const double *StepBound);
int XSLP_CC XSLPloaddcs(XSLPprob Prob, int nSLPDC, const int *RowIndex, const int *Delay,
					   const int *DCStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPadddcs(XSLPprob Prob, int nSLPDC, const int *RowIndex, const int *Delay,
					   const int *DCStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPloadtolsets(XSLPprob Prob, int nSLPTol, const double *SLPTol);
int XSLP_CC XSLPaddtolsets(XSLPprob Prob, int nSLPTol, const double *SLPTol);
int XSLP_CC XSLPparsecformula(XSLPprob Prob, const char *Formula, int *nToken, int *Type, double *Value);
int XSLP_CC XSLPparseformula(XSLPprob Prob, const int *inType, const double *inValue, int *nToken, int *Type, double *Value);
int XSLP_CC XSLPchgcoef(XSLPprob Prob, int RowIndex, int ColIndex, const double *Factor, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPchgccoef(XSLPprob Prob, int RowIndex, int ColIndex, const double *Factor,const char *Formula);
int XSLP_CC XSLPgetccoef(XSLPprob Prob, int RowIndex, int ColIndex, double *Factor, char *Formula, int fLen);
int XSLP_CC XSLPgetcoef(XSLPprob Prob, int RowIndex, int ColIndex, double *Factor, int Parsed, int *Type, double *Value);
int XSLP_CC XSLPitemname(XSLPprob Prob, int Type, double Value, char *Buffer);
int XSLP_CC XSLPvalidformula(const int *inType, const double *inValue, int *nToken, char *Name, char *StringTable);
int XSLP_CC XSLPqparse(const char *Record, char *Token[], int NumFields);
int XSLP_CC XSLPtokencount(const char *Record);
int XSLP_CC XSLPchgtolset(XSLPprob Prob, int nSLPTol, const int *Status, const double *Tols);
int XSLP_CC XSLPgettolset(XSLPprob Prob, int nSLPTol, int *Status, double *Tols);
int XSLP_CC XSLPcopycallbacks(XSLPprob NewProb, const XSLPprob OldProb);
int XSLP_CC XSLPcopycontrols(XSLPprob NewProb, const XSLPprob OldProb);
int XSLP_CC XSLPcopyprob(XSLPprob prob, XSLPprob oldprob, const char *probname);
int XSLP_CC XSLPevaluatecoef(XSLPprob Prob, int RowIndex, int ColIndex, double *dValue);
int XSLP_CC XSLPcascadeorder(XSLPprob Prob);
int XSLP_CC XSLPcascade(XSLPprob Prob);
int XSLP_CC XSLPgetsol(XSLPprob Prob);
int XSLP_CC XSLPgetslpsol(XSLPprob prob, double *dx, double *dslack, double *dual, double *dj);
int XSLP_CC XSLPconstruct(XSLPprob Prob);
int XSLP_CC XSLPconstructxf(XSLPprob Prob);
int XSLP_CC XSLPchgdc(XSLPprob Prob, int RowIndex, const char *RowType, const int *Delay, const int *IterCount, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPgetdc(XSLPprob Prob, int RowIndex, char *RowType, int *Delay, int *IterCount, int Parsed, int *Type, double *Value);
int XSLP_CC XSLPevaluateformula(XSLPprob Prob, int Parsed, const int *Type, const double *Value, double *dValue);
int XSLP_CC XSLPformatvalue(double dValue, char *Buffer);
int XSLP_CC XSLPtime(XSLPprob Prob);
int XSLP_CC XSLPgetvar(XSLPprob Prob, int ColIndex, int *DetRow,
							   double *InitStepBound, double *StepBound,
							   double *Penalty, double *Damp, double *InitValue, double *Value,
							   int *TolSet, int *History, int *Converged, int *VarType,
							   int *Delta, int *PenaltyDelta, int *UpdateRow,  double *OldValue);
int XSLP_CC XSLPchgvar(XSLPprob Prob, int ColIndex, const int *DetRow,
							    const double *InitStepBound, const double *StepBound,
							    const double *Penalty, const double *Damp, const double *InitValue, const double *Value,
							    const int *TolSet, const int *History, const int *Converged, const int *VarType);
int XSLP_CC XSLPsetuniqueprefix(XSLPprob Prob);
int XSLP_CC XSLPreadprob(XSLPprob prob, const char *probname, const char *flags);
int XSLP_CC XSLPwriteprob(XSLPprob prob, const char *filename, const char *flags);
int XSLP_CC XSLPgettime(XSLPprob prob, int *Seconds, int *MilliSeconds);
int XSLP_CC XSLPgetdtime(XSLPprob prob, double *dSeconds);
int XSLP_CC XSLPsave(XSLPprob prob);
int XSLP_CC XSLPsaveas(XSLPprob prob, const char *File);
int XSLP_CC XSLPrestore(XSLPprob prob, const char *ProbName);
int XSLP_CC XSLPgetuserfuncaddress(XSLPprob prob, int FuncNumber, void **Address);
int XSLP_CC XSLPchguserfuncaddress(XSLPprob prob, int FuncNumber, void **Address);
int XSLP_CC XSLPsetuserfuncaddress(XSLPprob prob, int FuncNumber, void *Address);
int XSLP_CC XSLPglobal(XSLPprob prob);
int XSLP_CC XSLPgetindex(XSLPprob prob, int Type, const char *Name, int *SeqNo);
int XSLP_CC XSLPgetrow(XSLPprob prob, int RowIndex, int *Status);
int XSLP_CC XSLPchgrow(XSLPprob prob, int RowIndex, int *Status);
int XSLP_CC XSLPpresolve(XSLPprob prob);
int XSLP_CC XSLPmaxim(XSLPprob Prob, const char *Flags);
int XSLP_CC XSLPminim(XSLPprob Prob, const char *Flags);
int XSLP_CC XSLPopt(XSLPprob Prob, const char *Flags);
int XSLP_CC XSLPremaxim(XSLPprob Prob, const char *Flags);
int XSLP_CC XSLPreminim(XSLPprob Prob, const char *Flags);
int XSLP_CC XSLPpostsolve(XSLPprob prob, const char *Flags);
int XSLP_CC XSLPsetlogfile(XSLPprob prob, const char *filename, int option);
int XSLP_CC XSLPgetmessagetype(int Code, int *Type);
int XSLP_CC XSLPrevise(XSLPprob prob, const char *probname);
int XSLP_CC XSLPscaling(XSLPprob prob);
int XSLP_CC XSLPsetuserfuncobject(XSLPprob prob, int Entity, void *Object);
int XSLP_CC XSLPchguserfuncobject(XSLPprob prob, int Entity, void **Object);
int XSLP_CC XSLPgetuserfuncobject(XSLPprob prob, int Entity, void **Object);
int XSLP_CC XSLPsetfuncobject(int *ArgCount, int ObjType, void *Object);
int XSLP_CC XSLPgetfuncobject(int *ArgCount, int ObjType, void **Object);
int XSLP_CC XSLPgetfuncobjectV(void *ArgInfo, int ObjType, void **Object);
double XSLP_CC XSLPcalluserfunc(XSLPprob prob, int FuncNumber, void *Arg1, void *Arg2, void *Arg3, void *Arg4, void *Arg5, void *Arg6);
int XSLP_CC XSLPsetuserfuncinfo(XSLPprob prob, int *ArgInfo, int CallerFlag, int nInput, int nReturn, int nDelta, int nInString, int nOutString);
int XSLP_CC XSLPgetfuncinfo(int *ArgInfo, int *CallerFlag, int *nInput, int *nReturn, int *nDelta, int *nInString, int *nOutString, int *nSLPUserFunc, int *nInstance);
int XSLP_CC XSLPgetfuncinfoV(void *ArgInfo, int *CallerFlags, int *nInput, int *nReturn, int *nDelta, int *nInString, int *nOutString, int *nSLPUserFunc, int *nInstance);
int XSLP_CC XSLPsetfunctionerror(XSLPprob prob);
int XSLP_CC XSLPprintmsg(XSLPprob prob, int Type, const char *format);
int XSLP_CC XSLPuprintmemory(XSLPprob prob);
int XSLP_CC XSLPloadivfs(XSLPprob prob, int nIVF, const int *ColIndex, 
						   const int *IVFStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPaddivfs(XSLPprob prob, int nIVF, const int *ColIndex, 
						   const int *IVFStart, int Parsed, const int *Type, const double *Value);
int XSLP_CC XSLPgetivf(XSLPprob prob, int ColIndex, int Parsed, int *Type, double *Value);
int XSLP_CC XSLPchgivf(XSLPprob prob, int ColIndex, int Parsed, const int *Type, const double *Value);

int XSLP_CC XSLPuserfuncinfo(XSLPprob prob, int iFunc, int InfoType, void *Info);
int XSLP_CC XSLPgetrowwt(XSLPprob Prob, int RowIndex, double *Value);
int XSLP_CC XSLPchgrowwt(XSLPprob Prob, int RowIndex, const double *Value);
int XSLP_CC XSLProwinfo(XSLPprob prob, int RowIndex, int InfoType, void *Info);

int XSLP_CC XSLPreaddfs(XSLPprob prob, const char *Filename);
int XSLP_CC XSLPwritedfs(XSLPprob prob, const char *Filename);
int XSLP_CC XSLPloaddfs(XSLPprob prob, int nDF, const int *ColIndex, const int *RowIndex, const double *DFValue);
int XSLP_CC XSLPadddfs(XSLPprob prob, int nDF, const int *ColIndex, const int *RowIndex, const double *DFValue);
int XSLP_CC XSLPgetdf(XSLPprob prob, int ColIndex, int RowIndex, double *DFValue);
int XSLP_CC XSLPchgdf(XSLPprob prob, int ColIndex, int RowIndex, const double *DFValue);

int XSLP_CC XSLPinterrupt(XSLPprob prob, int Reason);
int XSLP_CC XSLPcheckinterrupt(XSLPprob);
int XSLP_CC XSLPloadpvs(XSLPprob prob, int nSLPPV, const int *Index, const double *Value, const int *Priority);
int XSLP_CC XSLPaddpvs(XSLPprob prob, int nSLPPV, const int *Index, const double *Value, const int *Priority);
int XSLP_CC XSLPgetmpv(XSLPprob prob, int ColIndex, int Space, int *nItem, double *Value, int *Priority);
int XSLP_CC XSLPchgmpv(XSLPprob prob, int ColIndex, int nItem, double *Value, int *Priority);
int XSLP_CC XSLPrunpv(XSLPprob prob, const char *Options);
int XSLP_CC XSLPrestartpv(XSLPprob prob, int NodeNumber, const char* FileName, const char *Options);
int XSLP_CC XSLPrestorepv(XSLPprob prob, int NodeNumber, const char* FileName);
int XSLP_CC XSLPgetpvinfo(XSLPprob prob, int Type, void *Info, const char *FileName);
int XSLP_CC XSLPgetpvvalues(XSLPprob prob, int First, int Last, double *Values, int *Parents, const char *FileName);

int XSLP_CC XSLPcontrolname(int Param, char *Name, int *Type);
int XSLP_CC XSLPcontrolcode(int *Param, const char *Name, int *Type);
int XSLP_CC XSLPsetparam(XSLPprob prob, const char *Name, const char *Value);
int XSLP_CC XSLPgetparam(XSLPprob prob, const char *Name, char *Value);
int XSLP_CC XSLPsetdefaultcontrol(XSLPprob prob, int Param);
int XSLP_CC XSLPnextparam(char *Name, int *Code, int *Type, int *Ref);

int XSLP_CC XSLPgetfuncattrib(int *ArgInfo, int Param, void *Result);
int XSLP_CC XSLPthreadactive(int *FuncInfo);
int XSLP_CC XSLPdecompose(XSLPprob prob, int Count, const int *Index);
#ifdef __cplusplus
};
#endif

#endif
