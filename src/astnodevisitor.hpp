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

//Visitor that performs code generation for each AST node
class AstNodeCodeGenerator : public AstNodeVisitor
{
private:
    //class tags for basic classes
    static const int STR_CLASS_TAG = 5;
    static const int INT_CLASS_TAG = 6;
    static const int BOOL_CLASS_TAG = 7;

    //default size of basic classes in # of 32-bit words
    static const int STR_CONST_BASE = 4; // for string constants, the base size is 4 + length of string  / word size
    static const int INT_CONST_SIZE = 4;
    static const int BOOL_CONST_SIZE = 4;

    //object header size in # of 32-bit words
    static const int OBJECT_HEADER_SIZE = 3;

    //stack frame size in # of 32-bit words
    static const int AR_BASE_SIZE = 3;

    std::map<std::string, std::string> inherit_graph; //inheritance graph created from semantic analysis stage
    std::ostream& os; //code generation output

    std::size_t curr_attr_count; //used to keep track of current attribute count for a specific class when 
                                 //generating code for class_init methods

    Symbol curr_class; //current class where code is being generated for

    SymbolTable<Symbol, int> var_env; //the variable environment mapping that maps variable names
                                      //to offsets in the current AR

    std::size_t while_count;
    std::size_t if_count;

    //generic instructions
    void emit_align(int);
    void emit_ascii(const std::string&);
    void emit_asciiz(const std::string&);
    void emit_byte(int);
    void emit_globl(const std::string&);
    void emit_word(int);
    void emit_word(const std::string&);
    void emit_label(const std::string&);

    //arithmetic instructions
    void emit_add(const char*, const char*, const char*);
    void emit_addi(const char*, const char*, int);
    void emit_addiu(const char*, const char*, int);
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
    void emit_b(const std::string&);
    void emit_beq(const char*, const char*, const std::string&);
    void emit_beq(const char*, int, const std::string&);
    void emit_bge(const char*, const char*, const std::string&);
    void emit_bge(const char*, int, const std::string&);
    void emit_bne(const char*, const char*, const std::string&);
    void emit_bne(const char*, int, const std::string&);
    void emit_j(const std::string&);
    void emit_jal(const std::string&);
    void emit_jalr(const char*);
    void emit_jr(const char*);

    //load instructions
    void emit_la(const char*, const std::string&);
    void emit_lb(const char*, const char*);
    void emit_ld(const char*, const char*);
    void emit_lw(const char*, int, const char*);

    //store instructions
    void emit_sb(const char*, const char*);
    void emit_sd(const char*, const char*);
    void emit_sw(const char*, int, const char*);

    //data movement instructions
    void emit_move(const char*, const char*);

    //stack operations
    void emit_push(int);
    void emit_pop(int);

    //exception and trap instructions
    void emit_syscall();
    void emit_nop();

    void emit_initial_data();

    //helper functions for generating code for object layout, etc
    void code_constants();
    void code_class_name_table();
    void code_prototype_table();
    void code_dispatch_table(const std::string&);
    void code_prototype_objects();
    std::map<std::string, int> count_attrs();
    int calc_obj_size(std::map<std::string, int>&, const std::string&);
    void emit_obj_attribs(const std::string&);
    bool is_basic_class(const Symbol&);

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
