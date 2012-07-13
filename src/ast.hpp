#ifndef AST_H
#define AST_H

#include "symboltable.hpp"
#include <vector>
#include <memory>

class AstNodeVisitor;

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
};

class Formal : public AstNode
{
public:
    Symbol name;
    Symbol type_decl;

    Formal(const Symbol&, const Symbol&);
    void accept(AstNodeVisitor&) const;
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
};

class Program : public AstNode
{
public:
    std::vector<std::shared_ptr<Class>> classes;

    Program(const std::vector<std::shared_ptr<Class>>&);
    void accept(AstNodeVisitor&) const;
};

class StringConst : public Expression
{
public:
    Symbol token;

    StringConst(const Symbol&);
    void accept(AstNodeVisitor&) const;
};

class IntConst : public Expression
{
public:
    Symbol token;

    IntConst(const Symbol&);
    void accept(AstNodeVisitor&) const;
};

class BoolConst : public Expression
{
public:
    bool value;

    BoolConst(bool);
    void accept(AstNodeVisitor&) const;
};

class New : public Expression
{
public:
    Symbol type;

    New(const Symbol&);
    void accept(AstNodeVisitor&) const;
};

class IsVoid : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    IsVoid(const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class CaseBranch : public Expression
{
public:
    Symbol name;
    Symbol type_decl;
    std::shared_ptr<Expression> expr;

    CaseBranch(const Symbol&, const Symbol&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Assign : public Expression
{
public:
    Symbol name;
    std::shared_ptr<Expression> rhs;

    Assign(const Symbol&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Block : public Expression
{
public:
    std::vector<std::shared_ptr<Expression>> body;

    Block(const std::vector<std::shared_ptr<Expression>>&);
    void accept(AstNodeVisitor&) const;
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
};

class While : public Expression
{
public:
    std::shared_ptr<Expression> predicate;
    std::shared_ptr<Expression> body;

    While(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Complement : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    Complement(const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class LessThan : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    LessThan(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class EqualTo : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    EqualTo(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class LessThanEqualTo : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    LessThanEqualTo(const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Plus : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Plus(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Sub : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Sub(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Mul : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Mul(const std::shared_ptr<Expression>&, const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Div : public Expression
{
public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Div(const std::shared_ptr<Expression>&, 
            const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
};

class Not : public Expression
{
public:
    std::shared_ptr<Expression> expr;

    Not(const std::shared_ptr<Expression>&);
    void accept(AstNodeVisitor&) const;
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
};

class Case : public Expression
{
public:
    std::shared_ptr<Expression> expr;
    std::vector<std::shared_ptr<CaseBranch>> branches;

    Case(const std::shared_ptr<Expression>&, const std::vector<std::shared_ptr<CaseBranch>>&);
    void accept(AstNodeVisitor&) const;
};

class Object : public Expression
{
public:
    Symbol name;

    Object(const Symbol&);
    void accept(AstNodeVisitor&) const;
};

class NoExpr : public Expression
{
public:
    NoExpr();
    void accept(AstNodeVisitor&) const;
};


// Visitors for AST

class AstNodeVisitor 
{
public:
    virtual void visit(const Program&) = 0;
    virtual void visit(const Class&) = 0;
    virtual void visit(const Feature&) = 0;
    virtual void visit(const Attribute&) = 0;
    virtual void visit(const Method&) = 0;
    virtual void visit(const Formal&) = 0;
    virtual void visit(const StringConst&) = 0;
    virtual void visit(const IntConst&) = 0;
    virtual void visit(const BoolConst&) = 0;
    virtual void visit(const New&) = 0;
    virtual void visit(const IsVoid&) = 0;
    virtual void visit(const CaseBranch&) = 0;
    virtual void visit(const Assign&) = 0;
    virtual void visit(const Block&) = 0;
    virtual void visit(const If&) = 0;
    virtual void visit(const While&) = 0;
    virtual void visit(const Complement&) = 0;
    virtual void visit(const LessThan&) = 0;
    virtual void visit(const EqualTo&) = 0;
    virtual void visit(const LessThanEqualTo&) = 0;
    virtual void visit(const Plus&) = 0;
    virtual void visit(const Sub&) = 0;
    virtual void visit(const Mul&) = 0;
    virtual void visit(const Div&) = 0;
    virtual void visit(const Not&) = 0;
    virtual void visit(const StaticDispatch&) = 0;
    virtual void visit(const DynamicDispatch&) = 0;
    virtual void visit(const Let&) = 0;
    virtual void visit(const Case&) = 0;
    virtual void visit(const Object&) = 0;
    virtual void visit(const NoExpr&) = 0;

    virtual bool pre_visit(const Program&) { return false; } 
    virtual bool pre_visit(const Class&) { return false; } 
    virtual bool pre_visit(const Feature&) { return false; } 
    virtual bool pre_visit(const Attribute&) { return false; } 
    virtual bool pre_visit(const Formal&) { return false; } 
    virtual bool pre_visit(const Method&) { return false; } 
    virtual bool pre_visit(const StringConst&) { return false; } 
    virtual bool pre_visit(const IntConst&) { return false; } 
    virtual bool pre_visit(const BoolConst&) { return false; } 
    virtual bool pre_visit(const New&) { return false; } 
    virtual bool pre_visit(const IsVoid&) { return false; } 
    virtual bool pre_visit(const CaseBranch&) { return false; } 
    virtual bool pre_visit(const Assign&) { return false; } 
    virtual bool pre_visit(const Block&) { return false; } 
    virtual bool pre_visit(const If&) { return false; } 
    virtual bool pre_visit(const While&) { return false; } 
    virtual bool pre_visit(const Complement&) { return false; } 
    virtual bool pre_visit(const LessThan&) { return false; } 
    virtual bool pre_visit(const EqualTo&) { return false; } 
    virtual bool pre_visit(const LessThanEqualTo&) { return false; } 
    virtual bool pre_visit(const Plus&) { return false; } 
    virtual bool pre_visit(const Sub&) { return false; } 
    virtual bool pre_visit(const Mul&) { return false; } 
    virtual bool pre_visit(const Div&) { return false; } 
    virtual bool pre_visit(const Not&) { return false; } 
    virtual bool pre_visit(const StaticDispatch&) { return false; } 
    virtual bool pre_visit(const DynamicDispatch&) { return false; } 
    virtual bool pre_visit(const Let&) { return false; } 
    virtual bool pre_visit(const Case&) { return false; } 
    virtual bool pre_visit(const Object&) { return false; } 
    virtual bool pre_visit(const NoExpr&) { return false; } 

    virtual void post_visit(const Program&) {}
    virtual void post_visit(const Class&) {}
    virtual void post_visit(const Feature&) {}
    virtual void post_visit(const Attribute&) {}
    virtual void post_visit(const Formal&) {}
    virtual void post_visit(const Method&) {}
    virtual void post_visit(const StringConst&) {}
    virtual void post_visit(const IntConst&) {}
    virtual void post_visit(const BoolConst&) {}
    virtual void post_visit(const New&) {}
    virtual void post_visit(const IsVoid&) {}
    virtual void post_visit(const CaseBranch&) {}
    virtual void post_visit(const Assign&) {}
    virtual void post_visit(const Block&) {}
    virtual void post_visit(const If&) {}
    virtual void post_visit(const While&) {}
    virtual void post_visit(const Complement&) {}
    virtual void post_visit(const LessThan&) {}
    virtual void post_visit(const EqualTo&) {}
    virtual void post_visit(const LessThanEqualTo&) {}
    virtual void post_visit(const Plus&) {}
    virtual void post_visit(const Sub&) {}
    virtual void post_visit(const Mul&) {}
    virtual void post_visit(const Div&) {}
    virtual void post_visit(const Not&) {}
    virtual void post_visit(const StaticDispatch&) {}
    virtual void post_visit(const DynamicDispatch&) {}
    virtual void post_visit(const Let&) {}
    virtual void post_visit(const Case&) {}
    virtual void post_visit(const Object&) {}
    virtual void post_visit(const NoExpr&) {}
};

class AstNodePrintVisitor : public AstNodeVisitor
{
private:
    std::ostream& os;
    size_t depth;

public:
    AstNodePrintVisitor(std::ostream&);

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


typedef std::vector<std::shared_ptr<Class>> Classes;
typedef std::vector<std::shared_ptr<Formal>> Formals;
typedef std::vector<std::shared_ptr<CaseBranch>> Cases;
typedef std::vector<std::shared_ptr<Feature>> Features;
typedef std::vector<std::shared_ptr<Expression>> Expressions;

#endif
