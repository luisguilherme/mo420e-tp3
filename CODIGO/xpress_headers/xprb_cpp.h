/*************************************/
/* BCL :  Xpress Builder Component   */
/* ===    Library                    */
/*                                   */
/* File xprb_cpp.h                   */
/* ```````````````                   */
/* -- C++ header file --             */
/*                                   */
/* (c)2008 Fair Isaac Corporation    */
/*    Creation: 2000, rev. June 2008 */
/*************************************/

#ifndef _XPRB
#define BCL_CPP
#include "xprb.h"

/* Compatilibity with BCL 3 */
#define XPRBlinExp XPRBexpr
#define XPRBquadExp XPRBexpr
#define XPRBlinRel XPRBrelation

namespace dashoptimization
{

class XPRB
{
 private:
  XPRB();
 public:
  static int init() { return xbslinit(); }
  static int finish() { return xbslfinish(); }
  static int free() { return xbslfinish(); }
  static int setMsgLevel(int lev) { return xbsetmsglevel_prob(NULL,lev); }
  static int setRealFmt(const char *fmt) { return xbsetrealfmt_prob(NULL,fmt); }
  static int setColOrder(int num) { return xbsetcolorder_prob(NULL,num); }
  static const char *getVersion() { return xbgetversion(); }
  static int getTime() { return xbgettime(); }
};

class XPRBvar
{
 private:
  struct Xbvar *varRef;
 public:
  XPRBvar() { varRef=NULL; }
  XPRBvar(struct Xbvar *v) { varRef=v; }

  struct Xbvar *getCRef() const { return varRef; }
  bool isValid() const { return (varRef!=NULL); }
  int print() { return xbprintvar(varRef); }
 
  int setType(int type) 
   { return xbsetvartype(varRef, type); }
  int setUB(double val) 
   { return xbsetub(varRef, val); }
  int setLB(double val) 
   { return xbsetlb(varRef, val); }
  int setLim(double val) 
   { return xbsetlim(varRef, val); }
  int fix(double val) 
   { return xbfixvar(varRef, val); }
  int setDir(int type, double val)
   { return xbsetvardir(varRef, type, val); }
  int setDir(int type)
   { return xbsetvardir(varRef, type, 0); }
  const char *getName() const { return xbgetvarname(varRef); }
  int getColNum() const { return xbgetcolnum(varRef); }
  int getType() const { return xbgetvartype(varRef); }
  double getLB() const
   { double b; xbgetbounds(varRef, &b, NULL); return b; }
  double getUB() const
   { double b; xbgetbounds(varRef, NULL, &b); return b; }
  int getLim(double *val) const { return xbgetlim(varRef, val); }
  double getLim() const
   { double val; xbgetlim(varRef, &val); return val; }
  double getSol() const { return xbgetsol(varRef); }
  double getRCost() const { return xbgetrcost(varRef); }
  double getRNG(int rngtype) const { return xbgetvarrng(varRef,rngtype); }

};

class XPRBterm
{
 private:
  XPRBterm();
 public:
  struct Xbvar *varRef;
  double coeff;
  XPRBterm(double d, const XPRBvar& v) { varRef=v.getCRef(); coeff=d; }

  XPRBterm& neg() { coeff=-coeff; return *this; }
  static XPRBterm neg(const XPRBterm& l) { return XPRBterm(l).neg(); }
  
  XPRBterm operator - () const { return neg(*this); }

  friend class XPRBexpr;
};

class XPRBexpr
{
 protected:
  struct Xbexpr *expr;
 public:
  XPRBexpr(double d=0) { expr=xbnewexpr(d,NULL,NULL); }
  XPRBexpr(int i) { expr=xbnewexpr(i,NULL,NULL); }
  XPRBexpr(double d, const XPRBvar& v) { expr=xbnewexpr(d, v.getCRef(),NULL); }
  XPRBexpr(double d, const XPRBvar& v1,const XPRBvar& v2) { expr=xbnewexpr(d, v1.getCRef(),v2.getCRef()); }
  XPRBexpr(const XPRBvar& v) { expr=xbnewexpr(1, v.getCRef(),NULL); }
  XPRBexpr(struct Xbexpr *l) {expr=l;}
  XPRBexpr(const XPRBterm& t) { expr=xbnewexpr(t.coeff, t.varRef,NULL); }
  XPRBexpr(const XPRBterm& t1,const XPRBterm& t2) { expr=xbnewexpr(t1.coeff*t2.coeff, t1.varRef,t2.varRef); }
  XPRBexpr(const XPRBexpr& l) { xbdupexpr(&expr,l.expr);}
  ~XPRBexpr() { xbdelexpr(expr); }

  void reset() { xbclearexpr(expr); }
  double getSol() const { return xbevalexpr(expr); } 
  XPRBexpr& neg() { xbchsexpr(expr); return *this; }
  static XPRBexpr neg(const XPRBexpr& l) { return XPRBexpr(l).neg(); }
  XPRBexpr& add(const XPRBvar& v)
   { xbaddexpr_term(expr,v.getCRef(),NULL,1); return *this; }
  XPRBexpr& add(const XPRBterm& t)
   { xbaddexpr_term(expr,t.varRef,NULL,t.coeff); return *this; }
  XPRBexpr& add(const XPRBexpr& l)
   { xbaddexpr(expr,l.expr); return *this; }
  XPRBexpr& setTerm(const XPRBvar& var, double val) 
   { xbsetexpr_term(expr, var.getCRef(), NULL, val); return *this; }
  XPRBexpr& setTerm(double val, const XPRBvar& var) 
   { xbsetexpr_term(expr, var.getCRef(), NULL, val); return *this; }
  XPRBexpr& setTerm(double val) 
   { xbsetexpr_term(expr, NULL, NULL, val); return *this; }
  XPRBexpr& setTerm(const XPRBvar& var1, const XPRBvar& var2, double val) 
   { xbsetexpr_term(expr, var1.getCRef(), var2.getCRef(), val); return *this; }
  XPRBexpr& setTerm(double val, const XPRBvar& var1, const XPRBvar& var2) 
   { xbsetexpr_term(expr, var1.getCRef(), var2.getCRef(), val); return *this; }
  XPRBexpr& addTerm(const XPRBvar& var, double val) 
   { xbaddexpr_term(expr, var.getCRef(), NULL, val); return *this; }
  XPRBexpr& addTerm(double val, const XPRBvar& var) 
   { xbaddexpr_term(expr, var.getCRef(), NULL, val); return *this; }
  XPRBexpr& addTerm(const XPRBvar& var) 
   { xbaddexpr_term(expr, var.getCRef(), NULL, 1); return *this; }
  XPRBexpr& addTerm(double val) 
   { xbaddexpr_term(expr, NULL, NULL, val); return *this; }
  XPRBexpr& addTerm(const XPRBvar& var1, const XPRBvar& var2, double val) 
   { xbaddexpr_term(expr, var1.getCRef(), var2.getCRef(), val); return *this; }
  XPRBexpr& addTerm(double val, const XPRBvar& var1, const XPRBvar& var2) 
   { xbaddexpr_term(expr, var1.getCRef(), var2.getCRef(), val); return *this; }
  XPRBexpr& delTerm(const XPRBvar& var) 
   { xbsetexpr_term(expr, var.getCRef(), NULL,0); return *this; }
  XPRBexpr& delTerm(const XPRBvar& var1, const XPRBvar& var2) 
   { xbsetexpr_term(expr, var1.getCRef(), var2.getCRef(), 0); return *this; }
  XPRBexpr& mul(double d) { xbmulcexpr(expr,d); return *this; }
  XPRBexpr& mul(const XPRBexpr& l)
   {
    if(xbmulexpr(expr,l.expr))
    throw("Non-quadratic expression");
    else return *this;
   }

  static XPRBexpr add(const XPRBexpr& l1,const XPRBvar& v)
   { return XPRBexpr(l1).add(v); }
  static XPRBexpr add(const XPRBexpr& l1,const XPRBterm& t)
   { return XPRBexpr(l1).add(t); }
  static XPRBexpr add(const XPRBexpr& l1,const XPRBexpr& l2)
   { return XPRBexpr(l1).add(l2); }
  static XPRBexpr mul(const XPRBexpr& l,double d) { return XPRBexpr(l).mul(d);}
  static XPRBexpr mul(double d,const XPRBexpr& l) { return mul(l,d);}
  static XPRBexpr mul(const XPRBexpr& l1,const XPRBexpr& l2)
   { return XPRBexpr(l1).mul(l2); }

  XPRBexpr& assign(const XPRBexpr& l)
   { xbassignexpr(expr,l.expr); return *this;}
  int print() { return xbprintexpr(NULL,expr,0,0); }

  XPRBexpr operator - () const { return neg(*this); }
  XPRBexpr& operator += (const XPRBvar& v) { return add(v); }
  XPRBexpr& operator += (const XPRBterm& t) { return add(t); }
  XPRBexpr& operator += (const XPRBexpr& l) { return add(l); }
  XPRBexpr& operator -= (const XPRBvar& v)
   { xbaddexpr_term(expr,v.getCRef(),NULL,-1); return *this; }
  XPRBexpr& operator -= (const XPRBterm& t)
   { xbaddexpr_term(expr,t.varRef,NULL,-t.coeff); return *this; }
  XPRBexpr& operator -= (const XPRBexpr& l) { return add(neg(l)); }
  XPRBexpr& operator = (const XPRBexpr& l) { return assign(l); }

  friend class XPRBcut;
  friend class XPRBctr;
  friend class XPRBprob;
  friend class XPRBsos;
};

inline XPRBexpr operator + (const XPRBexpr& l1, const XPRBexpr& l2)
 { return XPRBexpr::add(l1,l2); }
inline XPRBexpr operator - (const XPRBexpr& l1, const XPRBexpr& l2)
 { return XPRBexpr::add(l1,XPRBexpr::neg(l2)); }
inline XPRBexpr operator * (const XPRBexpr& l, double d)
 { return XPRBexpr::mul(l,d); }
inline XPRBexpr operator * (double d, const XPRBexpr& l)
 { return XPRBexpr::mul(l,d); }
inline XPRBexpr operator + (const XPRBvar& v1, const XPRBvar& v2)
 { return XPRBexpr::add(v1,v2); }
inline XPRBexpr operator - (const XPRBvar& v1, const XPRBvar& v2)
 { return XPRBexpr::add(v1,XPRBexpr::neg(v2)); }

inline XPRBexpr operator + (const XPRBvar& v1, const XPRBterm& t)
 { return XPRBexpr(v1).add(t); }
inline XPRBexpr operator - (const XPRBvar& v1, const XPRBterm& t)
 { return XPRBexpr(v1).add(XPRBterm::neg(t)); }
inline XPRBexpr operator + (const XPRBterm& t,const XPRBvar& v1)
 { return XPRBexpr(v1).add(t); }
inline XPRBexpr operator - (const XPRBterm& t,const XPRBvar& v1)
 { return XPRBexpr::neg(v1).add(t); }
inline XPRBterm operator * (double d, const XPRBvar& v)
 { return XPRBterm(d,v); }
inline XPRBterm operator * (const XPRBvar& v, double d)
 { return XPRBterm(d,v); }
inline XPRBterm operator - (const XPRBvar& v)
 { return XPRBterm(-1,v); }
inline XPRBexpr operator + (const XPRBterm& t1, const XPRBterm& t2)
 { return XPRBexpr(t1).add(t2); }
inline XPRBexpr operator - (const XPRBterm& t1, const XPRBterm& t2)
 { return XPRBexpr(t1).add(XPRBterm::neg(t2)); }

inline XPRBexpr operator * (const XPRBvar& v1, const XPRBvar& v2)
 { return XPRBexpr(1,v1,v2); }

inline XPRBexpr operator * (const XPRBexpr& l, const XPRBvar& v)
 { return XPRBexpr(l).mul(XPRBexpr(v)); }
inline XPRBexpr operator * (const XPRBvar& v, const XPRBexpr& l)
 { return XPRBexpr(l).mul(XPRBexpr(v)); }

inline XPRBexpr operator * (const XPRBexpr& q1, const XPRBexpr& q2)
 { return XPRBexpr(q1).mul(q2); }

inline XPRBexpr sqr(const XPRBexpr& l) { return XPRBexpr(l).mul(l); }
inline XPRBexpr sqr(const XPRBterm& t) { return XPRBexpr(t,t); }
inline XPRBexpr sqr(const XPRBvar& v) { return XPRBexpr(1,v,v); }
inline double sqr(double r) { return r*r; }
inline int sqr(int i) { return i*i; }

class XPRBrelation: public XPRBexpr
{
 private:
  int qrtype;
 public:
  XPRBrelation(const XPRBexpr& l,int t=XB_N):XPRBexpr(l) {qrtype=t;}
  XPRBrelation(const XPRBvar& v):XPRBexpr(v) {qrtype=XB_N;}
  friend class XPRBctr;
  friend class XPRBcut;
  int getType() const { return qrtype; }
  int print() { return xbprintexpr(NULL,expr,qrtype,0); }
};

inline XPRBrelation operator <= (const XPRBexpr& l1, const XPRBexpr& l2)
 { return XPRBrelation(l1-l2,XB_L); }
inline XPRBrelation operator >= (const XPRBexpr& l1, const XPRBexpr& l2)
 { return XPRBrelation(l1-l2,XB_G); }
inline XPRBrelation operator == (const XPRBexpr& l1, const XPRBexpr& l2)
 { return XPRBrelation(l1-l2,XB_E); }

class XPRBctr
{
 private:
  struct Xbctr *ctrRef;
 public:
  XPRBctr() { ctrRef=NULL; }
  XPRBctr(struct Xbctr *c) { ctrRef=c; }
  XPRBctr(struct Xbctr *c, const XPRBrelation& ctr)
  { ctrRef=c; assign(ctr); }
  struct Xbctr *getCRef() const { return ctrRef; }
  bool isValid() const { return (ctrRef!=NULL); }

  void assign(const XPRBrelation& ctr)
  { xbsetctr(ctrRef, ctr.expr, ctr.qrtype); }
  void reset() { ctrRef=NULL; }
  int print() { return xbprintctr(ctrRef); }

  int setType(int type) 
   { return xbsetctrtype(ctrRef, type); }
  int setRange(double low, double up)
   { return xbsetrange(ctrRef, low, up); }
  int setModCut(bool m) { return xbsetmodcut(ctrRef,m?1:0); }
  bool isModCut() { return (xbgetmodcut(ctrRef)!=0); }
  const char *getName() const { return xbgetctrname(ctrRef); }
  int getRange(double *lw, double *up) const
   { return xbgetrange(ctrRef, lw, up); }
  double getRangeL() const
   {
    double lw;
    xbgetrange(ctrRef, &lw, NULL);
    return lw; 
   }
  double getRangeU() const
   {
    double up;
    xbgetrange(ctrRef, NULL, &up);
    return up;
   }
  double getRHS() const { return xbgetrhs(ctrRef); }
  int getRowNum() const { return xbgetrownum(ctrRef); }
  int getType() const { return xbgetctrtype(ctrRef); }
  double getSlack() const { return xbgetslack(ctrRef); }
  double getAct() const { return xbgetact(ctrRef); }
  double getDual() const { return xbgetdual(ctrRef); }
  double getRNG(int rngtype) const { return xbgetctrrng(ctrRef,rngtype); }
  int setTerm(const XPRBvar& var, double val) 
   { return xbsetterm(ctrRef, var.getCRef(), val); }
  int setTerm(double val, const XPRBvar& var) 
   { return xbsetterm(ctrRef, var.getCRef(), val); }
  int setTerm(const XPRBvar& var) 
   { return xbsetterm(ctrRef, var.getCRef(), 1); }
  int setTerm(double val) 
   { return xbsetterm(ctrRef, NULL, val); }
  int setTerm(double val, const XPRBvar& var1, const XPRBvar& var2) 
   { return xbsetqterm(ctrRef, var1.getCRef(), var2.getCRef(), val); }
  int setTerm(const XPRBvar& var1, const XPRBvar& var2, double val) 
   { return xbsetqterm(ctrRef, var1.getCRef(), var2.getCRef(), val); }
  int addTerm(const XPRBvar& var, double val) 
   { return xbaddterm(ctrRef, var.getCRef(), val); }
  int addTerm(double val, const XPRBvar& var) 
   { return xbaddterm(ctrRef, var.getCRef(), val); }
  int addTerm(const XPRBvar& var) 
   { return xbaddterm(ctrRef, var.getCRef(), 1); }
  int addTerm(double val) 
   { return xbaddterm(ctrRef, NULL, val); }
  int addTerm(double val, const XPRBvar& var1, const XPRBvar& var2) 
   { return xbaddqterm(ctrRef, var1.getCRef(), var2.getCRef(), val); }
  int addTerm(const XPRBvar& var1, const XPRBvar& var2, double val) 
   { return xbaddqterm(ctrRef, var1.getCRef(), var2.getCRef(), val); }
  int addTerm(const XPRBvar& var1, const XPRBvar& var2) 
   { return xbaddqterm(ctrRef, var1.getCRef(), var2.getCRef(), 1); }
  int delTerm(const XPRBvar& var) 
   { return xbdelterm(ctrRef, var.getCRef()); }
  int delTerm(const XPRBvar& var1, const XPRBvar& var2) 
   { return xbdelqterm(ctrRef, var1.getCRef(), var2.getCRef()); }
  void add(const XPRBexpr& l)
   { xbappctr(ctrRef, l.expr); }

  void operator = ( const XPRBrelation& ctr)
   { assign(ctr); }
  void operator += ( const XPRBexpr& l)
   { add(l); }
  void operator -= ( const XPRBexpr& l)
   { add(XPRBexpr::neg(l)); }
};

class XPRBcut
{
 private:
  struct Xbcut *cutRef;
 public:
  XPRBcut() { cutRef=NULL; }
  XPRBcut(struct Xbcut *c) { cutRef=c; }
  XPRBcut(struct Xbcut *c, const XPRBrelation& cut)
  { cutRef=c; assign(cut); }
  struct Xbcut *getCRef() const { return cutRef; }
  bool isValid() const { return (cutRef!=NULL); }

  void assign(const XPRBrelation& cut)
  { xbsetcut(cutRef, cut.expr, cut.qrtype); }
  void reset() { cutRef=NULL; }
  int print() { return xbprintcut(cutRef); }

  double getRHS() const { return xbgetcutrhs(cutRef); }
  int setType(int type) 
   { return xbsetcuttype(cutRef, type); }
  int getType() const { return xbgetcuttype(cutRef); }
  int setID(int type) 
   { return xbsetcutid(cutRef, type); }
  int getID() const { return xbgetcutid(cutRef); }
  int setTerm(const XPRBvar& var, double val) 
   { return xbsetcutterm(cutRef, var.getCRef(), val); }
  int setTerm(double val, const XPRBvar& var) 
   { return xbsetcutterm(cutRef, var.getCRef(), val); }
  int setTerm(const XPRBvar& var) 
   { return xbsetcutterm(cutRef, var.getCRef(), 1); }
  int setTerm(double val) 
   { return xbsetcutterm(cutRef, NULL, val); }
  int addTerm(const XPRBvar& var, double val) 
   { return xbaddcutterm(cutRef, var.getCRef(), val); }
  int addTerm(double val, const XPRBvar& var) 
   { return xbaddcutterm(cutRef, var.getCRef(), val); }
  int addTerm(const XPRBvar& var) 
   { return xbaddcutterm(cutRef, var.getCRef(), 1); }
  int addTerm(double val) 
   { return xbaddcutterm(cutRef, NULL, val); }
  int delTerm(const XPRBvar& var) 
   { return xbdelcutterm(cutRef, var.getCRef()); }
  void add(const XPRBexpr& l)
   { xbappcut(cutRef, l.expr); }

  void operator = ( const XPRBrelation& cut)
   { assign(cut); }
  void operator += ( const XPRBexpr& l)
   { add(l); }
  void operator -= ( const XPRBexpr& l)
   { add(XPRBexpr::neg(l)); }
};

class XPRBsos
{
 private:
  struct Xbsos *sosRef;
 public:
  XPRBsos() { sosRef=NULL; }
  XPRBsos(struct Xbsos *s) { sosRef=s; }
  XPRBsos(struct Xbsos *s, const XPRBexpr& l)
  { sosRef=s; assign(l); }

  void assign(const XPRBexpr& l)
   { xbsetsos(sosRef, l.expr); }
  struct Xbsos *getCRef() const { return sosRef; }
  bool isValid() const { return (sosRef!=NULL); }
  void reset() { sosRef=NULL; }
  int print() { return xbprintsos(sosRef); }

  const char *getName() const { return xbgetsosname(sosRef); }
  int getType() const { return xbgetsostype(sosRef); }
  int setDir(int type, double val)
   { return xbsetsosdir(sosRef, type, val); }
  int setDir(int type)
   { return xbsetsosdir(sosRef, type, 0); }
  int addElement(XPRBvar& var, double val) 
   { return xbaddsosel(sosRef, var.getCRef(), val); }
  int addElement(double val, XPRBvar& var) 
   { return xbaddsosel(sosRef, var.getCRef(), val); }
  int delElement(XPRBvar& var) 
   { return xbdelsosel(sosRef, var.getCRef()); }
  void add(const XPRBexpr& l)
   { xbappsos(sosRef, l.expr); }

  void operator = ( const XPRBexpr& l) { assign(l); }
  void operator += ( const XPRBexpr& l) { add(l); }
};

class XPRBindexSet
{
 private:
  struct Xbidxset *idxRef;
 public:
  XPRBindexSet() {idxRef=NULL; }
  XPRBindexSet(struct Xbidxset *i) { idxRef=i; }
  struct Xbidxset *getCRef() const { return idxRef; }
  bool isValid() const { return (idxRef!=NULL); }
  void reset() { idxRef=NULL; }
  int print() const { return xbprintidxset(idxRef); }

  const char *getName() const { return xbgetidxsetname(idxRef); }
  int getSize() const { return xbgetidxsetsize(idxRef); }

  int addElement(const char *text) 
   { return xbaddidxel(idxRef, text);}
  int getIndex(const char *text) const 
   { return xbgetidxel(idxRef, text); }
  const char *getIndexName(int i) const 
   { return xbgetidxelname(idxRef, i); }
  
  int operator += ( const char *text)
  { return addElement(text); }
  int operator[] (const char *text) const
  { return getIndex(text); }
  const char * operator[] (int i) const
  { return getIndexName(i); }
};

class XPRBbasis
{
 private:
  struct Xbbasis *basRef;
 public:
  XPRBbasis() { basRef=NULL; }
  XPRBbasis(struct Xbbasis *b) { basRef=b; }
  XPRBbasis(const XPRBbasis& b) 
   { //if(basRef!=NULL) xbdelbasis(basRef); //Windows-problem?
     basRef=b.basRef; }
  struct Xbbasis *getCRef() const { return basRef; }
  bool isValid() const { return (basRef!=NULL); }
  void reset() { if(basRef!=NULL) xbdelbasis(basRef); 
                 basRef=NULL; }
};

class XPRBprob
{
 private:
  struct Xbprob *prob_ref;
  void operator = (const struct Xbprob&);
  XPRBprob(const XPRBprob&);
 public:
  XPRBprob(const char *name=NULL) { prob_ref=xbnewprob(name); }
  ~XPRBprob() { xbdelprob_prob(prob_ref); }
  struct Xbprob *getCRef() const { return prob_ref; }
  int reset() { return xbresetprob_prob(prob_ref); }
  int print() { return xbprintprob_prob(prob_ref); }
  int printObj() { return xbprintobj_prob(prob_ref); }
  int setDictionarySize(int dict,int size)
   { return xbsetdictsize_prob(prob_ref,dict,size); }
  int setMsgLevel(int lev) { return xbsetmsglevel_prob(prob_ref,lev); }
  int setRealFmt(const char *fmt) { return xbsetrealfmt_prob(prob_ref,fmt); }
  int setColOrder(int num) { return xbsetcolorder_prob(prob_ref,num); }
  struct xo_prob_struct * getXPRSprob() { return xbgetsprob_prob(prob_ref); }

  XPRBvar newVar(const char *name=NULL,int type=XB_PL, double lob=0, double upb=XB_INFINITY)
   { return XPRBvar(xbnewvar_prob(prob_ref,type,name,lob,upb)); }

  XPRBctr newCtr(const char *name,const XPRBrelation& ac)
   { return XPRBctr(xbnewctr_prob(prob_ref,name,XB_N),ac); }
  XPRBctr newCtr(const char *name=NULL)
   { return XPRBctr(xbnewctr_prob(prob_ref,name,XB_N)); }
  XPRBctr newCtr(const XPRBrelation& ac)
   { return XPRBctr(xbnewctr_prob(prob_ref,NULL,XB_N),ac); }
  void delCtr(XPRBctr& ctr) { xbdelctr(ctr.getCRef()); ctr.reset(); }

  int setObj(double val) { return xbsetobjexpr(prob_ref, XPRBexpr(val).expr); }
  int setObj(XPRBctr ctr) { return xbsetobj_prob(prob_ref, ctr.getCRef()); }
  int setObj(const XPRBexpr& q) 
   { return xbsetobjexpr(prob_ref, q.expr); }

  XPRBsos newSos(int type)
   { return XPRBsos(xbnewsos_prob(prob_ref,NULL,type)); }
  XPRBsos newSos(const char *name=NULL, int type=XB_S1)
   { return XPRBsos(xbnewsos_prob(prob_ref,name,type)); }
  XPRBsos newSos(int type, const XPRBexpr& le)
   { return XPRBsos(xbnewsos_prob(prob_ref,NULL,type), le); }
  XPRBsos newSos(const char *name, int type, const XPRBexpr& le)
   { return XPRBsos(xbnewsos_prob(prob_ref,name,type), le); }
  void delSos(XPRBsos& sos) { xbdelsos(sos.getCRef()); sos.reset(); }

  XPRBindexSet newIndexSet(const char *name=NULL,int maxsize=0)
   { return XPRBindexSet(xbnewidxset_prob(prob_ref,name,maxsize)); }

  XPRBbasis saveBasis() { return XPRBbasis(xbsavebasis_prob(prob_ref)); }
  int loadBasis(const XPRBbasis& b) { return xbloadbasis(b.getCRef()); }
  
  void clearDir() { xbcleardir_prob(prob_ref); }
  int setSense(int dir) { return xbsetsense_prob(prob_ref,dir); }
  int getSense() const { return xbgetsense_prob(prob_ref); }
  const char *getName() { return xbgetprobname_prob(prob_ref); }
  int exportProb(int format=XB_LP, const char *filename=NULL) 
  { return xbexportprob_prob(prob_ref, format, filename); }
  int writeDir(const char *filename=NULL) 
  { return xbwritedir_prob(prob_ref, filename); }
  int loadMat() { return xbloadmat_prob(prob_ref); }

  int solve(const char *alg="") { return xbsolve_prob(prob_ref, alg); }
  int minim(const char *alg="") { return xbminim_prob(prob_ref, alg); }
  int maxim(const char *alg="") { return xbmaxim_prob(prob_ref, alg); }
  int getProbStat() { return xbgetprobstat_prob(prob_ref); }
  int getLPStat() { return xbgetlpstat_prob(prob_ref); }
  int getMIPStat() { return xbgetmipstat_prob(prob_ref); }
  int sync(int synctype) { return xbsync_prob(prob_ref, synctype); }
  double getObjVal() { return xbgetobjval_prob(prob_ref); }
  int getNumIIS() { return xbgetnumiis_prob(prob_ref); }
  
  XPRBvar getVarByName(const char *name)
  { return XPRBvar((struct Xbvar *)xbgetbyname_prob(prob_ref, name, XB_VAR)); }
  XPRBctr getCtrByName(const char *name)
  { return XPRBctr((struct Xbctr *)xbgetbyname_prob(prob_ref, name, XB_CTR)); }
  XPRBsos getSosByName(const char *name)
  { return XPRBsos((struct Xbsos *)xbgetbyname_prob(prob_ref, name, XB_SOS)); }
  XPRBindexSet getIndexSetByName(const char *name)
  { return XPRBindexSet((struct Xbidxset *)xbgetbyname_prob(prob_ref, name, 
  XB_IDX)); }

  XPRBcut newCut(int id=0)
   { return XPRBcut(xbnewcut_prob(prob_ref,XB_E,id)); }
  XPRBcut newCut(const XPRBrelation& ac, int id=0)
   { return XPRBcut(xbnewcut_prob(prob_ref,ac.getType(),id),ac); }
  void delCut(XPRBcut& cut) { xbdelcut(cut.getCRef()); cut.reset(); }
  int setCutMode(int mode) { return xbsetcutmode_prob(prob_ref, mode); }
  int addCuts(XPRBcut *cuts, int num) 
  { struct Xbcut **ccuts=new struct Xbcut*[num];
    int rts;
    for(int i=0;i<num;i++) ccuts[i]=cuts[i].getCRef();
    rts=xbaddcuts_prob(prob_ref, ccuts, num); 
    delete []ccuts;
    return rts;}
  };
  
} // end namespace dashoptimization
#endif
