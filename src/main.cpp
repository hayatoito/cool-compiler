#include "symboltable.hpp"
#include "tokentable.hpp"
#include "ast.hpp"
#include "semanticanalyzer.hpp"
#include "astnodevisitor.hpp"

#include <cstdio>
#include <iostream>
#include <fstream>

extern int yyparse(); 
extern int yynerrs; 
extern FILE* yyin;

// Root of AST used by the parser. This should be populated
// after parsing phase
ProgramPtr ast_root;

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
                std::cerr << argv[i] << ": error: cannot be opened\n";
            }
        }
    }

    if (yynerrs > 0)
    {
        std::cerr << "Compilation halted due to lexical or syntax errors.\n";
        exit(1);
    }
    
    SemanticAnalyzer semant;
    semant.install_basic(ast_root);
    if (!semant.validate_inheritance(ast_root->classes))
    {
        std:: cerr << "Compilation halted due to inheritance errors.\n";
        exit(1);
    }
    semant.type_check(ast_root);

    AstNodeDisplayer print(std::cout, AstNodeDisplayer::DISPLAYNONBASIC);
    ast_root->accept(print);

    //std::ofstream out("output.s");
    //AstNodeCodeGenerator codegen(semant.get_inherit_graph(), out);
    //ast_root->accept(codegen);

    return 0;
}
