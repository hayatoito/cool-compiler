// This is where the type checking functionality is implemented.
// Currently, the type checker handles errors by assigning a type of Object
// to the node that didn't type check properly. This will cause the errors
// to propagate if a type error is found 

#ifndef ASTNODETYPECHECKER_H
#define ASTNODETYPECHECKER_H

#include "astnodevisitor.hpp"

typedef std::map<Symbol, std::map<Symbol, std::vector<Symbol>>> MethodTypeTable;

class AstNodeTypeChecker : public AstNodeVisitor
{
private:
    SymbolTable<Symbol, Symbol> env; // used to verify scoping rules
    Symbol curr_class; // current class that's being type checked
    MethodTypeTable mtbl; // mapping of [class name][method name] -> param_type0 ... param_typeN, return type
    std::map<ClassPtr, ClassPtr> inherit_graph; // inheritance tree

    std::size_t err_count; // total number of errors encountered 

    // check if a type is a subtype of the other type
    bool is_subtype(const Symbol&, const Symbol&);

    // least upper bound of a list of types in the inheritance tree
    Symbol lub(const std::vector<Symbol>&);

    // wrapper for generic error functions in utility.hpp
    void error(const AstNode&, const std::string&);

public:
    AstNodeTypeChecker(const std::map<ClassPtr, ClassPtr>&);
    void visit(Program&);
    void visit(Class&);
    void visit(Attribute&);
    void visit(Method&);
    void visit(Formal&);
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

    std::size_t get_err_count() const; // return the total error count accumulated in the analysis
};

#endif
