// This is where the AST of COOL is defined.
// The visitor pattern is used to perform operations on AST including
// type checking and code generation.

#ifndef AST_H
#define AST_H

#include "symboltable.hpp"
#include "astnodevisitor.hpp"

#include <vector>
#include <memory>

class AstNodeVisitor;

// Base class of all AST nodes
class AstNode
{
public:
    // All AST nodes will have a line number and filename which is the line number where
    // the node is found in the source file
    std::size_t line_no;
    std::string filename;

    AstNode() {}

    // Convinience mutator to be used by parser to set the locations for each node
    void setloc(std::size_t, const std::string&);
};
typedef std::shared_ptr<AstNode> AstNodePtr;

// Base class of COOL's expressions
class Expression : public AstNode
{
public:
    // Each COOL expression will have a type
    // This member is useful for type checking functionality
    // as well as to allow easier testing
    Symbol type;

    Expression() {}
    virtual void accept(AstNodeVisitor&) = 0;
};
typedef std::shared_ptr<Expression> ExpressionPtr;
typedef std::vector<ExpressionPtr> Expressions;

class Attribute : public AstNode
{
public:
    Symbol name;
    Symbol type_decl;
    ExpressionPtr init;

    Attribute(const Symbol&, const Symbol&, const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Attribute> AttributePtr;
typedef std::vector<AttributePtr> Attributes;

// A Formal in COOL is the same as a method parameter
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

class Method : public AstNode
{
public:
    Symbol name;
    Symbol return_type;
    Formals params;
    ExpressionPtr body;

    Method(const Symbol&, const Symbol&, const Formals&,
            const ExpressionPtr&);
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Method> MethodPtr;
typedef std::vector<MethodPtr> Methods;

class Class : public AstNode
{
public:
    Symbol name;
    Symbol parent;
    Attributes attributes;
    Methods methods;

    Class(const Symbol&, const Symbol&, const Attributes&, const Methods&);
    void accept(AstNodeVisitor&);
};
typedef std::shared_ptr<Class> ClassPtr;
typedef std::vector<ClassPtr> Classes;

// The root of the AST
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
