#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;
class Statement;
class Goal;
class Expr;
class Factor;           // 2, 5, a
class BinaryOp;         // a + b, 2 - a, a = 2 * (2 + a)
class typeDecl;         // type int a, b;

class ASTVisitor
{
public:
  virtual void visit(AST &){};
  virtual void visit(Goal &) = 0;
  // virtual void visit(Statement &){};
  virtual void visit(Expr &){};
  virtual void visit(Factor &) = 0;
  virtual void visit(BinaryOp &) = 0;
  virtual void visit(typeDecl &) = 0;
};

class AST
{
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;
};

class Statement : public AST
{
public:
  Statement() {}
};

class Goal : public AST
{
private:
  using StatementVector = llvm::SmallVector<Statement *, 8>;
  StatementVector Statements;

public:
  Goal(llvm::SmallVector<Statement *, 8> Statements)
      : Statements(Statements) {}
  StatementVector::const_iterator begin() { return Statements.begin(); }
  StatementVector::const_iterator end() { return Statements.end(); }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class Expr : public Statement
{
public:
  Expr() {}
};

class Factor : public Expr
{
public:
  enum ValueKind
  {
    Ident,
    Number
  };

private:
  ValueKind Kind;
  llvm::StringRef Val;

public:
  Factor(ValueKind Kind, llvm::StringRef Val)
      : Kind(Kind), Val(Val) {}
  ValueKind getKind() { return Kind; }
  llvm::StringRef getVal() { return Val; }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class BinaryOp : public Expr
{
public:
  enum Operator
  {
    Plus,
    Minus,
    Mul,
    Div,
    Equal
  };

private:
  Expr *Left;
  Expr *Right;
  Operator Op;

public:
  BinaryOp(Operator Op, Expr *L, Expr *R)
      : Op(Op), Left(L), Right(R) {}
  Expr *getLeft() { return Left; }
  Expr *getRight() { return Right; }
  Operator getOperator() { return Op; }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class typeDecl : public Statement
{
  using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
  VarVector Vars;

public:
  typeDecl(llvm::SmallVector<llvm::StringRef, 8> Vars)
      : Vars(Vars) {}
  VarVector::const_iterator begin() { return Vars.begin(); }
  VarVector::const_iterator end() { return Vars.end(); }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};
#endif
