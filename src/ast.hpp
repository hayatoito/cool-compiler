#ifndef AST_H
#define AST_H

#include "symboltable.hpp"
#include "semanticanalyzer.hpp"
#include "astnodevisitor.hpp"
#include <vector>
#include <memory>

class AstNodeVisitor;
class SemanticAnalyzer;
struct Environment;

class AstNode
{
public:
    AstNode() {}
};

class Expression : public AstNode
{
public:
    Expression() {}
    virtual void accept(AstNodeVisitor&) const = 0;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const = 0;
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
    virtual void accept(AstNodeVisitor&) const = 0;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const = 0;
};

class Attribute : public Feature
{
public:
    Symbol name;
    Symbol type_decl;
    std::shared_ptr<Expression> init;

    Attribute(const Symbol&, const Symbol&, const std::shared_ptr<Expression>&);
    feature_type get_type() const;
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Formal : public AstNode
{
public:
    Symbol name;
    Symbol type_decl;

    Formal(const Symbol&, const Symbol&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
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
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
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
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Program : public AstNode
{
public:
    std::vector<std::shared_ptr<Class>> classes;

    Program(const std::vector<std::shared_ptr<Class>>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class StringConst : public Expression
{
public:
    Symbol token;

    StringConst(const Symbol&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class IntConst : public Expression
{
public:
    Symbol token;

    IntConst(const Symbol&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class BoolConst : public Expression
{
public:
    bool value;

    BoolConst(bool);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class New : public Expression
{
public:
    Symbol type;

    New(const Symbol&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class IsVoid : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    IsVoid(const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class CaseBranch : public Expression
{
public:
    Symbol name;
    Symbol type_decl;
    std::shared_ptr<Expression> expr;

    CaseBranch(const Symbol&, const Symbol&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Assign : public Expression
{
public:
    Symbol name;
    std::shared_ptr<Expression> rhs;

    Assign(const Symbol&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Block : public Expression
{
public:
    std::vector<std::shared_ptr<Expression>> body;

    Block(const std::vector<std::shared_ptr<Expression>>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class If : public Expression
{
public:
    std::shared_ptr<Expression> predicate;
    std::shared_ptr<Expression> iftrue;
    std::shared_ptr<Expression> iffalse;

    If(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class While : public Expression
{
public:
    std::shared_ptr<Expression> predicate;
    std::shared_ptr<Expression> body;

    While(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Complement : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    Complement(const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class LessThan : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    LessThan(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class EqualTo : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    EqualTo(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class LessThanEqualTo : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    LessThanEqualTo(const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Plus : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Plus(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Sub : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Sub(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Mul : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Mul(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Div : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Div(const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Not : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    Not(const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class StaticDispatch : public Expression
{
public:
    std::shared_ptr<Expression> obj;
    Symbol type;
    Symbol method;
    std::vector<std::shared_ptr<Expression>> actual;

    StaticDispatch(const std::shared_ptr<Expression>&, const Symbol&, const Symbol&,
           const std::vector<std::shared_ptr<Expression>>&); 
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class DynamicDispatch : public Expression
{
public:
    std::shared_ptr<Expression> obj;
    Symbol method;
    std::vector<std::shared_ptr<Expression>> actual;

    DynamicDispatch(const std::shared_ptr<Expression>&, const Symbol&, 
            const std::vector<std::shared_ptr<Expression>>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
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
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Case : public Expression
{
public:
    std::shared_ptr<Expression> expr;
    std::vector<std::shared_ptr<CaseBranch>> branches;

    Case(const std::shared_ptr<Expression>&, const std::vector<std::shared_ptr<CaseBranch>>&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class Object : public Expression
{
public:
    Symbol name;

    Object(const Symbol&);
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

class NoExpr : public Expression
{
public:
    NoExpr();
    void accept(AstNodeVisitor&) const;
    virtual Symbol type_check(SemanticAnalyzer&, Environment&) const;
};

typedef std::vector<std::shared_ptr<Class>> Classes;
typedef std::vector<std::shared_ptr<Formal>> Formals;
typedef std::vector<std::shared_ptr<CaseBranch>> Cases;
typedef std::vector<std::shared_ptr<Feature>> Features;
typedef std::vector<std::shared_ptr<Expression>> Expressions;

#endif
