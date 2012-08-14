#ifndef ASTNODECODEGENERATOR_H
#define ASTNODECODEGENERATOR_H

#include "astnodevisitor.hpp"

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

public:
    AstNodeCodeGenerator(const std::map<std::string, std::string>&, 
            std::ostream&);

    void visit(Program&);
    void visit(Class&);
    void visit(Feature&);
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
};

#endif
