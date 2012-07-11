#ifndef AST_H
#define AST_H

#include "symboltable.hpp"
#include <vector>

class AstNode
{

};

class Expression : AstNode
{

};

class Attribute : AstNode
{
public:
    Symbol name;
    Symbol type_decl;
    Expression init;

    Attribute(const Symbol&, const Symbol&, const Expression&);
};

class Formal : AstNode
{
public:
    Symbol name;
    Symbol type_decl;

    Formal(const Symbol&, const Symbol&);
};


class Method : AstNode
{
public:
    Symbol name;
    Symbol return_type;
    std::vector<Formal> params;
    Expression body;

    Method(const Symbol&, const Symbol&, const std::vector<Formal>&,
            const Expression&);
};

class Class : AstNode
{
public:
    Symbol name;
    Symbol parent;
    Symbol filename;
    std::vector<Attribute> attributes;
    std::vector<Method> methods;

    Class(const Symbol&, const Symbol&, const Symbol&, 
            const std::vector<Attribute>&, const std::vector<Method>&);
};

class Program : AstNode
{
public:
    std::vector<Class> classes;

    Program(const std::vector<Class>&);
};

class StringConst : Expression
{
public:
    Symbol token;

    StringConst(const Symbol&);
};

class IntConst : Expression
{
public:
    Symbol token;

    IntConst(const Symbol&);
};

class BoolConst : Expression
{
public:
    bool value;

    BoolConst(bool);
};

class New : Expression
{
public:
    Symbol type;

    New(const Symbol&);
};

class IsVoid : Expression
{
public:
    Expression expr;

    IsVoid(const Expression&);
};

class CaseBranch : Expression
{
public:
    Symbol name;
    Symbol type_decl;
    Expression expr;

    CaseBranch(const Symbol&, const Symbol&, const Expression&);
};

class Assign : Expression
{
public:
    Symbol name;
    Expression rhs;

    Assign(const Symbol&, const Expression&);
};

class Block : Expression
{
public:
    std::vector<Expression> body;

    Block(const std::vector<Expression>&);
};

class If : Expression
{
public:
    Expression predicate;
    Expression iftrue;
    Expression iffalse;

    If(const Expression&, const Expression&, const Expression&);
};

class While : Expression
{
public:
    Expression predicate;
    Expression body;

    While(const Expression&, const Expression&);
};

class Complement : Expression
{
public:
    Expression expr;

    Complement(const Expression&);
};

class LessThan : Expression
{
public:
    Expression lhs;
    Expression rhs;

    LessThan(const Expression&, const Expression&);
};

class EqualTo : Expression
{
public:
    Expression lhs;
    Expression rhs;

    EqualTo(const Expression&, const Expression&);
};

class LessThanEqualTo : Expression
{
public:
    Expression lhs;
    Expression rhs;

    LessThanEqualTo(const Expression&, const Expression&);
};

class Plus : Expression
{
public:
    Expression lhs;
    Expression rhs;

    Plus(const Expression&, const Expression&);
};

class Sub : Expression
{
public:
    Expression lhs;
    Expression rhs;

    Sub(const Expression&, const Expression&);
};

class Mul : Expression
{
public:
    Expression lhs;
    Expression rhs;

    Mul(const Expression&, const Expression&);
};

class Div : Expression
{
public:
    Expression lhs;
    Expression rhs;

    Div(const Expression&, const Expression&);
};

class Not : Expression
{
public:
    Expression expr;

    Not(const Expression&);
};

class StaticDispatch : Expression
{
public:
    Expression obj;
    Symbol type;
    Symbol method;
    Expression actual;

    StaticDispatch(const Expression&, const Symbol&, const Symbol&,
           const Expression&); 
};

class DynamicDispatch : Expression
{
public:
    Expression obj;
    Symbol method;
    Expression actual;

    DynamicDispatch(const Expression&, const Symbol&, const Expression&);
};

class Let : Expression
{
public:
    Symbol name;
    Symbol type_decl;
    Expression init;
    Expression body;

    Let(const Symbol&, const Symbol&, const Expression&,
            const Expression&);
};

class Case : Expression
{
public:
    Expression expr;
    std::vector<CaseBranch> branches;

    Case(const Expression&, const std::vector<CaseBranch>&);
};

class Object : Expression
{
public:
    Symbol name;

    Object(const Symbol&);
};

class NoExpr : Expression
{

};

#endif
