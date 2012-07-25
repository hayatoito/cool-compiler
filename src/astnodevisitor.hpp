#ifndef ASTNODEVISITOR_H
#define ASTNODEVISITOR_H

#include "ast.hpp"
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

//Visitor that performs code generation for each AST node
class AstNodeCodeGenerator : public AstNodeVisitor
{
private:
    std::map<std::string, std::string> inherit_graph;
    std::ostream& os;

    //arithmetic instructions
    void emit_addiu(const char*, const char*, int);
    void emit_add(const char*, const char*, const char*);
    void emit_div(const char*, const char*, const char*);
    void emit_divu(const char*, const char*, const char*);
    void emit_mul(const char*, const char*, const char*);
    void emit_sub(const char*, const char*, const char*);
    
    //logical instructions
    void emit_and(const char*, const char*, const char*);
    void emit_neg(const char*, const char*);
    void emit_nor(const char*, const char*, const char*);
    void emit_not(const char*, const char*);
    void emit_or(const char*, const char*, const char*);
    void emit_xor(const char*, const char*, const char*);

    //constant manipulating instructions
    void emit_li(const char*, int);
    void emit_lui(const char*, int);

    //comparison instructions
    void emit_seq(const char*, const char*, const char*);
    void emit_seq(const char*, const char*, int);
    void emit_sge(const char*, const char*, const char*);
    void emit_sge(const char*, const char*, int);
    void emit_sgt(const char*, const char*, const char*);
    void emit_sgt(const char*, const char*, int);
    void emit_sle(const char*, const char*, const char*);
    void emit_sle(const char*, const char*, int);
    void emit_sne(const char*, const char*, const char*);
    void emit_sne(const char*, const char*, int);

    //branch and jump instructions
    void emit_b(const char*);
    void emit_beq(const char*, const char*, const char*);
    void emit_beq(const char*, int, const char*);
    void emit_bge(const char*, const char*, const char*);
    void emit_bge(const char*, int, const char*);
    void emit_bne(const char*, const char*, const char*);
    void emit_bne(const char*, int, const char*);
    void emit_j(const char*);
    void emit_jal(const char*);
    void emit_jalr(const char*);
    void emit_jr(const char*);

    //load instructions
    void emit_la(const char*, const char*);
    void emit_lb(const char*, const char*);
    void emit_ld(const char*, const char*);
    void emit_lw(const char*, const char*);

    //store instructions
    void emit_sb(const char*, const char*);
    void emit_sd(const char*, const char*);
    void emit_sw(const char*, const char*);

    //data movement instructions
    void emit_move(const char*, const char*);

    //exception and trap instructions
    void emit_syscall();
    void emit_nop();

    //helper functions for generating code for object layout, etc

public:
    AstNodeCodeGenerator(const std::map<std::string, std::string>&, 
            std::ostream&);

    void install_basic();
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
