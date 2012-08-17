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
    virtual void visit(Program&) {}
    virtual void visit(Class&) {}
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
    virtual void visit(Div&) {}
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
public:
    enum display_option {
        DISPLAYALL,
        DISPLAYBASIC,
        DISPLAYNONBASIC
    };

    AstNodeDisplayer(std::ostream&, display_option = DISPLAYALL);

    void visit(Program&);
    void visit(Class&);
    void visit(Attribute&);
    void visit(Formal&);
    void visit(Method&);
    void visit(StringConst&);
    void visit(IntConst&);
    void visit(BoolConst&);
    void visit(New&);
    void visit(IsVoid&);
    void visit(CaseBranch&);
    void visit(Assign&);
    void visit(Block&);
    void visit(If&);
    void visit(While&);
    void visit(Complement&);
    void visit(LessThan&);
    void visit(EqualTo&);
    void visit(LessThanEqualTo&);
    void visit(Plus&);
    void visit(Sub&);
    void visit(Mul&);
    void visit(Div&);
    void visit(Not&);
    void visit(StaticDispatch&);
    void visit(DynamicDispatch&);
    void visit(Let&);
    void visit(Case&);
    void visit(Object&);
    void visit(NoExpr&);

private:
    std::ostream& os; 
    size_t depth; //Used to keep track of the AST depth of the visitor for proper indentation
    display_option opt;
};

#endif
