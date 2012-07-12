#ifndef AST_H
#define AST_H

#include "symboltable.hpp"
#include <vector>
#include <memory>


class AstNode
{
public:
    AstNode() {}
};

class Expression : public AstNode
{
public:
    Expression() {}
};

class Feature : public AstNode
{
public:
    enum feature_type
    {
        ATTRIBUTE,
        METHOD
    };

    virtual feature_type get_type() const = 0;
};

class Attribute : public Feature
{
public:
    Symbol name;
    Symbol type_decl;
    std::shared_ptr<Expression> init;

    Attribute(const Symbol&, const Symbol&, const std::shared_ptr<Expression>&);
    feature_type get_type() const;
};

class Formal : public AstNode
{
public:
    Symbol name;
    Symbol type_decl;

    Formal(const Symbol&, const Symbol&);
};


class Method : public Feature
{
public:
    Symbol name;
    Symbol return_type;
    std::vector<std::shared_ptr<Formal>> params;
    std::shared_ptr<Expression> body;

    Method(const Symbol&, const Symbol&, const std::vector<std::shared_ptr<Formal>>&,
            const std::shared_ptr<Expression>&);
    feature_type get_type() const;
};

class Class : public AstNode
{
public:
    Symbol name;
    Symbol parent;
    Symbol filename;
    std::vector<std::shared_ptr<Feature>> features;

    Class(const Symbol&, const Symbol&, const Symbol&, 
           const std::vector<std::shared_ptr<Feature>>&); 
};

class Program : public AstNode
{
public:
    std::vector<std::shared_ptr<Class>> classes;

    Program(const std::vector<std::shared_ptr<Class>>&);
};

class StringConst : public Expression
{
public:
    Symbol token;

    StringConst(const Symbol&);
};

class IntConst : public Expression
{
public:
    Symbol token;

    IntConst(const Symbol&);
};

class BoolConst : public Expression
{
public:
    bool value;

    BoolConst(bool);
};

class New : public Expression
{
public:
    Symbol type;

    New(const Symbol&);
};

class IsVoid : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    IsVoid(const std::shared_ptr<Expression>&);
};

class CaseBranch : public Expression
{
public:
    Symbol name;
    Symbol type_decl;
    std::shared_ptr<Expression> expr;

    CaseBranch(const Symbol&, const Symbol&, const std::shared_ptr<Expression>&);
};

class Assign : public Expression
{
public:
    Symbol name;
    std::shared_ptr<Expression> rhs;

    Assign(const Symbol&, const std::shared_ptr<Expression>&);
};

class Block : public Expression
{
public:
    std::vector<std::shared_ptr<Expression>> body;

    Block(const std::vector<std::shared_ptr<Expression>>&);
};

class If : public Expression
{
public:
    std::shared_ptr<Expression> predicate;
    std::shared_ptr<Expression> iftrue;
    std::shared_ptr<Expression> iffalse;

    If(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
};

class While : public Expression
{
public:
    std::shared_ptr<Expression> predicate;
    std::shared_ptr<Expression> body;

    While(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
};

class Complement : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    Complement(const std::shared_ptr<Expression>&);
};

class LessThan : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    LessThan(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
};

class EqualTo : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    EqualTo(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
};

class LessThanEqualTo : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    LessThanEqualTo(const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
};

class Plus : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Plus(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
};

class Sub : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Sub(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
};

class Mul : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Mul(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
};

class Div : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Div(const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
};

class Not : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    Not(const std::shared_ptr<Expression>&);
};

class StaticDispatch : public Expression
{
public:
    std::shared_ptr<Expression> obj;
    Symbol type;
    Symbol method;
    std::shared_ptr<Expression> actual;

    StaticDispatch(const std::shared_ptr<Expression>&, const Symbol&, const Symbol&,
           const std::shared_ptr<Expression>&); 
};

class DynamicDispatch : public Expression
{
public:
    std::shared_ptr<Expression> obj;
    Symbol method;
    std::shared_ptr<Expression> actual;

    DynamicDispatch(const std::shared_ptr<Expression>&, const Symbol&, 
            const std::shared_ptr<Expression>&);
};

class Let : public Expression
{
public:
    Symbol name;
    Symbol type_decl;
    std::shared_ptr<Expression> init;
    std::shared_ptr<Expression> body;

    Let(const Symbol&, const Symbol&, const std::shared_ptr<Expression>&,
            const std::shared_ptr<Expression>&);
};

class Case : public Expression
{
public:
    std::shared_ptr<Expression> expr;
    std::vector<CaseBranch> branches;

    Case(const std::shared_ptr<Expression>&, const std::vector<CaseBranch>&);
};

class Object : public Expression
{
public:
    Symbol name;

    Object(const Symbol&);
};

class NoExpr : Expression
{

};

typedef std::vector<std::shared_ptr<Class>> Classes;
typedef std::vector<std::shared_ptr<Formal>> Formals;
typedef std::vector<std::shared_ptr<Feature>> Features;
typedef std::vector<std::shared_ptr<Expression>> Expressions;

#endif
