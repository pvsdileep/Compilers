
#include "Address.h"
#include "Label.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

class Expr {
public:
  virtual ~Expr() {}
  virtual Address *genCode() = 0;
  virtual void genJmpCode(const Label& yesTarget, const Label& noTarget) = 0;
};

class Stmt {
public:
  virtual ~Stmt() {}
  virtual void genCode(const Label& next) = 0; // caller emits  next label
};

class BoolExpr : public Expr {
public:
  virtual Address *genCode();
};

class ArithExpr : public Expr {
public:
  virtual void genJmpCode(const Label& yesTarget, const Label& noTarget);
};

class OrExpr : public BoolExpr {
  Expr *left;
  Expr *right;
public:
  OrExpr(Expr *l, Expr *r) : left(l), right(r) {}
  virtual ~OrExpr() {delete left; delete right;}
  virtual void genJmpCode(const Label& yesTarget, const Label& noTarget);
};

class AndExpr : public BoolExpr {
  Expr *left;
  Expr *right;
public:
  AndExpr(Expr *l, Expr *r) : left(l), right(r) {}
  virtual ~AndExpr() {delete left; delete right;}
  virtual void genJmpCode(const Label& yesTarget, const Label& noTarget);
};

class NotExpr : public BoolExpr {
  Expr *expr;
public:
  NotExpr(Expr *e) : expr(e) {}
  virtual ~NotExpr() {delete expr;}
  virtual void genJmpCode(const Label& yesTarget, const Label& noTarget) {
    expr->genJmpCode(noTarget, yesTarget);
  }
};

class CmpExpr : public BoolExpr {
  std::string op;
  Expr *left;
  Expr *right;
public:
  CmpExpr(const std::string &o, Expr *l, Expr *r) 
    : op(o), left(l), right(r) {}
  virtual ~CmpExpr() {delete left; delete right;}
  virtual void genJmpCode(const Label& yesTarget, const Label& noTarget);
};

class BinaryArithExpr : public ArithExpr {
  std::string op;
  Expr *left;
  Expr *right;
public:
  BinaryArithExpr(const std::string& o, Expr *l, Expr *r)
    : op(o), left(l), right(r) {}
  virtual ~BinaryArithExpr() {delete left; delete right;}
  virtual Address *genCode();
};

class UnaryArithExpr : public ArithExpr {
  std::string op;
  Expr *expr;
public:
  UnaryArithExpr(const std::string& o, Expr *e) : op(o), expr(e) {}
  virtual ~UnaryArithExpr() {delete expr;}
  virtual Address *genCode();
};

class VarExpr : public ArithExpr {
  const std::string *name;
public:
  VarExpr(const std::string* n) : name(n) {}
  virtual ~VarExpr() {delete name;}
  virtual Address *genCode() {return new NameAddress(*name);}
};

class CallExpr : public ArithExpr {
  const std::string *name;
  std::vector<Expr*> *args;
public:
  CallExpr(const std::string* n, std::vector<Expr*> *a) : name(n), args(a) {}
  virtual ~CallExpr();
  virtual Address *genCode();
};

class ConstExpr : public ArithExpr {
  const float val;
public:
  ConstExpr(float v) : val(v) {}
  virtual Address *genCode() {return new ConstAddress(val);}
};

class ArrayLoadExpr : public ArithExpr {
  std::string *name;
  std::vector<Expr*> *indices;
public:
  ArrayLoadExpr(std::string *n, std::vector<Expr*> *i) : name(n), indices(i) {}
  virtual Address *genCode();
};

  
class AssignStmt : public Stmt {
  const std::string *name;
  Expr *expr;
public:
  AssignStmt(const std::string *n, Expr *e) : name(n), expr(e) {
	  //printf("calling constructor of assingstmt\n");
	  }
  virtual ~AssignStmt() {delete name; delete expr;
  //printf("deleting\n");
  }
  virtual void genCode(const Label& next);
};  

class CallStmt : public Stmt {
  const std::string *name;
  std::vector<Expr*> *args;
public:
  CallStmt(const std::string *n, std::vector<Expr*> *a) 
    : name(n), args(a) {}
  virtual ~CallStmt();
  virtual void genCode(const Label& next);
};
  
class BlockStmt : public Stmt {
  std::vector<Stmt*> *stmts;
public:
  BlockStmt(std::vector<Stmt*> *s) : stmts(s) {}
  virtual ~BlockStmt();
  virtual void genCode(const Label& next);
};

class IfStmt : public Stmt {
  Expr *cond;
  Stmt *body;
public:
  IfStmt(Expr *c, Stmt *s) : cond(c), body(s) {}
  virtual ~IfStmt() {delete cond; delete body;}
  virtual void genCode(const Label& next);
};  

class IfElseStmt : public Stmt {
  Expr *cond;
  Stmt *thenBody;
  Stmt *elseBody;
public:
  IfElseStmt(Expr *c, Stmt *t, Stmt *e) : cond(c), thenBody(t), elseBody(e) {}
  virtual ~IfElseStmt() {delete cond; delete thenBody; delete elseBody;}
  virtual void genCode(const Label& next);
};  

class WhileStmt : public Stmt {
  Expr *cond;
  Stmt *body;
public:
  WhileStmt(Expr *c, Stmt *s) : cond(c), body(s) {}
  virtual ~WhileStmt() {delete cond; delete body;}
  virtual void genCode(const Label& next);
};  


//for_stmt	 : FOR '(' assign_stmt ',' expr ',' assign_stmt ')' stmt {$$ = new ForStmt($3,$5,$7,$9);}

class ForStmt : public Stmt {
  Expr *cond;
  Stmt *body,*in_body,*first_arg;

public:
  ForStmt(Stmt *first,Expr *c, Stmt *in,Stmt *s) : cond(c),first_arg(first), in_body(in),body(s) {}
  virtual ~ForStmt() {delete cond; delete body;}
  virtual void genCode(const Label& next);
};  

