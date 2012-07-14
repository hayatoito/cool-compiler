#include "symboltable.hpp"
#include "tokentable.hpp"
#include "ast.hpp"

#include <iostream>

IdentifierTable idtable;
IntTable inttable;
StringTable stringtable;
std::shared_ptr<Program> ast_root;

extern int yyparse();
extern int yynerrs;

int main(int argc, char **argv)
{
    yyparse();

    if (yynerrs > 0)
    {
        std::cerr << argv[0] << ": **compilation halted due to lexical or syntax errors.**\n";
        exit(1);
    }
    
    AstNodePrintVisitor print(std::cout);
    ast_root->accept(print);

    return 0;
}
