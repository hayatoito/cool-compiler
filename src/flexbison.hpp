#ifndef FLEXBISON_H
#define FLEXBISON_H

#undef YYSTYPE
#define YYSTYPE ParserType

#include "ast.hpp"

class ParserType
{
public:
    bool boolean;
    Symbol symbol;
    ProgramPtr program;
    ClassPtr clazz;
    Classes classes;
    AttributePtr attribute;
    Attributes attributes;
    MethodPtr method;
    Methods methods;
    FormalPtr formal;
    Formals formals;
    CaseBranchPtr branch;
    Cases cases;
    ExpressionPtr expression;
    Expressions expressions;
    std::string error_msg;

    ParserType() {}
}; 

#endif
