#include "symboltable.hpp"
#include "tokentable.hpp"
#include "ast.hpp"

#include <iostream>

IdentifierTable idtable;
IntTable inttable;
StringTable stringtable;
std::shared_ptr<Program> ast_root;

extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();
    return 0;
}
