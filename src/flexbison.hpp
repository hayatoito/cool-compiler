#ifndef FLEXBISON_H
#define FLEXBISON_H

#undef YYSTYPE
#define YYSTYPE ParserType

#include "ast.hpp"

class ParserType
{
public:
    std::shared_ptr<BoolConst> boolean;
    Symbol symbol;
    std::shared_ptr<Program> program;
    std::shared_ptr<Class> clazz;
    Classes classes;
    std::shared_ptr<Feature> feature;
    Features features;
    std::shared_ptr<Formal> formal;
    Formals formals;
    std::shared_ptr<CaseBranch> branch;
    Cases cases;
    std::shared_ptr<Expression> expression;
    Expressions expressions;
    char* error_msg;

    ParserType() {}
}; 

#endif
