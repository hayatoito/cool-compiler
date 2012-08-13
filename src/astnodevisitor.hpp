#ifndef ASTNODEVISITOR_H
#define ASTNODEVISITOR_H

#include "ast.hpp"
#include "tokentable.hpp"
#include <iostream>

//Forward declarations since there are 
//circular dependencies of the ast node classes
//and the visitors
class Program;
class Class;
class Feature;
class Attribute;
class Method;
class Formal;
class StringConst;
class IntConst;
class BoolConst;
class New;
class IsVoid;
class CaseBranch;
class Assign;
class Block;
class If;
class While;
class Complement;
class LessThan;
class EqualTo;
class LessThanEqualTo;
class Plus;
class Sub;
class Mul;
class Div;
class Not;
class StaticDispatch;
class DynamicDispatch;
class Let;
class Case;
class Object;
class NoExpr;

//Abstract base class for all the AST node visitors
//
//There are three kinds of visits in order to provide more flexibility
//in the traversal order. For example, a visitor that dumps the 
//AST node would need a pre-order traversal while a code generating
//visitor would need to do a post-order traversal of the AST
class AstNodeVisitor 
{
public:
    virtual void visit(const Program&) {}
    virtual void visit(const Class&) {}
    virtual void visit(const Feature&) {}
    virtual void visit(const Attribute&) {}
    virtual void visit(const Method&) {}
    virtual void visit(const Formal&) {}
    virtual void visit(const StringConst&) {}
    virtual void visit(const IntConst&) {}
    virtual void visit(const BoolConst&) {}
    virtual void visit(const New&) {}
    virtual void visit(const IsVoid&) {}
    virtual void visit(const CaseBranch&) {}
    virtual void visit(const Assign&) {}
    virtual void visit(const Block&) {}
    virtual void visit(const If&) {}
    virtual void visit(const While&) {}
    virtual void visit(const Complement&) {}
    virtual void visit(const LessThan&) {}
    virtual void visit(const EqualTo&) {}
    virtual void visit(const LessThanEqualTo&) {}
    virtual void visit(const Plus&) {}
    virtual void visit(const Sub&) {}
    virtual void visit(const Mul&) {}
    virtual void visit(const Div&) {}
    virtual void visit(const Not&) {}
    virtual void visit(const StaticDispatch&) {}
    virtual void visit(const DynamicDispatch&) {}
    virtual void visit(const Let&) {}
    virtual void visit(const Case&) {}
    virtual void visit(const Object&) {}
    virtual void visit(const NoExpr&) {}

    virtual void visit(Program&) {}
    virtual void visit(Class&) {}
    virtual void visit(Feature&) {}
    virtual void visit(Attribute&) {}
    virtual void visit(Method&) {}
    virtual void visit(Formal&) {}
    virtual void visit(StringConst&) {}
    virtual void visit(IntConst&) {}
    virtual void visit(BoolConst&) {}
    virtual void visit(New&) {}
    virtual void visit(IsVoid&) {}
    virtual void visit(CaseBranch&) {}
    virtual void visit(Assign&) {}
    virtual void visit(Block&) {}
    virtual void visit(If&) {}
    virtual void visit(While&) {}
    virtual void visit(Complement&) {}
    virtual void visit(LessThan&) {}
    virtual void visit(EqualTo&) {}
    virtual void visit(LessThanEqualTo&) {}
    virtual void visit(Plus&) {}
    virtual void visit(Sub&) {}
    virtual void visit(Mul&) {}
    virtual void visit(Not&) {}
    virtual void visit(StaticDispatch&) {}
    virtual void visit(DynamicDispatch&) {}
    virtual void visit(Let&) {}
    virtual void visit(Case&) {}
    virtual void visit(Object&) {}
    virtual void visit(NoExpr&) {}
};

//Visitor that dumps (pretty prints) the AST
class AstNodeDisplayer : public AstNodeVisitor
{
private:
    std::ostream& os; 
    size_t depth; //Used to keep track of the AST depth of the visitor for proper indentation

public:
    AstNodeDisplayer(std::ostream&);

    void visit(const Program&);
    void visit(const Class&);
    void visit(const Feature&);
    void visit(const Attribute&);
    void visit(const Formal&);
    void visit(const Method&);
    void visit(const StringConst&);
    void visit(const IntConst&);
    void visit(const BoolConst&);
    void visit(const New&);
    void visit(const IsVoid&);
    void visit(const CaseBranch&);
    void visit(const Assign&);
    void visit(const Block&);
    void visit(const If&);
    void visit(const While&);
    void visit(const Complement&);
    void visit(const LessThan&);
    void visit(const EqualTo&);
    void visit(const LessThanEqualTo&);
    void visit(const Plus&);
    void visit(const Sub&);
    void visit(const Mul&);
    void visit(const Div&);
    void visit(const Not&);
    void visit(const StaticDispatch&);
    void visit(const DynamicDispatch&);
    void visit(const Let&);
    void visit(const Case&);
    void visit(const Object&);
    void visit(const NoExpr&);
};

#endif
