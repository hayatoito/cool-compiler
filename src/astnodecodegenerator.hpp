// This is where code generation for the AST is done.

#ifndef ASTNODECODEGENERATOR_H
#define ASTNODECODEGENERATOR_H

#include "astnodevisitor.hpp"

// Visitor that performs code generation for each AST node
class AstNodeCodeGenerator : public AstNodeVisitor
{
private:
    // Class tags for some basic classes.
    // Note that there are no pre-defined class tags for class Object
    // and IO simply because there is no need. These class tags are used
    // for determining equality since an Object of type String can only
    // be compared with another Object of the same type. The same goes
    // for Int and Bool. 
    static const int STR_CLASS_TAG = 5;
    static const int INT_CLASS_TAG = 6;
    static const int BOOL_CLASS_TAG = 7;

    // Default size of basic class objects in # of 32-bit words
    static const int STR_CONST_BASE = 4; // for string constants, the total size is 4 + length of string  / word size
    static const int INT_CONST_SIZE = 4;
    static const int BOOL_CONST_SIZE = 4;

    // Default word size for SPIM
    static const int WORD_SIZE = 4;

    // Object header size in # of 32-bit words
    static const int OBJECT_HEADER_SIZE = 3;

    // The object layout is as follows:
    /*

             -------------------------   <----
            |        CLASS TAG        |
             -------------------------
            |   SIZE (32-bit words)   |  Object header section
             -------------------------
            |   Dispatch Table Ptr.   |  
             -------------------------   <----
            |       Attribute1        |
             -------------------------
            |       AttributeN        |
             -------------------------

    */

    // Stack frame base size in # of 32-bit words
    // Each stack frame will at least need 3 words for: 
    //   1. frame pointer to previous AR
    //   2. self object
    //   3. return address
    static const int AR_BASE_SIZE = 3;

    // The activation record layout is as follows:
    /* 
             -----------------------
            |   OLD FRAME POINTER   |
             -----------------------
            |      SELF OBJECT      |
             -----------------------
            |       ARGUMENT1       |
             -----------------------
            |       ARGUMENTN       |
             -----------------------
            |    RETURN ADDRESS     | <---- current frame pointer
             ----------------------- 
    */

    std::map<ClassPtr, ClassPtr> inherit_graph; // inheritance graph created from semantic analysis stage
    std::ostream& os; // code generation output

    std::size_t curr_attr_count; // used to keep track of current attribute count for a specific class when 
                                 // generating code for class_init methods

    Symbol curr_class; // current class where code is being generated for, used by dynamic dispatch

    SymbolTable<Symbol, int> var_env; // the variable environment mapping that maps variable names
                                      // to offsets in the current AR relative to the fp. this allows for easier
                                      // addressing. eg. the first parameter is in 4($fp), next is 8($fp) ... n($fp)
    
    std::map<Symbol, std::map<Symbol, int>> method_tbl; // contains mapping of [class name][method name] -> offset in dispatch table
                                                        // used to implement dispatch

    std::map<Symbol, std::map<Symbol, int>> attr_tbl; // table of class attributes used to determine valid names 
                                                      // that are in scope

    std::size_t while_count; // running count of all while statements in the source file, used for label numbering
                             // in the generated code
    std::size_t if_count;

    // The following emit_* functions are all helper functions to make emitting MIPS code easier

    // generic instructions
    void emit_align(int);
    void emit_ascii(const std::string&);
    void emit_asciiz(const std::string&);
    void emit_byte(int);
    void emit_globl(const std::string&);
    void emit_word(int);
    void emit_word(const std::string&);
    void emit_label(const std::string&);

    // arithmetic instructions
    void emit_add(const char*, const char*, const char*);
    void emit_addi(const char*, const char*, int);
    void emit_addiu(const char*, const char*, int);
    void emit_div(const char*, const char*, const char*);
    void emit_divu(const char*, const char*, const char*);
    void emit_mul(const char*, const char*, const char*);
    void emit_sub(const char*, const char*, const char*);
    
    // logical instructions
    void emit_and(const char*, const char*, const char*);
    void emit_neg(const char*, const char*);
    void emit_nor(const char*, const char*, const char*);
    void emit_not(const char*, const char*);
    void emit_or(const char*, const char*, const char*);
    void emit_xor(const char*, const char*, const char*);

    // constant manipulating instructions
    void emit_li(const char*, int);
    void emit_lui(const char*, int);

    // comparison instructions
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

    // branch and jump instructions
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

    // load instructions
    void emit_la(const char*, const std::string&);
    void emit_lb(const char*, const char*);
    void emit_ld(const char*, const char*);
    void emit_lw(const char*, int, const char*);

    // store instructions
    void emit_sb(const char*, const char*);
    void emit_sd(const char*, const char*);
    void emit_sw(const char*, int, const char*);

    // data movement instructions
    void emit_move(const char*, const char*);

    // stack operations
    // note that these functions take the number of 32-bit words to push
    void emit_push(int);
    void emit_pop(int);

    // exception and trap instructions
    void emit_syscall();
    void emit_nop();

    void emit_initial_data();

    // emit code for string and integer constants
    void code_constants();

    //void code_class_name_table();
    //void code_prototype_table();

    // emit code for each object's dispatch table
    void code_dispatch_table(const ClassPtr&);

    // emit code for prototype objects
    void code_prototype_objects();

    // calculates the size of an object, used when generating code
    // for each object prototype
    int calc_obj_size(const ClassPtr&);

    // emit code for the prototype object attributes
    void emit_obj_attribs(const ClassPtr&);

public:
    AstNodeCodeGenerator(const std::map<ClassPtr, ClassPtr>&, 
            std::ostream&);

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
};

#endif
