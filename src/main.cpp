#include "symboltable.hpp"
#include "tokentable.hpp"
#include "ast.hpp"
#include "semanticanalyzer.hpp"

#include <cstdio>
#include <iostream>

extern int yyparse(); 
extern int yynerrs; 
extern FILE* yyin;

// Following tables are used by both the lexer and parser
// to manage symbols like constants
IdentifierTable idtable;
IntTable inttable;
StringTable stringtable;

// Root of AST used by the parser. This should be populated
// after parsing phase
std::shared_ptr<Program> ast_root;

// Used by the error handling routines in both lexer and
// parser to provide a more informative error message
std::string curr_filename;

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        curr_filename = "<stdin>";
        yyin = stdin;
        yyparse();
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            curr_filename = argv[i];
            yyin = std::fopen(argv[i], "r");

            if (yyin)
            {
                yyparse();
                std::fclose(yyin);
            }
            else
            {
                std::cerr << argv[i] << ":error:cannot be opened\n";
            }
        }
    }

    if (yynerrs > 0)
    {
        std::cerr << argv[0] << ": **compilation halted due to lexical or syntax errors.**\n";
        exit(1);
    }
    
    /*
    AstNodePrintVisitor print(std::cout);
    ast_root->accept(print);
    */

    SemanticAnalyzer semant;
    if (!semant.validate_inheritance(ast_root->classes))
    {
        std:: cerr << argv[0] << ": **compilation halted due to inheritance errors.**\n";
        exit(1);
    }

    return 0;
}
