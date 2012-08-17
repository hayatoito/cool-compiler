#ifndef AST_H
#define AST_H

#include "symboltable.hpp"
#include "astnodevisitor.hpp"
#include <vector>
#include <memory>

class AstNodeVisitor;

class AstNode
{
public:
    std::size_t line_no;
    std::string filename;

    AstNode() {}
    void setloc(std::size_t, const std::string&);
};
typedef std::shared_ptr<AstNode> AstNodePtr;

class Expression : public AstNode
{
public:
    Symbol type;

    Expression() {}
    virtual void accept(AstNodeVisitor&) = 0;
};
typedef std::shared_ptr<Expression> ExpressionPtr;
typedef std::vector<ExpressionPtr> Expressions;

class Feature : public AstNode
{
public:
    enum feature_type
    {
        ATTRIBUTE,
        METHOD
    };

    virtual feature_type get_type() const = 0;
    virtual void accept(AstNodeVisitor&) = 0;
};
typedef std::shared_ptr<Feature> FeaturePtr;
typedef std::vector<FeaturePtr> Features;

class Attribute : public Feature
{
public:
    Symbol name;
    Symbol type_decl;
    ExpressionPtr init;

    Attribute(const Symbol&, const Symbol&, const ExpressionPtr&);
    feature_type get_type() const;
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Attribute> AttributePtr;

class Formal : public AstNode
{
public:
    Symbol name;
    Symbol type_decl;

    Formal(const Symbol&, const Symbol&);
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Formal> FormalPtr;
typedef std::vector<FormalPtr> Formals;

class Method : public Feature
{
public:
    Symbol name;
    Symbol return_type;
    Formals params;
    ExpressionPtr body;

    Method(const Symbol&, const Symbol&, const Formals&,
            const ExpressionPtr&);
    feature_type get_type() const;
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Method> MethodPtr;

class Class : public AstNode
{
public:
    Symbol name;
    Symbol parent;
    Features features;

    Class(const Symbol&, const Symbol&, const Features&); 
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Class> ClassPtr;
typedef std::vector<ClassPtr> Classes;

class Program : public AstNode
{
public:
    Classes classes;

    Program(const Classes&);
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Program> ProgramPtr;

class StringConst : public Expression
{
public:
    Symbol token;

    StringConst(const Symbol&);
    void accept(AstNodeVisitor&);
};

class IntConst : public Expression
{
public:
    Symbol token;

    IntConst(const Symbol&);
    void accept(AstNodeVisitor&);
};

class BoolConst : public Expression
{
public:
    bool value;

    BoolConst(bool);
    void accept(AstNodeVisitor&);
};

class New : public Expression
{
public:
    Symbol type_decl;

    New(const Symbol&);
    void accept(AstNodeVisitor&);
};

class IsVoid : public Expression
{
public:
    ExpressionPtr expr;

    IsVoid(const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class CaseBranch : public Expression
{
public:
    Symbol name;
    Symbol type_decl;
    ExpressionPtr expr;

    CaseBranch(const Symbol&, const Symbol&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<CaseBranch> CaseBranchPtr;
typedef std::vector<CaseBranchPtr> Cases;

class Assign : public Expression
{
public:
    Symbol name;
    ExpressionPtr rhs;

    Assign(const Symbol&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Block : public Expression
{
public:
    Expressions body;

    Block(const Expressions&);
    void accept(AstNodeVisitor&);
};

class If : public Expression
{
public:
    ExpressionPtr predicate;
    ExpressionPtr iftrue;
    ExpressionPtr iffalse;

    If(const ExpressionPtr&, const ExpressionPtr&, 
            const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class While : public Expression
{
public:
    ExpressionPtr predicate;
    ExpressionPtr body;

    While(const ExpressionPtr&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Complement : public Expression
{
public:
    ExpressionPtr expr;

    Complement(const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class LessThan : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    LessThan(const ExpressionPtr&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class EqualTo : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    EqualTo(const ExpressionPtr&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class LessThanEqualTo : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    LessThanEqualTo(const ExpressionPtr&, 
            const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Plus : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    Plus(const ExpressionPtr&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Sub : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    Sub(const ExpressionPtr&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Mul : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    Mul(const ExpressionPtr&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Div : public Expression
{
public:
    ExpressionPtr lhs;
    ExpressionPtr rhs;

    Div(const ExpressionPtr&, 
            const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Not : public Expression
{
public:
    ExpressionPtr expr;

    Not(const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class StaticDispatch : public Expression
{
public:
    ExpressionPtr obj;
    Symbol type_decl;
    Symbol method;
    Expressions actual;

    StaticDispatch(const ExpressionPtr&, const Symbol&, const Symbol&,
           const Expressions&); 
    void accept(AstNodeVisitor&);
};

class DynamicDispatch : public Expression
{
public:
    ExpressionPtr obj;
    Symbol method;
    Expressions actual;

    DynamicDispatch(const ExpressionPtr&, const Symbol&, 
            const Expressions&);
    void accept(AstNodeVisitor&);
};

class Let : public Expression
{
public:
    Symbol name;
    Symbol type_decl;
    ExpressionPtr init;
    ExpressionPtr body;

    Let(const Symbol&, const Symbol&, const ExpressionPtr&,
            const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};

class Case : public Expression
{
public:
    ExpressionPtr expr;
    Cases branches;

    Case(const ExpressionPtr&, const Cases&);
    void accept(AstNodeVisitor&);
};

class Object : public Expression
{
public:
    Symbol name;

    Object(const Symbol&);
    void accept(AstNodeVisitor&);
};

class NoExpr : public Expression
{
public:
    NoExpr();
    void accept(AstNodeVisitor&);
};


#endif
